/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: identity_hold.c,v 1.10 2001/09/26 02:47:29 presto Exp $
 * $Log: identity_hold.c,v $
 * Revision 1.10  2001/09/26 02:47:29  presto
 * If either keep or identify is removed, then remove the shadow too
 *
 * Revision 1.9  2001/05/03 17:53:10  ceres
 * Fixed runtime
 *
 * Revision 1.8  2000/07/02 10:14:58  taffyd
 * Fixes for component pouch...
 *
 * Revision 1.7  2000/06/28 05:22:01  taffyd
 * Added query_full_identifier().
 *
 * Revision 1.6  2000/06/28 04:18:25  pinkfish
 * Fix it up to not get womelbed when people log in...
 *
 * Revision 1.5  2000/06/28 04:13:48  pinkfish
 * Fix it up when it is just used to keep something.
 *
 * Revision 1.4  2000/06/28 02:30:12  pinkfish
 * Fix up a typo in the object na,e.
 *
 * Revision 1.3  2000/06/28 00:20:37  pinkfish
 * Fix up the identity problems.
 *
 * Revision 1.2  2000/06/08 01:48:53  pinkfish
 * Fix up some stuff.
 *
 * Revision 1.1  1999/10/27 01:29:07  pinkfish
 * Initial revision
 *
 * Revision 1.3  1999/06/19 11:29:54  taffyd
 * Fixed runtime.
 *
 * Revision 1.2  1999/04/10 23:23:24  mansarde
 * Fixed the bug with the shadow being dested when you put something
 * into a container. now the shadow is only removed when the item
 * goes outside of the player.
 *
 * Revision 1.1  1998/01/06 04:40:23  ceres
 * Initial revision
 * 
 */
/**
 * Nothing in this shadow should be modified directly.  The shadow
 * should do it all for us.
 */
inherit "/std/effect_shadow";
#include <move_failures.h>

private int _keep;
private string _identifier;
private string* _adjs;
private object _owner;

int query_keep() { return _keep; }

void set_keep_value(int keep) { _keep = keep; }

void set_keep() { _keep = 1; }

void reset_keep() {
   _keep = 0;
   if (!_identifier)
      remove_this_effect();
}

string query_identifier() { return _identifier; }

/**
 * This is here to maintain compatibility with the magic spell effect
 * until it can be fixed.
 */
string query_full_identifier() {
    string tmp;

    tmp = "";

    if ( sizeof( _adjs ) ) {
        tmp += implode( _adjs, " " ) + " ";
    }

    if ( _identifier ) {
       tmp += _identifier;
    }

    if ( strlen( tmp ) > 0 ) {
       return tmp;
    }

    return 0;
} /* query_all_identifier() */
void set_identifier( string word ) {

   string* bits;

   if (word) {
      bits = explode(word, " ");
      _identifier = bits[<1];
      _adjs = bits[0..<2];
   } else {
      _identifier = 0;
      _adjs = ({ });
      if (!_keep)
         remove_this_effect();
   }
}

void set_identity_hold_owner(object ob) {
   _owner = ob;
} /* set_identity_hold_owner() */

object query_identity_hold_owner() {
   return _owner;
} /* query_identity_hold_owner() */

string *parse_command_id_list() {
   string *words;

   words = (string *)player->parse_command_id_list();
   if ( ( this_player() == _owner ) && stringp( _identifier ) &&
         !sizeof( filter_array( previous_object( -1 ),
         (: $1 && $1->ignore_identifier() :) ) ) ) {
      words += ({ _identifier });
   }
   return words;
} /* parse_command_id_list() */

string *parse_command_adjectiv_id_list() {
   string *words;

   words = (string *)player->parse_command_adjectiv_id_list();
   if ( ( this_player() == _owner ) && pointerp( _adjs ) &&
         !sizeof( filter_array( previous_object( -1 ),
         (: $1->ignore_identifier() :) ) ) ) {
      words += _adjs;
   }
   return words;
} /* parse_command_adjectiv_id_list() */

private void check_move_stuff() {
   object ob;

   ob = environment(player);
   while (ob &&
          ob != _owner) {
      ob = environment(ob);
   }

   if (ob != _owner) {
      remove_this_effect();
   }
} /* check_move_stuff() */

varargs int move( mixed dest, string messin, string messout ) {
   int flag;
    
   flag = player->move( dest, messin, messout );

   if ( flag == MOVE_OK &&
        objectp( dest ) ) {
      call_out((: check_move_stuff :), 2);
   }

   return flag;
} /* move() */
