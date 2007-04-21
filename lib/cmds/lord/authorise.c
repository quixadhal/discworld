/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: authorise.c,v 1.8 2003/04/15 19:33:32 ceres Exp $
 *
 *
 */

/*
 * $Locker:  $
 * $Id: authorise.c,v 1.8 2003/04/15 19:33:32 ceres Exp $
 * $Log: authorise.c,v $
 * Revision 1.8  2003/04/15 19:33:32  ceres
 * Added cache cleanout
 *
 * Revision 1.7  2003/03/21 02:01:31  ceres
 * Modified to use player handler
 *
 * Revision 1.6  2002/01/11 22:25:33  drakkos
 * Added compression handling.
 *
 * Revision 1.5  2001/03/12 19:24:07  ceres
 * Added support for authorising players while online
 *
 * Revision 1.4  1999/07/10 19:33:34  ceres
 * Modified to store the email address of the person this layer was authorised for.
 *
 * Revision 1.3  1999/06/28 17:08:46  ceres
 * Modified to set last_log_on since otherwise they expire immediately
 *
 * Revision 1.2  1999/03/04 08:17:57  pinkfish
 * Added in some stuff to create the player file if it does not exists.
 *
 * Revision 1.1  1999/03/03 06:08:14  ceres
 * Initial revision
 *
 * Revision 1.1  1998/01/06 05:29:21  ceres
 * Initial revision
 * 
*/
#include <parser.h>
#include <player_handler.h>

inherit "/cmds/base";

nosave string var_name = "map_prop ";
nosave string fname;
mapping map_prop;
mapping player_info;
string name;
string password;
string email;
int last_log_on;
string basename;

/*
 * This will add a property to a player.
 */
mixed cmd(string player) {
  if(find_player(player)) {
    find_player(player)->add_property("authorised player", 1);
    return 1;
  }
  
  if (PLAYER_HANDLER->test_user(player)) {
    if("/cmds/lord/playera_ddproperty"->cmd(player, "authorised player","1")) {
      PLAYER_HANDLER->remove_cache_entry(player);
      return 1;
    }
    return 0;
    
  }
  write(capitalize(player) + " does not exists.  Do you wish to create them? ");
  input_to("really_create", 0, player);
  add_succeeded_mess("");
  return 1;
}

void really_create(string response, string player) {
   response = lower_case(response);
   if (response[0] != 'y') {
      write("Aborting create.\n");
      return ;
   }
   write("Ok, creating player " + capitalize(player) + ".\n");
   write("What is their authorised email address: ");
   input_to("get_email", 0, player);
}
void get_email(string str, string player) {
  email = str;
  write("What password do you wish to use: ");
  input_to("get_password", 0, player);
} /* really_create() */

void get_password(string passw, string player) {
   string save_file_name;

   save_file_name = PLAYER_HANDLER->query_player_file_name(player);
   map_prop = ([ "authorised player" : 1,
                 "new player" : 1,
                 "player" : 1,
                 "authorised email" : email, ]);
   player_info = ([ "email" : ":"+email, ]);
   last_log_on = time();
   name = player;
   password = crypt(passw, 0);
   save_object(save_file_name);
   write("Create player " + capitalize(player) + " with authorisation to login "
         "from nologin sites.\n");
} /* get_password() */

int enable(string player, string address) {
  string *ips, file, *bits;
  int i;
  
  if(!PLAYER_HANDLER->test_user(player))
    return notify_fail("No such user " + player + ".\n");

  /* Restore them... */
  basename = "/save/players/" + player[0..0] + "/" + player;
  unguarded ( (: uncompress_file (basename + ".o.gz") :));    
  file = unguarded( (: read_file(basename + ".o") :));
  bits = explode(file, "\n");
  
  for (i = 0; i < sizeof(bits); i++) {
    if (bits[i][0..strlen("player_info")-1] == "player_info") {
      player_info = restore_variable(bits[i][strlen("player_info") + 1..]);
      break;
    }
  }

  ips = player_info["allowed_ips"];
  if(!ips)
    ips = ({ });
  
  if ( member_array(address, ips ) == -1 )  {
    ips += ({ address });
    player_info["allowed_ips"] = ips;

    if (i < sizeof(bits)) {
      bits[i] = "player_info " + save_variable(player_info);
    } else {
      bits += ({ "player_info " + save_variable(player_info) });
    }
    unguarded( (: rm(basename + ".old") :) );
    unguarded( (: rename(basename+".o", basename+".old") :) );
    unguarded( (: write_file(basename + ".o", implode($(bits), "\n") + "\n") :));
    unguarded ( (: compress_file (basename + ".o") :));     
  } else  {
    write( player + " is already allowed to login from  " + address + ".\n" );
  }
   
  write(player + " is now allowed to login from "+address+".\n");
  return 1;
}

int disable(string player, string address) {
  string *ips, file, *bits;
  int i;
  
  if(!PLAYER_HANDLER->test_user(player))
    return notify_fail("No such user " + player + ".\n");
  
  /* Restore them... */
  basename = "/save/players/" + player[0..0] + "/" + player;
  unguarded ( (: uncompress_file (basename + ".o.gz") :));     
  file = unguarded( (: read_file(basename + ".o") :));
  bits = explode(file, "\n");
  
  for (i = 0; i < sizeof(bits); i++) {
    if (bits[i][0..strlen("player_info")-1] == "player_info") {
      player_info = restore_variable(bits[i][strlen("player_info") + 1..]);
      break;
    }
  }

  ips = player_info["allowed_ips"];
  if(!ips)
    ips = ({ });
  
  if ( member_array(address, ips ) != -1 )  {
    ips -= ({ address });
    player_info["allowed_ips"] = ips;

    if (i < sizeof(bits)) {
      bits[i] = "player_info " + save_variable(player_info);
    } else {
      bits += ({ "player_info " + save_variable(player_info) });
    }
    unguarded( (: rm(basename + ".old") :) );
    unguarded( (: rename(basename+".o", basename+".old") :) );
    unguarded( (: write_file(basename + ".o", implode($(bits), "\n") + "\n") :));
    unguarded ( (: compress_file (basename + ".o") :));     
  } else  {
    write( player + " is not already allowed to login from  " +address+ ".\n");
  }
  
  write(player + " is now not allowed to login from "+address+".\n");
  return 1;
}

mixed *query_patterns() {
  return ({ "enable <word'player'> <string'address'>",
              (: enable($4[0], $4[1]) :),
              "disable <word'player'> <string'address'>",
              (: disable($4[0], $4[1]) :),
              "<word'player'>", (: cmd($4[0]) :), });
} /* query_patterns() */
