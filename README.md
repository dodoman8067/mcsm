# MCSM

## Description

MCSM (MineCraft Server Manager) is a command-line program designed to manage Minecraft servers. Note that there's a better option called Docker and I wouldn't recommend using this right now because this project is mostly for me leaning C++.

## Supported platforms

* OS:
  * Windows
  * Linux

* Compilers:
  * MinGW-w64
  * GCC
  * Clang

MSVC is not supported right now. It might lead to unexpected behaviour if you try it.

### Dependencies

This project requires the following dependencies :

* [cURL](https://github.com/curl/curl)
* [libgit2](https://github.com/libgit2/libgit2)
* [nlohmann/json](https://github.com/nlohmann/json)
* [termcolor](https://github.com/ikalnytskyi/termcolor)

Before building, you must build them and install them with CMake.

## Usage

To use the compiled executable, type:

    mcsm help

By doing this you can get a list of commands that would help you get started. Try this instead if it isn't working :

    ./mcsm help

## Contributing

Currently you can't contribute to this project because it isn't completed right now.

## License

This project is under [MIT](LICENSE) license.

* [cURL](https://curl.se/docs/copyright.html)
* [libgit2](https://github.com/libgit2/libgit2/blob/main/COPYING)
* [nlohmann/json](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT)
* [termcolor](https://github.com/ikalnytskyi/termcolor/LICENSE)