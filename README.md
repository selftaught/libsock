# libsockit

**Please note that this is in the very early stages of development.** 

## API

### Constructors
```
Socket(const uint16_t& port, int protocol = 0);
```
```
Socket(const std::string& hostname, const uint16_t& port, int protocol = 0);
```
```
Socket(const std::string& hostname, const std::string& port, int protcol = 0);
```

### Functions

```
void init();
```
```
void disconnect();
```
```
void connect();
```
```
bool ready(const uint32_t& events);
```
```
bool set_blocking(int file_descriptor, bool blocking = false);
```
```
std::string receive();
``` 
```
ssize_t send(const std::string& message, bool OOB = false);
```
```
void set_hostname(const std::string& hostname);
```
```
void set_port(const uint16_t& port);
```
```
void set_port(const std::string& port);
```
```
void set_address_family(const int& addrf);
```
```
void set_backlog(const uint32_t& backlog_size);
```
```
void set_protocol(const int& protocol);
```
```
pollfd* pfd();
```

