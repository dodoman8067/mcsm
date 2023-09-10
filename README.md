# MCSM

## Description

MCSM (MineCraft Server Manager) is a command-line program designed to manage Minecraft servers. Note that this project is still work in progress and features may be buggy or not completed.

## Building

The provided [CMakeLists.txt](CMakeLists.txt) is designed for Windows + MinGW setup. Other platforms may work but you will get NO support by doing this. Before building, ensure you have the necessary dependencies and files are in place :

1. Build the static dependencies as it showed in the below.

2. Use the provided [CMakeLists.txt](CMakeLists.txt) to build the project. This can be done with the following command :

    cmake -B ./bin

3. Run the compiled executable in the `bin` folder to start using MCSM.

### Dependencies

This project includes the following dependencies :

* [cURL](https://github.com/curl/curl)
* [nlohmann/json](https://github.com/nlohmann/json)

1. Build static libcurl and install it.

2. Copy the include files from the dependency's `include` folder (Copy the `single_include` when including nlohmann/json).

3. Run the cmake command.

## Usage

To use MCSM, execute the compiled executable. For a list of available commands, type:

    mcsm help

## Contributing

Currently, you can't contribute to this project because it isn't completed right now.

## License

This project is licensed under the [MIT](LICENSE).
