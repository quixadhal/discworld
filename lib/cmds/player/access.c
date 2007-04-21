/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: access.c,v 1.3 2003/04/30 19:59:22 ceres Exp $
 * 
 */
#include <drinks.h>
#include <language.h>
#include <player.h>
#include <player_handler.h>

#define TP this_player()

inherit "/cmds/base";

int cmd_list() {
   string *ips;

   ips = TP->query_rhosts();
   if(!ips) {
      ips = ({ });
   }
   if ( !sizeof(ips) )  {
     write( "You are allowed to login from anywhere.\n" );
   } else  {
     ips = sort_array(ips, 1);
     write( "You are currently allowed to login from: " +
            query_multiple_short(ips) + ".\n" );
   }
   return 1;
} /* cmd_list() */

int enable( string str) {
   string *ips;

   if(TP->query_property("access_restricted"))
     return notify_fail("Sorry, you are not allowed to change your "
                        "access list.\n");
   
   ips = TP->query_rhosts();
   if(!ips) {
      ips = ({ });
   }

   if(member_array(str, ips) == -1 )  {
     ips += ({ str });
     TP->set_rhosts(ips);
   } else  {
     write( "You are already allowed to login from  " + str + ".\n" );
   }
   
   write("You are now allowed to login from "+str+".\n");
   return 1;
}


int disable( string str) {
   string *ips;
 
   if(TP->query_property("access_restricted"))
     return notify_fail("Sorry, you are not allowed to change your "
                        "access list.\n");

   ips = TP->query_rhosts();
   if(!ips) {
      ips = ({ });
   }

   if ( member_array( str, ips ) != -1 )  {
     ips -= ({ str });
     if(!sizeof(ips))
       TP->set_rhosts(0);
     else
       TP->set_rhosts(ips);
   } else  {
     write( "You are already not allowed to login from  " + str + ".\n" );
   }
   if(!sizeof(ips))
     write("You are now allowed to login from anywhere.\n");
   else
     write("You are not now allowed to login from "+str+".\n");
   
   return 1;
}

int authorise_for (string name, string address) {
  string fname;
  string file,  *bits;
  string var_name = "player_info ";
  mapping info = ([ ]);
  int i, found = 0;

  if (!this_player()->query_lord()) {
    return notify_fail ("You may not do this.\n");
  }
    
  if (find_player (name)) {
    return notify_fail ("This player is online.\n");
  }
    
  if (!PLAYER_HANDLER->test_user(name)) {
    return notify_fail ("No such player.\n");
  }

  seteuid("Root");
   
  fname = "/save/players/" + name[0..0] + "/" + name;       
  
  uncompress_file (fname+ ".o.gz");
  file = unguarded( (: read_file($(fname)+".o") :));

  if (!file) {
    return notify_fail ("Error reading file.\n");
  }
  
   bits = explode(file, "\n");

   for (i = 0; i < sizeof(bits); i++) {
     if (bits[i][0..strlen(var_name)-1] == var_name) {
       info = restore_variable(bits[i][strlen(var_name)..]);
       found = 1;
       break;
     }
   }
  
  if(!found) {
    return notify_fail ("Cannot find mapping.\n");
  }

  if (address == "reset") {
    info["allowed_ips"] = ({ });
  }
  else if (sizeof (info ["allowed_ips"])) {
    if (member_array (address, info["allowed_ips"]) == -1) {
      info["allowed_ips"] += ({ address });
    }
    
  }
  else {
    info["allowed_ips"] = ({ address });
  }
  
  if (i < sizeof(bits)) {
     bits[i] = var_name + save_variable(info);
   } else {
     bits += ({ var_name + save_variable(info) });
  }

         
  unguarded( (: rm($(fname) + ".old") :) );
   
  unguarded( (: rename($(fname)+".o", $(fname)+".old") :) );
   
  unguarded( (: write_file($(fname) + ".o",
    implode($(bits), "\n") + "\n") :));
  compress_file (fname + ".o");
  unguarded( (: rm($(fname) + ".o") :) );

  PLAYER_HANDLER->remove_cache_entry(name);
  
  tell_object (this_player(), "Access for " + name + " changed.\n");   
  return 1;

  
}

mixed *query_patterns() {
  return ({ "enable <string'address'>", (: enable($4[0]) :),
              "disable <string'address'>", (: disable($4[0]) :),
              "for <string'name'> <string'address'>", 
                (: authorise_for ($4[0], $4[1]) :), 
              "reset for <string'name'>", (: authorise_for ($4[0], 
              "reset") :),
              "", (: cmd_list() :) });
} /* query_patterns() */
