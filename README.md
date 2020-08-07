# CPP UDP SOCKET
Is a basic client sending UDP datagrams at certain IP/PORT and a server create two threads one for the socket (receiving datagrams) and other for the user interruption (listening standard input)

## Makefile
### Server
Compile link and run 
```bash
$ make 
```

Run previous executable
```bash
$ make run
```

Erase previous executable
```bash
$ make clean
```

Erase previous executable and compiled files
```bash
$ make hardclean 
```

### Client
Compile link and run 
```bash
$ make client
```

# Test UDP connection from terminal 
nc -z -v -u 192.168.100.12 20777
