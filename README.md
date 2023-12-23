# MCSM

## Description

MCSM (MineCraft Server Manager) is a command-line program designed to manage Minecraft servers. Note that this project is still work in progress and it's not ready for actual usage by other peoples.

## Supported platforms

* OS:
  * Windows
  * Linux

* Compilers:
  * MinGW-w64
  * GCC
  * Clang

For those who wonder : MSVC is not supported right now. It might lead to unexpected behaviour if you try it.

### Dependencies

This project requires the following dependencies :

* [cURL](https://github.com/curl/curl)
* [nlohmann/json](https://github.com/nlohmann/json)
* [termcolor](https://github.com/ikalnytskyi/termcolor)

Before building, you must build them(static if libcurl) and install it with CMake.

NOTE: libcurl is built with the following cmake command : cmake -B ./build -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF -DZLIB_USE_STATIC_LIBS=ON -DCURL_USE_SCHANNEL=ON

For Linux builds : cmake -B ./build -DBUILD_SHARED_LIBS=OFF -DOPENSSL_USE_STATIC_LIBS=ON -DZLIB_USE_STATIC_LIBS=ON

## Usage

To use the compiled executable, type:

    mcsm help

By doing this, you can get a list of commands. If it's not working, try this:

    ./mcsm help

## Contributing

Currently you can't contribute to this project because it isn't completed right now.

## License

This project is licensed under [MIT](LICENSE).

* [cURL](https://curl.se/docs/copyright.html)
* [nlohmann/json](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT)
* [termcolor](https://github.com/ikalnytskyi/termcolor/LICENSE)

Special thanks to the creators of the program's dependencies.