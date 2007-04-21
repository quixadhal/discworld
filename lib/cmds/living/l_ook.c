/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: l_ook.c,v 1.42 2003/05/22 18:04:37 ceres Exp $
 * 
 */
/* look command, trial out by Turrican for a commands daemon. */
/* Modified by Ceres to add looking into adjacent rooms */
/* Oh I did some performance enhancements too */

#include <player.h>
#include <weather.h>
#include <dirs.h>
#include <error_handler.h>
#include <db.h>
#include <terrain_map.h>

inherit "/cmds/base";

#define TP this_player()

string weather_long(string str);
int check_exists(string file);
string look_around();
mixed cmd_string(string arg);

void finish_bug_summary(object player, int type, mixed data) {
   class error_complete summ;
   class error_complete* errors;
   string str;
   int pos;

   if (type != DB_SUCCESS) {
      return ;
   }
   errors = data;
   if (!sizeof(errors)) {
      tell_object(player, "No errors for this room.\n");
      return ;
   }
   str = "";
   str = sprintf("#<bug id>  ( <date> ) %5-s %4-s %-4s by %11-s %11-s\n",
                 "Status", "Type", "Cat", "Reporter", "Assigned");
   foreach (summ in errors) {
      pos = strsrch(summ->details->report, "\n");
      if (pos < 0 || pos > player->query_cols() - 5) {
         pos = player->query_cols() - 5;
      } else {
         pos --;
      }
      str += sprintf("$I$3=#%d (%s) %5-s %4-s %-4s by %11-s %11-s\n%s\n",
                     summ->details->summary->id,
                     ctime(summ->details->summary->entry_date)[4..15],
                     summ->details->summary->status[0..4],
                     summ->details->summary->type,
                     summ->details->summary->category[0..3],
                     summ->details->summary->reporter,
                     summ->details->summary->assigned_to,
                     summ->details->report[0..pos]);
   }
   tell_object(player, str);
}

void show_error_summary(object player, string* types) {
   class error_query query;

   if (!arrayp(types) || !sizeof(types)) {
      return ;
   }

   query = new(class error_query);
   query->file_name = file_name(environment(player));
   // Just show the open bugs.
   query->status = ({ ERROR_STATUS_OPEN, ERROR_STATUS_CONSIDERING,
                      ERROR_STATUS_FIXING });
   query->type = types;
   query->no_extra_details = 1;
   ERROR_HANDLER->do_query_multiple_bug_details(query, (: finish_bug_summary, player :));
}

int cmd_look_room()
{
   object room;
   int dark;
   string ret;
   int *coords;

   room = environment(TP);
   if (!room) {
      add_failed_mess("You are in limbo... sorry you can't look at " +
                      "anything.\n");
      return 0;
   }

   if (TP->query_blinded()) {
      add_failed_mess("You are blind and cannot see.\n");
      return 0;
   }

   if (!TP->query_property("dead")) {
      dark = (int) TP->check_dark((int) room->query_light());
   }

   if (TP->query_creator()) {
      coords = room->query_co_ord();
      if (sizeof(coords)) {
         ret = "  (" + coords[0] + "," + coords[1] + "," + coords[2] + ")";
      } else {
         ret = "  (unset)";
      }
      if (virtualp(room)) {
         write("%^CYAN%^" + file_name(room) + " [" +
                room->query_property("base path")  + "]" + ret + "%^RESET%^\n");
      } else {
         write("%^CYAN%^" + file_name(room) + ret + "%^RESET%^\n");
      }
   }

   if ((TP->query_creator() || TP->query_playtester()) &&
       TP->query_property(TERRAIN_MAP_IN_LOOK_PROP)) {
      ret = room->long_test(0, dark);
      if (ret) {
         write("$P$Look$P$" + ret);
      } else {
         write("$P$Look$P$" + (string) room->long(0, dark));
      }
   } else {
      write("$P$Look$P$" + (string) room->long(0, dark));
   }

   if (dark && TP->query_creator()) {
      write("%^YELLOW%^As a creator, you can see:%^RESET%^\n" +
            (string) room->long(0, 0));
   }

   // Ok, check and see if they want any errors shown.
   if (TP->query_creator() && TP->query_property(PLAYER_SHOW_ERRORS_PROP)) {
      show_error_summary(this_player(), TP->query_property(PLAYER_SHOW_ERRORS_PROP));
   }

   return 1;
}                               /* cmd_look_room() */

/**
 * This method returns the 'look' description of the object.  This should
 * be used for anyone want to do look like capabilities for their
 * objects.
 * @param thing the object to look at
 * @param player the player doing the looking
 * @param dark the darkness modifier
 * @param verb the verb to use when telling the player their item is being
 * looked at
 * @param arg the string used for the look
 * @return the look string
 */
string query_look_thing(object thing,
                        object player,
                        int dark,
                        string verb,
                        string arg)
{
   string ret;
   object env;
   string other;
   string replaced;

   if (TP->query_blinded()) {
      add_failed_mess("You are blind and cannot see.\n");
      return 0;
   }

   ret = "";
   if (living(thing)) {
      if (thing == player) {
         ret += "Looking at yourself again?  What a narcissist!\n";
      } else {
         tell_object(thing, "$one_short:" + file_name(player) +
                     "$ $V$0=looks,look$V$ at you.\n");
      }
   } else {
      env = environment(thing);
      while (env && !living(env)) {
         env = environment(env);
      }
   }
   if (env && env != player) {
      tell_object(env, player->the_short() + " " + verb + " "
                  "at your " + thing->pretty_short() + ".\n");
      other = thing->query_long(arg, dark);
      if (other) {
         replaced = thing->replace_long_dollars(player, other); 

         if (replaced) { 
            ret += replaced;
         }
         else {
            ret += other;
         }
      }
      other = thing->query_long_details(arg, dark, player);
      if (other) {
         ret += other;
      }
   } else {
      ret += thing->long(arg, dark);
   }
   return ret;
}                               /* query_look_thing() */

mixed cmd_object(object * obs,
                 string arg)
{
   object thing;
   object room;
   int dark;
   int retval;
   string ret;

   room = environment(TP);

   if (LENGTHEN[arg]) {
     arg = LENGTHEN[arg];
   }

   if (TP->query_blinded()) {
      add_failed_mess("You are blind and cannot see.\n");
      return 0;
   }

   if (!room) {
      add_failed_mess("You are in limbo... sorry you can't look at " +
                      "anything.\n");
      return 0;
   }


   if (!TP->query_property("dead")) {
      dark = (int) TP->check_dark((int) room->query_light());
   }

   if (dark == 2) {
      add_failed_mess("It is way too bright to see anything at all.\n");
      return 0;
   }
   if (dark == -2) {
      add_failed_mess("It is way too dark to see anything at all.\n");
      return 0;
   }

   ret = "";
   foreach(thing in obs) {
      ret += query_look_thing(thing, this_player(), dark, "looks", arg);
   }

   if (ret != "") {
      write("$P$Look$P$" + ret);
      retval = 1;
   }
   // Doors are a direction.
   if (room->query_exit(arg))
      cmd_string(arg);
   else {
      arg = (string) this_player()->find_abs(arg);
      if (room->query_exit(arg))
         cmd_string(arg);
   }
   return 1;
}                               /* cmd_object() */

mixed cmd_string(string arg)
{
   object room;
   int dark;
   int retval;
   string ret;
   string other;
   string orig_arg;
   mixed *func;

   room = environment(TP);

   if (TP->query_blinded()) {
      add_failed_mess("You are blind and cannot see.\n");
      return 0;
   }

   if (!room) {
      add_failed_mess("You are in limbo... sorry you can't look at " +
                      "anything.\n");
      return 0;
   }

   switch (arg) {
   case "soul":
      write((string) "/obj/handlers/new_soul"->help_list());
      return 1;
   case "sun":
   case "moon":
   case "stars":
   case "weather":
   case "sky":
      write(weather_long(arg));
      return 1;
   case "around":
      write(look_around());
      return 1;
   }

   orig_arg = arg;

   if (!TP->query_property("dead")) {
      dark = (int) TP->check_dark((int) room->query_light());
   }

   if (dark == 2) {
      add_failed_mess("It is way too bright to see anything at all.\n");
      return 0;
   }
   if (dark == -2) {
      add_failed_mess("It is way too dark to see anything at all.\n");
      return 0;
   }

   if (LENGTHEN[arg]) {
      arg = LENGTHEN[arg];
   }

   if (room->query_mirror_room()) {
      room = room->query_mirror_room();
   }

   if (!room->query_exit(arg)) {
      arg = (string) this_player()->find_abs(arg);
   }

   if (room->query_exit(arg)) {
      int tmp_ret_val;

      // got knows what this is for, as far as I know set_destination doesn't
      // even exist.
      room->set_destination(arg);
      other = (string) room->query_destination(arg);
      ret = room->query_look(arg);
      if (ret && ret != "") {
         write(ret + "\n");
         retval = 1;
         tmp_ret_val = 1;
      }

      func = room->query_look_func(arg);
      if (pointerp(func) && sizeof(func) >= 1 && func[0] && func[1]) {
         if (call_other(func[0], func[1], 1)) {
            retval = 1;
            tmp_ret_val = 1;
         }
      } else if (functionp(func)) {
         if (evaluate(func, 1)) {
            retval = 1;
            tmp_ret_val = 1;
         }
      }

      if (check_exists(other) && !tmp_ret_val) {
         object door;

         door = room->query_door_control(arg);

         if (room->query_door_open(arg) || door->query_transparent()) {
            // it mustn't be dark in the destination room.
            other->force_load();
            dark = (int) TP->check_dark((int) other->query_light());
            ret = other->long(0, dark);
            this_player()->set_looked(find_object(other));
            write(ret);
            retval = 1;
         } else {
            add_failed_mess(door->the_short() +
                            " is closed and not transparent.\n");
            retval = 0;
         }
      }
   }
   //notify_fail("You do not think that the "+orig_arg+" is here.\n");
   return retval;
}                               /* cmd() */

int check_exists(string file)
{
   int retval;

   if (objectp(find_object(file))) {
      return 1;
   }

   // Also check to see if it is a terrain or not.
   if (strsrch(file, ":")) {
      return 1;
   }

   if (file_size(file + ".c") > 0) {
      retval = 1;
   } else {
      retval = 0;
   }
   return retval;
}

string weather_long(string str)
{
   object env;
   string temp;
   string whandler;

   env = environment(TP);
   temp = (string) env->query_property("location");
   switch (temp) {
   case ("inside"):
      return "You are not outside.\n";
   }

   whandler = env->query_weather_handler() ;
   if ( ! whandler ) {
       whandler = WEATHER ;
   }
   
   if (str == "sun") {
      if (whandler->query_day(env)) {
         return "Ouch that hurts.\n";
      } else {
         return "The sun is not up, sorry.\n";
      }
   }

   if (str == "moon") {
      if (whandler->query_day(env)) {
         return "The moon is not up, try at night.\n";
      } else if ( (temp = whandler->query_moon_string(env)) ) {
         return temp;
      } else {
         return "The moon is not up at the moment, try again later.\n";
      }
   }

   if (str == "stars") {
       if (whandler->query_day(env)) {
           return "The stars are not out at the moment - try at night.\n" ;
       } else {
           if ( (temp = whandler->query_star_string(env)) ) {
               return temp ;
           } else {
               return "There aren't any visible stars right now.\n" ;
           }
       }
   }
   
   return "You look up at the sky.  " + whandler->weather_string(env) + ".\n";
}                               /* weather_long() */

string look_around()
{
   int i;
   int j;
   string prep;
   string room;
   string *parts;
   string *rooms;
   object place;
   object thing;
   object *contents;
   mixed *locations;

   if (TP->query_blinded()) {
      add_failed_mess("You are blind and cannot see.\n");
      return 0;
   }

   place = environment(this_player());
   if (place->query_linked()) {
      rooms = (string *) place->query_rooms();
   } else {
      rooms = ({ });
   }
   locations = ({ });
   say((string) this_player()->one_short() + " $V$0=looks,look$V$ around.\n");
   foreach(room in rooms) {
      if (!find_object(room)) {
         continue;
      }
      if (this_player()->check_dark((int) room->query_light())) {
         continue;
      }
      contents = ({ });
      foreach(thing in all_inventory(find_object(room))) {
         if (living(thing) && thing->query_visible(this_player()))
            contents += ({ thing });
      }
      if (!sizeof(contents)) {
         continue;
      }
      prep = (string) room->query_static_preposition();
      if (prep != "") {
         if (!stringp(prep)) {
            prep = sprintf("%O", prep);
         }
         prep += " ";
      }
      locations += ({ ({ contents, prep +
                         (string) this_player()->
                         convert_message((string) room->the_short())
                      }) });
   }
   if (!sizeof(locations)) {
      contents = ({ });
      if (this_player()->check_dark((int) place->query_light())) {
         return "You can't see well enough to look around here.\n";
      }
      foreach(thing in all_inventory(place)) {
         if (living(thing) && thing->query_visible(this_player()) &&
             (thing != this_player()))
            contents += ({ thing });
      }
      if (!sizeof(contents)) {
         return "You do not see anyone else about.\n";
      }
      return "$C$" + query_multiple_short(contents) +
         " $V$0=is,are$V$ here.\n";
   }
   for (i = 0; i < sizeof(locations) - 1; i++) {
      for (j = i + 1; j < sizeof(locations); j++) {
         if (locations[i][1] == locations[j][1]) {
            locations[i][0] += locations[j][0];
            locations = delete(locations, j, 1);
            j--;
         }
      }
   }
   parts = ({ });
   for (i = 0; i < sizeof(locations); i++) {
      parts += ({ query_multiple_short(locations[i][0]) + " " +
                  locations[i][1] });
   }
   if (sizeof(parts) == 1) {
      return "You can see " + parts[0] + ".\n";
   }
   return "You can see " + query_multiple_short(parts) + ".\n";
}                               /* look_around() */

mixed *query_patterns()
{
   return ({ "", (: cmd_look_room() :),
             "<string'direction'>", (: cmd_string($4[0]) :),
             "<indirect:object>", (: cmd_object($1, $4[0]) :),
             "at <indirect:object>", (: cmd_object($1, $4[0]) :),
             "in <indirect:object>", (: cmd_object($1, $4[0]) :),
             "inside <indirect:object>", (: cmd_object($1, $4[0]) :),
             "around", (: cmd_string("around") :) });
}                               /* query_patterns() */
