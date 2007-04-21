/* -*- LPC -*- */
/**
 * This room allows people to create clubs and do fluffy club stuff if
 * they need to.
 * @author Pinkfish
 * @started Sun Oct  4 02:24:46 EDT 1998
 */
#include <clubs.h>
#include <money.h>
#include <move_failures.h>
#include <player_handler.h>

inherit "/std/basic/club_listing";
inherit "/std/room/inherit/club_discuss";

private int _percentage_cut;
private int _room_type;
private string _region;

protected int do_create(string name);
protected int do_create_family(string name);
protected int do_replace(string name);
protected int do_vote(string name, string position, string club);
protected int do_nominate(string name, string position, string club);
protected int do_balance(string name);
protected int do_deposit(string mon, string name, string account, string reason);
protected int do_change(string name, string type);
protected int do_change_option(string name, string type);
protected int do_disband(string name);
protected int do_insignia(string number, int num);
protected int do_recruiter_add(string name, string recruit);
protected int do_recruiter_remove(string name, string recruit);
protected int do_financials(string name, string account);
protected int do_describe(string name);
protected int do_club_list_member(string name);
protected int do_withdrawl(string money, string club, string account, string reason);
protected int do_transfer(string money, string club, string from_account,
                          string to_account, string reason);
protected int do_force_election(string name);
protected void finish_description(string describe, string name);
protected int do_announcement(string name);
private int check_member_func(string club, string person, string mem);

// External defintions.
mixed query_property(string name);
void add_help_file(string name);

void create() {
   _percentage_cut = 5;
   _room_type = CLUB_ROOM_CLUB_OR_FAMILY;
} /* create() */

private int club_commands_allowed() {
   return _room_type != CLUB_ROOM_FAMILY_ONLY;
} /* club_commands_allowed() */

private int family_commands_allowed() {
   return _room_type != CLUB_ROOM_CLUB_ONLY;
} /* club_commands_allowed() */

/** @ignore yes */
void init() {
   string bit;

   if (club_commands_allowed()) {
      add_help_file("club_control_room");
      bit = "club name";
      add_command("create", "club called <string'club name'>",
                  (: do_create($4[0]) :));
      add_command("list", "clubs",
                  (: club_list(this_player()->query_name(), 0, 0) :) );
      add_command("list", "clubs [with member] <string'member name'>",
                  (: do_club_list_member($4[0]) :));
      add_command("list", "club <string'club name'>",
                  (: club_info($4[0], 1, this_player()->query_name()) :) );
      add_command("vote",
                  "<word'person'> for <string:small'position'> in <string'club name'>",
                  (: do_vote($4[0], $4[1], $4[2]) :));
      add_command("nominate",
                  "<word'person'> for <string:small'position'> in <string'club name'>",
                  (: do_nominate($4[0], $4[1], $4[2]) :));
      add_command("disband", "<string'club name'>", (: do_disband($4[0]) :));
      add_command("change", "<string'club name'> to {elected}",
                   (: do_change($4[0], "elected") :) );
      add_command("change", "<string'club name'> option {secret|open}",
                      (: do_change_option($4[0], $4[1]) :) );
      add_command("replace", "<number> insignia for <string'club name'>",
                      (: do_insignia($4[1], $4[0]) :));
      add_command("replace", "insignia for <string'club name'>",
                      (: do_insignia($4[0], 1) :));
      add_command("recruiter", "add <string'member'> for <string'club'>",
                      (: do_recruiter_add($4[1], $4[0]) :));
      add_command("recruiter", "remove <string'member'> for <string'club'>",
                      (: do_recruiter_remove($4[1], $4[0]) :));
      add_command("financials", "<string'club'>",
                      (: do_financials($4[0], CLUB_DEFAULT_ACCOUNT_NAME) :));
      add_command("financials", "<string'club'> account <string'account name'>",
                      (: do_financials($4[0], $4[1]) :));
      add_command("description", "of club <string'club name'>",
                   (: do_describe($4[0]) :));
      add_command("announce", "for club <string'club name'>",
                  (: do_announcement($4[0]) :));
      add_command("withdraw", "<string'money'> from <string'club name'> for <string:small'reason'>",
                  (: do_withdrawl($4[0], $4[1], CLUB_DEFAULT_ACCOUNT_NAME, $4[2]) :));
      add_command("withdraw", "<string'money'> from <string'club name'> for <string:quoted'reason'>",
                  (: do_withdrawl($4[0], $4[1], CLUB_DEFAULT_ACCOUNT_NAME, $4[2]) :));
      add_command("withdraw", "<string'money'> from <string'club name'> account <string'account name'> for <string:small'reason'>",
                  (: do_withdrawl($4[0], $4[1], $4[2], $4[3]) :));
      add_command("withdraw", "<string'money'> from <string'club name'> account <string'account name'> for <string:quoted'reason'>",
                  (: do_withdrawl($4[0], $4[1], $4[2], $4[3]) :));
      add_command("transfer", "<string'money'> from <string'club name'> account <string'source account'> to <string'desination account'> for <string:small'reason'>",
                  (: do_transfer($4[0], $4[1], $4[2], $4[3], $4[4]) :));
      add_command("transfer", "<string'money'> from <string'club name'> account <string'source account'> to <string'desination account'> for <string:quoted'reason'>",
                  (: do_transfer($4[0], $4[1], $4[2], $4[3], $4[4]) :));
      add_command("election", "force in <string'club'>",
                  (: do_force_election($4[0]) :));
      club_discuss::init();
   }

   if (family_commands_allowed()) {
      add_help_file("family_control_room");
      if (bit) {
         bit += "|family name";
      } else {
         bit = "family name";
      }
      add_command("create", "family called <string'family name'>",
                  (: do_create_family($4[0]) :));
      add_command("list", "families",
                  (: club_list(this_player()->query_name(), 1, 0) :) );
      add_command("list", "family <string'family name'>",
                  (: family_info($4[0], this_player()->query_name()) :) );
   }
   add_command("description", "of <string'" + bit + "'>",
                (: do_describe($4[0]) :));
   add_command("balance", "[of] <string'" + bit  +"'>",
               (: do_balance($4[0]) :));
   add_command("deposit", "<string:small'money'> to <string'" + bit + "'>",
               (: do_deposit($4[0], $4[1], CLUB_DEFAULT_ACCOUNT_NAME, 0) :));
   add_command("deposit", "<string:small'money'> to <string'" + bit + "'> for <string:small'reason'>",
               (: do_deposit($4[0], $4[1], CLUB_DEFAULT_ACCOUNT_NAME, $4[2]) :));
   add_command("deposit", "<string:small'money'> to <string'" + bit + "'> account <string'account name'>",
               (: do_deposit($4[0], $4[1], $4[2], 0) :));
   add_command("deposit", "<string:small'money'> to <string'" + bit + "'> account <string'account name'> for <string:small'reason'>",
               (: do_deposit($4[0], $4[1], $4[2], $4[3]) :));
} /* init() */

int is_valid_club_name(string name) {
   int i;

   if (strlen(name) > CLUB_MAX_NAME_LEN) {
      return 0;
   }

   for (i = 0; i < strlen(name); i++) {
      if (!((name[i] >= 'a' && name[i] <= 'z') ||
            (name[i] >= 'A' && name[i] <= 'Z') ||
            name[i] == '\'' ||
            name[i] == ' ' ||
            name[i] == '_')) {
          return 0;
       }
   }
   return 1;
} /* is_valid_club_name() */

/**
 * This method sets the type of the room.  This controls if the room is to 
 * be used for clubs, families or both.
 * @param type the type of the club
 * @see query_club_control_type()
 * @example
 * // Make the room only handle clubs.
 * set_club_control_type(CLUB_ROOM_CLUB_ONLY);
 */
void set_club_control_type(int room_type) {
   _room_type = room_type;
} /* set_club_control_type() */

/**
 * This method queries the type of the room.  This controls if the room is to 
 * be used for clubs, families or both.
 * @return the control type of the room
 * @see set_club_control_type()
 */
int query_club_control_type() {
   return _room_type;
} /* set_club_control_type() */

/**
 * This method sets the region for the club control room.
 * @param region the region for the clubs created here
 */
void set_club_region(string region) {
   _region = region;
} /* set_club_region() */

/**
 * This method returns the region for the club control room.
 * @return the region the clubs are created in
 */
string query_club_region() {
   return _region;
}

/**
 * This method is called to try and create a club of the specified name.
 * All clubs when they first start are personal clubs.  Once they have
 * a certain number of members, they can convert to being an elected
 * type of club if they wish.
 * @param name the name of the club to create
 * @return 1 on success, 0 on failure
 */
protected int do_create(string name) {
   string place;

   /* First check to see if the club already exists. */
   if (CLUB_HANDLER->is_club(name)) {
      add_failed_mess("Cannot create '" + name + "' as the name already "
                      "exists.\n");
      return 0;
   }

   if (!is_valid_club_name(name)) {
      add_failed_mess("The club name '" + name + "' has invalid characters in "
                      "it.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }
   /* Check to make sure they have enough money. */
   if (this_player()->query_value_in(place) < CLUB_STARTUP_COST) {
      add_failed_mess("You need " +
                      MONEY_HAND->money_value_string(CLUB_STARTUP_COST, place) +
                      " to start a club.\n");
      return 0;
   }
   write("It will cost you " + 
         MONEY_HAND->money_value_string(CLUB_STARTUP_COST, place) +
         " to create a club.  Double check the capitalization of " +
         name + ", you cannot change it after it is created.  "
         "Do you wish to continue? ");
   input_to("confirm_create", 0, name);
   return 1;
} /* do_create() */

/**
 * This method will confirm the creation of a club.
 * @param str the yes or no string
 * @param name the name of the club to create
 */
protected void confirm_create(string str, string name) {
   object badge;
   string place;

   place = query_property("place");
   if (!place) {
      place = "default";
   }
   str = lower_case(str);
   if (strlen(str) < 1 ||
       (str[0] != 'n' && str[0] != 'y' && str[0] != 'q')) {
      write("Please answer yes or no.\nDo you wish to continue creating "
            "the club " + name + " for " +
            MONEY_HAND->money_value_string(CLUB_STARTUP_COST, place) +
            "? ");
      input_to("confirm_create", 0, name);
      return ;
   }

   if (str[0] == 'n' || str[0] == 'q') {
      write("Aborted.\n");
      return ;
   }

   /* Ok, confirmed... */
   if (this_player()->query_value_in(place) < CLUB_STARTUP_COST) {
      write("Your money appears to have evaporated.  You need " +
                      MONEY_HAND->money_value_string(CLUB_STARTUP_COST, place) +
                      " to start a club.\nAborting club creation.\n");
      return 0;
   }

   /* Create the club. */
   if (CLUB_HANDLER->create_club(name,
                                 this_player()->query_name(),
                                 CLUB_PERSONAL,
                                 query_club_region())) {
      /* Make them pay! */
      this_player()->pay_money(MONEY_HAND->create_money_array(CLUB_STARTUP_COST,
                                                           place), place);
      write("Created the club called '" + name + "'.\n");
      /* Give them a badge... */
      badge = clone_object(CLUB_BADGE);
      badge->set_club(name);
      if (badge->move(this_player()) == MOVE_OK) {
         if (!this_player()->wear_armour(badge)) {
            write("You notice a badge turn up and wear itself, this is your "
                  "membership badge.\n");
         } else {
            write("You notice a badge turn up and attempt to wear itself, "
                  "this is your membership badge.\n");
         }
      } else {
         badge->move(environment(this_player()));
         badge->set_worn_by(this_player());
         write("You notice a badge turn up and wear itself, this is your "
               "membership badge.\n");
      }
      event(users(), "inform", this_player()->query_cap_name() + 
                     " created the club " + name, "club");
   }
} /* confirm_create() */

/**
 * This method is called to check the validity of a family name.
 * @param name the name of the family.
 * @return 1 if valid, 0 if invalid.
 */
int is_valid_family_name(string name) {
  
  if (PLAYER_HANDLER->test_user(lower_case(name))) {
    add_failed_mess("Cannot use the family name '" + name + "' since "
        "a player is already using it.\n");
    return 0;
  }
  
  if(!PLAYER_HANDLER->test_valid(name)) {
    add_failed_mess("The family name '" + name + "' is invalid.\n");
    return 0;
  }

  if (!is_valid_club_name(name)) {
    if (this_player()->query_creator()) {
      add_failed_mess("invalid.\n");
    }
    add_failed_mess("The family name '" + name + "' has invalid characters "
                    "in it.\n");
    return 0;
  }
  
  if (strlen(name) > MAX_FAMILY_NAME_LEN) {
    if (this_player()->query_creator()) {
      add_failed_mess("too long!\n");
    }
    add_failed_mess("The maximum length of a family name is " +
                    MAX_FAMILY_NAME_LEN + ".\n");
    return 0;
  }

  return 1;
}

/**
 * This method is called to try and create a family of the specified name.
 * @param name the name of the family to create
 * @return 1 on success, 0 on failure
 */
protected int do_create_family(string name) {
   string place;
   
   /* First check to see if the club already exists. */
   if (CLUB_HANDLER->is_club(name)) {
      if (this_player()->query_creator()) {
         write("exist\n");
      }
      add_failed_mess("Cannot create '" + name + "' as the name already "
                      "exists.\n");
      return 0;
   }

   if(!is_valid_family_name(name))
     return 0;
   
   place = query_property("place");
   if (!place) {
      place = "default";
   }
   /* Check to make sure they have enough money. */
   if (this_player()->query_value_in(place) < FAMILY_STARTUP_COST) {
      add_failed_mess("You need " +
                      MONEY_HAND->money_value_string(FAMILY_STARTUP_COST,
                                                     place) +
                      " to start a family.\n");
      return 0;
   }
   write("It will cost you " + 
         MONEY_HAND->money_value_string(FAMILY_STARTUP_COST, place) +
         " to create a family.  Do you wish to continue? ");
   input_to("confirm_create_family", 0, name);
   return 1;
} /* do_create_family() */

/**
 * This method will confirm the creation of a family.
 * @param str the yes or no string
 * @param name the name of the family to create
 */
protected void confirm_create_family(string str, string name) {
   string place;

   place = query_property("place");
   if (!place) {
      place = "default";
   }
   str = lower_case(str);
   if (strlen(str) < 1 ||
       (str[0] != 'n' && str[0] != 'y' && str[0] != 'q')) {
      write("Please answer yes or no.\nDo you wish to continue creating "
            "the family " + name + " for " +
            MONEY_HAND->money_value_string(FAMILY_STARTUP_COST, place) +
            "? ");
      input_to("confirm_create_family", 0, name);
      return ;
   }

   if (str[0] == 'n' || str[0] == 'q') {
      write("Aborted.\n");
      return ;
   }

   /* Ok, confirmed... */
   if (this_player()->query_value_in(place) < FAMILY_STARTUP_COST) {
      write("Your money appears to have evaporated.  You need " +
            MONEY_HAND->money_value_string(FAMILY_STARTUP_COST, place) +
            " to start a family.\nAborting family creation.\n");
      return 0;
   }

   /* Create the family. */
   if (CLUB_HANDLER->create_club(name,
                                 this_player()->query_name(),
                                 CLUB_FAMILY,
                                 query_club_region())) {
      if (this_player()->query_family_name()) {
         CLUB_HANDLER->move_family_member(this_player()->query_family_name(),
                                          this_player()->query_name(),
                                          name);
      } else {
         CLUB_HANDLER->add_member(name, this_player()->query_name());
      }
      /* Make them pay! */
      this_player()->pay_money(MONEY_HAND->create_money_array(FAMILY_STARTUP_COST,
                                                              place), place);
      write("Created the family called '" + name + "'.\n");
      this_player()->set_family_name(name);
      event(users(), "inform", this_player()->query_cap_name() + 
                     " created the family " + name, "club");
   }
} /* confirm_create_family() */

/**
 * This method nominates a person into a position in a club.
 * @param person the person to nominate
 * @param position the position to nominate them too
 * @param club_name the name of the club
 * @return 1 on success, 0 on failure
 * @see /obj/handlers/club_handler.c
 * @see do_vote()
 */
protected int do_nominate(string person, string position, string club_name) {
   string our_name;

   our_name = this_player()->query_name();
   if (!CLUB_HANDLER->is_member_of(club_name, our_name)) {
      add_failed_mess("You can only nominate people in a club your a "
                      "member of.\n");
      return 0;
   }

   person = lower_case(person);     

   if (!CLUB_HANDLER->is_member_of(club_name, person)) {
      person = this_player()->expand_nickname(person);
   }
   if (!CLUB_HANDLER->is_member_of(club_name, person)) {
      add_failed_mess("You can only nominate people in the club.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_election_in_progress(club_name)) {
      add_failed_mess("This club is not having an election right now.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_voting_in_progress(club_name)) {
      add_failed_mess("This club is not accepting nominations right now, "
                      "it is past the nomination phase.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_nomination_in_progress(club_name)) {
      add_failed_mess("This club not in the nomination phase of "
                      "voting.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_valid_position(club_name, position)) {
      add_failed_mess("You must nominate them in a valid position.\n"
                      "Valid positions are " +
                      sort_and_capitalise(CLUB_HANDLER->query_valid_positions(club_name)) +
                      ".\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_nomination_in_progress(club_name)) {
      add_failed_mess("This club is not accepting nominations for positions "
                      "right now.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_creator_club(club_name) &&
       PLAYER_HANDLER->test_creator(person)) {
      add_failed_mess("You cannot nominate a creator to a position in a "
                      "non-creator club.\n");
      return 0;
   }

   if (CLUB_HANDLER->has_nominated_person(club_name,
                                           position,
                                           person,
                                           our_name)) {
      add_failed_mess("You have already nominated " + capitalize(person) +
                      " for the position " + capitalize(position) + " in '" +
                      CLUB_HANDLER->query_club_name(club_name) + "'.\n");
      return 0;
   }

   /* Everything seems in order, try and nominate someone. */
   if (CLUB_HANDLER->nominate_person(club_name,
                                     position,
                                     person,
                                     our_name)) {
      add_succeeded_mess( ({
                      "$N nominated " + capitalize(person) + " to " +
            capitalize(position) + " in " +
            CLUB_HANDLER->query_club_name(club_name) + ".\n",
                      "$N nominated " + capitalize(person) + " to " +
            capitalize(position) + " in " +
            CLUB_HANDLER->query_club_name(club_name) + ".\n" }) );
      if (find_player(lower_case(person)) &&
          environment(find_player(lower_case(person))) != this_object()) {
         tell_object(find_player(lower_case(person)),
                     "You have a weird feeling that " +
                     this_player()->the_short() +
                     " just nominated you for " + capitalize(position) +
                     ".\n");
      }
      return 1;
   }
   add_failed_mess("Unable to nominate " + person + " to " +
                   position + " in '" + 
                   CLUB_HANDLER->query_club_name(club_name) + "'.\n");
   return 0;
} /* do_nominate() */

/**
 * This method attempts to vote for someone in a club.
 * @param person the person to vote for
 * @param position the position to vote for them in
 * @param club_name the name of the club to vote for them in
 * @return 1 on success, 0 on failure
 * @see do_nominate()
 * @see /obj/handlers/club_handler.c
 */
protected int do_vote(string person, string position, string club_name) {
   string our_name;
   string *stuff;

   our_name = this_player()->query_name();
   if (!CLUB_HANDLER->is_member_of(club_name, our_name)) {
      add_failed_mess("You can only vote for people in a club you're a "
                      "member of.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_member_of(club_name, person)) {
      person = this_player()->expand_nickname(person);
   }
   if (!CLUB_HANDLER->is_member_of(club_name, person)) {
      add_failed_mess("You can only vote for people in the club.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_election_in_progress(club_name)) {
      add_failed_mess("This club is not having an election at the moment.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_nomination_in_progress(club_name)) {
      add_failed_mess("This club is accepting nominations right now, "
                      "it is not ready for votes.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_voting_in_progress(club_name)) {
      add_failed_mess("This club is not accepting votes right now.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_valid_position(club_name, position)) {
      add_failed_mess("You must vote for them in a valid position.\n"
                      "Valid positions are " +
                      sort_and_capitalise(CLUB_HANDLER->query_valid_positions(club_name)) +
                      ".\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_nominated_for(club_name, position, person)) {
      stuff =  CLUB_HANDLER->query_nominated_for(club_name, position);
      if (sizeof(stuff)) {
         add_failed_mess("You must vote for a person that has been "
                         "nominated for the position.\n" +
                         sort_and_capitalise(stuff) +
                         " have been nominated for " + position + " in " +
                         club_name + ".\n");
      } else {
         add_failed_mess("No one has been nominated for the position "
                         + position + " in " + club_name + ".\n");
      }
      return 0;
   }

   if (CLUB_HANDLER->has_person_voted(club_name, position, our_name)) {
      add_failed_mess("You can only vote for each position once.\n");
      return 0;
   }

   /* Everything seems in order, try and nominate someone. */
   if (CLUB_HANDLER->vote_for_person(club_name,
                                     position,
                                     person,
                                     our_name)) {
      add_succeeded_mess(({
                      "$N vote for $C$" + person + " to " + position + " in " +
            CLUB_HANDLER->query_club_name(club_name) + ".\n",
                      "$N voted for " + position + " in " +
            CLUB_HANDLER->query_club_name(club_name) + ".\n" }));
      return 1;
   }
   add_failed_mess("Unable vote for " + person + " to " +
                   position + " in " + club_name +
            CLUB_HANDLER->query_club_name(club_name) + ".\n");
   return 0;
} /* do_vote() */

/**
 * This method prints the balance of this club.
 * @param name the name of the club to get the balance of
 * @return 1 on success, 0 on failure
 * @see do_deposit()
 */
protected int do_balance(string name) {
   string place;
   int amount;
   string bit;
   string *accounts;
   string account;
   string account_name;
   string ret;

   if (club_commands_allowed()) {
      bit = "club";
   }
   if (family_commands_allowed()) {
      if (bit) {
         bit += " or family";
      } else {
         bit = "family";
      }
   }

   if (!CLUB_HANDLER->is_club(name)) {
      add_failed_mess("You must get a balance of an existing " + bit + ".\n");
      return 0;
   }

   if (CLUB_HANDLER->is_family(name)) {
      bit = "family";
   } else {
      bit = "club";
   }

   if (!CLUB_HANDLER->is_member_of(name, this_player()->query_name())) {
      add_failed_mess("You must be a member of the " + bit +
                      " to get the balance.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   accounts = CLUB_HANDLER->query_account_names(name);
   ret = "";
   foreach (account in accounts) {
      if (account == CLUB_DEFAULT_ACCOUNT_NAME) {
         account_name = "main " + bit + " account";
      } else {
         account_name = "special " + bit + " account " + account;
      }
      amount = CLUB_HANDLER->query_balance(name, account);
      if (amount < 0) {
         ret += "The balance of the " + account_name + " in '" +
                CLUB_HANDLER->query_club_name(name) +
                "' is in remission by " +
                MONEY_HAND->money_value_string(-amount, place) + ".\n";
      } else {
         ret += "The balance of the " + account_name + " in '" +
                CLUB_HANDLER->query_club_name(name) + "' is " +
                MONEY_HAND->money_value_string(amount, place) + ".\n";
      }
   }
   ret += "The next pay period ends at " +
          ctime(CLUB_HANDLER->query_time_fees_due(name)) +
          ", which will cost " +
          MONEY_HAND->money_value_string(CLUB_HANDLER->query_club_cost_per_period(name), place) + " from the main " + bit + " account.\n",
   add_succeeded_mess(({ ret,
                      "$N checks the balance of '" +
                      CLUB_HANDLER->query_club_name(name) + "'.\n" }));
   return 1;
} /* do_balance() */

/**
 * This method allows to you deposit money into the club.
 * @param obs the money objects to deposit
 * @param name the name of the club to deposit them to
 * @param reason the reason of the deposit
 * @return 1 on success, 0 on failure
 * @see do_balance()
 * @see query_percentage_cut()
 * @see set_percentage_cut()
 */
protected int do_deposit(string mon, string name, string account, string reason) {
   int amt;
   int after;
   int total;
   string place;
   mixed money;
   string bit;
   string account_name;

   if (club_commands_allowed()) {
      bit = "club";
   }
   if (family_commands_allowed()) {
      if (bit) {
         bit += " or family";
      } else {
         bit = "family";
      }
   }

   if (!CLUB_HANDLER->is_club(name)) {
      add_failed_mess("You must deposit into an existing " + bit + ".\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_member_of(name, this_player()->query_name())) {
      add_failed_mess("You must be a member of the " + bit +
                      " to deposit into the clubs coffers.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_creator_club(name) &&
       this_player()->query_creator()) {
      add_failed_mess("Creators can only deposit to creator " + bit + "s.\n");
      return 0;
   }

   if (account == CLUB_DEFAULT_ACCOUNT_NAME) {
      account_name = "main " + bit + " account";
   } else {
      account_name = "special " + bit + " account " + account;
   }

   if (!CLUB_HANDLER->is_account_of(name, account)) {
      add_failed_mess("The account " + account_name + " is not in the " +
                      bit + " '" + CLUB_HANDLER->query_club_name(name) + "'\n");
      return 0;
   }

   if (strlen(reason) > 60) {
      add_failed_mess("The reason must be less than 60 characters.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   money = MONEY_HAND->parse_money(mon, this_player(), place);
   if (intp(money)) {
      switch (money) {
         case NO_MATCH :
            add_failed_mess("You do not have '" + mon + "'.\n");
            return 0;
         case NO_MONEY :
            add_failed_mess("You can only deposit money.\n");
            return 0;
         default :
            add_failed_mess("You can only deposit legal tender.\n");
            return 0;
      }
   }

   amt = money->query_value_in(place);
   if (place != "default") {
      amt += money->query_value_in("default");
   }
   after = amt - ((amt * _percentage_cut) / 100);
   CLUB_HANDLER->add_money(name,
                           after,
                           CLUB_TRANSACTION_DEPOSIT,
                           this_player()->query_name(),
                           account,
                           reason);
   total = CLUB_HANDLER->query_balance(name, account);
   money->move("/room/rubbish");
   add_succeeded_mess(({ "$N deposit " +
                         MONEY_HAND->money_value_string(amt, place) +
                         ", which after expenses is " +
                         MONEY_HAND->money_value_string(after, place) +
                         ", to the " +
                         (CLUB_HANDLER->is_family(name) ? "family" : "club") +
                         " '" +
                         CLUB_HANDLER->query_club_name(name) +
                         "' for a total of " +
                         MONEY_HAND->money_value_string(total, place) + ".\n",
                         "$N deposits some money to the " +
                         (CLUB_HANDLER->is_family(name) ? "family" : "club") +
                         " '" +
                         CLUB_HANDLER->query_club_name(name) +
                         "'.\n" }));

   return 1;
} /* do_deposit() */

/**
 * This method is used when someone wishes to withdraw some money from the
 * club account.
 * @param money the amount of money to withdraw
 * @param club the club to withdraw the money from
 * @param account the account to withdraw money from
 * @param reason the reason to withdraw money from the account
 * @return 1 if suecceded, 0 if failed.
 */
protected int do_withdrawl(string money,
                           string name,
                           string account,
                           string reason) {
   string place;
   int value;
   string bit;
   object obj;
   string account_name;
   string* stuff;
   string ret;

   if (club_commands_allowed()) {
      bit = "club";
   }

   if (!CLUB_HANDLER->is_club(name)) {
      add_failed_mess("You must withdraw from an existing " + bit + ".\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_member_of(name, this_player()->query_name())) {
      add_failed_mess("You must be a member of the " + bit +
                      " to withdraw money from the club.\n");
      return 0;
   }

   if (account == CLUB_DEFAULT_ACCOUNT_NAME) {
      account_name = "main " + bit + " account";
   } else {
      account_name = "special " + bit + " account " + account;
   }

   if (!CLUB_HANDLER->is_account_of(name, account)) {
      add_failed_mess("The account " + account_name + " is not in the " +
                      bit + " '" + CLUB_HANDLER->query_club_name(name) + "'\n");
      return 0;
   }

   if (strlen(reason) > 60) {
      add_failed_mess("The reason must be less than 60 characters.\n");
      return 0;
   }

    if(!CLUB_HANDLER->is_elected_club(name)) {
        add_failed_mess("You may only withdraw funds from an elected "
            "club.\n");   
        return 0;
    }

   if (!CLUB_HANDLER->is_allowed_to_modify_account(name,
                                                   account,
                                                   this_player()->query_name())) {
      stuff = CLUB_HANDLER->query_account_allowed_positions(name, account);
      ret = "Only the positions " + query_multiple_short(stuff);
      stuff = CLUB_HANDLER->query_account_allowed_people(name, account);
      if (sizeof(stuff)) {
         ret += " and the people " + query_multiple_short(stuff);
      }
      
      add_failed_mess(ret + " can withdraw from this account.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   value = MONEY_HAND->value_from_string(money, place);
   if (value > CLUB_HANDLER->query_balance(name, account)) {
      add_failed_mess("The " + account_name +
                      " does not have that much money!\n");
      return 0;
   }
   obj = MONEY_HAND->make_new_amount(value, place);
   if (!obj) {
      add_failed_mess("Unable to create the money object.\n");
      return 0;
   }
 
   if (CLUB_HANDLER->remove_money(name,
                              value,
                              CLUB_TRANSACTION_WITHDRAWL,
                              this_player()->query_name(),
                              account,
                              reason)) {
      obj->move(this_player());
      add_succeeded_mess(({ "$N withdraw " +
                            MONEY_HAND->money_value_string(value, place) +
                            " from the " +
                            (CLUB_HANDLER->is_family(name) ? "family" : "club") +
                            " '" +
                            CLUB_HANDLER->query_club_name(name) +
                            "'.\n",
                            "$N withdraws some money from the " +
                            (CLUB_HANDLER->is_family(name) ? "family" : "club") +
                            " '" +
                            CLUB_HANDLER->query_club_name(name) +
                            "'.\n" }));

      return 1;   
   } else {
      obj->dest_me();
      add_failed_mess("Unable to remove the money from the account.\n");
      return 0;
   }
} /* do_withdrawl() */

/**
 * This method is used when someone wishes to withdraw some money from the
 * club account.
 * @param money the amount of money to withdraw
 * @param club the club to withdraw the money from
 * @param from_account the account to withdraw money from
 * @param to_account the account to deposit the money to
 * @param reason the reason to withdraw money from the account
 * @return 1 if suecceded, 0 if failed.
 */
protected int do_transfer(string money,
                           string name,
                           string from_account,
                           string to_account,
                           string reason) {
   string place;
   int value;
   string bit;
   object obj;
   string from_account_name;
   string to_account_name;
   string* stuff;
   string ret;

   if (club_commands_allowed()) {
      bit = "club";
   }

   if (!CLUB_HANDLER->is_club(name)) {
      add_failed_mess("You must transfer money in an existing " + bit + ".\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_member_of(name, this_player()->query_name())) {
      add_failed_mess("You must be a member of the " + bit +
                      " to transfer money.\n");
      return 0;
   }

   if (from_account == CLUB_DEFAULT_ACCOUNT_NAME) {
      from_account_name = "main " + bit + " account";
   } else {
      from_account_name = "special " + bit + " account " + from_account;
   }

   if (to_account == CLUB_DEFAULT_ACCOUNT_NAME) {
      to_account_name = "main " + bit + " account";
   } else {
      to_account_name = "special " + bit + " account " + to_account;
   }

   if (!CLUB_HANDLER->is_account_of(name, from_account)) {
      add_failed_mess("The account " + from_account_name + " is not in the " +
                      bit + " '" + CLUB_HANDLER->query_club_name(name) + "'\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_account_of(name, to_account)) {
      add_failed_mess("The account " + to_account_name + " is not in the " +
                      bit + " '" + CLUB_HANDLER->query_club_name(name) + "'\n");
      return 0;
   }

   if (strlen(reason) > 60) {
      add_failed_mess("The reason must be less than 60 characters.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_allowed_to_modify_account(name,
                                                   from_account,
                                                   this_player()->query_name())) {
      stuff = CLUB_HANDLER->query_account_allowed_positions(name, from_account);
      ret = "Only the positions " + query_multiple_short(stuff);
      stuff = CLUB_HANDLER->query_account_allowed_people(name, from_account);
      if (sizeof(stuff)) {
         ret += " and the people " + query_multiple_short(stuff);
      }
      
      add_failed_mess(ret + " can withdraw from this account.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   value = MONEY_HAND->value_from_string(money, place);
   if (value > CLUB_HANDLER->query_balance(name, from_account)) {
      add_failed_mess("The " + from_account_name +
                      " does not have that much money!\n");
      return 0;
   }
 
   if (CLUB_HANDLER->remove_money(name,
                              value,
                              CLUB_TRANSACTION_TRANSFER,
                              this_player()->query_name(),
                              from_account,
                              "to " + to_account_name + ": " + reason)) {
      CLUB_HANDLER->add_money(name,
                              value,
                              CLUB_TRANSACTION_TRANSFER,
                              this_player()->query_name(),
                              to_account,
                              "from " + from_account_name + ": " + reason);
      add_succeeded_mess(({ "$N transfer " +
                            MONEY_HAND->money_value_string(value, place) +
                            " between the " +
                            (CLUB_HANDLER->is_family(name) ? "family" : "club") +
                            " '" +
                            CLUB_HANDLER->query_club_name(name) +
                            "' accounts " + from_account_name + " and " +
                            to_account_name + ".\n",
                            "$N transfers some money between accounts in the " +
                            (CLUB_HANDLER->is_family(name) ? "family" : "club") +
                            " '" +
                            CLUB_HANDLER->query_club_name(name) +
                            "'.\n" }));

      return 1;   
   } else {
      obj->dest_me();
      add_failed_mess("Unable to remove the money from the account.\n");
      return 0;
   }
} /* do_withdrawl() */

private void do_change_inform(string club_name) {
   object *people;
   object person;

   people = map(CLUB_HANDLER->query_members(club_name), (: find_player :));
   people -= ({ 0 });
   foreach (person in people) {
      all_inventory(person)->event_club_change(this_player(), club_name);
   }
} /* do_change_inform() */

/**
 * This allows the club to change into a new type.
 * @param name the name of the club to change
 * @param type the type to change the club into
 * @see do_create()
 * @see do_disband()
 */
protected int do_change(string name, string type) {
   int ret;

   if (!CLUB_HANDLER->is_club(name)) {
      add_failed_mess("You must change a club that exists.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_personal_club(name)) {
      add_failed_mess("The club must be a founder type before you can change "
                      "it to another type.\n");
      return 0;
   }

   switch (type) {
      case "elected" :
         if (CLUB_HANDLER->is_founder_of(name, this_player()->query_name())) {
            if (sizeof(CLUB_HANDLER->query_members(name)) < 
                CLUB_MINIMUM_ELECTED_MEMBERS) {
               add_failed_mess("You must have at least " +
                               query_num(CLUB_MINIMUM_ELECTED_MEMBERS) +
                               " members to change to an elected club.\n");
            } else if (CLUB_HANDLER->change_club_type(name, CLUB_ELECTED)) {
               ret = 1;
               add_succeeded_mess("$N change$s the type of '" +
                                  CLUB_HANDLER->query_club_name(name) +
                                  "' to elected.\n");
               // Inform all the people who were in the club's badges...
               call_out((: do_change_inform :), 0, name);
            } else {
               add_failed_mess("You are unable to change the club type.\n");
            }
         } else {
            add_failed_mess("You must be the founder to change the "
                            "club type.\n");
         }
         break;
   }
   return ret;
} /* do_change() */

/**
 * This allows the club to change its options.
 * @param name the name of the club to change
 * @param option the option to change on the club
 * @see do_create()
 * @see do_disband()
 * @see do_change()
 */
protected int do_change_option(string name, string option) {
   int ret;

   if (!CLUB_HANDLER->is_club(name)) {
      add_failed_mess("You must change a club that exists.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_elected_club(name)) {
      add_failed_mess("You must change options through discussion items.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_personal_club(name) &&
       !CLUB_HANDLER->is_founder_of(name, this_player()->query_name())) {
      add_failed_mess("You must be the founder of the club to change the "
                      "options of the club.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_personal_club(name)) {
      add_failed_mess("The club must be a personal club before the options "
                      "can be changed in this way.\n");
      return 0;
   }

   switch (option) {
      case "secret" :
         if (CLUB_HANDLER->set_club_secret(name)) {
            add_succeeded_mess("$N wipe$s the records on the club '" +
                            CLUB_HANDLER->query_club_name(name) + "'.\n");
            ret = 1;
         } else {
            add_failed_mess("Unable to change the club to a secret type.\n");
         }
         break;
      case "open" :
         if (CLUB_HANDLER->reset_club_secret(name)) {
            add_succeeded_mess("$N replace$s the records of the club '" +
                            CLUB_HANDLER->query_club_name(name) + "'.\n");
            ret = 1;
         } else {
            add_failed_mess("Unable to change the club to an open type.\n");
         }
         break;
   }
   return ret;
} /* do_change() */

/**
 * This method will forcibly disband the club.  This can only be done
 * for founder type clubs.
 * @param name the name of the club to disband
 * @see do_change()
 * @see do_create()
 * @see disband_confirm()
 */
protected int do_disband(string name) {
   if (!CLUB_HANDLER->is_club(name)) {
      add_failed_mess("You must disband a club that exists.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_elected_club(name)) {
      add_failed_mess("You cannot disband an elected club.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_personal_club(name)) {
      add_failed_mess("The club must be a personal club type for you to "
                      "be able to disband it.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_founder_of(name, this_player()->query_name())) {
      add_failed_mess("You must be the founder of the club to disband it.\n");
      return 0;
   }

   write("Are you sure you wish to disband the club '" +
         CLUB_HANDLER->query_club_name(name) +
         "'? ");
   input_to("disband_confirm", 0, name);
   add_succeeded_mess("");
   return 1;
} /* do_disband() */

/**
 * This method is called to confirm the disbanding of the club.
 * @param str the response the user type
 * @param name the name of the club to disband
 * @see do_disband()
 * @see do_create()
 * @see create_confirm()
 */
protected void disband_confirm(string str, string name) {
   string club_name;

   str = lower_case(str);
   if (str == "" ||
       str[0] == 'n' ||
       str[0] == 'q') {
      write("Canceling disbanding of the club '" + name + "'.\n");
      return ;
   }
   if (str[0] == 'y') {
      if (!CLUB_HANDLER->is_club(name)) {
         write("The club appears to have been disbanded already.\n");
         return 0;
      }
   
      if (CLUB_HANDLER->is_elected_club(name)) {
         write("The club has changed to an elected club and you cannot "
               "disband it.\n");
         return 0;
      }
   
      if (!CLUB_HANDLER->is_personal_club(name)) {
         write("The club must be a personal club for you to be able to "
                         "disband it.\n");
         return 0;
      }

      if (!CLUB_HANDLER->is_founder_of(name, this_player()->query_name())) {
         write("You must be the founder of the club to disband it.\n");
         return 0;
      }

      club_name = CLUB_HANDLER->query_club_name(name);
      CLUB_HANDLER->disband_club(name);
      write("You disband the club '" + club_name + "'.\n");
      say(this_player()->the_short() + " disbands the club '" +
                      club_name + "'.\n");
   } else {
      write("You must answer yes or no.\n");
      write("Are you sure you wish to disband the club '" +
            CLUB_HANDLER->query_club_name(name) +
            "'? ");
      input_to("disband_confirm", 0, name);
   }
} /* disband_confirm() */

/**
 * This method will create a bunch of insignia for the club.
 * @param number the number of insignia to create
 * @param name the name of the club to create insignia for
 */
protected int do_insignia(string name, int num) {
   int i;
   object *insignias;
   object ob;
   string place;

   if (!CLUB_HANDLER->is_club(name)) {
      add_failed_mess("You must create the insignia for a club that exists.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_family(name)) {
      add_failed_mess("You cannot create insignia for a family.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_member_of(name, this_player()->query_name())) {
      add_failed_mess("You must be a member of the club to create the "
                      "insignia for it.\n");
      return 0;
   }

   if (num > 1 &&
       !CLUB_HANDLER->is_recruiter_of(name, this_player()->query_name())) {
      add_failed_mess("You must be a recruiter to ask for more than one "
                      "insignia.\n");
      return 0;
   }

   if (num > 20) {
      add_failed_mess("You cannot ask for more than 20 insignia.\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }
   /* Check to make sure they have enough money. */
   if (this_player()->query_value_in(place) < (num * CLUB_COST_INSIGNIA)) {
      add_failed_mess("You need " +
                      MONEY_HAND->money_value_string(CLUB_COST_INSIGNIA * num, place) +
                      " to pay for " +
                      query_num(num) + " insignia.\n");
      return 0;
   }

   insignias = ({ });
   for (i = 0; i < num; i++) {
      ob = clone_object(CLUB_HANDLER->query_insignia_path(name));
      ob->set_cond(ob->query_max_cond());
      ob->set_club(name);
      if (ob->move(this_player()) == MOVE_OK) {
         insignias += ({ ob });
         this_player()->pay_money(MONEY_HAND->create_money_array(CLUB_COST_INSIGNIA,
                                                           place), place);
      } else {
         ob->dest_me();
      }
   }

   if (!sizeof(insignias)) {
      add_failed_mess("Unable to move the insignia into your inventory, "
                      "insignia not created.\n");
      return 0;
   } else {
      add_succeeded_mess("$N create$s " + query_num(sizeof(insignias)) +
                         " club '" + CLUB_HANDLER->query_club_name(name) +
                         "' insignia.\n");
      return 1;
   }
} /* do_insignia() */

/**
 * This method attempts to add a recruiter to the club.
 * @param name the name of the club
 * @param recruit the person to add as a recruiter
 */
protected int do_recruiter_remove(string name, 
                                  string recruit) {
   string *all_names;
   string one_name;

   if (!CLUB_HANDLER->is_club(name)) {
      add_failed_mess("You must remove a recruiter to a club that exists.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_personal_club(name)) {
      if (!CLUB_HANDLER->is_founder_of(name, this_player()->query_name())) {
         add_failed_mess("Only the founder can remove recruiters.\n");
         return 0;
      }
   } else if (CLUB_HANDLER->is_elected_club(name)) {
      if (!CLUB_HANDLER->is_president_of(name, this_player()->query_name()) &&
          !CLUB_HANDLER->is_vice_president_of(name, this_player()->query_name())) {
         add_failed_mess("Only the president and vice president can remove " 
                         "recruiters.\n");
         return 0;
      }
   } else {
      add_failed_mess("Unknown club type.\n");
      return 0;
   }

   all_names = map(explode(recruit, ","),
                   (: this_player()->expand_nickname($1) :));
   if (recruit == "all") {
      all_names = CLUB_HANDLER->query_recruiters(name);
   }

   foreach (one_name in all_names) {
      if (!CLUB_HANDLER->is_recruiter_of(name, one_name)) {
         add_failed_mess("The person(s) you are removeing must be a recruiter "
                         " of the club.\n");
         all_names -= ({ name });
         continue;
      }
      CLUB_HANDLER->remove_recruiter(name, one_name);
      one_name = lower_case(one_name);
      if (find_player(one_name) &&
          environment(find_player(one_name)) != this_object()) {
         tell_object(find_player(one_name), "You feel suddenly feel like "
                     "you are unable to "
                     "recruit people into the club '" +
                     CLUB_HANDLER->query_club_name(name) +
                     "'.\n");
      }
   }

   if (!sizeof(all_names)) {
      return 0;
   }

   add_succeeded_mess("$N remove$s " + query_multiple_short(all_names) + 
                      " as " +
                      (sizeof(all_names) > 1 ? "recruiters " : "a recruiter ") +
                      "to the club '" + CLUB_HANDLER->query_club_name(name) +
                      "'.\n");
   return 1;
} /* do_recruiter_remove() */

/**
 * This method attempts to add a recruiter to the club.
 * @param name the name of the club
 * @param recruit the person to add as a recruiter
 */
protected int do_recruiter_add(string name, string recruit) {
   string *all_names;
   string one_name;

   if (!CLUB_HANDLER->is_club(name)) {
      add_failed_mess("You must add a recruiter to a club that exists.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_personal_club(name)) {
      if (!CLUB_HANDLER->is_founder_of(name, this_player()->query_name())) {
         add_failed_mess("Only the founder can add recruiters.\n");
         return 0;
      }
   } else if (CLUB_HANDLER->is_elected_club(name)) {
      if (!CLUB_HANDLER->is_president_of(name, this_player()->query_name()) &&
          !CLUB_HANDLER->is_vice_president_of(name, this_player()->query_name())) {
         add_failed_mess("Only the president and vice president can add "
                         "recruiters.\n");
         return 0;
      }
   } else {
      add_failed_mess("Unknown club type.\n");
      return 0;
   }

   all_names = map(explode(recruit, ","),
                   (: this_player()->expand_nickname($1) :));
   if (recruit == "all") {
      all_names = CLUB_HANDLER->query_members(name);
   }
   
   foreach (one_name in all_names) {
      if (!CLUB_HANDLER->is_member_of(name, one_name)) {
         add_failed_mess("The person you are making a recruiter must be a "
                         "member of the club.\n");
         all_names -= ({ one_name });
         continue;
      }

      if (CLUB_HANDLER->is_recruiter_of(name, one_name)) {
         add_failed_mess("The person you are making a recruiter is already a "
                         "recruiter of the club.\n");
         all_names -= ({ one_name });
         continue;
      }


      CLUB_HANDLER->add_recruiter(name, one_name);
      one_name = lower_case(one_name);
      if (find_player(one_name) &&
          environment(find_player(one_name)) != this_object()) {
         tell_object(find_player(one_name), "You feel suddenly empowered to "
                     "recruit people into the club '" +
                     CLUB_HANDLER->query_club_name(name) +
                     "'.\n");
      }
   }

   if (!sizeof(all_names)) {
      return 0;
   }

   add_succeeded_mess("$N add$s " + query_multiple_short(all_names) + 
                      (sizeof(all_names) == 1? " as a recruiter " : " as recruiters ") +
                      "to the club '" + CLUB_HANDLER->query_club_name(name) +
                      "'.\n");
   return 1;
} /* do_recruiter_add() */

/**
 * This method prints out financial reports for the club.
 * @param club_name the name of the club to get financials on
 * @return 1 on success, 0 on failure
 * @see do_deposit()
 * @see do_balance()
 */
protected int do_financials(string name, string account) {
   string place;
   class club_transaction* transactions;
   int i;
   int deposit_total;
   int withdrawl_total;
   string ret;
   string account_name;

   if (!CLUB_HANDLER->is_club(name)) {
      add_failed_mess("The club '" + name + "' does not exist.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_member_of(name, this_player()->query_name())) {
      add_failed_mess("You must be a member of a club to see its "
                      "financials.\n");
      return 0;
   }

   if (account == CLUB_DEFAULT_ACCOUNT_NAME) {
      account_name = "main club account";
   } else {
      account_name = "special club account " + account;
   }

   if (!CLUB_HANDLER->is_account_of(name, account)) {
      add_failed_mess("The account " + account_name + " is not in the "
                      "club '" + CLUB_HANDLER->query_club_name(name) + "'\n");
      return 0;
   }

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   ret = "The club '" + CLUB_HANDLER->query_club_name(name) +
         "' " + account_name + " has a current balance of " +
         MONEY_HAND->money_value_string(CLUB_HANDLER->query_balance(name, account), place) +
         " and the next pay period ends at " +
         ctime(CLUB_HANDLER->query_time_fees_due(name)) +
         ", which will cost " +
         MONEY_HAND->money_value_string(CLUB_HANDLER->query_club_cost_per_period(name), place) +
         ".\n";

   // Let all players know about the transaction history of the club.
   transactions =  CLUB_HANDLER->query_transactions(name, account);
   for (i = 0; i < sizeof(transactions); i++) {
      if (transactions[i]->amount > 0) {
         ret += i + ": DEPOSIT " +
               MONEY_HAND->money_value_string(transactions[i]->amount, place) +
               " from " + transactions[i]->who +
               " at " + ctime(transactions[i]->date) + " (" +
               CLUB_HANDLER->query_transaction_type_name(transactions[i]->transaction_type) +
               ")\n";
         if (transactions[i]->information) {
            ret += "   - " + transactions[i]->information + "\n";
         }
         deposit_total += transactions[i]->amount;
      } else {
         ret += i + ": WITHDRAWL " +
               MONEY_HAND->money_value_string(-transactions[i]->amount, place) +
               " from " + transactions[i]->who +
               " at " + ctime(transactions[i]->date) + " (" +
               CLUB_HANDLER->query_transaction_type_name(transactions[i]->transaction_type) +
               ")\n";
         if (transactions[i]->information) {
            ret += "   - " + transactions[i]->information + "\n";
         }
         withdrawl_total += -transactions[i]->amount;
      }
   }

   ret += "\nTotal withdrawals: " + 
         MONEY_HAND->money_value_string(withdrawl_total, place) + "\n";
   ret += "Total deposits  : " + 
         MONEY_HAND->money_value_string(deposit_total, place) + "\n";

   write("$P$Club Financials$P$" + ret);
   return 1;
} /* do_financials() */

/**
 * This method sets the club description.
 * @param name the name of the club to describe
 * @see finish_description()
 */
protected int do_describe(string name) {
   string bit;

   if (club_commands_allowed()) {
      bit = "club";
   }
   if (family_commands_allowed()) {
      if (bit) {
         bit += " or family";
      } else {
         bit = "family";
      }
   }

   if (!CLUB_HANDLER->is_club(name)) {
      add_failed_mess("The " + bit + " '" + name +
                      "' must exist for you to describe it.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_elected_club(name)) {
      add_failed_mess("You cannot change the description of an elected club, "
                      "it must be done as a discussion item.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_personal_club(name) &&
       !CLUB_HANDLER->is_founder_of(name, this_player()->query_name())) {
      add_failed_mess("You must be the founder of a personal club to "
                      "change the description.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_family(name) &&
       !CLUB_HANDLER->is_founder_of(name, this_player()->query_name())) {
      add_failed_mess("Only the founder of the family can set the "
                      "description of the family.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_personal_club(name) &&
       !CLUB_HANDLER->is_family(name)) {
      add_failed_mess("Unknown club type.\n");
      return 0;
   }

   write("What do you wish to change the description of '" +
         CLUB_HANDLER->query_club_name(name) + "' to?\n");
   this_player()->do_edit("", (: finish_description($1, $(name)) :));
   add_succeeded_mess("");
   return 1;
} /* do_describe() */

/**
 * This is the method that is called when the club description has been
 * finished edited.  It will set the description on the club handler.
 * @param describe the new description
 * @param name the name of the club to describe
 * @see do_describe()
 */
protected void finish_description(string describe, string name) {
   if (!describe ||
       describe == "") {
      write("Aborting setting the description.\n");
      return ;
   }

   CLUB_HANDLER->set_club_description(name, describe);
   if (CLUB_HANDLER->is_family(name)) {
      write("Set the description of the family '" +
             CLUB_HANDLER->query_club_name(name) + "' to:\n" +
             CLUB_HANDLER->query_club_description(name) + "\n");
   } else {
      write("Set the description of the club '" +
             CLUB_HANDLER->query_club_name(name) + "' to:\n" +
             CLUB_HANDLER->query_club_description(name) + "\n");
   }
} /* finish_description() */

/**
 * This method sets the percentage cut the room takes for deposits to the
 * club account.
 * @param cut the nice new percentage cut
 * @see query_percentage_cut()
 * @see do_deposit()
 */
void set_percentage_cut(int value) {
   _percentage_cut = value;
} /* set_percentage_cut() */

/**
 * This method returns the percentage cut the room takes for deposits
 * to the club account.
 * @return the current percentage cut
 * @see set_percentage_cut()
 * @see do_deposit()
 */
int query_percentage_cut() {
   return _percentage_cut;
} /* query_percentage_cut() */

/**
 * This method creates the warning message for the club, saying how much
 * it costs per year and how everything works.  This should probably
 * be used in a sign or something in the main room.
 * @return the warning message
 */
string query_club_warning_message() {
   string place;
   string ret;

   place = query_property("place");
   if (!place) {
      place = "default";
   }

   ret = "";

   if (club_commands_allowed()) {
      ret += "Clubs have a few rules and regulations associated with "
         "them.  They cost " +
         MONEY_HAND->money_value_string(CLUB_STARTUP_COST, place) + 
         " to start.  Every quarter of a " + mud_name() + " year the club "
         "costs " + 
         MONEY_HAND->money_value_string(CLUB_COST_PER_YEAR, place) +
         " plus " +
         MONEY_HAND->
            money_value_string(CLUB_COST_PER_MEMBER_PER_YEAR, place) +
         " per member.  Each insignia will cost " +
         MONEY_HAND->money_value_string(CLUB_COST_INSIGNIA, place) +
         " to be made, and the cost an insignia is taken from the "
         "person requesting the insignia, not the club.\n\n"
         "If the club goes into remission for 2 pay periods then the "
         "club will be disbanded.  If the club is not touched for 8 "
         "real weeks then the club will be disbanded.  A club is "
         "considered touched if a member is added or removed, if an "
         "election is held or nominations are added or if money is " 
         "deposited into the account.\n\n" 
         "Clubs default to being founder controlled clubs, meaning "
         "that the founder has all the control of the club.  Once a "
         "club gets over " + 
         CLUB_MINIMUM_ELECTED_MEMBERS +
         " members it can change to an elected club.  In this mode "
         "the committee members of the club are elected to their "
         "positions.  Once a club gets over " + 
         CLUB_MAXIMUM_ELECTED_MEMBERS + " it must become an elected "
         "club.  A new election is held every " + mud_name() + " year for all "
         "the positions.  If the founder of a founder base club leaves "
         "the mud or is idle is for 8 real weeks then the club will be "
         "disbanded.\n\n" 
         "In this room, clubs can be created and disbanded.  Money can "
         "be deposited into a club, or withdrawn from a club.  It is "
         "possible to see the balance of a given club, and also to see "
         "various lists of clubs.  It is also possible to nominate "
         "someone for position, vote for someone and to force an "  
         "election in a particular club.  Furthermore, announcements "
         "and discussion items for clubs can be added here.  Club "
         "insignias can be replaced, and types and options of a club "
         "can be changed.  Finding out more about a club's financials "
         "is also possible, as well as changing the description of a "
         "club.  Last, but not least, a recruiter can be added to a "
         "club.\n";
   }
   if (family_commands_allowed()) {
      ret += "Families have a few rules and regulations associated "
        "with them.  They cost " + 
       MONEY_HAND->money_value_string(FAMILY_STARTUP_COST, place) +
       " to start and cost " +
       MONEY_HAND->money_value_string(FAMILY_COST_PER_YEAR, place) +
       " in upkeep every quarter of a " + mud_name() + " year, plus " +
       MONEY_HAND->
         money_value_string(FAMILY_COST_PER_MEMBER_PER_YEAR, place) +
       " per member.\n\n"
       "You can create relationships between people, and also create "
       "families.  It is possible to list families, or details about "
       "any particular " 
       "family.  On a family of yours, you can set the description, " 
       "see its bank balance and deposit more money into its account.  "
       "If your family balance is less than the required amount when "
       "it is time for your family to pay for its fees, the family "
       "will be disbanded.\n\n" 
       "You cannot choose a family name from the books for "
       "your families.\n";
   }
   return ret;
} /* query_club_warning_message() */

/**
 * This method does a listing of all the clubs that a certain person is
 * in.
 * @param name the person whose name to check
 * @return 1 on success, 0 on failure
 * @see /std/basic/club_listing.c
 */
protected int do_club_list_member(string name) {
   if (!PLAYER_HANDLER->test_user(name)) {
      add_failed_mess(name + " does not play " + mud_name() + ".\n");
      return 0;
   }

   if (club_list(this_player()->query_name(),
                 0,
                 (: check_member_func($1, $2, $(name)) :))) {
      return 1;
   }
   add_failed_mess("Could not find any clubs with " + name +
                   " as a member.\n");
   return 0;
} /* do_club_list_member() */

/** @ignore yes */
private void finish_announcement(string str, string name) {
   if (!str) {
      write("Aborting.\n");
      return ;
   }

   if (CLUB_HANDLER->set_announcement(name,
                                      str)) {
      write("Added the announcement to the club " +
            CLUB_HANDLER->query_club_name(name) +
            ".\n");
   } else {
      write("Unable to add the announcement to the club " +
            CLUB_HANDLER->query_club_name(name) +
            " for some reason.\n");
   }
} /* finish_announcement() */

/**
 * This method sets an announcement for the club.
 * @return 1 on success, 0 on failure
 */
protected int do_announcement(string name) {
   if (!CLUB_HANDLER->is_elected_club(name)) {
      add_failed_mess("The club must be an elected club for an announcement "
                      "to be set.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_president_of(name, this_player()->query_name()) &&
       !CLUB_HANDLER->is_secretary_of(name, this_player()->query_name())) {
      add_failed_mess("Only the president and secretary can set an "
                      "announcement.\n");
      return 0;
   }

   write("What announcement do you wish to set?\n");
   this_player()->do_edit(0,
                          (: finish_announcement :),
                          0,
                          0,
                          name);
   return 1;
} /* do_announcement() */

private int check_member_func(string club,
                              string person,
                              string mem) {
   if (!CLUB_HANDLER->is_member_of(club, mem)) {
      return 0;
   }

   if (CLUB_HANDLER->query_club_secret(club) &&
       !CLUB_HANDLER->is_member_of(club, person)) {
      return 0;
   }

   return 1;
} /* check_member_func() */

protected int do_force_election(string name) {
   if (!CLUB_HANDLER->is_elected_club(name)) {
      add_failed_mess("The club must be elected for you to force an "
                      "election in it.\n");
      return 0;
   }

   if (!CLUB_HANDLER->is_president_of(name, this_player()->query_name())) {
      add_failed_mess("You must be the president of the club to force an "
                      "election.\n");
      return 0;
   }

   if (CLUB_HANDLER->is_election_in_progress(name) > time()) {
      add_failed_mess("There is already an election in progress, you cannot "
                      "start another one!\n");
      return 0;
   }

   if (CLUB_HANDLER->query_minimum_next_election(name) > time()) {
      add_failed_mess("It is too soon to force an election, that cannot "
                      "be done until " +
                       ctime(CLUB_HANDLER->query_minimum_next_election(name)) +
                       ".\n");
      return 0;
   }

   if (!CLUB_HANDLER->start_election(name)) {
      add_failed_mess("Could not start the election for some reason.\n");
      return 0;
   }

   add_succeeded_mess("$N start$s an election in " +
                      CLUB_HANDLER->query_club_name(name) + ".\n");
   return 1;
} /* do_force_election() */
