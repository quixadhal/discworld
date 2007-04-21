/**
 * This is the method for forcing someone to have a title of some sort.
 * It allows a range of different titles to be chosen from.
 * This punihshment needs to
 * be inherited to work correctly.
 * @author Pinkfish
 * @started Thu Jun 26 14:46:36 PDT 2003
 */
#include <money.h>
#include <nomic_system.h>
#include <player_handler.h>
#include <player.h>
inherit "/std/room/inherit/punishment_inherit";

private nosave string* _titles;

void create() {
   _titles = ({ });
   set_description_file(NOMIC_PUNISHMENT_DOCS_DIR + "title");
   set_short("title");
   ::create();
} /* create() */

/**
 * This method sets the allowable list of titles used for this area.
 * @param titles the list of titles to use
 */
void set_titles(string* titles) {
   _titles = titles;
}

/**
 * This method returns the list of allowable titles to use for this
 * area.
 * @return the list of allowable titles
 */
string* query_titles() {
   return _titles;
}

//
// The punishment handle area
//

/**
 * @ignore yes
 */
void start_punishment(string area,
                      class nomic_case the_case,
                      string type,
                      function finish) {
   int i;

   for (i = 0; i < sizeof(_titles); i++) {
      write(sprintf("%c) %s\n", i + 'A', _titles[i]));
   }
   write("Which title do you wish " +
         query_multiple_short(the_case->people) + " to have? ");
   input_to("choose_title", 0, area, the_case, type, finish);
} /* start_punishment() */

/** @ignore yes */
string query_short(string area,
                      class nomic_case the_case,
                      string type,
                      mixed data) {
   return "set to a title of " + data[0] + " for " + data[1] + " hours";
} /* query_short() */

/**
 * @ignore yes
 */
void choose_title(string title, string area, class nomic_case the_case,
                 string type, function finish) {
   int i;
   int pos;

   if (!strlen(title) || title[0] == 'q') {
      write("Aborting the title setting.\n");
      return ;
   }

   title = lower_case(title);
   if (strlen(title) == 1) {
      pos = title[0] = 'a';
      if (pos >= 0 && pos < sizeof(_titles)) {
         title = _titles[pos];
      }
   }
   if (member_array(title, _titles) == -1) {
      write("You did not specify a valid title.\n");
      for (i = 0; i < sizeof(_titles); i++) {
         write(sprintf("%c) %s\n", i + 'A', _titles[i]));
      }
      write("What title do you want to give " +
            query_multiple_short(the_case->people) + "? ");
      input_to("choose_title", 0, area, the_case, type, finish);
      return ;
   }

   write("How long do you wish " +
            query_multiple_short(the_case->people) + " to keep the title " +
         title +
         "(this is in hours and is online time, not real time)? ");
   input_to("choose_timeout", 0, area, the_case, type, finish, title);
} /* fine_amount() */

void choose_timeout(string str, string area, class nomic_case the_case,
                 string type, function finish, string title) {
   int timeout;

   if (!strlen(str) || str[0] == 'q') {
      write("Aborting the title setting.\n");
      return ;
   }

   timeout = to_int(str);
   if (timeout <= 0) {
      write("The timeout must be greator than 0 hours.\n");
      write("How long do you wish " +
            query_multiple_short(the_case->people) + " to keep the title " +
            title + "?");
      input_to("choose_timeout", 0, area, the_case, type, finish, title);
      return ;
   }

   if (timeout > 30) {
      write("The timeout must be less than 30 hours.\n");
      write("How long do you wish " +
            query_multiple_short(the_case->people) + " to keep the title " +
            title + "?");
      input_to("choose_timeout", 0, area, the_case, type, finish, title);
      return ;
   }

   evaluate(finish, the_case, type, ({ title, timeout }));
}

/**
 * @ignore yes
 */
void complete_punishment(string area,
                      class nomic_case the_case,
                      string type,
                      mixed data) {
   string person;

   foreach (person in the_case->people) {
      PLAYER_TITLE_HANDLER->set_forced_title_on_player(person, data[0], data[1] * (60 * 60));
   }

   return ;
} /* complete_punishment() */

/**
 * @ignore yes
 */
void suspend_punishment(string area,
                      class nomic_case the_case,
                      string type,
                      mixed data) {
   string person;

   foreach (person in the_case->people) {
      PLAYER_TITLE_HANDLER->remove_forced_title(person, data[0]);
   }

   return ;
} /* suspend_punishment() */
