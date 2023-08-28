# MCSM

## Description

MCSM (MineCraft Server Manager) is a command-line program designed to manage Minecraft servers. Note that this project is still work in progress and features may be buggy or not completed.

## Building

The provided Makefile is designed for Windows + MinGW setup. Other platforms may work but you will get NO support by doing this. Before building, ensure you have the necessary dependencies and files in place:

1. Build the non-static libcurl library. Place the compiled `.a` file in the `lib` directory and the `.dll` file in the `bin` directory.

2. Use the provided Makefile to build the project. This may involve running commands in the terminal, such as:

    make

3. Run the compiled executable to start using MCSM.

## Usage

To use MCSM, execute the compiled executable. The program provides command-line commands for managing Minecraft servers. For a list of available commands, type:

    mcsm help

## Contributing

This project is open to contributions. If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

## License

This project is licensed under the [MIT](LICENSE).
