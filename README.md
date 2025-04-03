# Networking Library

This is a static library that abstracts the Berkeley Socket API, designed for use in HTTP/TCP servers and clients.

## Usage

- **Client**: Include `client.h` in your project.
- **Server**: Include `server.h` in your project.

## Todo

- Functionality for building HTTP requests and responses.
- `Request` and `Response` structures.
- Constructors for `Request` and `Response` structs.
[*] Generalise beyond HTTP
- Stats (Packets recieved and lost)
[*] Make the endpoint hold more info (like the type/protocol)