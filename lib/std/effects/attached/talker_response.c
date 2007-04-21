	/**
 * Talker Response effect/shadow pair
 * @see help::talker_response shadow
 */

#include <effect.h>
#define CLASS "object.talker.response"
#define SHADOW "/std/shadows/attached/"

/** @ignore yes */
string query_classification() { return CLASS; }

int query_indefinite() { return 1; }

/** @ignore yes */
string query_shadow_ob() { return SHADOW + "talker_response"; }

/** @ignore yes */
mixed beginning( object item, mixed arg, int id )
{   
//    if(!objectp(arg)) {
//        item->delete_effect(item->sid_to_enum(id));
//        return -1;
//    }
//
//    return arg;
} /* beginning() */

mixed restart( object player, mixed arg, int id )
{
/** @ignore yes */
  return beginning(player, arg, id);
} /* restart() */

/** @ignore yes */
mixed merge_effect(object player, mixed oldarg, mixed newarg, int id)
{
	return oldarg;
}

/** @ignore yes */
void end( object player, mixed arg, int id )
{} /* end() */
