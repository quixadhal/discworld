/**
 * This is the standard punishment file for banishing someone from a
 * city.
 * @author Pinkfish
 * @started Tue Sep 19 14:37:28 PDT 2000
 */
#include <nomic_system.h>

inherit "/std/room/inherit/punishment_inherit";

private nosave string _banish_handler;

void create() {
   set_short("banishment");
   set_description_file(NOMIC_PUNISHMENT_DOCS_DIR + "banishment");
   ::create();
} /* setup() */

/**
 * This method sets the banish handler.  The banish handler will be called
 * with two methods, one is:<br>
 * banish_person(string name, int days)<br>
 * banish_person_suspend(string name)
 * @param handler the banish handler
 */
void set_banish_handler(string hand) {
   _banish_handler = hand;
} /* set_banish_handler() */

/**
 * This method returns the banish handler.
 * @return the banish handler
 */
string query_banish_handler() {
   return _banish_handler;
} /* query_banish_handler() */

/** @ignore yes */
string query_short(string area, class nomic_case the_case, string type, mixed data) {
   return ::query_short() + " for " + data + " days";
} /* query_short() */

/**
 * @ignore yes
 */
void start_punishment(string area,
                      class nomic_case the_case,
                      string type,
                      function finish) {
   if (sizeof(filter(the_case->people,
                    (: NOMIC_HANDLER->is_magistrate_of($2, $1) :), area))) {
      write("You cannot banish a magistrate.\n");
      return ;
   }

   write("How long do you want the person banished from the city for (in "
         "days, 0 means forever)? ");
   input_to("days_of_banishment", 0, area, the_case, type, finish);
} /* start_punishment() */

/** @ignore yes */
void days_of_banishment(string input,
                        string area,
                        class nomic_case the_case,
                        string type,
                        function finish) {
   int num;

   input = lower_case(input);
   if (input == "" || input[0] == 'q') {
      write("Aborting banishment.\n");
      return ;
   }

   if (input[0] >= '0' && input[0] <= '9') {
      if (sscanf(input, "%d", num) == 1) {
         evaluate(finish, the_case, type, num);
         return ;
      }
   }
   write("Invalid input.\n");
   write("How long do you want the person banished from the city for (in "
         "days, 0 means forever)? ");
   input_to("days_of_banishment", 0, area, the_case, type, finish);
} /* days_of_banishment() */

/**
 * @ignore yes
 */
void complete_punishment(string area,
                      class nomic_case the_case,
                      string type,
                      mixed data) {
   string person;

   foreach (person in the_case->people) {
      _banish_handler->banish_person(person, data);
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
      _banish_handler->banish_person_suspend(person);
   }
   return ;
} /* suspend_punishment() */
