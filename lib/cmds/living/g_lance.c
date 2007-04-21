/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: glance.c,v 1.15 2003/01/30 08:29:01 pinkfish Exp $
 *
*/
/* glance command, trial out by Turrican for a commands daemon. */
/* Modified by Ceres to add glancing into adjacent rooms */
/* Oh I did some performance enhancements too */
#include <obj_parser.h>
#include <dirs.h>
#include <player.h>

inherit "/cmds/base";

int check_exists(string file);
string room_glance(object person, object room, int dark);
mixed cmd_direction(string dir);

int check_pre_stuff() {
   int dark;
   object room;

   room = environment(previous_object());
   if (!room) {
      return notify_fail("You are in limbo... sorry you can't look at "+
             "anything.\n");
   }

   if (!previous_object()->query_property("dead")) {
      dark = (int)previous_object()->check_dark((int)room->query_light());
   }

   if( dark < 0 ) {
      write("It's too dark to look at anything.\n");
      return 1;
   } else if( dark > 0 ) {
      write("It's too bright to look at anything.\n");
      return 1;
   }
   return 2;
} /* check_pre_stuff() */

mixed cmd_ob(object* obs) {
   object thing;
   int retval;
   int dark;
   object room;

   retval = check_pre_stuff();
   if (retval != 2) {
      return retval;
   }

   room = environment(previous_object());
   dark = (int)previous_object()->check_dark((int)room->query_light());
   if (room->query_mirror_room()) {
      room = room->query_mirror_room();
   }

   foreach (thing in obs) {
      string tmp;
      if(thing == this_player()) {
        write("That's you that is.\n");
      } else {
        tmp = (string)thing->a_short( dark );
        if (tmp) {
          write(capitalize(tmp) + ".\n");
        }
      }
   }
   return 1;
} /* cmd_ob() */

mixed cmd_direction(string dir, int from_string) {
   object room;
   int dark;
   int retval;
   string ret;
   string dest;
   mixed *func;

   retval = check_pre_stuff();
   if (retval != 2) {
      return retval;
   }
   retval = 0;

   room = environment(previous_object());
   dark = (int)previous_object()->check_dark((int)room->query_light());
   if (room->query_mirror_room()) {
      room = room->query_mirror_room();
   }
   dir = EXPAND_EXIT(dir);

   if ( room->query_relative( dir ) ) {
      add_failed_mess( "You do not think that the "+ dir +" is here.\n" );
      return 0;
   }

   dir = (string)this_player()->find_abs(dir);

   if(room->query_exit(dir)) {
    // if there is an exit with the name that they looked at then see if they
    // can see into the next room.

      room->set_destination( dir );

      if ( check_exists( (string)room->query_destination( dir ) ) ) {
         write("You glance " + dir + " and see:\n");
         ret = room->query_look(dir);
         if (ret) {
            write(ret+"\n");
            return 1;
         }

         func = room->query_look_func(dir);
         if(sizeof(func)) {
            call_other(func[0], func[1], 1);
            return 1;
         }

         dest = room->query_destination(dir);
         if(room->query_door_open(dir) != 0) {
            // it mustn't be dark in either room.
            if (!dark && !previous_object()->query_property("dead") &&
                !previous_object()->query_creator()) {
               dark = previous_object()->check_dark(dest->query_light());
            }

            load_object(dest);
            this_player()->set_looked( find_object( dest ) );

            if (dark) {
               write("It's too dark to see that far.\n");
            } else  {
               write( (string)dest->a_short( dark ) +" "+
                     (string)dest->query_short_exit_string()+".\n"+
                     (string)dest->query_contents(""));
            }
            return 1;
         } else if(room->query_door_open(dir) == 0) {
            write("The "+dir+" door is closed.\n");
         }
      }
   }

   if (!retval) {
      add_failed_mess("You do not think that the "+dir+" is here.\n");
   }
   return retval;
} /* cmd() */

/**
 * This method does a glance around the room.  This can be used to do other
 * terribly exciting things as well.
 * @param person the person doing the glancing
 * @param room the room that is being glanced at
 * @param dark how dark it is
 * @return the glance string
 */
string room_glance(object person, object room, int dark) {
   string ret;
   string sees;
   object mirror;

   ret = "";
   if (person->query_creator()) {
      ret += " %^CYAN%^" + file_name(room) + "%^RESET%^";

      sees=(string)room->query_property("creator sees");
      if (sees) {
         ret += sees +"\n"+ (string)room->query_contents();
      }
   }

   mirror = room->query_mirror_room();
   if (mirror) {
      room = mirror;
   }

   if(!dark || previous_object()->query_creator()) {
      if (mirror) {
         //
         // Do the glance description of the room itself.
         //
         ret += ".\n" + (string)mirror->query_contents();
      } else {
         ret += (string)room->query_short_exit_string()+".\n"+
           (string)room->query_contents();
      }
   } else if((dark == 2) || (dark == -2)) {
      if (!mirror) {
         ret += (string)room->query_short_exit_string()+"\n";
      } else {
         ret += "\n";
      }
      if((string)room->query_contents("") != "") {
         ret += person->colour_event("inventory", "%^GREEN%^") +
           "Some objects you can't make out are here.%^RESET%^\n";
      }
   } else {
      ret += "\n";
   }

   if (dark == 1 || dark == -1) {
      ret += "You can't see your hand in front of your face.\n";
   } else  {
      ret = "$a_short:"+ file_name(room) + "$"+ ret;
   }

   if (mirror) {
      ret += "Around " + room->the_short() + " is:\n";
      ret += room_glance(person, mirror, dark);
   }

   return ret;
} /* room_glance() */

int cmd_here() {
   object room;
   int dark;
   int retval;
   string str;

   retval = check_pre_stuff();
   if (retval != 2) {
      return retval;
   }

   room = environment(previous_object());
   if (room->query_mirror_room()) {
      room = room->query_mirror_room();
   }

   dark = previous_object()->check_dark((int)room->query_light());

   str = room_glance(previous_object(), room, dark);
   write(str);
   //this_player()->more_string(str, "glance");
   return 1;
} /* cmd_here() */

int check_exists(string file) {

  if ( objectp( find_object( file ) ) )
    return 1;
  if (file_size(file+".c") > 0) {
    return 1;
  }
  return 0;
} /* check_exists() */

mixed* query_patterns() {
   return ({
             "", (: cmd_here() :),
             "exit <string'exit'>", (: cmd_direction($4[0], 1) :),
             "[at] <indirect:object>", (: cmd_ob($1) :),
             "<string'exit'>", (: cmd_direction($4[0], 0) :),
            });
} /* query_patterns() */
