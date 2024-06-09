
# Minishell 42 Project

Welcome to the Minishell project! This project is part of the 42 school curriculum, where you will create a simple shell program that mimics some basic functionalities of the Unix shell.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
## Introduction

The Minishell project aims to help you understand how a shell works by implementing your own version. This will deepen your understanding of process creation and synchronization, as well as input/output redirection.

## Features

- Display a prompt and wait for user input
- Execute commands with their arguments
- Handle basic error messages
- Implement a history feature
- Support input and output redirection (`<`, `>`, `>>`)
- Handle pipes (`|`)
- Support environment variables
- Handle signals (like `Ctrl-C`,`Ctrl-\`)
- Handle end of file (EOF)
- Handle $?
- Implement a built-in command (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`)


## Installation

To get started with Minishell, follow these steps:

1. Clone the repository:
    \`\`\`bash
    git clone https://github.com/abel-hid/minishell.git
    cd minishell
    \`\`\`

2. Build the project:
    \`\`\`bash
    make
    \`\`\`

## Usage

After building the project, you can run the shell with:

\`\`\`bash
./minishell
\`\`\`

You will be greeted with a prompt where you can start typing commands.

### Example

\`\`\`sh
$ ls -l
total 8
-rwxr-xr-x 1 user group 20736 Jun  1 12:34 minishell
$ echo "Hello, World!"
Hello, World!
$ cat file.txt
Contents of the file.
$ exit
\`\`\`

## Built-in Commands

Minishell supports the following built-in commands:

- \`echo [args]\` - Display arguments
- \`cd [directory]\` - Change the current directory
- \`pwd\` - Print the current working directory
- \`export [name[=value] ...]\` - Set environment variables
- \`unset [name ...]\` - Unset environment variables
- \`env\` - Display all environment variables
- \`exit [status]\` - Exit the shell
