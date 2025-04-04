# Socker

This is a C static library that simplifies working with the Unix socket API. 

It provides wrapper functions for socket creation, binding, listening, connecting, and communication, handling common errors and boilerplate code.


## Installation

To build the library, you'll need to have CMake installed on your system.

Clone the repository:
```bash
git clone git@github.com:khlkarim/Socker.git
cd Socker
```

Build the library using CMake:
```bash
mkdir build
cd build
cmake ..
make
```

## Usage

- **Client**: Include `client.h` in your project.
- **Server**: Include `server.h` in your project.

## Todo

- Functionality for building HTTP requests and responses.
- `Request` and `Response` structures.
- Stats (Packets recieved and lost)