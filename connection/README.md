# connection

A simple and re-usable C++ network abstraction

### Listeners

The "Listener" family class is an abstraction to server sockets.

* [ATCPListener](ATCPListener.md): base class for Listeners.

* [TCPListener](TCPListener.md): final implementation of a listening socket.

* [OpenSSLTCPListener](OpenSSLTCPListener.md): final implementation over an OpenSSL secure connection.

### Example server code
Here's the code for a simple server that listens on port 4242, prints messages from clients on stdout and sends "Hello there!" as a reply.
You can test this server with "nc 127.0.0.1 4242": anything you type should be echoed server-side, and nc should print "Hello there!" back.

```cpp
#include <iostream>
#include "TCPListener.hpp"

int main()
{
    // Listen on port 4242
    putils::TCPListener server(4242);
    std::cout << "Server listening on port 4242" << std::endl;

    // Observe server for new clients
    server.addObserver([&server]()
    {
        auto &client = server.getLastClient();
        // Capture client's fd as the client reference may be invalidated

        std::cout << "New client connected" << std::endl;
        client.send("Welcome!\n");

        client.newMessage += [&client]
        {
            std::cout << "[" << client.getMsg() << "]" << std::endl;
            client.send("Hello there!\n");
        };

        client.disconnected += []
        {
            std::cout << "Client disconnected" << std::endl;
        };
    });

    while (true)
        server.select();
}
```

### Connections

The "Connection" family class is an abstraction to client sockets.

* [ATCPConnection](ATCPConnection.md): base class for Connections.

* [TCPConnection](TCPConnection.md): final implementation of a client socket.

* [OpenSSLTCPConnection](OpenSSLTCPConnection.md): final implementation of a client socket over an OpenSSL secure connection.

### Example client code

Here's the code for a simple client that connects to localhost on port 4242, sends messages entered on stdin and prints replies from the server.
You can test this client with "nc -l -p 4242": anything you type in the client should be echoed by nc, and vice versa.

```cpp
#include <iostream>
#include "TCPConnection.hpp"

int main()
{
  // Connect to localhost on port 4242
  putils::TCPConnection client("127.0.0.1", 4242);

  while (true)
  {
    std::string buff;
    std::getline(std::cin, buff);

    // Send input to server
    client.send(buff);

    // Print reply from server
    std::cout << client.receive() << std::endl;
  }
}
```
