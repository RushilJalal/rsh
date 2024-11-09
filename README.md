# rsh - A Simple Shell in C

Welcome to `rsh`, a simple shell program written in C for UNIX-based systems. `rsh` provides basic shell functionalities with built-in commands like `cd`, `help`, and `exit`.

### Note: This shell program will not work on Windows systems due to its dependence on Unix system calls and libraries. 

## Table of Contents

- [About](#about)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Examples](#examples)

## About

`rsh` is a minimalistic shell created to help users understand and experiment with shell basics. This project includes essential features and built-in commands, aiming to provide a foundation for further shell development.

## Features

- **Built-in Commands**: Support for essential commands like `cd`, `help`, and `exit`.
- **Command Execution**: Execute both built-in and external commands.
- **Error Handling**: Basic error handling for invalid commands and parameters.

## Installation

To install and use `rsh` on your UNIX-based system, follow these steps:

1. Clone the repository:

    ```bash
    git clone https://github.com/rushiljalal/rsh.git
    ```

2. Navigate to the project directory:

    ```bash
    cd rsh
    ```

3. Compile the code:

    ```bash
    gcc -o rsh rsh.c
    ```

4. Run `rsh`:

    ```bash
    ./rsh
    ```

## Usage

Once `rsh` is running, you can enter commands at the prompt. `rsh` will recognize both built-in and external commands, providing a basic interactive shell experience.

```bash
rsh> <command>
```

## Built-in Commands

`rsh` includes the following built-in commands:

- **cd**: Change the current directory.
  - **Usage**: `cd <directory>`

- **help**: Display a list of available built-in commands and their usage.
  - **Usage**: `help`

- **exit**: Exit the shell.
  - **Usage**: `exit`

## Examples

```bash
rsh> cd /home/user
rsh> help
rsh> ls -la
rsh> exit