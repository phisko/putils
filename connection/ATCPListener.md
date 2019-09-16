# [ATCPListener](ATCPListener.hpp)

### Members

Basic interface for the Listener classes. It inherits from `putils::Observable` and will notify its observers whenever a new client connects. Observers can then get to the new client by using `getLastClient()`.

##### select

```cpp
void select(timeval &&timeout = {-1, -1});
```

Wait for new connections and/or new messages from existing clients. Observers will be notified if one of these happens.
If no timeout is specified, the function will block until something happens.

### Virtual members

The following functions are pure virtual and are all that needs to be redefined by inheriting classes.

##### doRemove

```cpp
virtual void doRemove(int fd) = 0;
```

Perform whatever should happen when removing a client (such as freeing the SSL* pointer associated with it in the case of OpenSSL).

##### doAccept

```cpp
virtual bool doAccept(int fd) = 0;
```

Perform whatever should happen when accepting a new client (such as SSL_accept in the case of OpenSSL)

##### doRead

```cpp
virtual int doRead(int fd, char *dest, size_t length) = 0;
```

Perform a read operation, reading up to `length` bytes into `dest`

##### doWrite

```cpp
virtual int doWrite(int fd, const char *data, size_t length) = 0;
```

Perform a write operation, writing `length` bytes from `data`

### Client structure

Clients contain two `putils::Observable`:

* `newMessage`: notifies observers whenever a new message arrives
* `disconnected`: notifies observers when the client disconnects

The following functions let users access received message and send data to the client:

##### getBuff
```cpp
T getBuff() const;
```
Recovers the last message from the client as a `T` (for binary protocols).
```cpp
std::string getBuff() const;
std::string getMsg() const;
```
Recovers the last message from the client as an `std::string` (for text protocols).

##### send
```cpp
void send(T &&data);
```
Sends data to the client as a `T` (specialized for std::string to send the underlying c_str).

##### receive
```cpp
T receive(bool selectAlreadyRunning = false, timeval timeout = { -1, -1 })
```
Blocks until a new message is received, or until `timeout` is reached. If `select` is already being called from another thread, `selectAlreadyRunning` should be set to `true`.

##### disconnect
```cpp
void disconnect() const;
```
Disconnects the client

