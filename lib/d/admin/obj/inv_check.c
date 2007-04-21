/*
 * Inventory checker made by Funtime to periodically check players inventories
 * 5/5/1994
 * heavily rewritten by Olorin to actually remove the illegal objects
 * Modified so it runs independantly, starting the inv checking process in create() and calling itself out 
 * Terano, 18/1/98
 */

#include <playtesters.h>
#include <move_failures.h>
#define FILENAME "/save/inv_check"

int do_seek();

nosave string to_log;
string *forbidden = ({ });

void save_me() {
  unguarded ((: save_object (FILENAME) :));
}

void dest_me() {
   destruct( this_object() );
   save_me();
} /* dest_me() */


void create() {
  seteuid( "Admin" );
  unguarded ((: restore_object (FILENAME) :));
  do_seek();
} /* create() */

void log_it() {
   if( sizeof( to_log ) > 0 ) {
      log_file( "ILLEGAL_OBJECT",
               "Illegal objects removed %s\n%s\n", 
               ctime( time() ), 
               to_log );
   }
} /* log_it() */

void check_sub_inv( object *things, string owner, object owner_ob ) {
   string *pre_filename, filename, *switcher;
   object *obs, thing;
   object *contents, *fragile;
   string mess = "";
   object bag;
   
   obs = ({ });
   foreach ( thing in things ) {
     if ( thing->query_property( "virtual name" ) ) {
       filename = thing->query_property( "virtual name" );
     } else {      
       pre_filename = explode( file_name( thing ),"#" );
       filename = pre_filename[ 0 ];
     }
      
      switcher = explode( filename, "/" );
      
      if( ( switcher[0]=="w" ) &&
          ( !"/obj/handlers/playtesters"->query_playtester( owner ) ) ) {
         to_log += sprintf("%s in %s, cloned by %s\n",
                           filename, owner,
                           (string)thing->query_cloned_by() );
         obs += ({ thing });   
      }
      
      if ( ( sizeof( switcher ) > 2 ) && 
          ( switcher[ 0 ] == "obj" ) && 
          ( switcher[ 1 ] == "misc" ) ) {
         switch(switcher[2]){
          case "mailer_ob":
            if ( !"/obj/handlers/playtesters"->query_playtester( owner ) &&
                 ( !owner_ob->query_property("allowed mailer")) ) {
               to_log += sprintf("%s in %s, cloned by %s\n",
                                 filename, owner,
                                 (string)thing->query_cloned_by() );
               obs += ({ thing });   
            }
            break;
          case "board_mas":
            to_log += sprintf("%s in %s, cloned by %s\n",
                              filename, owner,
                              (string)thing->query_cloned_by() );
            obs += ({ thing });  
            break;
         }
         
      }
      if (sizeof (switcher) ==4 &&
        switcher [0] == "d" &&
        switcher [1] == "liaison" &&
        switcher [2] == "items") {  
           to_log += sprintf("%s in %s, cloned by %s\n",
                             filename, owner,
                             (string)thing->query_cloned_by() );
           obs += ({ thing });   
      }
      reset_eval_cost();
      if (member_array (filename, forbidden) != -1) {            
            to_log += sprintf("%s in %s, cloned by %s\n",
                              filename, owner,
                              (string)thing->query_cloned_by() );            
            obs += ({ thing });  
            thing->add_property ("no recycling", 1);
            if (function_exists ("query_max_weight", thing)) {
              contents = all_inventory (thing);
            }
      }       
      
   }
   if ( sizeof( obs ) ) {
      tell_object (owner_ob, owner_ob->colour_event ("say", "%^CYAN%^") + 
        "A small official looking dwarf walks up to you and says: In "
        "accordance with paragraph 5, sub-paragraph 7 of the Domestic "
        "& Domesticated Animals (Care & Protection) Act, 1673, I hereby "
        "remove from you: "+ (string)query_multiple_short( obs, "one" ) 
        + ".%^RESET%^");
      
      if (sizeof (contents)) {
        mess += "\nThe dwarf removes all the inventory from the confiscated "
          "items.";
           
        fragile = filter (contents, (: $1->query_property("fragile") :));
        
        if (sizeof (fragile)) {
          mess += "\nHe hands " + query_multiple_short (fragile) + " to "
            "you.\n";          
          contents -= fragile;
          fragile->move (owner_ob);

        }
        if (sizeof (contents)) {
          bag = clone_object ("/obj/baggage");
          bag->set_name ("bag");
          bag->set_short ("brown paper bag");
          bag->add_adjective (({"brown", "paper"}));
          bag->set_weight(5);
          bag->set_max_weight (implode (map (contents, 
            (: $1->query_complete_weight() :)),(: $1 + $2 :)) + 1);
          bag->set_value (0);
          bag->set_long ("This is a plain, brown, paper bag... the kind grubby "
            "old men are often found in possession of after leaving their "
            "local newsagent.\n");
          contents->move (bag);                
          if (bag->move (owner_ob) == MOVE_TOO_HEAVY) {
            mess +=  "He sticks " + query_multiple_short (contents) + " in a "
              "brown paper bag and leaves it on the ground.";
          }
          else {
            mess +=  "He sticks " + query_multiple_short (contents) + " in a "
              "brown paper bag and hands it to you.";
          }            
        }        
      }
            
      mess += "\nHe scribbles something on a clipboard, then walks away.\n";      
      tell_object (owner_ob, mess);
      obs->move( "/room/rubbish" );
      call_out( "check_obs", 7, obs ); /* dest them soonest */
   } 
} /* check_sub_inv() */

void check_obs( object* obs ) {
   object thing;
   foreach( thing in obs ) {
      if( thing ) {
   thing->dest_me();
      }
   }
} /* check_obs() */

void check_inv( object player ) {
   object *things;
   
   things = deep_inventory( player );
   check_sub_inv( things, (string)player->query_name(), player );
} /* check_inv() */

int do_seek() {
  object user;
  
  to_log = "";
  foreach ( user in users() ) {
    
    if( user && !user->query_property( "guest" ) &&
       !user->query_creator() &&
       !PLAYTESTER_HAND->query_tester (user) &&
       !user->query_property ("test character") ) {
      check_inv( user );
    }
  }
  log_it();
 
  call_out( "do_seek", 300 ); //Every 5 minutes
  return 1;
} /* do_seek() */
 
int query_theft_command() { return -1; }

string parse_filename (string str) {

  if (strsrch (str, ".wep") != -1 ||
      strsrch (str, ".clo") != -1 ||
      strsrch (str, ".ob") != -1 ||
      strsrch (str, ".arm") != -1 ||
      strsrch (str, ".food") != -1 ||
      strsrch (str, ".sca") != -1) {
        if (file_size(str) <= 0) {
          return 0;
        }
        else {
          return str;
        }
  }
  else {
    str = explode (str, ".c")[0];
    
    if (file_size(str + ".c") <= 0 ) {
      return 0;
    }
    else {
      return str;
    }
  }
}
                       
int ban_object(string filename) {
  
  if (!master()->query_lord(previous_object(-1))) {
    printf ("Please get a lord to ban your object for you.\n");
    return 0;
  }    
  
  filename = parse_filename (filename);
  
  if (filename == 0) {
    printf ("No file with that filename.\n", filename);
    return 0;
  }
  else {
    if (member_array (filename, forbidden) == -1) {
      forbidden += ({ filename });
      printf ("%s added to the list of banned objects.\n", filename);
      save_me();
    }
    else {
      printf ("%s already banned.\n", filename);
    }
  }
  return 1;
}

int unban_object(string filename) {
  
  if (!master()->query_lord(previous_object(-1))) {
    printf ("Please get a lord to unban your object for you.\n");
    return 0;
  }    
        
  filename = parse_filename (filename);
  
  if (filename == 0) {
    printf ("No file with that filename.\n", filename);
    return 0;
  }
  else {
    if (member_array (filename, forbidden) != -1) {
      forbidden -= ({ filename });
      printf ("%s removed from the list of banned objects.\n", filename);
      save_me();
    }
    else {
      printf ("%s not currently banned.\n", filename);
    }
  }
  return 1;
}

string list_banned() {
  if (sizeof (forbidden)) {    
    return sprintf ("Objects cloned from the following filenames "
      "will be confiscated by the dwarves:\n%s\n", 
      implode (forbidden, "\n"));
  }
  else {
    return "There are no objects currently forbidden.\n";
  }    
}


mixed stats()
{
  return ({
    ({ "Time until next tick", query_num (find_call_out ("do_seek")) 
      + " seconds"}),
  });
}    