#ifndef __OOB_SERVER_H
#define __OOB_SERVER_H

#define TIMEOUT 600

class connection {
  string MudName;
  mixed Buffer;
}

void eventTimeout(int fd);
protected void eventSocketClosed(int fd);
protected void eventRead(int fd, mixed *packet);

#endif /* __OOB_SERVER_H */
