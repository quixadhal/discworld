/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mon-shadow.c,v 1.1 1998/01/06 04:29:08 ceres Exp $
 * $Log: mon-shadow.c,v $
 * Revision 1.1  1998/01/06 04:29:08  ceres
 * Initial revision
 * 
*/
object npc;

void setup_shadow(object ob) {
  shadow(ob,1);
  npc = ob;
}

void death_shadow() { destruct( this_object() ); }

object query_npc() { return npc; }
