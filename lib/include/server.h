#ifndef __SERVER_H__
#define __SERVER_H__

#define STRING 0
#define FILE 1

#define BLOCK_SIZE 65536

class buff {
    int Type;
    int Size;
    int Pos;
    string Filename;
    mixed Value;
}

class server {
    int Descriptor;
    int Blocking;
    int Closing;
    class buff *Buffer;
}

int eventCreateSocket(int port);
protected void eventServerListenCallback(int fd);
protected void eventServerAbortCallback(int fd);
protected void eventServerReadCallback(int fd, mixed val);
protected void eventRead(int fd, mixed val);
protected void eventServerWriteCallback(int fd);
varargs void eventWrite(int fd, mixed val, int close);
protected void eventClose(class server sock);
protected void eventSocketClosed(int fd);
int eventDestruct();
protected void eventNewConnection(int fd);
protected void eventSocketError(string str, int x);
function SetRead(function f);
int SetDestructOnClose(int x);

#endif /* __SERVER_H__ */
