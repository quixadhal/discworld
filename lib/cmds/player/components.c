/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: components.c,v 1.1 2002/04/17 02:28:51 presto Exp $
 *
 *
 */

#define OBJ 0

inherit "/cmds/base";

int cmd( string spell_name ) {
  mapping spells = this_player()->query_spells_nocase();
  string spell_ob;
  string *consumed, *needed;
  string mess;

  if( !sizeof( spells ) ) {
    add_failed_mess( "You don't know any magic spells.\n" );
    return 0;
  }
  
  spell_name = this_player()->expand_nickname( spell_name );
  spell_name = capitalize( spell_name );

  if( !spells[ lower_case(spell_name) ] ) {
    add_failed_mess( "You don't know a spell named " + spell_name + 
        ".\n" );
    return 0;
  }
  
  spell_ob = spells[ lower_case(spell_name) ][ OBJ ];  
  consumed = spell_ob->query_consumables();
  needed   = spell_ob->query_needed();
  
  if( !sizeof( consumed ) && !sizeof( needed ) ) {
    mess = "You don't need anything to cast " + spell_name;
  } else if( sizeof( consumed ) ) {  
    mess = spell_name + " will consume " + query_multiple_short(
           consumed );
    if( sizeof( needed ) ) {
      mess += ".  It also requires " + query_multiple_short(
                 needed ) + " but will not consume " + 
                 ( sizeof( needed ) > 1 ? "them" : "it" );
    }
  } else {
    mess = spell_name + " requires " + query_multiple_short( needed ) +
           " but will not consume " + ( sizeof ( needed ) > 1 ?
           "them" : "it" );
  }
   
  add_succeeded_mess( ({ mess + ".\n", "" }) );
  return 1;
} /* cmd() */

mixed *query_patterns() {
  return ({ "[for] <string'spell'>", (: cmd( $4[0] ) :) });
} /* query_patterns() */
