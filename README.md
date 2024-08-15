

# 🖥️ Minishell

## Overview

**Minishell** is a lightweight Unix shell implementation that provides a basic command-line interface for executing commands and managing processes. This project is designed to help you understand shell concepts, process management, and command execution in Unix-like systems. 🌟

## 🚀 Project Objectives

- Implement a basic Unix shell that can interpret and execute commands.
- Support built-in commands and external commands with arguments.
- Manage processes, handle input/output redirection, and implement piping between commands.

## 🛠️ Key Features

### Command Execution

- **Execute External Commands:** Run programs and scripts from the command line.
- **Support for Arguments:** Pass arguments to commands and handle them appropriately.

### Built-in Commands

- **`cd`:** Change the current working directory.
- **`pwd`:** Print the current working directory.
- **`echo`:** Display a line of text.
- **`exit`:** Exit the shell with a specified exit status.

### Input/Output Redirection

- **Redirect Input:** Use `<` to redirect input from a file.
- **Redirect Output:** Use `>` to redirect output to a file.
- **Append Output:** Use `>>` to append output to a file.

### Piping

- **Pipe Commands:** Use `|` to pass the output of one command as input to another.

## 📋 Project Structure

```plaintext
.
├── LICENSE
├── Makefile
├── README.md
├── en.subject.pdf
└── srcs
    ├── cd.c
    ├── cd2.c
    ├── check_builtins.c
    ├── cmd.c
    ├── create_lexer.c
    ├── dollar.c
    ├── echo.c
    ├── env.c
    ├── exec_builtins.c
    ├── exec_utils.c
    ├── exec_utils2.c
    ├── execute.c
    ├── execute_bin.c
    ├── execute_bin2.c
    ├── execution_utils.c
    ├── exit.c
    ├── expand_value.c
    ├── expander.c
    ├── expander_utils.c
    ├── export.c
    ├── export2.c
    ├── export_tools.c
    ├── free.c
    ├── ft_env.c
    ├── ft_error.c
    ├── heredoc.c
    ├── lexing.c
    ├── lexing_utils.c
    ├── libft.c
    ├── libft2.c
    ├── list.c
    ├── main.c
    ├── main_helper.c
    ├── minishell.h
    ├── parse_args.c
    ├── parsing.c
    ├── parsing_error.c
    ├── parsing_redirction.c
    ├── parsing_utils.c
    ├── parsing_utils2.c
    ├── pipe_execution.c
    ├── pwd.c
    ├── quote.c
    ├── syntax.c
    ├── syntax_utils.c
    ├── t.c
    ├── t2.c
    ├── test.c
    ├── unset.c
    └── update_oradd_env.c

2 directories, 54 files

```

## 🔧 Compilation

To compile the project, use the provided `Makefile`:

```bash
make
```

This will generate an executable called `minishell`.

## 📈 Usage

To run the shell, execute the `minishell` binary:

```bash
./minishell
```

You will enter the shell prompt where you can type commands and interact with the shell.

### Example Commands

- **List Files:** `ls -l`
- **Change Directory:** `cd /path/to/directory`
- **Print Working Directory:** `pwd`
- **Redirect Output to a File:** `echo "Hello World" > output.txt`
- **Pipe Commands:** `cat file.txt | grep "keyword"`

## 🧪 Testing

Test your shell by:

- Executing a variety of commands and built-in commands.
- Using input/output redirection to handle files.
- Testing piping between multiple commands.
- Checking edge cases like invalid commands or syntax errors.

## 📚 Learning Objectives

By completing this project, you will:
- Gain a solid understanding of how Unix shells work.
- Learn about process creation, management, and synchronization.
- Understand command parsing, redirection, and piping in a shell environment.

## 🌟 Bonus Objectives

For additional practice and enhancement, consider the following:

- **Job Control:** Implement job control features like background processes and signal handling.
- **Command History:** Add support for command history and navigation.
- **Tab Completion:** Implement tab completion for commands and file paths.

## 📜 License

This project is open-source and available under the [MIT License](LICENSE). Feel free to fork, modify, and contribute!


