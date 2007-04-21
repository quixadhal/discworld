/**
 * This inherit has all the basic things that all the council rooms
 * of various sorts share.
 * @author Pinkfish
 * @started Wed Oct 25 21:44:37 PDT 2000
 */
#define NOMIC_SYSTEM_NO_CLASSES
#include <nomic_system.h>
private nosave string _council_area;

/**
 * This method returns the current area this council chambers deals with.
 * @return the area the council chambers deal with
 */
string query_council_area() {
   return _council_area;
} /* query_council_area() */

/**
 * This method sets the current area the council chambers deal with.
 * @param area the new council area
 */
void set_council_area(string area) {
   _council_area = area;
} /* set_council_area() */

/**
 * This method broadcasts a message to the citizens or magistrates of
 * the area.
 * @param magistrate_only 1 if only for magistrates, 0 for citizens
 * @param message the message to send
 */
void send_council_inform(int magistrate_only, string message) {
   NOMIC_HANDLER->send_council_inform(query_council_area(),
                                       magistrate_only, message);
} /* send_council_message() */
