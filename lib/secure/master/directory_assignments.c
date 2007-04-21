/**
 * This is the file which handles assigning creators to specific
 * directories for bug control.
 * @author Pinkfish
 * @started Tue Mar  4 13:40:44 PST 2003
 */
#include <player_handler.h>
#include <error_handler.h>

private mapping _directory_assignments = ([ ]);

int query_leader(string name);

/**
 * Finds the person assigned to the specific directory.  It will climb
 * up the directory tree looking for who to assign the directory too.
 * @param dir the directory to find assignments in
 * @return the array of people assigned to the directory
 */
string* query_assigned_to_directory(string dir) {
   string *bits;
   string str;
   int i;

   bits = explode(dir, "/") - ({ "" });
   switch (bits[0]) {
   case "w" :
      if (sizeof(bits) > 1 && file_size("/d/" + bits[1]) == -2) {
         return ({ bits[1] });
      }
   case "d" :
      if (sizeof(bits) > 1 && file_size("/d/" + bits[1]) == -2) {
         return ("/d/" + bits[1] + "/master")->query_assigned_to_directory(bits);
      }
   default :
      i = sizeof(bits) - 1;
      do {
         str = "/" + implode(bits[0..<i], "/");
         i--;
      } while (i >= 0 && !_directory_assignments[str]);
      if (_directory_assignments[str]) {
         return _directory_assignments[str];
      }
      return ({ });
   }
}

/**
 * This method allows a specific person to be assigned to the given
 * directory.
 * @param dir the directory to assign someone too
 * @param people the people who are assigned to the directory
 */
int assign_people_to_directory(string dir, string* people) {
   string* new_people;
   string* dirs;

   if ( !sizeof( filter( previous_object( -1 ), (: interactive( $1 ) :) ) ) ) {
      user_event( "inform", this_player()->query_name() +
            " illegally attempted to call assign_person_to_directory( "+ dir +
            ", " + query_multiple_short(people) +
            " )", "cheat" );
      unguarded((: write_file, "/log/CHEAT", ctime( time() ) +
            ": illegal attempt to call assign_person_to_directory( "+ dir +
            ", " + query_multiple_short(people) +
            " ).\n"+ back_trace() :));
      return 0;
   }
   new_people = filter(people, (: PLAYER_HANDLER->test_creator( $1 ) ||
                                  $1 == ERROR_ASSIGNED_NO_ONE :));
   if ( sizeof(new_people) == sizeof(people) &&
        query_leader( previous_object(-1) ) &&
        file_size(dir) == -2) {
      dirs = explode(dir, "/") - ({ "" });
      if (sizeof(dirs) > 1 && dirs[0] == "d" &&
          file_size("/d/" + dirs[1]) == -2) {
         return ("/d/" + dirs[1] + "/master")->assign_people_to_directory(dir, people);
      } else {
         if (!_directory_assignments) {
            _directory_assignments = ([ ]);
         }
         // Set back to nothing.
         if (sizeof(people) == 1 && people[0] == ERROR_ASSIGNED_NO_ONE) {
            people = ({ });
         }
         if (sizeof(people)) {
            if (sizeof(_directory_assignments[dir])) {
               ERROR_HANDLER->do_update_directory_assignment(_directory_assignments[dir][0], people[0], dir, (: 1 :));
            } else {
               ERROR_HANDLER->do_update_directory_assignment(ERROR_ASSIGNED_NO_ONE, people[0], dir, (: 1 :));
            }
            _directory_assignments[dir] = people;
         } else {
            map_delete(_directory_assignments, dir);
         }
         save_object("/secure/master");
      }
      return 1;
   }
   debug_printf("Bad pumpkins. %O %O\n", new_people, people);
   return 0;
}

/**
 * This method returns all the current assignments from directory to the
 * people who are looking after them.
 * @return the directory assignment mapping
 */
mapping query_directory_assignments() {
   if (!_directory_assignments) {
      return ([ ]);
   }
   return copy(_directory_assignments);
}
