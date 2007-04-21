/*  -*- LPC -*-  */
/*
 * $Locker: wodan $
 * $Id: route_handler.c,v 1.11 2002/03/16 22:08:01 wodan Exp wodan $
 * $Log: route_handler.c,v $
 * Revision 1.11  2002/03/16 22:08:01  wodan
 * don't do routes for npcs in /room/virtual
 *
 * Revision 1.10  1999/07/28 18:00:25  wodan
 * don't do anything for no longer existing npcs
 *
 * Revision 1.9  1999/06/09 13:02:37  wodan
 * used same variable name twice, oops :)
 *
 * Revision 1.8  1999/04/15 13:39:17  wodan
 * removed old route stuff.
 *
 * Revision 1.7  1999/04/15 13:33:49  wodan
 * changed to use the clusters handler
 *
 * Revision 1.6  1999/04/14 12:06:11  ceres
 *  Forcibly unlocked by wodan
 *
 * Revision 1.5  1998/12/17 08:05:16  terano
 * not quite sure :)
 *
 * Revision 1.4  1998/09/07 15:54:40  terano
 * Took out the cluster stuff after it was taking newbies to the UU via the sto lat plains
 *
 * Revision 1.3  1998/09/04 13:39:55  wodan
 * changed the search_room call to a call to the clusters handler.
 *
 * Revision 1.2  1998/01/14 13:34:26  wodan
 * changed it so it searches 3000 rooms instead of 200 like it says in the comment, 200 isn't enough for any practical use
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/**
 * Figures out the route between two destinations for an npc to travel.
 * @author Wodan
 */
/* routes handler */

int use_count;

/**
 * Try and figure out a route between two locations.  It figures out
 * the route bwteen the two locations and then calls the
 * call_back function with the found route, or not found route.
 * If the route is not found, an empty array will be returend.  The
 * function in the npc object move_me_to() handles the call
 * to this function transparently.
 * @param end the end location
 * @param start where to start from
 * @param call_back the function to call when finished
 * @see /obj/monster->move_me_to()
 * @example
 * // Find how to get from here back to the drum.
 * // Then print the route out.
 * ROUTE_HANDLER->get_route("/d/am/am/mendeddrum",
 *                          file_name(environment(this_object())),
 *                          (: printf("%O\n", $1) :));
 */
varargs void get_route(string end, string start, function call_back, int count) {
  string *result;

  if(start == "/room/virtual"){
    call_out((: evaluate($(call_back), ({ })) :), 1);
    return;
  }
  
  if(!(stringp(start) && stringp(end)))
    error("illegal argument 1 or 2 to get_route");
  
  if(!function_owner(call_back))
    return; //don't bother

  if ((end == start) || (count > 3)){
    call_out((: evaluate($(call_back), ({ })) :), 1);
    if(end != start)
      log_file("ROUTEHANDLER","route for %s from %s to %s failed\n",
               file_name(function_owner(call_back)), start, end);
    return;
  }

  result = "/obj/handlers/clusters"->get_route(start, end);
  
  if(!result){
    call_out((:get_route($(end), $(start), $(call_back), $(count + 1)):), 10);
    return;
  }

  use_count++;
  call_out((: evaluate($(call_back), $(result)) :), 1);
  return;
} /* get_route */

/** @ignore yes */
void dest_me() {
  destruct(this_object());
} /* dest_me() */

/** @ignore yes */
mixed *stats() {
  return  ({
    ({ "routes done", use_count }),
  });
}
