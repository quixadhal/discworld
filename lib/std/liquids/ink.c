/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ink.c,v 1.1 1998/01/06 04:28:23 ceres Exp $
 * $Log: ink.c,v $
 * Revision 1.1  1998/01/06 04:28:23  ceres
 * Initial revision
 * 
*/
/*
 * Ink defintion file...
 */

int splash_func(object ob) {
  ob->add_extra_look(this_object());
  ob->add_property("splashed with", previous_object()->query_liquid_name());
  return 1;
}

int query_ink() { return 1; }

string extra_look() {
  return capitalize((string)previous_object()->query_name())+
         " has been splashed with "+
         previous_object()->query_property("splashed with");
}
