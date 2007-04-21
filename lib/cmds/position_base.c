/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: position_base.c,v 1.9 2000/09/01 22:06:56 ceres Exp $
 * $Log: position_base.c,v $
 * Revision 1.9  2000/09/01 22:06:56  ceres
 * Made it use pluralize(position) rather than just "s" since that doesn't work for crouches
 *
 * Revision 1.8  2000/09/01 22:03:29  pinkfish
 *  Forcibly unlocked by ceres
 *
 * Revision 1.7  2000/06/09 23:52:38  pinkfish
 * Add in a silent option for putting people onto the floor.
 *
 * Revision 1.6  2000/03/22 00:39:06  taffyd
 * Added sit 'in' items.
 *
 * Revision 1.5  1999/07/06 23:38:18  olorin
 * I can no longer remember if I made any changes
 *
 * Revision 1.4  1998/04/04 18:34:17  pinkfish
 * Fix it up wto work correctly at all times!
 *
 * Revision 1.3  1998/03/26 11:40:53  pinkfish
 * Changes to fix up poroblems with not being able to do some
 * sort of actions
 *
 * Revision 1.2  1998/02/27 17:04:32  pinkfish
 * Handle allowing rooms to selectvely disable commands.
 *
 * Revision 1.1  1998/01/06 05:31:24  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";
#include <position.h>

string position;
string up_down;
string position_type;

int query_position_command() {
   return 1;
} /* query_position_command() */

string query_up_down() {
   return up_down;
} /* query_up_down() */

string query_position() {
   return position;
} /* query_position() */

string query_position_type() {
   return position_type;
} /* query_position_type() */

void setup_position(string pos, string up, string type) {
   position = pos;
   up_down = up;
   position_type = type;
} /* setup_position() */

int position(object person, int silent) {
   string pos_type;

   if (person->query_position() == position_type) {
      if (person == this_player()) {
         add_failed_mess("You are already "+position_type+".\n");
      }
      return 0;
   }

   if (!environment(person) ||
       !environment(person)->is_allowed_position(position_type)) {
      if (person == this_player()) {
         add_failed_mess("You cannot " + position +  " " + up_down +
                         " here.\n");
      }
      return 0;
   }

   if (!silent) {
      if (up_down != "") {
         if (person->query_position_on()) {
            pos_type = person->query_position_type();
            tell_object(person, "You " + position+" " + up_down +
                             " " + pos_type + " " +
                             person->query_position_on_short() + ".\n");
            tell_room(environment(person),
                      person->one_short() + " $V$0=" + pluralize(position) + ","+
                      position+"$V$ "  + up_down +
                      " " + pos_type + " " +
                      person->query_position_on_short() + ".\n",
                      ({ person }));
         } else {
            tell_object(person, "You " + position+" " + up_down + ".\n");
            tell_room(environment(person),
               person->one_short() + " $V$0=" + pluralize(position) + "," +
                      position+"$V$ " +
                      up_down + ".\n",
                      ({ person }));
         }
      } else {
         if (person->query_position_on()) {
            pos_type = person->query_position_type();
            tell_object(person, "You " + position +
                             " " + pos_type + " " +
                             person->query_position_on_short() + ".\n");
            tell_room(environment(person),
               person->one_short() + " $V$0=" + pluralize(position) + ","+position+
                             "$V$ " + pos_type + " " +
                             person->query_position_on_short() + ".\n",
               ({ person }));
         } else {
            tell_object(person, "You " + position + ".\n");
            tell_room(environment(person),
               person->one_short() + " $V$0=" + pluralize(position) + ","+
                      position+"$V$.\n",
               ({ person }));
         }
      }
   }
   person->set_position(position_type);
   return 1;
} /* position() */

int position_floor(object person, int silent) {
   if (!environment(person) ||
       !environment(person)->is_allowed_position(position_type)) {
      if (person == this_player()) {
         add_failed_mess("You cannot " + position +  " " + up_down +
                         " here.\n");
      }
      return 0;
   }
 
   if (person->query_position_on()) {
      if (!silent) {
         if (up_down != "") {
            tell_object(person, "You get off " +
                person->query_position_on_short() +
                " and " + position + " " + up_down + ".\n");
            tell_room(environment(person),
                person->one_short() + " gets off " +
                person->query_position_on_short() +
                " and $V$0=" + pluralize(position) + ","+position+"$V$ " + up_down + ".\n",
                ({ person }));
         } else {
            tell_object(person, "You get off " +
                person->query_position_on_short() +
                " and " + position + ".\n");
            tell_room(environment(person),
                person->one_short() + " gets off " +
                person->query_position_on_short() +
                " and $V$0=" + pluralize(position) + ","+position+"$V$.\n",
                ({ person }));
         }
      }
      person->set_position(position_type);
      person->set_position_on(0);
      person->set_position_type(0);
      person->set_position_multiple(0);
      return 1;
   }
   return position(person, 0);
} /* position_floor() */

string *query_position_strings(object person) {
   string bit;
   string bit_other;

   switch (person->query_position_type()) {
      case AT_TYPE :
      case BESIDE_TYPE :
         bit = "move away from";
         bit_other = "moves away from";
         break;

      case ON_TYPE :
      default :
         bit = "get off";
         bit_other = "gets off";
         break;
   }
   return ({ bit, bit_other });
} /* query_position_strings() */

int position_object(object *obs, string pos_type, object person) {
   int i;
   int mult;
   string rabbit;
   string *pos_strings;

   if (!environment(person) ||
       !environment(person)->is_allowed_position(position_type)) {
      if (person == this_player()) {
         add_failed_mess("You cannot " + position +  " " + up_down +
                         " here.\n");
      }
      return 0;
   }
 
   pos_strings = query_position_strings(person);

   for (i = 0; i < sizeof(obs); i++) {
      if (obs[i]->query_property(CAN_POSITION_PROPERTY)) {
         mult = obs[i]->query_property(MULTIPLE_POSITION_PROPERTY);
         if (person->query_position_on()) {
            if (person->query_position_on() == obs[i] &&
                person->query_position_type() == pos_type) {
               if (person->query_position() != position_type) {
                  return position(person, 0);
               }
               if (person == this_player()) {
                  add_failed_mess("You are already " + position_type + " " +
                                  pos_type + " $I.\n",
                                  obs[i..i]);
               }
               return 0;
            }
            tell_object(person, "You " + pos_strings[0] + " " +
                person->query_position_on_short() +
                " and " + position + " " + pos_type+" "+
                obs[i]->a_short()+".\n");
            tell_room(environment(person),
                person->one_short() + " " + pos_strings[1] + " " +
                person->query_position_on_short() +
                      
                " and $V$0=" + pluralize(position) + ","+position+"$V$ " + pos_type + " " +
                obs[i]->a_short() + ".\n",
                ({ person }));
            person->set_position(position_type);
            person->set_position_on(obs[i]);
            person->set_position_type(pos_type);
            person->set_position_multiple(mult);
            return 1;
         }
         tell_object(person, "You " + position + " " + pos_type +
                             " " + obs[i]->a_short()+".\n");
         tell_room(environment(person),
                   person->one_short() + " $V$0=" + pluralize(position) + ","+position+
                   "$V$ " + pos_type + " " +
                   obs[i]->a_short() + ".\n",
                   ({ person }));
         person->set_position(position_type);
         person->set_position_on(obs[i]);
         person->set_position_type(pos_type);
         person->set_position_multiple(mult);
         return 1;
      } else  {
         rabbit = obs[i]->query_position_string(position_type);
         if (rabbit) {
            mult = obs[i]->query_position_multiple(position_type);
            if (person->query_position_on()) {
               if (person->query_position_on() == rabbit &&
                   person->query_position_type() == pos_type) {
                  if (person->query_position() != position_type) {
                     return position(person, 0);
                  }
                  add_failed_mess("You are already " + position_type + " " +
                                  pos_type + " $I.\n",
                                  obs[i..i]);
                  return 0;
               }
               tell_object(person, "You " + pos_strings[0] +  " " +
                   person->query_position_on_short() +
                   " and " + position + " " + pos_type + " " +
                   rabbit + ".\n");
               tell_room(environment(person),
                         person->one_short() + " " + pos_strings[1] + " " +
                         person->query_position_on_short() +
                         " and $V$0=" + pluralize(position) + ","+position+"$V$ " +
                         pos_type + " " +
                         rabbit + ".\n",
                         ({ person }));
               person->set_position(position_type);
               person->set_position_on(rabbit);
               person->set_position_type(pos_type);
               person->set_position_multiple(mult);
               return 1;
            }
            tell_object(person, "You "+position+" " + pos_type + " " +
                                 rabbit + ".\n");
            
            tell_room(environment(person),
                person->one_short() + " $V$0=" + pluralize(position) + ","+position+
                      "$V$ "+pos_type + " " +rabbit + ".\n",
                      ({ person }));
            person->set_position(position_type);
            person->set_position_on(rabbit);
            person->set_position_type(pos_type);
            person->set_position_multiple(mult);
            return 1;
         }
      }
   }
   return 0;
} /* position_object() */

mixed *query_patterns() {
   return ({ "", (: position(this_player(), 0) :),
             "on [the] floor", (: position_floor(this_player(), 0) :),
             "{on|at|in|beside} <indirect:object>",
                    (: position_object($1, $4[0], this_player()) :) });
} /* query_patterns() */
