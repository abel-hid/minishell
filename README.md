# 🐚 Minishell

> A small Bash-like shell written in C, built as part of the **42 School** curriculum.

Minishell reads a command line, splits it into tokens, checks the syntax, handles heredocs, expands variables, builds a list of commands, and runs them. It supports pipes, redirections, environment variables, signals, and the builtins required by the subject.

```
~minishell~$ echo "Hello $USER" | tr a-z A-Z > out.txt
~minishell~$ cat < out.txt
HELLO ABEL-HID
~minishell~$ echo $?
0
```

---

## 📑 Table of Contents

- [Features](#-features)
- [Requirements](#-requirements)
- [Build & Run](#-build--run)
- [How It Works](#-how-it-works)
  - [1. Read the line](#1-read-the-line)
  - [2. Lexer](#2-lexer)
  - [3. Syntax check](#3-syntax-check)
  - [4. Heredoc](#4-heredoc)
  - [5. Expander](#5-expander)
  - [6. Parser](#6-parser)
  - [7. Executor](#7-executor)
- [Data Structures](#-data-structures)
- [Builtins](#-builtins)
- [Signals](#-signals)
- [Exit Status](#-exit-status)
- [Project Structure](#-project-structure)
- [Limitations](#-limitations)
- [Authors](#-authors)
- [License](#-license)

---

## ✨ Features

| Feature | Details |
|---|---|
| **Prompt & history** | Interactive prompt `~minishell~$ ` with line editing and history (GNU Readline) |
| **External commands** | Found through `$PATH`, or run directly with a relative/absolute path (`./a.out`, `/bin/ls`) |
| **Pipes** | `cmd1 \| cmd2 \| cmd3 ...` with any number of commands |
| **Redirections** | `<` input, `>` output (truncate), `>>` output (append), `<<` heredoc |
| **Quotes** | `'single'` (nothing is expanded) and `"double"` (only `$` is expanded) |
| **Variables** | `$VAR` expansion from the shell's own environment, `$?` for the last exit status |
| **Builtins** | `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit` |
| **Signals** | `Ctrl-C`, `Ctrl-D`, `Ctrl-\` behave like in Bash |
| **Errors** | Bash-style messages: `command not found`, `No such file or directory`, `Permission denied`, `is a directory`, `ambiguous redirect`, `syntax error near unexpected token` |

---

## 📦 Requirements

- A Unix-like system (macOS or Linux)
- `cc` (clang or gcc) and `make`
- **GNU Readline** library

The Makefile finds Readline with Homebrew (`brew --prefix readline`), which is the setup used on 42 macOS machines:

```bash
brew install readline
```

On Linux, install the development package (for example `sudo apt install libreadline-dev`) and change the `READLINE` line in the Makefile to a path that exists, such as `/usr`.

---

## 🔨 Build & Run

```bash
git clone <repo-url> minishell
cd minishell
make            # builds ./minishell
./minishell
```

| Rule | Action |
|---|---|
| `make` / `make all` | Compile the `minishell` binary |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and the binary |
| `make re` | `fclean` then `all` |

Flags: `-Wall -Wextra -Werror`. To quit the shell, type `exit` or press `Ctrl-D`.

---

## ⚙️ How It Works

Each line goes through the same pipeline:

```
 readline()
     │
     ▼
 ┌─────────┐   ┌──────────────┐   ┌──────────┐   ┌──────────┐   ┌──────────┐   ┌──────────┐
 │  Lexer  │──▶│ Syntax check │──▶│ Heredoc  │──▶│ Expander │──▶│  Parser  │──▶│ Executor │
 └─────────┘   └──────────────┘   └──────────┘   └──────────┘   └──────────┘   └──────────┘
  t_lexer list   stop on error     /tmp/heredoc    $VAR, $?      t_command list   fork/pipe/execve
```

The main loop is in [srcs/main.c](srcs/main.c) (`minishell_exec`). After each line, everything allocated for it is freed (`free_stuff`) and the loop starts again.

### 1. Read the line

[srcs/main.c](srcs/main.c), [srcs/main_helper.c](srcs/main_helper.c)

- At startup, the environment (`envp`) is copied into a linked list of `t_env` (`craete_env` in [srcs/env.c](srcs/env.c)). From then on, the shell uses only this list.
- `readline()` shows the prompt. Non-empty lines are added to history.
- `NULL` from readline (Ctrl-D) prints `exit` and quits.
- Empty lines and lines made only of spaces are skipped.

### 2. Lexer

[srcs/create_lexer.c](srcs/create_lexer.c), [srcs/lexing.c](srcs/lexing.c), [srcs/lexing_utils.c](srcs/lexing_utils.c)

The line is cut into a doubly linked list of tokens. Each token has a type:

| Input | Token |
|---|---|
| any word | `WORD` |
| `\|` | `PIPE_LINE` |
| `<` | `REDIR_IN` |
| `>` | `REDIR_OUT` |
| `<<` | `HEARDOC` |
| `>>` | `APPEND` |

A word ends at a space or at one of `| < >`, **except inside quotes**: quoted text stays in the same word. So `echo "a | b"` gives two tokens, `echo` and `"a | b"`. Quotes are kept at this stage and removed later.

Example: `cat < in.txt | grep "hello world" >> out`

```
[WORD cat] ⇄ [REDIR_IN <] ⇄ [WORD in.txt] ⇄ [PIPE_LINE |] ⇄ [WORD grep] ⇄ [WORD "hello world"] ⇄ [APPEND >>] ⇄ [WORD out]
```

### 3. Syntax check

[srcs/syntax.c](srcs/syntax.c), [srcs/syntax_utils.c](srcs/syntax_utils.c)

Three checks run on the token list. If one fails, an error is printed and the line is dropped:

- **Pipes**: a `|` can't be first, last, or next to another operator → `syntax error near unexpected token '|'`
- **Quotes**: every `'` or `"` must be closed
- **Redirections**: every `<`, `>`, `>>`, `<<` must be followed by a `WORD` → `syntax error near unexpected token 'newline'`

### 4. Heredoc

[srcs/heredoc.c](srcs/heredoc.c)

For each `<<`, the shell reads lines with the `>` prompt until it gets the delimiter, and writes them to `/tmp/heredoc`.

- If the delimiter has **no quotes** (`<< EOF`), `$VAR` in the body is expanded.
- If the delimiter has **quotes** (`<< "EOF"` or `<< 'EOF'`), the body is kept as typed.
- `Ctrl-C` during a heredoc stops it and cancels the whole command.

Heredocs run **before** expansion and parsing, like in Bash.

### 5. Expander

[srcs/expander.c](srcs/expander.c), [srcs/expand_value.c](srcs/expand_value.c), [srcs/expander_utils.c](srcs/expander_utils.c), [srcs/dollar.c](srcs/dollar.c), [srcs/quote.c](srcs/quote.c)

`WORD` tokens that contain `$` are expanded with the shell's environment list:

- `$VAR` → value of `VAR`, or an empty string if it isn't set
- `$?` → last exit status (`g_exit_st.status`)
- Text inside **single quotes** is skipped, so `'$HOME'` stays `$HOME`
- `$` followed by a digit, a space, a quote, `$` or end of string is left alone
- `$'...'` is handled by removing the `$`
- Heredoc delimiters (the word after `<<`) are not expanded

### 6. Parser

[srcs/parsing.c](srcs/parsing.c), [srcs/parse_args.c](srcs/parse_args.c), [srcs/parsing_redirction.c](srcs/parsing_redirction.c), [srcs/parsing_utils.c](srcs/parsing_utils.c), [srcs/parsing_utils2.c](srcs/parsing_utils2.c), [srcs/parsing_error.c](srcs/parsing_error.c)

The token list becomes a linked list of `t_command`, one per pipe segment.

- **Words** become `args` (the `argv` for `execve`). Quotes are removed here. An unquoted variable whose value contains spaces is split into several arguments (word splitting), so with `X="ls -l"`, `$X` runs `ls` with `-l`.
- **Redirections** are opened right away and stored in `fd.fd_in` / `fd.fd_out`:
  - `>` → `open(O_CREAT | O_WRONLY | O_TRUNC, 0644)`
  - `>>` → `open(O_CREAT | O_WRONLY | O_APPEND, 0644)`
  - `<` → `open(O_RDONLY)`
  - `<<` → opens `/tmp/heredoc` for reading
- If several redirections of the same kind are given, the last one wins.
- If a file can't be opened, the fd is set to `-1`, the error is printed, and later redirections of that command are skipped. The command then exits with status `1`.
- A redirection target that expands to nothing or to several words (`> $EMPTY`) gives `ambiguous redirect`.

Example: `cat < in.txt | grep hi > out`

```
t_command #1: args = {"cat", NULL}         fd_in = fd(in.txt)  fd_out = 1
      │
      ▼
t_command #2: args = {"grep", "hi", NULL}  fd_in = 0           fd_out = fd(out)
```

### 7. Executor

[srcs/pipe_execution.c](srcs/pipe_execution.c), [srcs/exec_utils.c](srcs/exec_utils.c), [srcs/exec_utils2.c](srcs/exec_utils2.c), [srcs/exec_builtins.c](srcs/exec_builtins.c), [srcs/check_builtins.c](srcs/check_builtins.c), [srcs/execute_bin.c](srcs/execute_bin.c), [srcs/execute_bin2.c](srcs/execute_bin2.c)

The entry point is `execute_the_shot`. There are three cases:

**a) A single builtin** (`cd`, `export`, `exit`, ...). It runs **in the shell process**, without forking, so it can change the shell's state (current directory, environment, exit). stdin/stdout are saved with `dup`, redirected with `dup2`, and restored afterwards.

**b) A single external command.** The shell forks. The child applies its redirections and calls `execve`. The parent waits and saves the exit status.

**c) A pipeline.** For each command:
1. Create a pipe with `pipe()`
2. `fork()`
3. In the child: read end of the previous pipe → stdin, write end of the current pipe → stdout (unless it's the last command), apply the command's own redirections (they take priority over the pipe), then run the builtin or `execve`
4. In the parent: close the fds that are no longer needed and continue

After all commands are started, the parent waits for every child. The status of the last one becomes `$?`. In a pipeline, builtins run in a child, so `cd` or `export` there doesn't change the parent shell (same as Bash).

**Finding the binary:**
- If the name starts with `/` or `.`, it's used as a path
- Otherwise each directory of `$PATH` is tried (`dir/name`) with `access(F_OK | X_OK)`
- If `PATH` was unset, only paths work
- Errors: `command not found` (127), `No such file or directory` (127), `Permission denied` (126), `is a directory` (126)

---

## 🧱 Data Structures

All defined in [srcs/minishell.h](srcs/minishell.h).

```c
typedef enum e_tokens {          // token types
    WORD, PIPE_LINE, REDIR_IN, REDIR_OUT, HEARDOC, APPEND
} t_tokens;

typedef struct s_lexer {         // one token (doubly linked list)
    char            *content;
    t_tokens        token;
    struct s_lexer  *next;
    struct s_lexer  *prev;
} t_lexer;

typedef struct s_fd {            // fds of a command
    int fd_in;                   // 0 = stdin, -1 = open failed
    int fd_out;                  // 1 = stdout, -1 = open failed
} t_fd;

typedef struct s_command {       // one command of a pipeline
    char              **args;    // argv, NULL-terminated
    t_fd              fd;
    pid_t             pid;
    struct s_command  *next;
} t_command;

typedef struct s_env {           // one environment variable
    char          *key;
    char          *value;        // NULL if exported without a value
    struct s_env  *next;
} t_env;

typedef struct s_exit {          // the single global variable
    int   status;                // last exit status ($?)
    char  *path;                 // cached PATH
    int   is_unset;              // 1 after `unset PATH`
    int   in_here_doc;           // 1 while reading a heredoc
    int   in_cmd;                // 1 while a command runs
    int   done;                  // 1 if a heredoc was cancelled
    int   in, out;               // saved stdin/stdout for builtins
} t_exit;

t_exit g_exit_st;
```

The subject allows only one global variable. It is `g_exit_st`, used mainly so the signal handler can see the shell's state.

---

## 🧰 Builtins

| Builtin | File | Behavior |
|---|---|---|
| `echo [-n] [args...]` | [echo.c](srcs/echo.c) | Prints the args separated by spaces. `-n` (also `-nnn`, and repeated `-n -n`) removes the trailing newline |
| `cd [path]` | [cd.c](srcs/cd.c), [cd2.c](srcs/cd2.c) | Changes directory. No argument goes to `$HOME`. Updates `PWD`. Prints `No such file or directory` or `HOME not set` on error |
| `pwd` | [pwd.c](srcs/pwd.c) | Prints the current directory using `getcwd` |
| `export [KEY[=VALUE]]...` | [export.c](srcs/export.c), [export2.c](srcs/export2.c), [export_tools.c](srcs/export_tools.c), [update_oradd_env.c](srcs/update_oradd_env.c) | No arguments: lists variables as `declare -x KEY="VALUE"`. `KEY=VALUE` sets a variable, `KEY+=VALUE` appends to it, `KEY` alone declares it without a value. Invalid names give `not a valid identifier` |
| `unset KEY...` | [unset.c](srcs/unset.c) | Removes variables. Checks the name is valid (letters, digits, `_`, not starting with a digit) |
| `env` | [ft_env.c](srcs/ft_env.c) | Prints variables that have a value, as `KEY=VALUE` |
| `exit [n]` | [exit.c](srcs/exit.c) | Exits with status `n` (or 0). A non-numeric argument gives `numeric argument required` (255). More than one argument gives `too many arguments` |

---

## 🚦 Signals

Handled in `signal_handler` ([srcs/execute.c](srcs/execute.c)) with `sigaction`.

| Key | At the prompt | While a command runs | In a heredoc |
|---|---|---|---|
| `Ctrl-C` (SIGINT) | New line and new prompt, `$?` = 1 | Command is interrupted, `$?` = 130 | Heredoc is cancelled (stdin is closed) |
| `Ctrl-\` (SIGQUIT) | Does nothing | Prints `Quit: 3` | Does nothing |
| `Ctrl-D` (EOF) | Prints `exit` and quits | — | Ends the heredoc |

---

## 🔢 Exit Status

`$?` follows Bash's conventions:

| Code | Meaning |
|---|---|
| `0` | Success |
| `1` | General error (bad redirection, invalid identifier, `cd` error, Ctrl-C at the prompt) |
| `126` | Found but can't run (permission denied, is a directory) |
| `127` | Command not found / no such file |
| `130` | Stopped by Ctrl-C |
| `255` | `exit` with a non-numeric argument |

---

## 🗂️ Project Structure

```
minishell/
├── Makefile
├── LICENSE
├── README.md
└── srcs/
    ├── minishell.h            # types, the global, all prototypes
    │
    ├── main.c                 # entry point and main loop
    ├── main_helper.c          # history, Ctrl-D, cleanup, signal setup
    │
    │   ── Lexer ──
    ├── create_lexer.c         # splits the line into tokens
    ├── lexing.c               # reads words (quote-aware), runs the syntax checks
    ├── lexing_utils.c         # ft_strlen, ft_substr, heredoc-cancel check
    │
    │   ── Syntax ──
    ├── syntax.c               # pipe / redirection / quote errors
    ├── syntax_utils.c         # unclosed quote check, operator helper
    │
    │   ── Heredoc ──
    ├── heredoc.c              # << handling, writes to /tmp/heredoc
    │
    │   ── Expander ──
    ├── expander.c             # $VAR and $? on tokens
    ├── expand_value.c         # replaces a variable with its value
    ├── expander_utils.c
    ├── dollar.c               # $ helpers, quote removal
    ├── quote.c                # quote and space helpers used by the lexer
    │
    │   ── Parser ──
    ├── parsing.c              # tokens → t_command list
    ├── parse_args.c           # counting args, word splitting
    ├── parsing_redirction.c   # opens < > files
    ├── parsing_utils.c        # space / quote checks on words
    ├── parsing_utils2.c       # opens >> files, grows the args array
    ├── parsing_error.c        # ambiguous redirect and file errors
    ├── cmd.c                  # t_command list helpers
    │
    │   ── Executor ──
    ├── pipe_execution.c       # single command vs pipeline
    ├── exec_utils.c           # child setup, pipes, waitpid
    ├── exec_utils2.c          # dup2 redirections in the child
    ├── exec_builtins.c        # running a builtin in the parent
    ├── check_builtins.c       # is it a builtin? which one?
    ├── execute_bin.c          # PATH search, permission checks
    ├── execute_bin2.c         # execve and its errors
    ├── execute.c              # signal handler, small helpers
    ├── execution_utils.c      # ft_atoi (with overflow check), env node helper
    │
    │   ── Builtins ──
    ├── echo.c
    ├── cd.c  cd2.c
    ├── pwd.c
    ├── export.c  export2.c  export_tools.c
    ├── unset.c
    ├── ft_env.c
    ├── exit.c
    │
    │   ── Environment ──
    ├── env.c                  # envp → t_env list, lookups
    ├── update_oradd_env.c     # add or update a variable
    │
    │   ── Utilities (own libft) ──
    ├── libft.c  libft2.c      # ft_strcmp, ft_strjoin, ft_putstr_fd, ...
    ├── test.c                 # ft_split, ft_strdup
    ├── t.c                    # ft_split1 (split on several separators)
    ├── t2.c                   # ft_itoa, ft_strncpy
    ├── list.c                 # token list helpers, ft_strncmp
    ├── ft_error.c             # error printing to stderr
    └── free.c                 # freeing lexer, commands, env, arrays
```

---

## ⚠️ Limitations

These are outside the mandatory part of the subject and are **not** supported:

- `&&`, `||`, and parentheses (bonus part)
- Wildcards `*` (bonus part)
- `;` and backslash `\`
- Job control (`&`, `fg`, `bg`)
- `cd -` and `cd ~`

Also good to know:

- All heredocs share one temporary file, `/tmp/heredoc`, so with several `<<` in a line only the last one's content is used.
- `cd` reads `HOME` with `getenv`, so it uses the `HOME` the shell started with, not one changed later with `export`.

---

## 👥 Authors

- **abel-hid**: [abel-hid@student.42.fr](mailto:abel-hid@student.42.fr)
- **heddahbi**: [heddahbi@student.42.fr](mailto:heddahbi@student.42.fr)

---

## 📄 License

MIT. See [LICENSE](LICENSE).
