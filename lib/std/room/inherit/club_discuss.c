/**
 * This file is for adding discussion items to clubs.  It should be included
 * into the main club control room.
 * @author Pinkfish
 * @started April 21st 1999
 * @see /obj/handlers/club_handler.c
 */
#include <clubs.h>

#define MAX_SUBJECT_LEN 40
#define MAX_CHOICE_LEN 10
#define MIN_TIMEOUT 1
#define MAX_TIMEOUT 30
#define MIN_POSITION_LEN 5
#define SAVE_START "club_discuss -- "
#define CLUB_ALL_POSITIONS "all positions"

private void discuss_finish_description(string str, int open);
protected int do_discuss(int open, string type, string club_name);

/**
 * This class is used by the memo discussion item addition code.
 * @element subject the subject of the discussion item
 * @element description the description of the discussion item
 * @element choices the choices for the discussion item
 * @element timeout the length of time the item runs for
 * @element open if the discussion item is open or not
 */
class memo_item {
   string club_name;
   string subject;
   string description;
   string *choices;
   int timeout;
   int open;
}

/** @ignore yes */
void init() {
   add_command("discuss",
               "[open] {description|add position|"
               "change position name|memo|secret|"
               "remove member|no confidence|remove position|"
               "create account|remove account} in <string'club name'>",
                (: do_discuss(1, $4[0], $4[1]) :));
   add_command("discuss",
               "committee {description|add position|"
               "change position name|memo|secret|remove member|"
               "no confidence|remove position|"
               "create account|remove account} in <string'club name'>",
                     (: do_discuss(0, $4[0], $4[1]) :));
} /* init() */

/**
 * This method is the return from the editing for the information
 * bit and will now try and confirm this.
 * @param str the confirmation string
 * @param data the passed in data
 * @see discussion_add_name()
 * @see do_discuss_add_position()
 */
protected void discussion_finish_information(string info,
                                             mixed *data) {
   int type;
   string short;
   string name;
   string optional;
   string club_name;

   club_name = data[0];
   short = data[1];
   type = data[2];
   name = data[3];
   optional = data[4];

   write("Discussion item: " + short + ".\n");
   if (!info) {
      write("No description used.\n");
   } else {
      write(info);
   }

   write("\nDo you wish to add this? ");
   input_to("discussion_finish_confirm", 
            0, 
            club_name,
            short, 
            type, 
            name, 
            info, 
            optional);
} /* discussion_finish_information() */

/**
 * @ignore yes
 */
protected void discussion_finish_confirm(string str,
                                         string club_name,
                                         string short,
                                         int type,
                                         string name,
                                         string info,
                                         string optional) {
   str = lower_case(str);
   if (!strlen(str) ||
       str[0] != 'y') {
      write("Aborting.\n");
      return ;
   }

   if (CLUB_HANDLER->add_discussion_item(club_name,
                                         type,
                                         this_player()->query_cap_name(),
                                         name,
                                         info,
                                         ({ }),
                                         2 * (7 * 24 * 60 * 60),
                                         optional)) {
      write("Added in the discussion item: " + short + ".\n");
   } else {
      write("Unable to add in the discussion item: " + short + ".\n");
   }
} /* discussion_add_name_confirm() */

/**
 * This is the entry point for setting up a discussion on changing
 * the description of the club.
 * @see description_confirm()
 */
protected int do_discuss_description(int open,
                                     string club_name) {
   // Must be a member of the committee to discuss it.
   if (CLUB_HANDLER->query_discussion_item_by_type(club_name,
                                                   CLUB_DISCUSS_DESCRIPTION,
                                                   0)) {
      add_failed_mess("A description is already being discussed, you can only "
                      "discuss one description at a time.\n");
      return 0;
   }

   if (!open) {
      write("%^YELLOW%^This discussion item will be committee "
            "only!\n%^RESET%^");
   }
   write("Please enter the description you wish to have discussed as the "
         "new description of the club '" + 
         CLUB_HANDLER->query_club_name(club_name) + "'.\n");
   this_player()->do_edit(CLUB_HANDLER->query_description(club_name),
                          (: discuss_finish_description :),
                          0,
                          0,
                          ({ open,
                             club_name }));
   return 1;
} /* do_discuss_description() */

private void discuss_finish_description(string str, mixed *junk) {
   int open;
   string club_name;

   if (!str) {
      write("Description aborted.\n");
      return ;
   }

   open = junk[0];
   club_name = junk[1];

   write("Adding the description:\n" + str +
         "\nIs this the correct description (yes/no/edit)? ");
   input_to("description_confirm", 0, str, open, club_name);
} /* discuss_finish_description() */

/**
 * This method confirms the description typed in is correct.
 * @param confirm the string the used typed to confirm the description
 * @param str the description
 * @see do_discuss_description()
 */
protected void description_confirm(string confirm, string str, int open, string club_name) {
   confirm = lower_case(confirm);
   if (strlen(confirm) &&
       confirm[0] == 'e') {
      this_player()->do_edit(str,
                             (: discuss_finish_description :),
                             0,
                             0,
                             ({ open, club_name }));
      return ;
   }

   if (!strlen(confirm) ||
       confirm[0] != 'y') {
      write("Aborting.\n");
      return ;
   }

   if (CLUB_HANDLER->add_discussion_item(club_name,
                                         CLUB_DISCUSS_DESCRIPTION | open,
                                         this_player()->query_name(),
                                         "Change club description",
                                         str,
                                         ({ }),
                                         2 * (7 * 24 * 60 * 60),
                                         0)) {
      write("Successfuly added the item for discussion.\n");
   } else {
      write("Unable to add the item for discussion.\n");
   }
} /* description_confirm() */

/**
 * This is the method used to start the discussion of an add position
 * discussion item.
 * @return 1 on success, 0 on failure
 * @see do_discuss()
 * @see discussion_add_name()
 * @see discussion_add_name_confirm()
 */
protected int do_discuss_add_position(int open, string club_name) {
   if (!open) {
      write("%^YELLOW%^This discussion item will be committee only!\n%^RESET%^");
   }
   write("What committee position do you wish to add to the club '" + 
         club_name + "'? ");
   input_to("discussion_add_name", 0, open, club_name);
   return 1;
} /* do_discuss_add_position() */

/**
 * This method checks to make sure that the entered position name is
 * valid.
 * @param str the position to check
 * @see do_discuss_add_position()
 * @see discussion_add_name_confirm()
 */
protected void discussion_add_name(string str, int open, string club_name) {
   if (!str ||
       !strlen(str)) {
      write("Aborting.\n");
      return ;
   }

   str = strip_colours(str);
   
   // Check to see if this position is already in the club.
   if (CLUB_HANDLER->is_valid_position(club_name,
                                       str)) {
      write("The position '" + str + "' already exists in the club.  You "
            "cannot add it.\nAborting.\n");
      return ;
   }
   
   if (strlen(str) > CLUB_MAX_POSITION_NAME) {
      write("The position name '" + str + "' is too long, it can be a "
            "maximum length of " + CLUB_MAX_POSITION_NAME +
            ".\nAborting.\n");
      return ;
   }

   if (strlen(str) < MIN_POSITION_LEN) {
      write("The position name '" + str + "' is too short, the minimum is " +
            MIN_POSITION_LEN + ".\n");
      return;
   }

   write("What message do you wish display as to why you wish this position "
         "added?\n");
   this_player()->do_edit("",
                          (: discussion_finish_information :),
                          0,
                          0,
                          ({ club_name,
                                "Adding the position " + str,
                                CLUB_DISCUSS_NEW_POSITION | open,
                                str,
                                0 }) );
} /* discussion_add_name() */

/**
 * This is the main entry point for the function to setup a discussion item
 * to remove a member from the club.
 * @param open the open flag
 */
protected int do_discuss_remove_member(int open, string club_name) {
   if (!open) {
      write("%^YELLOW%^This discussion item will be committee only!\n%^RESET%^");
   }
   write("Which member to do you wish to try and remove? ");
   input_to("discussion_remove_member_name", 0, open, club_name);
   return 1;
} /* do_discuss_remove_member() */

/**
 * @ignore yes
 */
protected void discussion_remove_member_name(string str, int open, string club_name) {
   if (!str ||
       !strlen(str)) {
      write("Aborting.\n");
      return ;
   }
   
   // Check to see if this position is already in the club.
   if (!CLUB_HANDLER->is_member_of(club_name,
                                   str)) {
      write("The person " + str + " is not a member of the club " +
            CLUB_HANDLER->query_club_name(club_name) +
            " and therefor you cannot remove them.\nAborting.\n");
      return ;
   }

   write("What do you want to say about removing " + str + "?\n");
   this_player()->do_edit(CLUB_HANDLER->query_description(club_name),
                          (: discussion_finish_information :),
                          0,
                          0,
                          ({ club_name,
                                "Removing the member " + str,
                                CLUB_DISCUSS_MEMBER_REMOVE | open,
                                str,
                                0 }) );
} /* discussion_remove_member_confirm() */

/**
 * This method is the main entry point to adding a discussion type to make
 * the club secret.
 * @param open if the item is being added as an open item
 */
protected int do_discuss_secret(int open, string club_name) {
   string short;

   if (CLUB_HANDLER->query_discussion_item_by_type(club_name,
                                                   CLUB_DISCUSS_SECRET_TYPE)) {
      notify_fail("There is already a discussion item to change this club's "
                  "secret status.\n");
      return 0;
   }

   if (!open) {
      write("%^YELLOW%^This discussion item will be committee only!\n%^RESET%^");
   }
   if (CLUB_HANDLER->query_club_secret(club_name)) {
      write("What description to wish to use to make the club open?\n");
      short = "make the club open";
   } else {
      write("What description to wish to use to make the club secret?\n");
      short = "make the club secret";
   }

   this_player()->do_edit(CLUB_HANDLER->query_description(club_name),
                          (: discussion_finish_information :),
                          0,
                          0,
                          ({ club_name,
                                short,
                                CLUB_DISCUSS_SECRET_TYPE | open,
                                "Change to be secret",
                                0 }) );
   return 1;
} /* do_discuess_secret() */

/**
 * This is the main entry point for setting up a discussion item to 
 * remove a position from the club.
 * @return 1 on success, 0 on failure
 * @see do_discuss()
 */
protected int do_discuss_remove_position(int open, string club_name) {
   string *choices;
   string *ok;
   string bing;

   choices = CLUB_HANDLER->query_valid_positions(club_name);
   ok = filter(choices, (: !CLUB_HANDLER->is_basic_position($(club_name),
                                                            $1) :));
   if (!sizeof(ok)) {
      add_failed_mess("You cannot remove the basic positions of " + 
                      query_multiple_short(choices) + ".\n");
      return 0;
   }

   if (!open) {
      write("%^YELLOW%^This discussion item will be committee only!\n%^RESET%^");
   }
   write("Which one of the positions do you wish to remove?\n");
   foreach (bing in ok) {
      write("   " + bing + "\n");
   }
   write("Choice? ");
   input_to("remove_position", 0, open, club_name, ok);
   return 1;
} /* do_discuss_remove_position() */

/**
 * @ignore yes
 */
protected void remove_position(string str, int open, string club_name) {
   str = lower_case(str);

   if (!CLUB_HANDLER->is_valid_position(club_name,
                                        str)) {
      write("The position '" + str + "' does not exist in the club " +
            CLUB_HANDLER->query_club_name(club_name) + ".\n");
      return ;
   }

   if (CLUB_HANDLER->is_basic_position(club_name,
                                       str)) {
      write("Unable to remove the basic positions from the club.\n");
      return ;
   }

   write("What description do you wish to associate with removing the "
         "position " + str + ".\n");
   this_player()->do_edit("",
                          (: discussion_finish_information :),
                          0,
                          0,
                          ({ club_name,
                                "Remove the positon " + str,
                                CLUB_DISCUSS_POSITION_REMOVE | open,
                                str,
                                0 }) );
} /* remove_position() */

/**
 * This is the main entry point for setting up a discussion item to 
 * change a position name in the club.
 * @return 1 on success, 0 on failure
 * @see change_position()
 * @see change_position_new_name()
 * @see change_position_new_name_confirm()
 * @see do_discuss()
 */
protected int do_discuss_change_position_name(int open, string club_name) {
   string *choices;
   string bing;

   choices = CLUB_HANDLER->query_valid_positions(club_name);
   if (!open) {
      write("%^YELLOW%^This discussion item will be committee only!\n%^RESET%^");
   }
   write("Which one of the positions do you wish to change?\n");
   foreach (bing in choices) {
      write("   " + bing + "\n");
   }
   write("Choice? ");
   input_to("change_position", 0, open, club_name);
   return 1;
} /* do_discuss_change_position_name() */

/**
 * Figure out which of the choices the player wishes to change.
 * @param str the name of the position to be changed
 * @see do_discuss_change_position_name()
 * @see change_position_new_name()
 * @see change_position_new_name_confirm()
 */
protected void change_position(string str, int open, string club_name) {
   str = lower_case(str);

   if (!CLUB_HANDLER->is_valid_position(club_name,
                                        str)) {
      write("The position '" + str + "' does not exist in the club " +
            CLUB_HANDLER->query_club_name(club_name) + ".\n");
      return ;
   }

   write("What name do you wish to change '" + str + "' to? ");
   input_to("change_position_new_name", 0, str, open, club_name);
} /* change_position() */

/**
 * This method checks to make sure that the entered name to change the
 * position to is valid.
 * @param str the new name for the position
 * @param position the position to change
 * @see change_position()
 * @see do_discuss_change_position_name()
 * @see change_position_new_name_confirm()
 */
protected void change_position_new_name(string str,
                                        string position,
                                        int open, string club_name) {
   str = strip_colours(str);

   if (CLUB_HANDLER->is_valid_position(club_name,
                                       str)) {
      write("The name '" + str + "' already exists in the club " +
            CLUB_HANDLER->query_club_name(club_name) + ".\n");
      return ;
   }

   if (strlen(str) > CLUB_MAX_POSITION_NAME) {
      write("The position name '" + str + "' is too long, the maximum is " +
            CLUB_MAX_POSITION_NAME + ".\n");
      return ;
   }

   if (strlen(str) < MIN_POSITION_LEN) {
      write("The position name '" + str + "' is too short, the minimum is " +
            MIN_POSITION_LEN + ".\n");
      return;
   }

   write("What description to you wish to give for changing the position " +
         position + " to " + str + ".\n");
   this_player()->do_edit("",
                          (: discussion_finish_information :),
                          0,
                          0,
                          ({ club_name,
                                "change the position " + position + " to " + str,
                                CLUB_DISCUSS_POSITION_NAME | open,
                                position,
                                str }) );
} /* change_position_new_name() */

/**
 * This is the main entry point for the no confidence discussion votes.
 * @param open the open flag
 * @return 1 on success, 0 on failure
 * @see do_discuss()
 */
protected int do_discuss_no_confidence(int open, string club_name) {
   string *choices;
   string bing;

   choices = CLUB_HANDLER->query_valid_positions(club_name);
   if (!open) {
      write("%^YELLOW%^This discussion item will be committee "
            "only!\n%^RESET%^");
   }
   write("Which one of the positions do you wish to add a no confidence "
         "vote in?\n");
   foreach (bing in choices) {
      write("   " + bing + "\n");
   }
   write("   " + CLUB_ALL_POSITIONS + "\n");
   write("Choice? ");
   input_to("no_confidence", 0, open, club_name);
   return 1;
} /* do_discuss_no_confidence() */

/** @ignore yes */
protected void no_confidence(string str,
                             int open, string club_name) {
   string bing;

   str = lower_case(str);
   if (!CLUB_HANDLER->is_valid_position(club_name, str) &&
       str != CLUB_ALL_POSITIONS) {
      write("Sorry, '" + str + "' is not a valid position in " +
            CLUB_HANDLER->query_club_name(club_name) +
            ".\n");
      return ;
   }

   if (str == CLUB_ALL_POSITIONS) {
      bing = 0;
   } else {
      bing = str;
   }
   write("What description do you wish to give for a vote of no confidence "
         "in " + str + "?\n");
   this_player()->do_edit("",
                          (: discussion_finish_information :),
                          0,
                          0,
                          ({ club_name,
                                "no confidence in " + str,
                                CLUB_DISCUSS_NO_CONFIDENCE | open,
                                bing,
                                0 }) );
} /* no_confidence() */

/**
 * This is the main entry point for setting up a discussion item to 
 * remove an account from the club.
 * @return 1 on success, 0 on failure
 * @see do_discuss()
 */
protected int do_discuss_remove_account(int open, string club_name) {
   if (!open) {
      write("%^YELLOW%^This discussion item will be committee only!\n%^RESET%^");
   }

   write("What is the name of the special account you wish to close? ");
   input_to("remove_account", 0, open, club_name);
   return 1;
} /* do_discuss_create_account() */

/**
 * @ignore yes
 */
protected void remove_account(string str, int open, string club_name) {
   str = lower_case(str);

   if (!CLUB_HANDLER->is_account(club_name, str)) {
      write("You cannot remove an account that doesn't exist.\n");
      return ;
   }

   if (str == CLUB_DEFAULT_ACCOUNT_NAME) {
      write("You cannot remove the default account name.\n");
      return ;
   }

   write("What description do you wish to associate with createing the "
         "account " + str + ".\n");
   this_player()->do_edit("",
                          (: discussion_finish_information :),
                          0,
                          0,
                          ({ club_name,
                                "Create the special account " + str,
                                CLUB_DISCUSS_REMOVE_ACCOUNT | open,
                                str,
                                0 }) );
} /* remove_account() */

/**
 * This is the main entry point for setting up a discussion item to 
 * remove a position from the club.
 * @return 1 on success, 0 on failure
 * @see do_discuss()
 */
protected int do_discuss_create_account(int open, string club_name) {
   if (!open) {
      write("%^YELLOW%^This discussion item will be committee only!\n%^RESET%^");
   }

   write("What is the name of the special account you wish to open? ");
   input_to("create_account", 0, open, club_name);
   return 1;
} /* do_discuss_create_account() */

/**
 * @ignore yes
 */
protected void create_account(string str, int open, string club_name) {
   str = lower_case(str);

   if (CLUB_HANDLER->is_account(club_name, str)) {
      write("You cannot create an account that already exists.\n");
      return ;
   }
   write("What description do you wish to associate with createing the "
         "account " + str + ".\n");
   this_player()->do_edit("",
                          (: discussion_finish_information :),
                          0,
                          0,
                          ({ club_name,
                                "Create the special account " + str,
                                CLUB_DISCUSS_CREATE_ACCOUNT | open,
                                str,
                                0 }) );
} /* create_account() */

/** @ignore yes */
protected void discuss_memo_show(class memo_item memo) {
   write("The memo in club " +
         CLUB_HANDLER->query_club_name(memo->club_name) +
         " will complete in " + memo->timeout + " days.\n");
   if (memo->subject) {
      write("Subject: " + memo->subject + "\n");
   } else {
      write("No subject set.\n");
   }
   if (sizeof(memo->choices)) {
      write("Choices: " + query_multiple_short(memo->choices) + "\n");
   } else {
      write("No choices set.\n");
   }
   if (memo->description) {
      write("Description:\n" + memo->description + "\n");
   } else {
      write("No description set.\n");
   }
} /* discuss_memo_show() */

/**
 * This method runs the menu for the memo discussion item.
 * @param memo the memo item to be used in the menu
 */
protected void discuss_memo_menu(class memo_item memo) {
   discuss_memo_show(memo);
   write("\n");
   write("a) Set subject\n");
   write("b) Make a yes/no memo item\n");
   write("c) Add new choice\n");
   write("d) Remove choice\n");
   write("e) Set completion time\n");
   if (memo->description) {
      write("f) Change description\n");
      write("g) Zap description\n");
   } else {
      write("f) Add description\n");
   }
   write("m) Redisplay this menu\n");
   write("s) Save and Quit\n");
   write("q) Quit\n\n");
   write("Choice: ");
   input_to("discuss_memo_menu_choice", 0, memo);
} /* discuss_memo_menu() */

/** @ignore yes */
protected void discuss_memo_subject(class memo_item memo) {
   write("What do you wish the new subject to be? ");
   input_to("discuss_memo_subject_name", 0, memo);
} /* discuss_memo_subject() */

/** @ignore yes */
protected void discuss_memo_subject_name(string name,
                                         class memo_item memo) {
   if (!strlen(name)) {
      write("Not changing the subject.\n");
      write("Choice: ");
      input_to("discuss_memo_menu_choice", 0, memo);
      return ;
   }
   if (strlen(name) > MAX_SUBJECT_LEN) {
      write("The subject name is too long, the maximum "
            "length is " + MAX_SUBJECT_LEN + ".\n");
      write("Choice: ");
      input_to("discuss_memo_menu_choice", 0, memo);
      return ;
   }

   memo->subject = name;
   discuss_memo_menu(memo);
} /* discuss_memo_subject_name() */

/** @ignore yes */
protected void discuss_memo_add_choice(class memo_item memo) {
   write("What is the name of the choice you wish to add? ");
   input_to("discuss_memo_add_choice_response", 0, memo);
} /* discuss_memo_add_choice() */

/** @ignore yes */
protected void discuss_memo_add_choice_response(string response,
                                                class memo_item memo) {
   response = lower_case(implode(explode(response, " ") - ({ "" }), " "));
   response = strip_colours(response);
   if (!strlen(response)) {
      write("Not adding any choices.\n");
      write("Choice: ");
      input_to("discuss_memo_menu_choice", 0, memo);
      return ;
   }
   if (strlen(response) > MAX_CHOICE_LEN) {
      write("The choice name " + response + " is too long, maximum "
            "length is " + MAX_CHOICE_LEN + ".\n");
      write("Choice: ");
      input_to("discuss_memo_menu_choice", 0, memo);
      return ;
   }
   if (member_array(response, memo->choices) != -1) {
      write("The choice name " + response + " is already in the list of "
            "choices.  Current choices are " +
            query_multiple_short(memo->choices) + ".\n");
      write("Choice: ");
      input_to("discuss_memo_menu_choice", 0, memo);
      return ;
   }
   memo->choices += ({ response });
   discuss_memo_menu(memo);
} /* discuss_memo_add_choice_response() */

/** @ignore yes */
protected void discuss_memo_remove_choice(class memo_item memo) {
   write("What is the name of the choice you wish to remove? ");
   input_to("discuss_memo_remove_choice_response", 0, memo);
} /* discuss_memo_remove_choice() */

/** @ignore yes */
protected void discuss_memo_remove_choice_response(string response,
                                                   class memo_item memo) {
   response = lower_case(implode(explode(response, " ") - ({ "" }), " "));
   if (member_array(response, memo->choices) == -1) {
      write("The choice '" + response + "' is not in current memo.\n");
      write("Choice: ");
      input_to("discuss_memo_menu_choice", 0, memo);
      return ;
   }
   memo->choices -= ({ response });
   discuss_memo_menu(memo);
} /* discuss_memo_remove_choice_response() */

/** @ignore yes */
protected void discuss_memo_timeout(class memo_item memo) {
   write("What do you wish to change the length of the item to (in days)? ");
   input_to("discuss_memo_timeout_response", 0, memo);
} /* discuss_memo_timeout() */

/** @ignore yes */
protected void discuss_memo_timeout_response(string str,
                                             class memo_item memo) {
   int tim;

   if (sscanf(str, "%d", tim) != 1) {
      write("Must specify a number.\n");
      write("Choice: ");
      input_to("discuss_memo_menu_choice", 0, memo);
      return ;
   }
   if (tim < MIN_TIMEOUT) {
      write("The timeout " + tim + " is lower than the minimum allowed "
            "length of " + MIN_TIMEOUT + " days.\n");
      write("Choice: ");
      input_to("discuss_memo_menu_choice", 0, memo);
      return ;
   }
   if (tim > MAX_TIMEOUT) {
      write("The timeout " + tim + " is higher than the maximum allowed "
            "length of " + MAX_TIMEOUT + " days.\n");
      write("Choice: ");
      input_to("discuss_memo_menu_choice", 0, memo);
      return ;
   }
   memo->timeout = tim;
   discuss_memo_menu(memo);
} /* discuss_memo_timeout_response() */

/** @ignore yes */
protected void discuss_memo_finish_description(string desc,
                                               class memo_item memo) {
   if (!desc) {
      write("Not changing the description.\n");
   } else {
      memo->description = desc;
   }
   discuss_memo_menu(memo);
} /* discuss_memo_finish_description() */

/** @ignore yes */
protected void discuss_memo_description(class memo_item memo) {
   if (memo->description) {
      write("Edit your description.\n");
   } else {
      write("Add your description.\n");
   }
   this_player()->do_edit(memo->description,
                          (: discuss_memo_finish_description :),
                          0,
                          0,
                          memo);
} /* discuss_memo_description() */

/** @ignore yes */
protected void discuss_memo_save(class memo_item memo) {
   if (!memo->subject) {
      write("The memo must have a subject to save it!\n");
      write("Choice: ");
      input_to("discuss_memo_menu_choice", 0, memo);
      return ;
   }
   if (sizeof(memo->choices) < 2) {
      write("The memo must have at least two choices!  Currently you have " +
            sizeof(memo->choices) + ".\n");
      write("Choice: ");
      input_to("discuss_memo_menu_choice", 0, memo);
      return ;
   }

   if (CLUB_HANDLER->add_discussion_item(memo->club_name,
                                         CLUB_DISCUSS_MEMO | memo->open,
                                         this_player()->query_name(),
                                         memo->subject,
                                         memo->description,
                                         memo->choices,
                                         memo->timeout * 24 * 60 * 60,
                                         0)) {
      write("Successfuly added the memo item.\n");
   } else {
      write("Unable to add the memo item for some reason.\n");
   }
} /* discuss_memo_save() */

/** @ignore yes */
protected void discuss_memo_menu_choice(string str, class memo_item memo) {
   str = lower_case(str);
   if (!strlen(str)) {
      write("Invalid choice\nChoice: ");
      input_to("discuss_memo_menu_choice", 0, memo);
      return ;
   }

   switch (str[0]) {
      case 'a' :
         discuss_memo_subject(memo);
         break;
      case 'b' :
         memo->choices = ({ "yes", "no", "abstain" });
         discuss_memo_menu(memo);
         break;
      case 'c' :
         discuss_memo_add_choice(memo);
         break;
      case 'd' :
         discuss_memo_remove_choice(memo);
         break;
      case 'e' :
         discuss_memo_timeout(memo);
         break;
      case 'f' :
         discuss_memo_description(memo);
         break;
      case 'g' :
         memo->description = 0;
         write("Description zapped.\n");
         discuss_memo_menu(memo);
         break;
      case 'q' :
         write("Quiting memo addition!\n");
         break;
      case 's' :
         discuss_memo_save(memo);
         break;
      case 'm' :
         discuss_memo_menu(memo);
         break;
      default :
         write("Invalid choice\nChoice: ");
         input_to("discuss_memo_menu_choice", 0, memo);
         break;
   }
} /* discuss_meno_menu_choice() */

/**
 * This is the main entry point for setting up a memo discussion type.
 * A memo is a user run discussion item which has no effect on any
 * real game mechanics.
 * @see do_discuss()
 */
protected int do_discuss_memo(int op, string club_name) {
   discuss_memo_menu(new(class memo_item, choices : ({ }),
                                          timeout : 14,
                                          club_name : club_name,
                                          open : op));
   return 1;
} /* do_discuss_memo() */

/**
 * The main entry point for all the discussion item types.  Figures out
 * which discussion item it is and calls the correct method to start the
 * ball rolling.
 * @param type the type of the discussion item
 * @return 1 on success, 0 on failure
 * @see /obj/handlers/club_handler.c
 * @see do_discuss_description()
 * @see do_discuss_add_position()
 * @see do_discuss_change_position_name()
 * @see do_discuss_memo()
 */
int do_discuss(int open, string type, string club_name) {
   if (open) {
      open = CLUB_DISCUSS_OPEN;
   }

   if (!CLUB_HANDLER->is_club(club_name)) {
      add_failed_mess("No such club '" + club_name + "'.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_elected_club(club_name)) {
      add_failed_mess("The club " + CLUB_HANDLER->query_club_name(club_name) +
                      " is not an elected club.\n");
      return 0;
   }

   if (!CLUB_HANDLER->holds_any_position_of(club_name, this_player()->query_name())) {
      if (type != "no confidence" &&
          type != "remove member") {
         add_failed_mess("Non elected officals can only add discussion items "
                         "of 'no confidence' and 'remove member'.\n");
         return 0;
      }
   }

   switch (type) {
      case "description" :
         return do_discuss_description(open, club_name);
      case "add position" :
         return do_discuss_add_position(open, club_name);
      case "change position name" :
         return do_discuss_change_position_name(open, club_name);
      case "memo" :
         return do_discuss_memo(open, club_name);
      case "remove member" :
         return do_discuss_remove_member(open, club_name);
      case "secret" :
         return do_discuss_secret(open, club_name);
      case "remove position" :
         return do_discuss_remove_position(open, club_name);
      case "no confidence" :
         return do_discuss_no_confidence(open, club_name);
      case "create account" :
         return do_discuss_create_account(open, club_name);
      case "remove account" :
         return do_discuss_remove_account(open, club_name);
      default :
         add_failed_mess("Horrible, horrible error.  The worms!  "
                         "The worms!\n");
         return 0;
   }
} /* do_discuss() */
