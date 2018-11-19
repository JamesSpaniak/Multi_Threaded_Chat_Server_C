# Multi_Threaded_Chat_Server_C
Socket Programming in C - Multi Threaded

Author: James Spaniak 2018

Chat room built in C to demonstrate sockets. There is currently a client and a server side that is connectable via any valid port, with a variety of additional commands.

Start server with available port.
Connect clients to server and chat using messaging and commands available.

/me
/exit
/nick

To compile and run program:
./gcc server.c -o server
./gcc client.c -o client
./server 8080
./client localhost 8080

Enjoy!