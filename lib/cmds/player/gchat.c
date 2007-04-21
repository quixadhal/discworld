/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: gchat.c,v 1.8 1999/06/21 09:10:39 tape Exp $
 * $Log: gchat.c,v $
 * Revision 1.8  1999/06/21 09:10:39  tape
 * This command is no longer used.  --Tape/21/06/99
 *
 * Revision 1.7  1998/10/27 02:52:45  gruper
 * Made the command ear muffable.
 *
 * Revision 1.6  1998/10/20 01:08:25  presto
 * Removed an extra space in the output messages
 *
 * Revision 1.5  1998/10/11 02:18:04  presto
 * Changed "You ask BlahGroup asking: ..." to "You ask BlahGroup: ..."
 *
 * Revision 1.4  1998/09/27 20:33:20  ceres
 * Added chats to players history
 *
 * Revision 1.3  1998/06/23 23:55:29  presto
 * Fixed something I stuffed up with the last fix. :-b
 *
 * Revision 1.2  1998/06/21 18:37:03  presto
 * fixed typo, added "in <language>" to "asking" messages
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
#include <language.h>
#include <player.h>
#include <drinks.h>

inherit "cmds/base";
inherit "cmds/speech";

// This command is no longer used.  --Tape/21/06/99

#define TEAM_HANDLER "/obj/handlers/team"
#define TP this_player()

mixed cmd(string arg) {
  string word, cur_lang, group;
  object ob, *members, *earmuffed;

  return notify_fail( "This command is no longer used.  Please "
   "use \"group say\" instead.\n" );
   
  /* Don't allow someone with gchat ear muffed to use the command. */
  if( TP->check_earmuffs( "gchat" ) )
     return notify_fail( "You have gchat ear muffed.\n" );

  group = TEAM_HANDLER->query_group(this_player());
  if(!group)
    return notify_fail("You are not a member of any group.\n");
  
  if(!arg)
    return notify_fail("Syntax: gchat <message>\n");

  cur_lang = TP->query_current_language();
  
  if (!LANGUAGE_HAND->query_language_spoken(cur_lang))
    return notify_fail(capitalize(cur_lang)+" is not a spoken language.\n");
  
  if (!LANGUAGE_HAND->query_language_distance(cur_lang))
    return notify_fail(capitalize(cur_lang)+" is not able to spoken at a "
                       "distance.\n");
  
  word = query_word_type(arg, "");
  if (word != "")
    word = word + "ing";
  
  if (TP->query_volume(D_ALCOHOL))
    arg = drunk_speech(arg);
  
  if(function_exists("mangle_tell", environment(TP)))
    arg = environment(TP)->mangle_tell(arg, ob, 0);
  
  /* Who's a member of the group? */
  members = TEAM_HANDLER->query_members(group);
  members -= ({ this_player() });

  /* Check that the group has members. (Strange Phenomenon(tm)?)
   * This check wasn't made when word == asking - was this intentinal? */
  if ( !members ) {
    TEAM_HANDLER->leave_group( group, this_player() );
    TEAM_HANDLER->end_group( group );
    write( "Somehow your group has no members.  "
        "Your group has been ended.  "
        "I hope this is okay.\n" );
    return 1;
  }

  /* Remove people with gchat earmuffed from array of
   * people to give the message to. */
  earmuffed = ({ });
  foreach( ob in members ) {
    if( ob->check_earmuffs( "gchat" ) ) {
      earmuffed += ({ ob });
      members -= ({ ob });
    }
  }
  if( sizeof( earmuffed ) ) {
    write( capitalize( query_multiple_short( earmuffed ) ) +
        ( sizeof( earmuffed ) > 1 ? " have":" has" )+
        " gchat ear muffed.\n");
  }

  if( !sizeof( members ) ) {
    return notify_fail( "It seems you are the only one listening to "
          "this channel.\n" );
  } else if(word != "asking") {

    if( word != "" )
      word = " "+ word;

    foreach(ob in members) {
      if ( ob && interactive( ob ) )
        ob->event_person_tell(TP, capitalize((string)TP->query_name())+
                              " tells "+ group + word +": ", arg,
                              cur_lang );
    }
    if(cur_lang != "common")
      word += " in "+cur_lang;
 
    my_mess("You tell "+ group + word +": ", arg);
    TP->add_tell_history( "You tell "+ group + word + ": ", arg );
  } else {
    foreach(ob in members) {
      if ( ob && interactive( ob ) )
        ob->event_person_tell(TP, capitalize((string)TP->query_name())+
                              " asks "+group+": ", arg, cur_lang );
    }
    if(cur_lang != "common")
      word = " in "+ cur_lang;
    else word = "";
 
    my_mess( "You ask "+ group + word +": ", arg);
    TP->add_tell_history( "You ask "+ group + word + ": ", arg );
  }
  TP->adjust_time_left(-5);
  return 1;
}

