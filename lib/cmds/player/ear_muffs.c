/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ear_muffs.c,v 1.17 2003/06/05 00:37:41 pinkfish Exp $
 *
 * Command to allow the earmuffing of channels that a person doesn't wish to
 * hear.
 *
 */
#define TP this_player()
#include <player.h>
#include <newbiehelpers.h>
inherit "/cmds/base";

private string *_normal_types;
private string *_cre_types;
private string _player_format;

void create() {
   ::create();
   _normal_types = ({ "shout", "newbie", "cryer", "remote-soul",
         "multiple-soul", "multiple-tell",  "teach",
         "tell", "remote", "multiple-remote" });
   _cre_types = ({ "cre", "lord", "code", "liaison",
         "intermud-all", "intercre", "intergossip",
         "dwcre", "dwchat", "remote-spam", "newbiehelpers",
                     "verbose-logon"});
   _player_format = "{" + implode(_normal_types, "|") + "}";
} /* create() */

int check_earmuffs() {
  string *on;
  string *types;

  on = TP->query_property(PLAYER_EARMUFF_PROP);
  if (!on) {
    on = ({ });
  }

  types = _normal_types;
  if (TP->query_creator()) {
    types += _cre_types + TP->channel_list();
  } else if (NEWBIEHELPERS_HANDLER->query_can_chat(TP)) {
    types += ({"newbiehelpers"});
  }

  if (TP->query_earmuffs()) {
    if (TP->query_earmuffs() == PLAYER_ALLOW_FRIENDS_EARMUFF) {
      printf("Your earmuffs are set to allow friends through.\n");
    } else {
      printf("Your earmuffs are on.\n");
    }
  } else { 
    printf("Your earmuffs are off.\n");
  }
  if (!sizeof(on)) {
    printf("You have nothing set to be earmuffed.\n");
    write("$I$5=Earmuffable events are "+ query_multiple_short(types)+
          ".\n");
  } else {
    write("$I$5=You have "+ query_multiple_short(on)+ " earmuffed.\n");
    types -= on;
    if (sizeof(types)) {
      write("$I$5=You are not currently earmuffing "+
            query_multiple_short(types)+ " events.\n");
    }
  }
  return 1;
} /* check_earmuffs() */

int do_earmuff_control(string str) {
  string *types;
  string *on;

  types = _normal_types;
  if (TP->query_creator()) {
    types += _cre_types + TP->channel_list();
  } else if (NEWBIEHELPERS_HANDLER->query_can_chat(TP)) {
    types += ({"newbiehelpers"});
  }

  on = TP->query_property(PLAYER_EARMUFF_PROP);
  if (!on) {
    on = ({ });
  }

  switch (str) {
    case "on" :
      if (!TP->query_earmuffs()) {
        TP->toggle_earmuffs();
      } else if (TP->query_allow_friends_earmuffs()) {
        TP->toggle_earmuffs();
        TP->toggle_earmuffs();
      }
      write("Earmuffs turned on.\n");
      break;
    case "off" :
      if (TP->query_earmuffs()) {
        TP->toggle_earmuffs();
      }
      write("Earmuffs turned off.\n");
      break;
    case "allowfriends" :
      TP->set_allow_friends_earmuffs();
      write("Earmuffs set to allow friends through.\n");
      break;
    case "all" :
      on = types;
      if (sizeof(on)) {
        write("$I$5=Your "+ query_multiple_short(on)+
              " events will now be earmuffed.\n");
      }
      break;
    case "none" :
      write("Nothing will be earmuffed any more.  (No news is good news?)\n");
      on = ({ });
      break;
  }

  TP->add_property(PLAYER_EARMUFF_PROP, on);

  return 1;
} /* do_earmuff_control() */

int do_earmuff(string str, string onOff) {
  string *types, *on, *off;

  types = _normal_types;
  if (TP->query_creator()) {
    types += _cre_types;
  }

  on = (string *)TP->query_property(PLAYER_EARMUFF_PROP);
  if (!on) {
    on = ({ });
  }

  off = ({ });
  switch (onOff) {
    case "on" :
      if (member_array(str, on) == -1) {
        on += ({ str });
      }
      write("You are now earmuffing " + str + " events.\n");
      break;
    case "off" :
      off += ({ str });
      write("You are not earmuffing " + str + " events anymore.\n");
      break;
    default :
      if (member_array(str, on) == -1) {
        on += ({ str });
        write("You are now earmuffing " + str + " events.\n");
      } else {
        off += ({ str });
        write("You are not earmuffing " + str + " events anymore.\n");
      }
      break;
  }
  on -= off;
  TP->add_property(PLAYER_EARMUFF_PROP, on);
  return 1;
} /* do_earmuff() */

mixed *query_patterns() {
  string cre_format;
  
  if (this_player()->query_creator()) {
    cre_format = "{" + implode(_normal_types +
                     _cre_types + this_player()->channel_list(), "|") + "}";
    
    return ({ "", (: check_earmuffs() :),
                "{on|off|all|none|allowfriends}",
                (: do_earmuff_control($4[0]) :),
                cre_format, (: do_earmuff($4[0], "") :),
                cre_format + " {on|off}", (: do_earmuff($4[0], $4[1]) :) });
  } else {
    if( NEWBIEHELPERS_HANDLER->query_can_chat(this_player()) ) {
      _player_format = "{" +
                       implode(_normal_types + ({"newbiehelpers"}), "|") + "}";
    }
    return ({ "", (: check_earmuffs() :),
                "{on|off|all|none|allowfriends}",
                (: do_earmuff_control($4[0]) :),
                _player_format, (: do_earmuff($4[0], "") :) ,
                _player_format + " {on|off}",
                (: do_earmuff($4[0], $4[1]) :) });
  }
}
