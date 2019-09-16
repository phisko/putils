# OpenSSLTCPListener

A final implementation of a listening socket over an OpenSSL secure connection.

### Members

##### Constructor

```cpp
OpenSSLTCPListener(short port, const std::string &host = "127.0.0.1",
                   const std::string &certificate, const std::string &key);
```

If no `certificate` or `key` is provided, these will be automatically searched for in a "certificates" directory at run-time.

