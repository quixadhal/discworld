/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: extra_look.c,v 1.5 2001/09/16 14:27:45 wodan Exp $
 * $Log: extra_look.c,v $
 * Revision 1.5  2001/09/16 14:27:45  wodan
 * hope i fixed the type error, seems some code adds extra looks to strings, rather then objects.
 *
 * Revision 1.4  2001/07/15 03:25:42  ceres
 * Can't figure out the problem
 *
 * Revision 1.3  2001/01/28 18:40:13  drakkos
 * Updated the autodoc a leetle.
 *
 * Revision 1.2  2000/09/14 01:27:05  shrike
 * added a bit more documentation to add_extra_look.
 *
 * Revision 1.1  1998/01/06 03:59:10  ceres
 * Initial revision
 *
*/
/**
 * Handles adding bits to the long description of objects.
 * @author Pinkfish
 */
/* /std/basic/extra_looks */

private nosave object *extra_looks;

void create() {
  extra_looks = ({ });
} /* create() */

/**
 * This method creates the extra long stringsbased on the current list
 * of objects to use an extra_look on.  The function extra_look() is
 * called on every object with this_object() passed in as the first
 * (and only) parameter.  If the result is non-zero it is added onto
 * the output.
 * @see add_extra_look()
 * @see remove_extra_look()
 * @see query_extra_look_list()
 * @return the extra look string
 */
string calc_extra_look() {
  int i;
  string s1, s2;

  s1 = "";
  if (!extra_looks) {
    extra_looks = ({ });
  }
  for (i = 0; i < sizeof(extra_looks); i++) {
    if (extra_looks[i]) {
      s2 = (string)((extra_looks[i])->extra_look(this_object()));
      if(s2)
        s1 += s2;
    } else {
      extra_looks = delete(extra_looks, i, 1);
      i--;
    }
  }
  return s1;
} /* calc_extra_look() */

/**
 * This method adds an object into the list of extra look objects.
 * ob will need to have a string extra_look( object ) function defined on
 * it which will actually give the extra look string. ob->extra_look() is
 * called with the object to which you are adding the extra look.
 * @param ob the object to add
 * @see calc_extra_look()
 * @see remove_extra_look()
 * @see query_extra_look_list()
 */
void add_extra_look(object ob) {
  if(member_array(ob, extra_looks) != -1) {
    return;
  }
  if(stringp(ob))
    ob = find_object((string)ob);
  extra_looks += ({ ob });
} /* add_extra_look() */

/**
 * This method removes the object from the list of extra look objects.
 * @param ob the object to remove
 * @see calc_extra_look()
 * @see add_extra_look()
 * @see query_extra_look_list()
 */
void remove_extra_look(object ob) {
  int i;

  i = member_array(ob, extra_looks);
  if (i < 0) {
    return;
  }
  extra_looks = delete(extra_looks, i, 1);
} /* remove_extra_look() */

/**
 * This method returns the current list of objects to be used for
 * the extra look list.
 * @see add_extra_look()
 * @see remove_extra_look()
 * @see calc_extra_look()
 * @return the array of extra look objects
 */
object *query_extra_look_list() {
  return extra_looks;
} /* query_extra_look_list() */
