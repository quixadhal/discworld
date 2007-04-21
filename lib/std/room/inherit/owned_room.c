/**
 * This is an owned room inherit.  It allows controllable entrance and
 * things like 'allow' and 'throwout' and stuff.  Make sure the create on
 * this is called after the create in the basic room.
 * @author Pinkfish
 * @started 30th of June
 */
#include <room.h>

private string _owner;
private string* _allowed;
private nosave string _save_file;
private nosave string _front_door;

protected int do_allow(string name);
protected int do_throwout(string name);
protected int do_disallow(string name);
void load_room();
varargs mixed call_door( string direc, 
                         string func, 
                         mixed arg1, 
                         mixed arg2, 
                         mixed arg3);
string query_owner();

void create() {
   _allowed = ({ });
   load_room();
} /* create() */

/** @ignore yes */
void init() {
   if (this_player()->query_name() == query_owner()) {
      add_command("room",
                  "allow <string'person'>",
                  (: do_allow($4[0]) :));
      add_command("room",
                  "disallow <string'person'>",
                  (: do_disallow($4[0]) :));
      add_command("room",
                  "throwout <indirect:living'person'>",
                  (: do_throwout($4[0]) :));
   }
} /* init() */

/**
 * This method sets the save file for the owned room.
 * @param save_file the name of the save file
 */
void set_save_file(string name) {
   _save_file = name;
} /* set_save_file() */

/**
 * This method returns the save file the owned room.
 * @return the save file for the owned room
 */
string query_save_file() {
   return _save_file;
} /* query_save_file() */

/**
 * This method returns the current allowed list on the room.
 * @return the current allowed array
 */
string *query_allowed() {
   return _allowed;
} /* query_allowed() */

/**
 * This method sets the allowed array for the room.
 * @param allowed the new allowed array.
 */
protected void set_allowed(string *allowed) {
   _allowed = allowed;
} /* set_allowed() */

/**
 * This method returns the owner of the room.
 * @return the owner of the room
 */
string query_owner() {
   return _owner;
} /* query_owner() */

/**
 * This method sets the owner of the room.
 * @param owner the new owner of the room
 */
void set_owner(string owner) {
   _owner = owner;
} /* set_owner() */

/**
 * This the main entry point for the allow command.
 * @paran name the name of the person to allow
 */
protected int do_allow(string name) {
   name = lower_case(name);
   if (member_array(name, _allowed) != -1) {
      add_failed_mess("The person " + name + " is already allowed.\n");
      return 0;
   }
   _allowed += ({ name });
   write("Ok, you have allowed " + name + " to enter your room.\n");
   return 1;
} /* do_allow() */

/**
 * This the main entry point for the disallow command.
 * @paran name the name of the person to disallow
 * @return 1 on succes, 0 on failure
 */
protected int do_disallow(string name) {
   name = lower_case(name);
   if (member_array(name, _allowed) == -1) {
      add_failed_mess("The person " + name + " is not allowed to disallow.\n");
      return 0;
   }
   _allowed -= ({ name });
   write("Ok, you have disallowed " + name + " from entering your room.\n");
   return 1;
} /* do_disallow() */

/**
 * This is the main entry point for the throwout command.
 * @param name the name of the person to throwout
 * @return 1 on success, 0 on failure
 */
protected int do_throwout(object *people) {
   object per;
   object *moved;

   moved = ({ });

   foreach (per in people) {
      if ( ROOM_HANDLER->exit_move( _front_door, 0, 0, per) ) {
         moved += ({ per });
      }
   }

   if (!sizeof(moved)) {
      add_failed_mess("Unable to move $I for some reason.\n", people);
      return 0;
   }

   add_succeeded_mess("$N throw$s $I out of the room.\n", moved);
   return 1;
} /* do_throwout() */

/**
 * This method sets the exit that is the front door.
 * @param exit the exit to use as the front door
 */
void set_front_door(string exit) {
   _front_door = exit;
} /* set_front_door() */

/**
 * This method queries the current front door.
 * @return the current front door
 */
string query_front_door() {
   return _front_door;
} /* query_front_door() */

/**
 * This method checks to see if they are allowed in.  Allow anyone in
 * if the room is not locked.
 * @param person the person to check
 * @return 1 if they are allowed in, 0 if not
 */
int is_allowed_in(string person) {
   person = lower_case(person);
   if (person != query_owner()) {
      if (call_door(_front_door,
                    "query_locked")) {
         if (member_array(person, _allowed) != -1) {
            return 0;
         }
      }
   }
   return 1;
} /* is_allowed_in() */

/**
 * This method tests to see if the person in question is either the owner
 * or is allowed.
 * @param person the person to check
 */
int is_allowed(string person) {
   if (person == query_owner() ||
       member_array(person, query_allowed()) != -1) {
      return 1;
   }
   return 0;
} /* is_allowed() */

/**
 * This method loads the room setup.
 */
void load_room() {
   if (_save_file) {
      restore_object(_save_file);
      // Default the room to a locked door.
      call_door(_front_door,
                "set_locked",
                1);
   }
} /* load_room() */

/**
 * This method saves the room setup.
 */
void save_room() {
   if (_save_file) {
      save_object(_save_file);
   }
} /* save_room() */
