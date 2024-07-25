# Sub-C Compiler

## Building and Getting Started

1. Clone repo:

    ```bash
    git clone https://github.com/michael-re/sub-c.git
    ```

2. Navigate to the project directory:

    ```bash
    cd subc/
    ```

3. Build:

    ```bash
    # debug build
    make debug

    # release build
    make release

    # remove all compiled binaries and build artifacts
    make clean
    ```

    > [!NOTE]
    > after running the makefile, the compiled binaries are placed in the
    > `bin/` directory located in the project root.
