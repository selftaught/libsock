# libsockit

libsockit is an open source cross-platform C++ socket library 

## License

## Installation

## Examples

## API

### Socket classes

* `SocketBase`
* `TcpSocket`
* `UdpSocket`

#### SocketBase

#### TcpSocket

#### UdpSocket

Constructors

Functions


* `void connect()`
* `void disconnect()`
* `void send(const std::string& message)`
* `std::string receive()`
* `void set_hostname(const std::string& hostname)`
* `void set_port(const uint16_6& port)`
* `void set_port(const std::string& port)`
* `void set_address_family(const int& address_family)`
* `void set_type(const int& type)`
* `void set_backlog(const uint32_t& backlog)`


Member variables

### Exception handling

There is only one exception class that get's thrown and that is `SocketException`. The class is derived from `std::exception` and has a `what()` member function (like you'd expect) that provides some relatively specific information about why the exception was thrown.

