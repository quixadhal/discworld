/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: wiz_object_comm.c,v 1.2 1998/04/13 14:26:20 pinkfish Exp $
 * $Log: wiz_object_comm.c,v $
 * Revision 1.2  1998/04/13 14:26:20  pinkfish
 * Added documentation and removed unnessessary bits.
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
*/
#include <creator.h>
 

/**
 * This file conatins all the object based creator commands.
 * @author Ember
 */
private nosave mapping obvar;

void create() {
   obvar = ([ ]);
} /* create() */

/** @ignore yes */
protected void wiz_commands() {
} /* wiz_commands() */

/** @ignore yes */
int affirmative(string s) /* returns true if s is a 'yes' equiv response */
{
    s = lower_case(s);
    return (s == "y" || s == "yes" || s == "ok" || s == "please");
} /* affirmative() */

/* is there an error?  print it */
/** @ignore yes */
void handle_error(string erm, string desc) {
    if (erm) {
         write(desc + " failed : " + erm);
    }
} /* handle_error() */

/**
 * this method sets the given object variable to the given
 * value.
 * @param key the variable name
 * @param value the object variable name
 * @see del_obvar()
 * @see get_obvar()
 * @see get_obvars()
 */
void set_obvar(string key, object value) {
  obvar[key] = value;
} /* set_obvar() */

/**
 * This method deletes the given object variable,
 * @param key the object variable to delet.
 * @see set_obvar()
 * @see get_obvar()
 * @see get_obvars()
 */
void del_obvar(string key) {
  map_delete(obvar, key);
} /* del_obvar() */

/**
 * This method returns the object associated witht the object
 * variable.
 * @param key the object variablename
 * @return the object associated with the name
 * @see del_obvar()
 * @see set_obvar()
 * @see get_obvars()
 */
object get_obvar(string key) {
  return obvar[key];
} /* get_obvar() */

/**
 * This method returns the mapping of all the object variables
 * currently set on this creator.
 * @return all the object variables as a mapping
 * @see del_obvar()
 * @see set_obvar()
 * @see get_obvar()
 */
mapping get_obvars() {
  return obvar;
} /* get_obvars() */

/** @ignore yes */
string desc_object(mixed o) {
  log_file("WIZ_PRESENT", sprintf("desc_object() called by %O.\n", 
      previous_object()));
  return "";
} /* desc_object() */

/** @ignore yes */
string desc_f_object(mixed o) {
  log_file("WIZ_PRESENT", sprintf("desc_f_object() called by %O.\n",
      previous_object()));
  return "";
} /* desc_f_object() */

/** @ignore yes */
object *wiz_present(string str, object onobj, int nogoout) {
  log_file("WIZ_PRESENT", sprintf("wiz_present() called by %O.\n",
      previous_object()));
  return ({});
} /* wiz_present() */
