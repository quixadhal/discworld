inherit "/cmds/base";
#include <playtesters.h>

object find_location(string str) {
   if (find_object(str)) {
      return find_object(str);
   }

   return load_object(str);
}

int do_add_jump(string from, string to) {
   object from_ob;
   object to_ob;

   from_ob = find_location(from);
   to_ob = find_location(to);

   if (!from_ob) {
      add_failed_mess("The from location does not exist.\n");
      return 0;
   }
   if (!to_ob) {
      add_failed_mess("The to location does not exist.\n");
      return 0;
   }

   PLAYTESTER_HAND->add_jump_point(from, to);
   write("You added the jump point from " + from + " to " + to + ".\n");
   return 1;
}

int do_path_list() {
   string str;
   string from;
   string* dest;

   str = "";
   foreach (from, dest in PLAYTESTER_HAND->query_jump_points()) {
      str += from + ": " + query_multiple_short(dest) + ".\n";
   }
   write(str);
   return 1;
}

int do_remove_jump(string from, string to) {
   string* dests;

   dests = PLAYTESTER_HAND->query_jump_destination(from);
   if (!sizeof(dests)) {
      add_failed_mess("There are no destiations from " + from + ".\n");
      return 0;
   }
   if (member_array(to, dests) == -1) {
      add_failed_mess("Unable to remove a jump from " + from + " to " +
                      to + ".\nValid to's are: " +
                      query_multiple_short(dests) + ".\n");
      return 0;
   }
   PLAYTESTER_HAND->remove_jump_point(from, to);
   write("Removed the jump from " + from + " to " + to + ".\n");
   return 1;
}

mixed* query_patterns() {
   return ({ "add <string'from'> <string'to'>", (: do_add_jump($4[0], $4[1]) :),
        "remove <string'from'> <string'to'>", (: do_remove_jump($4[0], $4[1]) :),
        "creator list", (: do_path_list :) });
}
