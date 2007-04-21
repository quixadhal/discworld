/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: more_string.c,v 1.7 2003/06/04 23:16:08 pinkfish Exp $
 * 
 */
/**
 * This file will handle the real moreing of a string to the player.  The
 * player object will call out to this, so the all the code and the
 * variables are not stored in the player object.
 * @see /global/more_file.c
 *
 * @author Pinkfish
 */
#define COLS (int)this_object()->query_cols()
#define ROWS ( (int)this_object()->query_rows() - 1 )

/**
 * Information on the string being mored.
 * @element fsize the size of the file
 * @element topl the top line being displayed
 * @element botl the bottom line being display
 * @element last_search the last attempted search
 * @element the_bit the actual file itself
 * @element finish_func the function to call on finishing
 * @element more_bit the string to print at the bottom of the screen
 * @element stat_line the status line
 * @element finish_ob the finish object
 */
class more_string_info {
   int fsize;
   int topl;
   int botl;
   string last_search;
   string *the_bit;
   string finish_func;
   string more_bit;
   string stat_line;
   object finish_ob;
}

private nosave mixed *_pending_more;
private class more_string_info _current_info;

int internal_more_string();

void create() {
   add_command("Internal_More_String", "<string>", (:internal_more_string:));
} /* create() */

/**
 * @ignore yes
 */
private void string_display_file(class more_string_info info) {
  int i;
  string stuff;

  info->botl = info->topl + ROWS;
  stuff = "";
  for ( i = info->topl; ( i < info->botl ) && ( i < info->fsize ); i++ ) {
    stuff += info->the_bit[ i ] +"\n";
  }
  efun::tell_object( this_object(), stuff );
} /* string_display_file() */

/**
 * @ignore yes
 */
private void more_string_status_line(class more_string_info info) {
   string *frog;
   string s;
   int i;
   int percentage;

   if (!info->stat_line) {
      info->stat_line = "$N From $T to $B of $S ($%%) - h for help. ";
   }
   s = "";
   frog = explode(info->stat_line, "$");
   for (i=0;i<sizeof(frog);i++) {
      if (frog[i] == "") {
         s += "$";
         i ++;
      } else switch (frog[i][0]) {
         case 'N' :
            s += info->more_bit+frog[i][1..];
            break;
         case 'T' :
            s += ( info->topl + 1 ) + frog[ i ][ 1 .. ];
            break;
         case 'B' :
            if (info->botl > info->fsize)
               s += info->fsize+frog[i][1..];
            else
               s += info->botl+frog[i][1..];
            break;
         case '%' :
            percentage = (info->botl*100)/info->fsize;
            if (percentage > 100) {
               percentage = 100;
            }
            s += percentage + frog[i][1..];
            break;
         case 'S' :
            s += info->fsize + frog[i][1..];
            break;
      }
   }
   efun::tell_object( this_object(), (string)this_object()->fix_string( s ) );
   return;
} /* more_string_status_line() */

/**
 * @ignore yes
 */
private void string_next_page(string str, class more_string_info info) {
   int num;
   int noargs;
   int i;
   int redraw;
   string s1;

   if (!str) {
     str = "";
   }

   if (sscanf(str,"%d%s", num, str) != 2) {
     noargs = 1;
   }
   s1 = str[1..];
   /* case statements WEEEEEE */
   if (str == "") {
      str = "f";
   }
   switch(str[0]) {
      case 'h' :
         cat("/doc/helpdir/more");
         break;
      case 'q' :
         if (info->finish_func) {
            if (functionp(info->finish_func)) {
               evaluate(info->finish_func);
            } else {
               call_other(info->finish_ob, info->finish_func);
            }
         }
         info->finish_func = 0;
         _current_info = 0;
         return;
      case 'f' :
      case 'F' :
         /* go on a number of pages... */
         info->topl += ROWS;
         redraw = 1;
         break;
      case 'b' :
      case 'B' :
         info->topl -= ROWS;
         redraw = 1;
         break;
      case '/' :
         /* sigh */
         i = info->topl + 4;
         if (!s1 || s1 == "") {
            s1 = info->last_search;
         }
         if (!s1 || s1 == "") {
            s1 = "bing!";
         }
         for (i = info->topl + 4; i < info->fsize; i++) {
            if (strsrch(info->the_bit[i], s1) != -1) {
               if (num-- <= 0) {
                  break;
               }
            }
         }
         if (i == info->fsize) {
            tell_object( this_object(), "Sorry "+ s1 +" not found.\n" );
         } else {
            tell_object( this_object(), "Found " + s1 + " on line "+ i +".\n");
            info->topl = i - 3;
         }
         redraw = 1;
         break;
      case '?' :
         if (!s1 || s1 == "") {
            s1 = info->last_search;
         }
         if (!s1 || s1 == "") {
            s1 = "bing!";
         }
         for (i = info->topl + 2; i > 0; i--) {
            if (strsrch(info->the_bit[i], s1) != -1) {
               if (num-- <= 0) {
                  break;
               }
            }
         }
         if (i == 0) {
            tell_object(this_object(), "Sorry " + s1 + " not found.\n");
         } else {
            info->topl = i - 3;
         }
         redraw = 1;
         break;
      case 'b' :
         if (info->topl > 0) {
            info->topl -= ROWS;
            redraw = 1;
            if (info->topl < 0) {
               info->topl = 0;
            }
         }
         break;
      case 'g' :
         info->topl = num;
         if (info->topl >= info->fsize)
            info->topl = info->fsize - 2;
         redraw = 1;
         break;
      case 'G' :
         redraw = 1;
         if (noargs)
            info->topl = info->fsize - ROWS;
         else
            info->topl = num;
         if (info->topl > info->fsize)
            info->topl = info->fsize - 2;
         break;
   }

   if (info->topl < 0) {
      info->topl = 0;
   }

   if (redraw) {
      string_display_file(info);
      if ( info->botl < info->fsize ) {
         more_string_status_line(info);
         input_to((: string_next_page :), 0, info);
      } else {
         if (info->finish_func) {
            if ( info->finish_ob ) { // sometimes screwed up:(
               call_other(info->finish_ob, info->finish_func);
            }
            info->finish_func = 0;
         }
         _current_info = 0;
      }
   } else {
      more_string_status_line(info);
      input_to((: string_next_page :), 0, info);
   }
} /* string_next_page() */

/**
 * Puts a long string through a more function.
 * @param text the text to place through the pager
 * @param bity the title of the text
 * @param noreline do not do realignment of the text
 * @see /global/more_file.c->more_file()
 * @see set_finish_func()
 */
varargs int more_string( string text, string bity, int noreline ) {
  int i, ncols;
  string *bits;
  class more_string_info info;
  mixed func;

  if ( this_player() != this_object() ){
     if ( !_pending_more ) {
        _pending_more = ({ ({ text, bity, noreline }) });
     } else {
        _pending_more += ({ ({ text, bity, noreline }) });
     }
     this_object()->ignore_from_history("Internal_More_String something");
     command( "Internal_More_String something" );
     return 1;
  }

  if (!_current_info) {
     _current_info = new(class more_string_info);
  }

  info = _current_info;
  if ( bity ) {
    info->more_bit = bity;
  } else {
    info->more_bit = "--- MORE";
  }
  info->last_search = "";
  if ( !stringp( text ) || !strlen( text ) ) {
    return notify_fail( "Empty string.\n" );
  }
  if ( noreline ) {
    info->the_bit = explode( this_object()->fix_string( text ), "\n" );
  } else {
    text = this_object()->convert_message( text );
    /* for observer-dependent shorts */
    text = this_object()->fit_message( text );
    /* for capitalisation and indentation */
    text = this_object()->fix_string( text );

    info->the_bit = ({ });
    ncols = COLS;
    bits = explode( text, "\n" );
    for ( i = 0; i < sizeof( bits ); i++ ) {
      reset_eval_cost();
      if ( bits[ i ] == "" ) {
        info->the_bit += ({ "" });
      } else {
        info->the_bit +=
          explode( this_object()->fix_string( bits[ i ], ncols ), "\n" );
      }
    }
  }
  info->fsize = sizeof( info->the_bit );
  info->topl = 0;
  string_display_file(info);
  if ( info->botl < info->fsize ) {
    more_string_status_line(info);
    input_to( (: string_next_page :), 0, info );
  } else {
      if ( info->finish_func ) {
         if ( !info->finish_ob ) {
            //screwed up:
            info->finish_func = 0;
            return 1;
         }
         func = info->finish_func;
         info->finish_func = 0;
         call_other( info->finish_ob, func );
      }
      _current_info = 0;
  }
  return 1;
} /* more_string() */

/**
 * Sets the finish function.  This is called when the more_string
 * command exits.  If the ob is set to 0 then previous_object() is used
 * for it.  The str can be a function pointer as well.
 * @param str the function name or function pointer to use
 * @param ob the object to call it on (ignore for function pointers)
 * @see more_string()
 */
varargs void set_finish_func(string str, object ob) {
  if (!_current_info) {
     _current_info = new(class more_string_info);
  }
  _current_info->finish_func = str;
  if (!ob) {
    _current_info->finish_ob = previous_object();
  } else {
    _current_info->finish_ob = ob;
  }
} /* set_finish_func() */

/**
 * @ignore yes
 */
private int internal_more_string() {
   string text, bity;
   int noreline;
   
   if ( sizeof( _pending_more ) ) {
      text = _pending_more[ 0 ][ 0 ];
      bity = _pending_more[ 0 ][ 1 ];
      noreline = _pending_more[ 0 ][ 2 ];
      _pending_more = _pending_more[ 1 .. <1 ];
      more_string( text, bity, noreline );
   }
   return 1;
} /* internal_more_string() */
