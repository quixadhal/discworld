/**
 * THis is the controller object to deal with the club rooms.  It
 * keeps track of which club owns the room, if the club room has
 * a board and any ownership issues that are needed.
 * @author Pinkfish
 * @started May 16th
 */
inherit "/std/basic/setup";

#include <clubs.h>
#include <room/club_room.h>

private nosave string _save_directory;
private mapping _options;
private string _club;
private mapping _ownership;
private nosave mapping _default_ownership;
private int _board_contained;

class ownership {
   string name;
   int type;
   int control_type;
}

private void setup_discussions();
void save_me();
void load_me();

void create() {
   _options = ([ ]);
   _ownership = ([ ]);
   _default_ownership = ([ ]);
   do_setup();
   // Make sure the save file is setup correctly.
   call_out((: load_me() :), 0);
} /* create() */

private void add_board_to(string club) {
   int board_count;

   // Next add the board to the new club.
   board_count = CLUB_HANDLER->query_elected_option(club, 
                                                    CLUB_ROOM_BOARD_COUNT);
   board_count++;
   if (board_count == 1) {
      CLUB_HANDLER->create_club_board(club);
   }
   CLUB_HANDLER->set_elected_option(club,
                                    CLUB_ROOM_BOARD_COUNT,
                                    board_count);
} /* add_board_to() */

private void remove_board_from(string club) {
   int board_count;

   board_count = CLUB_HANDLER->query_elected_option(club, 
                                                    CLUB_ROOM_BOARD_COUNT);
   board_count--;
   if (board_count == 0 &&
       CLUB_HANDLER->query_club_board(club)) {
      CLUB_HANDLER->remove_club_board(club);
   }
   if (board_count <= 0) {
      CLUB_HANDLER->remove_elected_option(club,
                                          CLUB_ROOM_BOARD_COUNT);
   } else {
      CLUB_HANDLER->set_elected_option(club,
                                       CLUB_ROOM_BOARD_COUNT,
                                       board_count);
   }
} /* remove_board_from() */

/**
 * This method sets the club that currently owns the set of rooms.
 * @param club the club that owns the rooms
 * @return 1 on success, 0 on failure
 */
int set_club(string club) {
   class ownership bing;
   class ownership fluff;
   string name;

   if (!CLUB_HANDLER->is_elected_club(club)) {
      return 0;
   }

   if (_club &&
       CLUB_HANDLER->query_club_name(club) ==
       CLUB_HANDLER->query_club_name(_club)) {
      return 1;
   }

   // Changing clubs.
   if (_board_contained) {
      // First remove the board from the old club...
      if (_club) {
         remove_board_from(_club);
      }

      add_board_to(club);
   }
   _club = club;

   //
   // Ok, need to set all the owned items as unowned and then
   // setup things for them.
   //
   foreach (name, bing in _ownership) {
      if (bing->control_type != CLUB_ROOM_CONTROLLED_FIXED) {
         if (_default_ownership[name]) {
            fluff = _default_ownership[name];
            bing->type = fluff->type;
            if (functionp(fluff->name)) {
               bing->name = evaluate(fluff->name,
                                     name,
                                     club);
            } else {
               bing->name = fluff->name;
            }
         }
      }
   }
   call_out((: setup_discussions :), 1);
   save_me();
   return 1;
} /* set_club() */

/**
 * This method queries the club that currently owns the set of rooms.
 * @return the club that owns the rooms
 */
string query_club() {
   return _club;
} /* query_club() */

/**
 * This method sets if a board is associated with the club room set or
 * not.
 * @parm board_associated is there a board associated
 */
void set_board_associated(int board_associated) {
   // Make sure it only has a 0 or 1 value.
   board_associated = board_associated != 0;
   if (_club) {
      if (_board_contained != board_associated) {
         if (board_associated) {
            add_board_to(_club);
         } else {
            remove_board_from(_club);
         }
      }
   }
   _board_contained = board_associated;
} /* set_board_associated() */

/**
 * This method queries to see if there is a board associated or not
 * @return 1 if there is aboard associated with this club room set
 */
int query_board_associated() {
   return _board_contained;
} /* query_board_associated() */

/**
 * This method checks to see if there is an option associated with the
 * club room.
 * @param name the name of the optionto check
 * @return 1 if there is, 0 if not
 */
int is_option(string name) {
   return !undefinedp(_options[name]);
} /* is_option() */

/**
 * This method queries and option associated with the club room.
 * @param name the name of the option
 * @return the value of the option
 */
mixed query_option(string name) {
   return _options[name];
} /* query_option() */

/**
 * This remove removes an option associated with the club room.
 * @param name the name of the optoin
 * @return 1 on success, 0 on failure
 */
int remove_option(string name) {
   if (is_option(name)) {
      map_delete(_options, name);
      return 1;
   }
   return 0;
} /* remove_option() */

/**
 * This method sets the value of the option associated with the club room.
 * @param name the name of the option
 * @param value the new value for the option
 * @return 1 on success, 0 on failure
 */
int set_option(string name, mixed value) {
   _options[name] = value;
} /* set_option() */

/**
 * This method adds an item to have ownership.  If a function pointer
 * is set as the default name, then the method will be evaluated each
 * time the club changes.  The first arguement will be the name of the
 * owned item being changed and the second argument will be the new
 * club name.
 * @param name the name of the option that needs ownership control
 * @param type the type of controlled ownership
 * @param default_type the default ownership of the item
 * @param default_name the default name of the owner
 * @return 1 on success, 0 on failure
 */
int add_controlled_ownership(string name,
                             int type,
                             int default_type,
                             mixed default_name) {
   class ownership bing;

   if (!_ownership[name]) {
      bing = new(class ownership);
      bing->name = CLUB_UNKNOWN_MEMBER;
      bing->type = CLUB_ROOM_OWNED_PERSON;
      bing->control_type = type;
      _ownership[name] = bing;
      bing = new(class ownership);
      bing->name = default_name;
      bing->type = default_type;
      bing->control_type = type;
      _default_ownership[name] = bing;
      //save_me();
      return 1;
   }
   return 0;
} /* add_controlled_ownership() */

/**
 * This method sets the owner of the controlled ownership type.
 * @param controlled the item being controlled
 * @param owner the owner of the item
 * @param type the type of the owner
 * @return 1 on success, 0 on failure
 */
int set_controlled_ownership_owner(string controlled,
                                   string owner,
                                   int type) {
   class ownership bing;

   if (!_ownership[controlled]) {
      return 0;
   }
   bing = _ownership[controlled];
   bing->type = type;
   bing->name = owner;
   save_me();
   return 1;
} /* set_controlled_ownership_owner() */

/**
 * This is the function used for nominating someone exciting the the
 * owned position of the club rooms.
 * @param message the message being sent
 * @param club_name the name of the club
 * @param frog the discussion item
 * @param arg1 the first argument
 * @param arg2 the second argument
 */
mixed discuss_nominate_ownership(string message,
                                 string club_name,
                                 class discussion_idea frog,
                                 string arg1,
                                 string arg2) {
   string *nominated;
   string body;
   string *nom;
   string name;
   int ret;

   switch (message) {
   case CLUB_DISCUSS_MESSAGE_VERIFY :
      if (CLUB_HANDLER->query_club_name(club_name) != 
          CLUB_HANDLER->query_club_name(_club)) {
         return 0;
      }

      if (!_ownership[frog->information]) {
         return 0;
      }
      return 1;

   case CLUB_DISCUSS_MESSAGE_COMPLETE :
      body = "Hello!\n\nThe discussion item " 
         "added at " + ctime(frog->time_added) + ", finished at " +
         ctime(time()) + " and added by " + frog->added_by +
         " has finished.";
      body += "  The vote for nominations for the ownership of " +
         frog->information + " in the club " +
         CLUB_HANDLER->query_club_name(club_name) + 
         " has been completed and the "
         "results are:\n";
      nominated = ({ });
      foreach (name, nom in frog->choices) {
         body += name + ": " + query_multiple_short(nom);
         if ((member_array(name, nom) != -1 ||
              name[0..strlen(CLUB_ROOM_POSITION_START)] == CLUB_ROOM_POSITION_START) &&
              sizeof(nom) >= 2) {
            body += " (accepted)\n";
            nominated += ({ name });
         } else {
            body += "\n";
         }
      }
      if (frog->information) {
         body += frog->information;
      }
      if (sizeof(nominated)) {
         if (_ownership[frog->information]) {
            if (sizeof(nominated) != 1) {
               body += "Therefor we had added an election with " + 
                  query_multiple_short(nominated) +
                  " for the ownership of " +
                  frog->information + ".\n";
               CLUB_HANDLER->add_discussion_item(club_name,
                                   CLUB_DISCUSS_FUNCTION,
                                   frog->added_by,
                                   ({ file_name(this_object()),
                                      "discuss_elect_ownership" }),
                                   frog->information,
                                   nominated,
                                   7 * (24 * 60 * 60),
                                   0);
            } else {
               body += "Only " + nominated[0] +
                  " was nominated, so they are given ownership immediately.\n";
               if (name[0..strlen(CLUB_ROOM_POSITION_START)] == CLUB_ROOM_POSITION_START) {
                  
                  set_controlled_ownership_owner(frog->information,
                                                 nominated[0][strlen(CLUB_ROOM_POSITION_START)..],
                                                 CLUB_ROOM_OWNED_POSITION);
               } else {
                  set_controlled_ownership_owner(frog->information,
                                                 nominated[0],
                                                 CLUB_ROOM_OWNED_PERSON);
               }
            }
         } else {
            body += "The ownership item appears to have vanished!\n";
         }
      } else {
         if (_ownership[frog->information]) {
            body += "No one accepted their nomination, so nothing "
               "happened.\n";
            CLUB_HANDLER->add_discussion_item(club_name,
                                CLUB_DISCUSS_FUNCTION,
                                frog->added_by,
                                frog->name,
                                frog->information,
                                ({ }),
                                7 * (24 * 60 * 60),
                                frog->optional_data);
         } else {
            body += "The position in the club appears to have vanished!\n";
         }
      }
      return body;

   case CLUB_DISCUSS_MESSAGE_SHORT_DESCRIPTION :
      return "Nominations for ownership of " + frog->information;

   case CLUB_DISCUSS_MESSAGE_LONG_DESCRIPTION :
      body = "Nominations for the ownership of " + frog->information +
         ", positions or people can be nominated to have ownership.  "
         "Positions are nominated by putting the word 'position' in front "
         "ie: 'position president'.\n"
         "Currently nominated are:\n";
      foreach (name, nom in frog->choices) {
         body += name + ": " + query_multiple_short(nom);
         if ((member_array(name, nom) != -1 ||
              name[0..strlen(CLUB_ROOM_POSITION_START) - 1] == CLUB_ROOM_POSITION_START) &&
              sizeof(nom) >= 2) {
            body += " (accepted)\n";
         } else {
            body += "\n";
         }
      }
      return body;

   case CLUB_DISCUSS_MESSAGE_NOMINATION :
      if (CLUB_HANDLER->is_member_of(_club, arg1)) {
         // Ok, so the person nominating is a member.
         if (CLUB_HANDLER->is_member_of(_club, arg2) ||
             (name[0..strlen(CLUB_ROOM_POSITION_START) - 1] == CLUB_ROOM_POSITION_START &&
              CLUB_HANDLER->is_valid_position(_club, 
                                              name[strlen(CLUB_ROOM_POSITION_START)..]))) {
            if (!frog->choices[arg2]) {
               frog->choices[arg2] = ({ arg1 });
               return 1;
            } else if (member_array(arg1,
                                    frog->choices[arg2]) == -1) {
               frog->choices[arg2] += ({ arg1 });
               return 1;
            }
         }
      }
      return 0;

   case CLUB_DISCUSS_MESSAGE_TYPE :
      return CLUB_DISCUSS_MESSAGE_TYPE_NOMINATE;

   case CLUB_DISCUSS_MESSAGE_REMOVE_MEMBER :
      // Check out to see if they have been nominated or nominated someone.
      if (frog->choices[arg1]) {
         map_delete(frog->choices, arg1);
         ret = 1;
      }
      foreach (name, nom in frog->choices) {
         if (member_array(arg1, nom) != -1) {
            frog->choices[name] = nom - ({ arg1 });
            if (!sizeof(frog->choices[name])) {
               map_delete(frog->choices, name);
            }
            ret = 1;
         }
      }
      return ret;

   case CLUB_DISCUSS_MESSAGE_REMOVE_POSITION :
      if (frog->choices[CLUB_ROOM_POSITION_START + arg1]) {
         map_delete(frog->choices, CLUB_ROOM_POSITION_START + arg1);
         ret = 1;
      }
      return ret;

   case CLUB_DISCUSS_MESSAGE_CHANGE_POSITION :
      if (frog->choices[CLUB_ROOM_POSITION_START + arg1]) {
         frog->choices[CLUB_ROOM_POSITION_START + arg2] = frog->choices[CLUB_ROOM_POSITION_START + arg1];
         map_delete(frog->choices, CLUB_ROOM_POSITION_START + arg1);
         ret = 1;
      }
      return ret;

   case CLUB_DISCUSS_MESSAGE_NOMINATION_QUERY :
      return frog->choices;

   }
} /* discuss_nominate_ownership() */

/**
 * This is the function used for electing whoever gets to have the 
 * nice owned room.
 * @param message the message being sent
 * @param club_name the name of the club
 * @param frog the discussion item
 * @param arg1 the first argument
 * @param arg2 the second argument
 */
mixed discuss_elect_ownership(string message,
                              string club_name,
                              class discussion_idea frog,
                              string arg1,
                              string arg2) {
   string body;
   string name;
   int votes;
   int ret;
   int max_votes;
   string *names;

   switch (message) {
   case CLUB_DISCUSS_MESSAGE_VERIFY :
      if (CLUB_HANDLER->query_club_name(club_name) != 
          CLUB_HANDLER->query_club_name(_club)) {
         return 0;
      }

      if (!_ownership[frog->information]) {
         return 0;
      }
      return 1;

   case CLUB_DISCUSS_MESSAGE_COMPLETE :
      body = "Hello!\n\nThe discussion item " 
         "added at " + ctime(frog->time_added) + ", finished at " +
         ctime(time()) + " and added by " + frog->added_by +
         " has finished.";
      body += "  The vote for elections of the ownership of " +
              frog->information + 
         " in the club " +
         CLUB_HANDLER->query_club_name(club_name) + 
         " has been completed and the "
         "results are:\n";
      names = ({ });
      max_votes = 0;
      foreach (name, votes in frog->choices) {
         body += name + ": " + votes + "\n";
         if (votes > max_votes) {
            max_votes = votes;
            names = ({ name });
         } else if (votes == max_votes) {
            names += ({ name });
         }
      }
      if (_ownership[frog->information]) {
         if (sizeof(names) == 1) {
            body += "Therefor we set " + names[0] +
                  " as owning " + frog->information + ".\n";
            if (names[0][0..strlen(CLUB_ROOM_POSITION_START) - 1] == CLUB_ROOM_POSITION_START) {
               set_controlled_ownership_owner(frog->information,
                                              names[0][strlen(CLUB_ROOM_POSITION_START)..],
                                              CLUB_ROOM_OWNED_POSITION);
             } else {
               set_controlled_ownership_owner(frog->information,
                                              names[0],
                                              CLUB_ROOM_OWNED_PERSON);
             }
         } else {
            body += "There was a tie for the results of the position, readding "
                    "a discussion with the tied members: " +
                    query_multiple_short(names) + ".\n";
            CLUB_HANDLER->add_discussion_item(club_name,
                                   CLUB_DISCUSS_FUNCTION,
                                   frog->added_by,
                                   ({ file_name(this_object()),
                                      "discuss_elect_ownership" }),
                                   frog->information,
                                   names,
                                   7 * (24 * 60 * 60),
                                   frog->optional_data);
         }
      } else {
         body += "The ownership item appears to have vanished!\n";
      }
      return body;

   case CLUB_DISCUSS_MESSAGE_SHORT_DESCRIPTION :
      return "Election for ownership of " + frog->information;

   case CLUB_DISCUSS_MESSAGE_LONG_DESCRIPTION :
      body = "Election for the ownership of " + frog->information +
         ".\n";
      return body;

   case CLUB_DISCUSS_MESSAGE_TYPE :
      return CLUB_DISCUSS_MESSAGE_TYPE_VOTE;

   case CLUB_DISCUSS_MESSAGE_REMOVE_MEMBER :
      // Check out to see if they have been nominated or nominated someone.
      if (frog->choices[arg1]) {
         map_delete(frog->choices, arg1);
         ret = 1;
      }
      return ret;

   case CLUB_DISCUSS_MESSAGE_REMOVE_POSITION :
      if (frog->choices[CLUB_ROOM_POSITION_START + arg1]) {
         map_delete(frog->choices, CLUB_ROOM_POSITION_START + arg1);
         ret = 1;
      }
      return ret;

   case CLUB_DISCUSS_MESSAGE_CHANGE_POSITION :
      if (frog->choices[CLUB_ROOM_POSITION_START + arg1]) {
         frog->choices[CLUB_ROOM_POSITION_START + arg2] = frog->choices[CLUB_ROOM_POSITION_START + arg1];
         map_delete(frog->choices, CLUB_ROOM_POSITION_START + arg1);
         ret = 1;
      }
      return ret;
   }
} /* discuss_elect_ownership() */

private void setup_discussions() {
   string name;
   class ownership bing;

   foreach (name, bing in _ownership) {
      if (bing->control_type != CLUB_ROOM_CONTROLLED_FIXED) {
         CLUB_HANDLER->add_discussion_item(_club,
                                           CLUB_DISCUSS_FUNCTION,
                                           ({ file_name(this_object()),
                                              "discuss_nominate_ownership" }),
                                           name,
                                           ({ }),
                                           7 * 24 * 60 * 60,
                                           0);
      }
   }
} /* setup_discussions() */

/**
 * This method loads all the data from the save file.
 */
void load_me() {
//tell_creator("pinkfish", "Loading " + _save_file + "\n");
   if (_save_directory) {
      unguarded( (: restore_object(_save_directory + CLUB_ROOM_CONTROLLER_SAVE_FILE ) :) );
//tell_creator("pinkfish", "%O, %O, %O\n", _options, _club, _ownership);
   }
} /* load_me() */

/**
 * This method saves all the data to the save file.
 */
void save_me() {
   if (_save_directory) {
      unguarded( (: save_object(_save_directory + CLUB_ROOM_CONTROLLER_SAVE_FILE ) :) );
   }
} /* save_me() */

/**
 * This method sets the save directory for the club room controller.
 * @param save_file the new save file
 */
void set_save_directory(string save_directory) {
   while (save_directory[<1] == '/') {
      save_directory = save_directory[0..<2];
   }
   _save_directory = save_directory;
} /* set_save_directory() */

/**
 * This method returns the save file for the club room controller.
 * @return the save file for the club room controller
 */
string query_save_directory() {
   return _save_directory;
} /* query_save_directory() */

/**
 * This method returns the save file name to use for the room.
 * @param fname the file name to use for the room
 * @return the save file name to use
 */
string query_room_save_file_name(mixed fname) {
   if (objectp(fname)) {
      fname = file_name(fname);
   } 
   fname = fname[strsrch(fname, '/', -1)..];
   return _save_directory + fname;
} /* query_room_save_file_name() */
