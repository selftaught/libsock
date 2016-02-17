# libsockit

libsockit is an open source socket library developed in C++, which makes writing portable socket code easy. 

## License

## Installation

## Examples

## API

### Socket classes

* SocketBase
* TcpSocket
* UdpSocket

### Exception handling

There is only one exception class that get's thrown which is `SocketException`. The class is derived from std::exception and has a `what()` member function which returns a const char* that provides a message about why the exception was thrown.