/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: state_change.c,v 1.4 2002/09/30 14:07:19 terano Exp $
 * $Log: state_change.c,v $
 * Revision 1.4  2002/09/30 14:07:19  terano
 * Added some extra functionality in the form of a third arg to transform.
 *
 * Revision 1.3  2002/09/19 19:51:39  tilly
 * Added a check for collective objects along with the continuous, as collective obs were getting fucked up amounts set.
 *
 * Revision 1.2  1998/11/14 05:00:52  jeremy
 * Added autodoc stuff.
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
 */
/**
 * This handler is meant to simulate the changing of an object's
 * "state", such as melting, grinding, etc.  In reality, rather than
 * changing the source object, the transform() method clones a
 * completely new object, based on the internal lookup table; it is
 * the caller's responsibility to dest the source object and put the
 * new one in its place.
 * <p>
 * The handler's lookup table is stored in a series of files:
 * any files in /obj/state_change ending in .dat will be added
 * to the table when the handler is loaded.  See /include/state_change.h
 * for field definitions.  The data files are loaded by the data
 * handler, and can contain fancy things like expressions and function
 * pointers.
 * @author Jeremy
 * @see /include/state_change.h
 * @see /obj/handlers/data.c
 */

#include <state_change.h>

#define PROP "state name"
// I can't think of anyplace good to put this
#define DATA_DIR "/obj/state_change/"

mapping table = ([ ]);

void set_amount_of_ob( object ob, int amount );

/**
 * This method returns the entire lookup table.  It is mainly for
 * debugging.
 * @return the state_change lookup table
 */
mixed query_table() { return table; }

/** @ignore yes */
void create() {
    string *files;
    int i;

    seteuid("/secure/master"->creator_file(file_name(this_object())));
    files = ({ "base.hdr" });
    files += get_dir(DATA_DIR + "*.dat");
    for (i = 0; i < sizeof(files); i++) {
        files[i] = DATA_DIR + files[i];
    }
    table += "/obj/handlers/data"->compile_data(files);
} /* create() */

// TODO: this should be able to clone and return an array of objects.
// TODO: being able to pass in an argument for the func field would be nice.
/**
 * This method is generally called by the object doing the state
 * change (e.g., a food grinder).  It looks up the specified source
 * object in the table and clones the "transformed" object as specified.
 * <p>
 * The table is indexed by the source object as follows: if the source
 * object has a property "state name", the value of this is used.  If
 * not, the object's medium alias (if any, regardless of whether the
 * object is continuous) is used.  Failing these, the object's short
 * description is used.  The first one resulting in a valid string is
 * concatenated with the transformation string (with a ":" between)
 * and the result is used as the index.  If the lookup fails, 0 is
 * returned.
 * <p>
 * Note that the resulting object is only cloned.  It is the responsibility
 * of the caller to dest the source and put the result in its place.
 * @param ob the source object
 * @param transformation a string specifying the transformation (e.g., "grind")
 * @param The player to whom this state change will be attributed to! [optional]
 * @return the result of the transformation */
varargs object transform( object ob, string transformation, object player ) {
    string index, name;
    object ret;
    class state_change c;
    function f;
    int *weight_unit;

    // First try a property
    if (!stringp(name = ob->query_property(PROP)))
      // Next see if it has a medium alias (this would be the norm)
      if (!stringp(name = ob->query_medium_alias()))
        // Finally, use the short
        name = ob->short();

    if ( !player )
      player = this_player();
    
    index = transformation + ":" + name;
    if (undefinedp(table[index]))
      return 0;
    c = table[index];
    ret = clone_object(c->result);

    foreach (f in c->func)
      evaluate(f, ob, ret, player);

    if (ret->query_continuous()) {
        if ( ob->query_continuous() || ob->query_collective() ) {
            ret->set_amount(to_int(ob->query_amount() * c->factor));
        } else {
            weight_unit = ret->query_weight_unit();
            set_amount_of_ob( ret, to_int( ob->query_weight() * c->factor *
              weight_unit[1] / weight_unit[0] ) );
        }
    } else {
        if ( ob->query_continuous() || ob->query_collective() ) {
            weight_unit = ob->query_weight_unit();
            ret->set_weight(to_int(ob->query_amount() * c->factor *
                                   weight_unit[0] / weight_unit[1]));
        } else {
            ret->set_weight(to_int(ob->query_weight() * c->factor));
        }
    }
    log_file( "STATE_CHANGE", "Changed %d amount of %s to %d amount of %s via %s.\n", 
      ob->query_amount(), ob->query_medium_alias(), ret->query_amount(), 
      ret->query_medium_alias(), transformation );
    return ret;
} /* transform() */

/**
 * @ignore
 * Helper function to ensure 0 amount cont obs are destroyed.
 */
void set_amount_of_ob( object ob, int amount ) {
  if ( !ob ) 
      return;

  if ( !amount )
      ob->dest_me();

  ob->set_amount( amount );
  return;
}

