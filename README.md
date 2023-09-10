# MCSM

## Description

MCSM (MineCraft Server Manager) is a command-line program designed to manage Minecraft servers. Note that this project is still work in progress and features may be buggy or not completed.

## Building

The provided [CMakeLists.txt](CMakeLists.txt) is designed for Windows + MinGW setup. For Linux, you may build  Before building, ensure you have the necessary dependencies and files are in place :

1. Build the static dependencies as it showed in the below.

2. Use the provided [CMakeLists.txt](CMakeLists.txt) to build the project. This can be done with the following command :

    cmake -B ./bin

3. Run the generated build files in bin directory and run `./mcsm` to test program is sucessfully compiled.

### Dependencies

This project includes the following dependencies :

* [cURL](https://github.com/curl/curl)
* [nlohmann/json](https://github.com/nlohmann/json)

1. Build static libcurl and install it.

2. Copy the include files from the dependency's `include` folder (Copy the `single_include` when including nlohmann/json) and place them to project's `include` folder (you can create it yourself)

3. Run the cmake command.

## Usage

To use the compiled executable, type:

    mcsm help

By doing this, you can get list of commands. If it's not working, try this:

    ./mcsm help

## Contributing

Currently, you can't contribute to this project because it isn't completed right now.

## License

This project is licensed under the [MIT](LICENSE).
