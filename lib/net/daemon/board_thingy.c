/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: board_thingy.c,v 1.2 2003/03/21 02:29:17 ceres Exp $
 * $Log: board_thingy.c,v $
 * Revision 1.2  2003/03/21 02:29:17  ceres
 * Modified to use player handler
 *
 * Revision 1.1  1998/01/06 05:17:08  ceres
 * Initial revision
 * 
*/
#include <network.h>
#include <localtime.h>
#include <board.h>
#include <player_handler.h>

inherit SERVER;

string *board_names;
string *creator_board_names;
mapping sockets;

void create() {
  sockets = ([ ]);
  server::create();
  SetSocketType(STREAM);
  SetDestructOnClose(1);
  call_out("setup", 2);
  board_names = ({ "frog", "never wending story", "role_playing",
                   "fightersguild", "witchesguild", "thievesguild",
                   "assassinsguild", "alt.fan.pratchett",
                   "wizardsguild", "flame", "newspaper",
                   "priestsguild" });
  creator_board_names = ({ "commonroom", "mudlib", "development",
                           "liaison", "am", "sur", "ram",
                           "klatch", "applications", "learning" });
} /* create() */

protected void setup() {
  if (eventCreateSocket(5700) < 0)
    if (this_object()) destruct(this_object());
} /* setup() */

void eventRead(int fd, string str) {
   string *bits;
   string *frogs;
   mixed *stuff;
   int i;
   int j;
   int creator;

tell_object(find_player("pinkfish"), "Got: "+str);
   /* Ok, we need to mugwump this apart and figure out what it is */
   frogs = explode(replace_string(str, "\r", ""), "\n");
   creator = PLAYER_HANDLER->test_creator(sockets[fd]);
   for (j = 0; j < sizeof(frogs); j++) {
      bits = explode(frogs[j], " ");
      switch (bits[0]) {
         case "boardnames" :
            if (creator) {
               for (i = 0; i < sizeof(board_names); i++) {
                  eventWrite(fd, "board "+board_names[i]+"\n");
               }
               for (i = 0; i < sizeof(board_names); i++) {
                  eventWrite(fd, "board "+creator_board_names[i]+"\n");
               }
            } else {
               for (i = 0; i < sizeof(board_names); i++) {
                  eventWrite(fd, "board "+board_names[i]+"\n");
               }
            }
            break;
         case "board" :
            if (member_array(bits[1], board_names) != -1 ||
                (creator && member_array(bits[1], board_names) != -1)) {
               stuff = BOARD_HAND->get_subjects(bits[1]);
               for (i = 0; i < sizeof(stuff); i++) {
                  eventWrite(fd, "subject "+stuff[i][B_NUM]+" "+bits[1]+" "+
                                 stuff[i][B_NAME]+" "+stuff[i][B_SUBJECT]+" "+
                                 ctime(stuff[i][B_TIME])[4..9]+"\n");
               }
            }
            break;
         case "message" :
            if (member_array(bits[2], board_names) != -1 &&
                (creator && member_array(bits[1], board_names) != -1)) {
               stuff = BOARD_HAND->get_subjects(bits[2]);
               for (i = 0; i < sizeof(stuff); i++) {
                  if (bits[1] == stuff[i][B_NUM]+"") {
                     eventWrite(fd, "message "+bits[1]+" "+bits[2]+"\n");
                     eventWrite(fd, "Note #"+(i+1)+" by "+stuff[i][B_NAME]+
                                    " posted at "+ctime(stuff[i][B_TIME])+"\n"
                                    "Title: \""+stuff[i][B_SUBJECT]+"\"\n"+
                                    BOARD_HAND->get_message(bits[2], i)+
                                    "\n.\n");
                  }
               }
            }
            break;
         case "quit" :
            eventWrite(fd, 0, 1);
            break;
         case "login" :
            if (PLAYER_HANDLER->test_user(bits[1])) {
               sockets[fd] = bits[1];
               eventWrite(fd, "login Ok\n");
            } else {
               eventWrite(fd, "login Failed\n");
            }
            break;
         case "logout" :
            eventWrite(fd, "logout Ok\n");
            sockets[fd] = "No one";
            break;
      }
   }
} /* eventRead() */

protected void eventNewConnection(int fd) {
   sockets[fd] = "No one";
} /* eventNewConnection() */

protected void eventSocketClosed(int fd) {
   map_delete(sockets, fd);
} /* eventSocketClosed() */
