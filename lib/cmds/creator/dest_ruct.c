/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: dest_ruct.c,v 1.2 2000/10/27 03:31:10 taffyd Exp $
 * $Log: dest_ruct.c,v $
 * Revision 1.2  2000/10/27 03:31:10  taffyd
 * Fixed buglets!
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* destruct command, trial out by Turrican for a commands daemon. */

#include <creator.h>

inherit "/cmds/base";

void main_dest_bit(object *ob);

void ask_dest(object *obs) {
  if (!pointerp(obs) || !sizeof(obs)) {
    write("No more things to dest.\n");
    return;
  }
  write("Dest object " + WIZ_PRESENT->desc_object(obs[0]) + " ? ");
  input_to("dest_answer", 0, 0, obs);
  return;
} /* ask_dest() */

void dest_answer(string s, int majd, object *obs) {
  string err, shrt;

  if (this_player()->affirmative(s)) {
    if (majd) {
      shrt = (string)obs[0]->short();
      err = catch(obs[0]->dwep());
      this_player()->handle_error(err, "DWEP");
      if (obs[0]) {
        write("It REALLY doesn't want to be dested.\n");
        err = catch(destruct(obs[0]));
        this_player()->handle_error(err, "destruct()");
      }
      majd = 0;
      if (obs[0]) write("It didn't dest.\n");
      else {
        say((string)this_player()->query_cap_name()+" disintegrates "+
            (shrt ? shrt : "something") +".\n"); 
        write("Ok.\n");
      }
      ask_dest(obs[1..]);
      return;
    } else {
      obs[0]->move("/room/rubbish");
      err = catch(obs[0]->dest_me());
      this_player()->handle_error(err, "dest_me");
      if (obs[0]) {
        write("This object does NOT want to be dested.  Are you sure? ");
        input_to("dest_answer", 0, 1, obs);
        return;
      } 
      write("Ok.\n");
      ask_dest(obs[1..]);
      return;
    }
  } else if (s == "q" || s == "quit") {
    write("Ok.  No more objects will be destd.\n");
    return;
  }
  write("Ok.  Not destd.\n");
  ask_dest(obs[1..]);
  return;
} /* dest_answer() */
         
mixed cmd(string str) {
  object *ob, *dest_obj;
  string qstr;

  dest_obj = ({ });
  notify_fail("Can't find " + str + " to dest.\n");
  
  if ( !str || ( str == "" ) )
    return notify_fail( "Destruct what?\n" );
  if (this_player()->query_name() == "simidh") {
     write("Bye bye...\n");
     this_player()->dest_me();
     return 1;
  }

  if (sscanf(str, "query %s", qstr) == 1) {
    dest_obj = WIZ_PRESENT->wiz_present(qstr, this_player());
    if (!sizeof(dest_obj)) return 0;
    ask_dest(dest_obj);
    return 1;
  }
  ob = WIZ_PRESENT->wiz_present(str,this_player());
  if (!sizeof(ob)) return 0;
  main_dest_bit(ob);
  return 1;
} /* cmd() */

void main_dest_bit(object *ob) {
  object thing, *rest, *dest_obj;
  string err, shrt, dobj;

  rest = ({ });
  dest_obj = ({ });
  foreach (thing in ob) {
    if (interactive(thing) && sizeof(ob) != 1) {
      write("You DON'T destruct " + thing->query_name() + ".\n");
      continue;
    }
    catch(shrt = (string)thing->short());
    dobj = WIZ_PRESENT->desc_object(thing);
    thing->move("/room/rubbish");
    err = catch(thing->dest_me());
    this_player()->handle_error(err, "dest_me");
    if (thing) 
      dest_obj += ({ thing });
    else {
      write("You destruct " + dobj + ".\n");
      say((string)this_player()->query_cap_name()+" disintegrates "+
          (shrt ? shrt : "something") + ".\n"); 
    }
  }
  if (sizeof(dest_obj) > 0) {
    ask_dest(dest_obj);
    return;
  }
  return;
} /* main_dest_bit() */
