/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: em_ote.c,v 1.10 2001/11/22 17:53:05 taffyd Exp $
 * $Log: em_ote.c,v $
 * Revision 1.10  2001/11/22 17:53:05  taffyd
 * Patched to allow double spaces only.,
 *
 * Revision 1.9  2000/03/05 16:34:35  mansarde
 * added the same support for possessive emotes that i did with cre@
 *
 * Revision 1.8  1999/05/19 06:34:37  pinkfish
 * Fix up some stuff.
 *
 * Revision 1.7  1999/05/04 22:56:36  pinkfish
 * Fix it up to really capitlise npc names :)
 *
 * Revision 1.6  1999/05/04 22:23:51  pinkfish
 * Fix it up so that npcs capitalize correctly.
 *
 * Revision 1.5  1999/05/04 22:22:12  ceres
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.4  1999/04/28 05:36:38  pinkfish
 * Changed the emote length limit to only work on players.
 *
 * Revision 1.3  1999/04/26 22:16:32  ceres
 * Modified to prevent abuses.
 *
 * Revision 1.2  1998/11/01 12:10:34  terano
 * added a call too room->event_emote...
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
#include <library.h>
#include <player.h>

inherit "/cmds/base";

mixed cmd( string words ) {
  string pad = " ";
  
  if(!environment(this_player())) {
    return notify_fail("You are in limbo, you cannot emote.\n");
  }
  
  if ( userp( this_player() ) && !this_player()->query_creator() ) {
    if ( this_player()->query_property( "emote" ) ) {
      LIBRARY->set_quest( (string)this_player()->query_name(), "emote" );
      this_player()->remove_property( "emote" );
    }
    if ( !LIBRARY->query_quest_done( (string)this_player()->query_name(),
                                     "emote" ) ) {
      return notify_fail( NOT_ALLOWED );
    }

    // so we can remove the permission to do emotes.
    if(this_player()->query_property("no emote")) {
      return notify_fail(NOT_ALLOWED);
    }
    
    if ( (int)this_player()->adjust_sp( -EMOTE_COST ) < 0 ) {
      return notify_fail( NO_POWER );
    }
  }

  if ( !words || ( words == "" ) ) {
    return notify_fail( "Syntax: emote <text>\n" );
  }

  words = strip_colours(words);
  // have a go at replacing multiple spaces with a single one. This should
  // stop anyone trying to format the emote to look as though it's an emote
  // followed by a tell or somesuch.
  words = replace(words, ({"        ", " ",
                           "       ", " ",
                           "      ", " ",
                           "     ", " ",
                           "    ", " ",
                           "   ", " ",
//                           "  ", " "}));
                            }) );

  
  this_player()->adjust_time_left( -5 );
  if (words[0..0] == "'") pad = "";
  environment( this_player() )->event_emote( this_player(),
                                             "$C$" + this_player()->one_short() + pad +
                                             words +"\n" );
  all_inventory( environment( this_player() ) )->
    event_emote( this_player(), "$C$" + this_player()->one_short() +
                                pad + words + "%^RESET%^\n" );
  write( "You emote: $C$"+
         (string)this_player()->pretty_short( this_player() ) + pad + words +
         "%^RESET%^\n" );
  return 1;
} /* cmd() */
