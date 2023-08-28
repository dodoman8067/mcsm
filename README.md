# MCSM

## Description

MCSM (MineCraft Server Manager) is a command-line program designed to manage Minecraft servers. Note that this project is still work in progress and features may be buggy or not completed.

## Building

The provided Makefile is designed for Windows + MinGW setup. Other platforms may work but you will get NO support by doing this. Before building, ensure you have the necessary dependencies and files in place :

1. Build the non-static dependencies as it showed in the below.

2. Use the provided Makefile to build the project. This can be done with the following command :

    make

3. Run the compiled executable to start using MCSM.

### Dependencies

This project includes the following dependencies :

* [cURL](https://github.com/curl/curl)
* [nlomann/json](https://github.com/nlohmann/json)

First, build the dependencies and place the `.a` files in the `lib` folder (you can create it yourself), `.dll` files in the `bin` folder and finally, copy include files from the dependency's include folder. (single_include folder if you are copying the nlomann/json library.)

## Usage

To use MCSM, execute the compiled executable. The program provides command-line commands for managing Minecraft servers. For a list of available commands, type:

    mcsm help

## Contributing

This project is open to contributions. If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

## License

This project is licensed under the [MIT](LICENSE).
