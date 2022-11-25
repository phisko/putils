# [curl](curl.hpp)

Helper functions to call command-line `curl` from C++ (for lazy bums who don't want to build the curl library).

## download_file

```cpp
void download_file(const std::string & url, const std::string & output_location) noexcept;
```

## http_request

```cpp
std::string http_request(const std::string & base_url, const std::unordered_map<std::string, std::string> & params = {}) noexcept;
```

## build_curl_command

```cpp
std::string build_curl_command(const std::string & base_url, const std::unordered_map<std::string, std::string> & params = {}) noexcept;
```

Returns the generated command-line.