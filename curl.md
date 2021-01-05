# [curl](curl.hpp)

Helper functions to call command-line `curl` from C++ (for lazy bums who don't want to build the curl library).

## downloadFile

```cpp
void downloadFile(const std::string & url, const std::string & outputLocation) noexcept;
```

## httpRequest

```cpp
std::string httpRequest(const std::string & baseURL, const std::unordered_map<std::string, std::string> & params = {}) noexcept;
```

## buildCurlCommand

```cpp
std::string buildCurlCommand(const std::string & baseURL, const std::unordered_map<std::string, std::string> & params = {}) noexcept;
```

Returns the generated command-line.