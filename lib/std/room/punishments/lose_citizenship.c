/**
 * This is the standard punishment file for losing someone's
 * citizenship.
 * @author Pinkfish
 * @started Tue Sep 19 14:37:28 PDT 2000
 */
#include <nomic_system.h>

inherit "/std/room/inherit/punishment_inherit";

void setup() {
   set_short("lose citizenship");
   set_description_file(NOMIC_PUNISHMENT_DOCS_DIR + "lose_citizenship");
} /* setup() */


/**
 * @ignore yes
 */
void start_punishment(string area,
                      class nomic_case the_case,
                      string type,
                      function finish) {
   string* not_citizens;
   string* magistrates;

   not_citizens = filter(the_case->people,
                     (: !NOMIC_HANDLER->is_citizen_of($2, $1) :), area);
   if (sizeof(not_citizens)) {
      write(query_multiple_short(not_citizens) + " cannot lose "
            "their citizenship since they are not citizens.\n");
      return ;
   }

   magistrates = filter(the_case->people,
                    (: NOMIC_HANDLER->is_magistrate_of($2, $1) :), area);
   if (sizeof(magistrates)) {
      write(query_multiple_short(magistrates) + " cannot lose "
            "their citizenship since they are magistrates.\n");
      return ;
   }
   evaluate(finish, the_case, type, 0);
} /* start_punishment() */

/**
 * @ignore yes
 */
void complete_punishment(string area,
                      class nomic_case the_case,
                      string type,
                      mixed data) {
   string citizen;

   foreach (citizen in the_case->people) {
      NOMIC_HANDLER->remove_citizen(area, citizen);
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
   string citizen;

   //
   // Make them citizens again...
   //
   foreach (citizen in the_case->people) {
      NOMIC_HANDLER->add_citizen(area, citizen);
   }

   return ;
} /* suspend_punishment() */
