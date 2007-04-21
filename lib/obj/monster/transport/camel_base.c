// Seems to fail when moving sometimes.
#include <npc/transport.h>
#include <broadcaster.h>
inherit TRANSPORT_INHERIT;

private int _quiet;

/**
 * This method sets the quiet flag on the camel.  If they are quiet
 * they will not say anything when they do commands and so on.
 * @param quiet the new value for the quiet flag
 */
void set_quiet( int quiet ) {
   _quiet = quiet;
} /* set_quiet() */

/**
 * This method queries the quiet flag on the camel.  If they are quiet
 * they will not say anything when they do commands and so on.
 * @return the current setting of the quiet flag
 */
int query_quiet() {
   return _quiet;
} /* query_quiet() */

/** @ignore yes */
void hear_name( object player, string mess ) {
   string *bits;
   string str;
   int allowed;

   if ( !player ) {
      return ;
   }

   if ( is_allowed_to_use( player->query_name() ) ) {
      //
      // Ok, lets see if they asked us to do anything...
      //
      bits = explode( mess, " " ) - ({ "" });
      foreach ( str in bits ) {
         switch ( str ) {
         case "follow" :
            do_command( "follow " + player->query_name() );
            break;
         case "stop" :
            do_command( "unfollow " + player->query_name() );
            break;
         case "stay" :
            do_command( "unfollow all" );
            break;
         case "quiet" :
            set_quiet(1);
            break;
         case "speak" :
         case "talk" :
            set_quiet(0);
            break;
         }
      }
      allowed = 1;
   }

   if ( !query_quiet() ) {
      if ( allowed ) {
        if( player->query_visible( this_object() ) )
          do_command( "slook " + player->query_name() );
      }
      switch ( random(3) ) {
      case 0:
         do_command( "spit" );
         break;
      case 1:
         do_command( ":ruminates." );
         break;
      case 2:
         do_command( ":looks thoughtful." );
         break;
      }
   }
} /* hear_name() */

/** @ignore yes */
void event_person_shout( object thing, string start, string mess, string lang,
                         int *co_ord, int range ) {
   if ( !environment() )
      return;
   if ( environment()->query_property( "shout zone" ) !=
        environment( this_player() )->query_property( "shout zone" ) ||
        !thing ) {
      return;
   }
   mess = lower_case( mess );
   if ( is_allowed_to_use( thing->query_name() ) ) {
      if ( strsrch( mess, lower_case( query_transport_name() ) )  != -1 ) {
         if (strsrch( mess, "come here" ) != -1) {
            if ( !query_quiet() ) {
               do_command( "grumble" );
            }
            add_effect( "/std/effects/npc/goto_destination",
                        environment(thing) );
         }
      }
   }
} /* event_person_shout() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   return ([ "::" : ::query_dynamic_auto_load(),
             "quiet" : query_quiet() ]);
} /* query_dynamic_auto_load() */

/** @ignore yes */
void setup_responses() {
  add_respond_to_with( explode(
                       lower_case( query_transport_base_name() ), " " ),
                       (: hear_name :) );
  BROADCASTER->npc_hear_shouts( this_object() );
} /* setup_responses() */

/** @ignore yes */
void init_dynamic_arg( mapping map ) {
   ::init_dynamic_arg( map["::"] );
   if ( map["quiet"] ) {
      set_quiet( map["quiet"] );
   }
   if ( query_transport_name() ) {
      setup_responses();
      set_short( query_transport_name() );
      add_adjective( explode( lower_case( query_transport_name() ), " " )
                     - ({ "the" }) );
      add_property( "determinate", "" );
      add_alias( explode( lower_case( query_transport_name() ), " " )
                 - ({ "the" }) );
   }
} /* init_dynamic_arg() */

/** @ignore */
int do_name( string name ) {
   if ( ::do_name( name ) ) {
      setup_responses();
      set_short( query_transport_name() );
      return 1;
   }
   return 0;
} /* do_name() */
