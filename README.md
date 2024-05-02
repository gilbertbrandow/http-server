# Primitive HTTP Server written in C

## Description
This is a minimalist HTTP server in C, configured from an .env file in the root directory. Concurrent handling of client connections is achieved, with each connection managed on a separate thread. The routing module supports all request types and is designed for easy expansion. In this application there are 5 GET endpoints specified in the main.c file. 4 of the endpoints return a simple static HTML page, while the last one is configured to handle requests for serving binary files from the /public/images folder.. There is also a GET route setup to handle a redirect from /favicon. Additionally, a single POST route is set up for creating and saving comments in the data/ directory, though without any graphical interface.The server incorporates a graceful shutdown mechanism to ensure the proper closure of server socket when terminated.

**Note:** This server is intended for basic web serving needs and does not include advanced features such as dynamic content generation or extensive security measures. In other words, it is not suitable for production use.

## Table of Contents

- [Dependencies](#dependencies)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [Routes](#routes)
- [License](#license)

## Dependencies
None.
## Installation

```bash
# Example installation commands
git clone git@github.com:gilbertbrandow/http-server.git
cd http-server
make
```
## Configuration
To configure the behavior and network settings of the C HTTP server, you can adjust the following parameters in the config.ini file:

```
ADDRESS_FAMILY: Specifies the communication domain, such as AF_INET for IPv4.
SOCKET_TYPE: Defines the communication characteristics of the socket, for instance, SOCK_STREAM for TCP.
DOMAIN: Indicates the interface address to bind to. INADDR_ANY is used to bind to any available interface.
PROTOCOL: Refers to the specific protocol to be used with the socket. Use 0 for the default.
PORT: The port number to which the server should bind, e.g., 8000.
CONNECTION_BACKLOG: Sets the maximum length of the queue with pending connections.
SO_REUSEADDR_ENABLED: Enable or disable the SO_REUSEADDR socket option. When enabled, it allows immediate reuse of the server socket, which can be useful for fast server restarts but may lead to unexpected behavior in certain scenarios.
````

Adjust these configurations according to your specific requirements and environment setup. Ensure to review the implications of enabling SO_REUSEADDR_ENABLED, as it might affect the server behavior during restarts.

## Usage
Run the server using the following command:
```bash
# Make sure you have compiled the code first using the 'make' command.
./source/bin/http-server
```

If you would like to clear the directory of compiled files:
```bash
make clear
```

## Routes
As mentioned in the description, a couple of example routes have been created for testing purposes.

### GET: Frida Kahlo page

Description: Get a webpage with a quote and image from Frida Kahlo.

```
GET /frida-kahlo HTTP/1.1
Host: localhost
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Connection: keep-alive

```

### POST: Create comment

Description: Persist a comment in the NFS

Example Request:

```http
POST /comments HTTP/1.1
Content-Type: application/json

{
    "name": "Georgia O'Keeffe",
    "comment": "I hate flowers - I paint them because they're cheaper than models and they don't move."
}
```

Note:
* This route only supports requests with Content-Type: application/json.
* Ensure that the request body includes the "name" and "comment" fields.
* The "name" field must be between 1 and 25 characters long.
* The "comment" field must be between 1 and 200 characters long.
* Proper error handling is implemented for cases where Content-Type does not match, fields are missing or content length mismatch long.

## License
[License file](LICENSE)
