# Streamfiler project
![C/C++ CI](https://github.com/dRacz3/streamfiler/workflows/C/C++%20CI/badge.svg)
#### NAME
streamfiler - Receives TCP/IP stream and writes it to disk.

#### SYNOPSIS
```bash
streamfiler [-options] portnumber
```

#### DESCRIPTION
Streamfiler is an application that is listening on a specified port. It expects a stream of data from a connected client and will writes it to the specified location. The created file's name equals to the connections setup timestamp with millisec precision. Can handle multiple connections based on parametrisation.

#### OPTIONS
-c connections  
`Maximum number of parralel connections.`
    

-f folder  
`Folder, where the content of the TCP stream is written. Will be created if does not exist.`

-h  
`Display this help message`

-l limit  
`overall bandwith limitation in kb/s.`

-t timeout  
`Timeout in seconds. Idle connections after this time will be closed. If set to -1, connections will not be timed out.`



## Dependencies
- cmake (>3.17)
- c++17 (for filesystem )
- Linux system headers ( for low level socket operations)

## Setup:
```bash
mkdir build && cd build
cmake ..
make
```

Or, as an alternative you can just run the 
`./build.sh`


The project will build the following targets:
- streamfiler (main application)
- streamfilerclient (manual testing application which will stream a rather long message to the application on port 2222)
- tests (unit tests, not too many for now.)
