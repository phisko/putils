# [OpenSSLTCPConnection](OpenSSLTCPConnection.hpp)

A final implementation of a client socket over an OpenSSL secure connection.

### Members

##### Constructor

```cpp
OpenSSLTCPConnection(const std::string &host, short port,
                     const std::string &certificate, const std::string &key);
```

If no `certificate` or `key` is provided, these will be automatically searched for in a "certificates" directory at run-time.

