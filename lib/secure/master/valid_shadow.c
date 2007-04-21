/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: valid_shadow.c,v 1.3 2000/10/16 19:32:13 wodan Exp $
 * 
*/
/*
 * The master object is asked if it is ok to shadow object ob. Use
 * previous_object() to find out who is asking.
 *
 * In this example, we allow shadowing as long as the victim object
 * hasn't denied it with a query_prevent_shadow() returning 1.
 */
int valid_shadow(object ob) {

  if (previous_object() == ob)
    return 0;
  return (!ob->query_prevent_shadow(previous_object()) &&
          !sscanf(file_name(ob), "/secure/%*s") &&
          !sscanf(file_name(ob), "/obj/handlers/%*s") &&
          !function_exists("heart_beat" , previous_object(), 1) &&
          !sscanf(file_name(ob), "/cmds/%*s"));
} /* query_allow_shaodw() */
