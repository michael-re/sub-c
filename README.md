# Sub-C Compiler

A compiler for a strict subset of the C programming language.

```c
int main()
{
    __says("Hello World!"); // print string using built-in function __says
    __sayl();               // print newline using built-in function __sayl
    return 0;
}
```

## Grammar

For detailed information about the syntax and structure supported by the Sub-C
Compiler, please refer to the [**`grammar.md`**](docs/grammar.md) file in the
docs folder.

## Getting Started

### Prerequisites

To build the compiler, you'll need the following:

- A C compiler (e.g., [`clang`](https://clang.llvm.org/), [`gcc`](https://gcc.gnu.org/), [`msvc`](https://visualstudio.microsoft.com/))
- [`Make`](https://www.gnu.org/software/make/)
- [`CMake`](https://cmake.org/) (3.20 or greater)

### Building the Compiler

1. Clone repository:

    ```bash
    git clone https://github.com/michael-re/sub-c.git
    ```

2. Navigate to project directory:

    ```bash
    cd sub-c/
    ```

3. Build:

    ```bash
    # Debug build
    make debug

    # Release build
    make release

    # Clean up compiled binaries and artifacts
    make clean
    ```

### Usage

To compile a Sub-C file, use the following command:

```bash
# format
./bin/subc <input_filename>.c [output_filename].s

# example 1 - outputs test01.s
./bin/subc tests/test01.c

#example 2 - outputs a.s
./bin/subc tests/test02.c a.s
```

|   arg                 |  type       |  description                    | default             |
| --------------------- | ----------- | ------------------------------- | ------------------- |
|  `input_filename`.c   |  required   |  The source file to compile     |  none               |
|  `output_filename`.s  |  optional   |  Name of output assembly file   |  `input_filename`.s |
