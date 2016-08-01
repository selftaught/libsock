# libsockit

**Please note that this is in the very early stages of development.** 

## API

### Constructors
```c++
Socket(const uint16_t& port, int protocol = 0);
```
```c++
Socket(const std::string& hostname, const uint16_t& port, int protocol = 0);
```
```c++
Socket(const std::string& hostname, const std::string& port, int protcol = 0);
```

### Functions

```c++
void init();
```
```c++
void disconnect();
```
```c++
void connect();
```
```c++
bool ready(const uint32_t& events);
```
```c++
bool set_blocking(int file_descriptor, bool blocking = false);
```
```c++
std::string receive();
``` 
```c++
ssize_t send(const std::string& message, bool OOB = false);
```
```c++
void set_hostname(const std::string& hostname);
```
```c++
void set_port(const uint16_t& port);
```
```c++
void set_port(const std::string& port);
```
```c++
void set_address_family(const int& addrf);
```
```c++
void set_backlog(const uint32_t& backlog_size);
```
```c++
void set_protocol(const int& protocol);
```
```c++
pollfd* pfd();
```

