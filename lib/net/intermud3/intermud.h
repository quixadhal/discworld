#ifndef __INTERMUD_H__
#define __INTERMUD_H__

#define SAVE_INTERMUD "/net/save/intermud"

class list {
    int ID;
    mapping List;
}

protected void create();
protected void Setup();
protected void eventRead(int fd, mixed *packet);
protected void eventRequestMudList();
protected void eventRequestChannelList();
protected void eventSocketClose(int fd);
protected void eventConnectionFailure();
int SetDestructOnClose(int x);
int SetSocketType(int x);
string GetMudName(string mud);
mapping GetMudList();
string *GetMuds();
mapping GetChannelList();
string *GetMatch(string mud);
string GetNameserver();

#endif /* __INTERMUD_H__ */
