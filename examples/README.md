# libsockit

**Please note that this is in the very early stages of development.** 

## Compile example binary

```
make
```

## bin/sockit examples

Start a TCP server on port 80

```
cd examples && make && bin/sockit -s server -t tcp -p 80
```

Start a UDP server on port 53

```
cd examples && make && bin/sockit -s server -t udp -p 53
```

Start a TCP client on port 80

```
cd examples && make && bin/sockit -s client -t tcp -p 80
```

Start a TCP client on port 80

```
cd examples && make && bin/sockit -s client -t tcp -p 80
```
