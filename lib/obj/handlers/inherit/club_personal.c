/**
 * This is the handler for all things clubby, a club being a group of
 * players.  Each club must have a unique name.  It also handles elections
 * for various club positions.
 * @author Pinkfish
 * @started Sun Sep 27 03:35:42 EDT 1998
 */
inherit "/obj/handlers/inherit/club_basic";

#define __CLUBS_H_NO_CLASSES

#include <clubs.h>

/**
 * This is for a personal club, a personal club will be removed when
 * the founder disappears.
 */
class personal_info {
}

protected void create_extra_data(string name) {
   class personal_info p_info;
   class club_info data;

   data = (class club_info)query_club_info(name);
   if (query_club_type(name) == CLUB_PERSONAL) {
         p_info = new(class personal_info);
      data->extra_data = p_info;
      set_club_changed(name);
   } else {
      ::create_extra_data(name);
   }
} /* create_extra_data() */

/**
 * This method returns the personal info associated with the club.
 * @param name the name of the club to get the info for
 * @return the personal club class, 0 if no club or not personal type
 */
private class personal_info query_personal_info(string name) {
   class club_info data;

   if (is_personal_club(name)) {
      data = (class club_info)query_club_info(name);
      return data->extra_data;
   }
   return 0;
} /* query_personal_info() */
