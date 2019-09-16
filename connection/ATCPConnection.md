# [ATCPConnection](ATCPConnection.hpp)

Base class for the Connection classes.

### Members

##### send

```cpp
void send(const T &data);
```

Send `data` to the server. Specialized for `std::string` to send the underlying c_str.

##### receive

```cpp
std::string receive();
void receive(T &dest);
```

Read a string from the server, or a `T` into `dest`.

### Virtual members

The following functions are pure virtual and are all that needs to be redefined by inheriting classes.

##### `doRead(int fd, char *dest, size_t length)`
Perform a read operation, reading up to `length` bytes into `dest`

##### `doWrite(int fd, const char *data, size_t length)`
Perform a write operation, writing `length` bytes from `data`

