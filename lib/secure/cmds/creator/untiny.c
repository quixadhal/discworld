/*  -*- LPC -*-  */

/*
 * An interface to the untiny perl script that
 * URLbot uses to get information about URL's
 * to post to the url I3 channel.
 */

inherit "/cmds/base";
#include <creator.h>
#include <network.h>

mapping globals = ([]), files = ([]), ret = ([]);

#define CMD_NUM 1
#define TP globals[fd]
#define RET ret[fd]
#define CMDS cmds[this_player()]

mixed cmd(string arg) {
    int fd;
    string *bits;

    if(!arg) {
        return notify_fail("You must provide a URL!");
    } else {
        // Arguments to untiny are format, url, channel, speaker -- format should be "wiley" to get pinkfish codes.
        bits = ({ "wiley", arg, "intergossip", "quixadhal" });
        fd = external_start(CMD_NUM, bits, "read_call_back", "write_call_back", "close_call_back");
    }
    if(fd < 0)
        return notify_fail("untiny failed somehow.\n");

    TP = this_player();
    RET = "";
    // tell_object(TP, sprintf("Spawning untiny %s on descriptor %d\n", implode(bits, " "), fd));

    return 1;
}

void read_call_back(int fd, mixed mess) {
    RET += mess;
}

void write_call_back(int fd) {
    tell_object(TP, "untiny: The write_call_back() was called, somehow...\n");
}

void close_call_back(int fd) {
    if(RET != "")
        //tell_object(TP, RET);
        SERVICES_D->eventSendChannel("URLbot", "url", RET);
    else
        tell_object(TP, "untiny: no result.\n");

    map_delete(ret, fd);
    map_delete(globals, fd);
}
