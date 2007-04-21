/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: duplicate.c,v 1.3 2000/06/23 03:40:56 pinkfish Exp $
 * $Log: duplicate.c,v $
 * Revision 1.3  2000/06/23 03:40:56  pinkfish
 * Fix up the references to find_match.
 *
 * Revision 1.2  1998/08/19 10:51:00  pinkfish
 * Fix up to work withj the new base object.
 *
 * Revision 1.1  1998/01/06 05:26:23  ceres
 * Initial revision
 * 
*/
#include <artifacts.h>
#include <move_failures.h>

#define COST 50
#define LEVEL 100
#define SHAPING "crafts.pottery.forming.shaping"
#define SMITHING "crafts.smithing"
#define TURNING "crafts.carpentry.turning"
#define WHITTLING "crafts.carpentry.whittling"

inherit "/cmds/guild_base";
inherit "/std/basic/cute_look";

void create() {
   ::create();
   set_nroff_file("duplicate");
} /* create() */

int cmd( object* things) {
   int i, bonus;
   object form;
   mapping map;
 
   for ( i = 0; i < sizeof( things ); i++ ) {
      if ( !function_exists( "query_artifact_type", things[ i ] ) ) {
         write( "You cannot duplicate "+  things[ i ]->the_short() +".\n" );
         continue;
      }
      if ( (int)this_player()->query_specific_gp( "crafts" ) < COST ) {
         write( "You don't have enough guild points to duplicate "+
               things[ i ]->the_short() +".\n" );
         continue;
      }
      switch( (string)things[ i ]->query_artifact_type() ) {
       case "wand" :
         bonus = (int)this_player()->query_skill_bonus( TURNING );
         break;
       default :
         write( "Oops, something went wrong with "+ things[ i ]->the_short() +
               ".  Please contact a creator.\n" );
        continue;
      }
      if ( bonus < LEVEL ) {
         write( "Your skill is insufficient to make a form from "+
               things[ i ]->the_short() +".\n" );
         continue;
      }
      this_player()->adjust_gp( -COST );
      switch ( (string)things[ i ]->query_artifact_type() ) {
       case "wand" :
         form = clone_object( "/obj/weapon" );
         break;
      }
      map = (mapping)things[ i ]->query_static_auto_load();
      if ( !m_sizeof( map ) ) {
         map = (mapping)things[ i ]->int_query_static_auto_load();
         form->init_static_arg( map );
      } else
      form->init_static_arg( map[ "::" ] );
      map = (mapping)things[ i ]->query_dynamic_auto_load();
      form->init_dynamic_arg( map[ "::" ] );
      form->set_enchant( 0 );
      form->set_value_info( "artifact", 0 );
      if ( bonus < LEVEL + random( LEVEL ) )
         form->remove_property( "artifact form" );
      write( "You duplicate a form from "+ things[ i ]->the_short() +".\n" );
      if ( (int)form->move( this_player() ) != MOVE_OK )
         form->move( environment( this_player() ) );
   }
   return 1;
} /* cmd() */

mixed *query_pattern() {
   return ({ "<indirect:object:me>", (: cmd($1) :) });
} /* query_pattern() */
