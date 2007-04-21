#include <position.h>
#include <soul.h>
#include <talker.h>

/* Current time for broom flight is 12 seconds */
#define TIME_LIMIT 15
#define BROOMSTICK "/d/guilds/witches/items/broomstick"
#define GRANNY "/d/guilds/witches/chars/granny_weatherwax"

inherit "/std/outside";

int screen_command( string cmd );

string *people;

string *allowed_commands = ({ "l", "look", "glance", /* "eat", "drink",
    "taste", "t", "tell", "say", "lsay", "shout", "emote", "hedgehog",
    "i", "inventory", "sing", "croon", */ });

void setup() {
  set_light( 100 );
  set_short( "high above the Disc" );
  add_property( "determinate", "" );
  set_day_long( "You are high above the Disc, flying through the "
      "air.\n" );
  set_night_long( "You are high above the Disc, flying through the "
      "night sky.\n" );
  add_item( ({ "down", "disc", }), "The Disc spreads out below "
      "you, much too far away to make out anything distinctly." );
  people = ({ });
}

int query_flying( string who ) {
  if( member_array( who, people ) > -1 )
    return 1;
  else
    return 0;
}

void drop_non_living( object ob ) {
  object broom = load_object( BROOMSTICK );
  object player;
  object dest;

  if( ob->query_corpse() ) {
    string name = ob->query_owner();
    name = lower_case( explode( name, " " )[0] );
    player = find_player( name );
  }

  if( !player )
    player = load_object( GRANNY );

  if( !broom || !player )
    return;  // bugger.  Let it hover.

  dest = broom->random_dest( player );
  ob->move( dest, "$N streaks down from above and crashes into the ground "
                  "with a thud, narrowly missing you.",
                  "$N disappear$s from view far below." );
}

/*  Damn, they're still here.  Put them down somewhere.  */
void down_they_go( object witch, object *riding ) {
  object broom;    // the broom she's riding
  object *brooms;  // all the brooms in her inventory
  object *held;    // all the brooms she's *holding*
  string path;     // the path of the place she's headed
  object dest;     // the place she's headed

  // Maybe she's not even here!
  if( !witch )
    return;

  people -= ({ witch->query_name() });

  if( environment( witch ) != this_object() )
    return;

  // Did she enter the air room by flying?  (As opposed to eg restarting
  // here, being here when the room was updated, or some other rare event)
  // If so, use the broom she flew in on to land her.
  // NB: Fireflies, fruitbats etc. don't have a broom in this list.

  if( sizeof( riding ) && riding[0] != 0 ) {
    broom = riding[0];
    tell_creator( "tannah", "broom: %O\n", broom );
  } else {

    // We've lost track of which broom she flew in on.  Let's try to find
    // another one to use.  This might drop her into a destination from a
    // previous flight, but at least it'll drop her somewhere.
    brooms = match_objects_for_existence( "witches' broomstick",
                                          ({ witch }), 0 );

    // does she have any?
    if( !sizeof( brooms ) ) {
      // if she's not holding any brooms, check to see if she's still
      // autoloading
      if( witch->query_auto_loading() ) {
        call_out( (: down_they_go :), TIME_LIMIT, witch, riding );
        tell_object( witch, "Your thighs twitch reflexively and your "
                            "hands scrabble at the air, searching "
                            "for your broom.\n" );
        return;
      } else { // she isn't autoloading and she hasn't got one.  SUX0R!
        broom = load_object( BROOMSTICK );
      }

    } else if( sizeof( brooms ) > 1 ) {

      // if she's got more than one, pick the one she's holding.  If
      // she isn't holding any, pick any of them (she'll crash when she
      // lands)

      held = filter( brooms, (: $1->query_holder() == $(witch) :) );
      if( sizeof( held ) )
        broom = held[0];
      else
        broom = brooms[0];
    } else {
      // she's only got the one.  Use it.
      broom = brooms[0];
    }
  }

  // if it's got a destination, use it.  If not, pick a new one.
  path = broom->query_destination();
  if( path )
    dest = load_object( path );
  else
    dest = broom->random_dest(witch);

  tell_creator( "tannah", "Broom: %O\nLanding in: %O\n", broom, dest );
  broom->land( witch, dest );
}

void event_enter( object ob, string message, object from ) {

  if( !living( ob ) ) { // objects fall straight away
    ob->set_position( "dropping towards the ground" );
    call_out( (: drop_non_living :), 2, ob );
    return;
  }

  /* Block all actions they might take up here and screen out those that
   * aren't allowed */

  ob->command_override( (: screen_command :) );

  /* Sometimes the call_out in the broom gets lost.  This shouldn't be
   * needed too often, but still...  */
  people += ({ ob->query_name() });
  call_out( (: down_they_go :), TIME_LIMIT, ob,
            filter( previous_object(-1), (: $1->id( "broomstick" ) :) ) );
}

mixed query_default_position() {
   return ({ "flying through the air" });
}

void event_exit(object ob, string message, object to) {

   tell_object( ob, "You plummet towards the ground.\n" );
   tell_room( this_object(), ob->the_short()
         +" plummets towards the ground.\n", ob );

   STANDING_CMD->position( ob, 1 );
}

int screen_command( string cmd ) {
  string garbage;  /* the part of the command string we don't need */

  sscanf( cmd, "%s %s", cmd, garbage );

  /* We'll completely ignore aliases unless they're broken down. */

  if( cmd == "END_ALIAS" ||
      (function_exists("query_aliases",this_player()) &&
       member_array( cmd, keys( this_player()->query_aliases() ) ) != -1 ))
    return notify_fail( "" );

  /* We'll check cmd against the list of allowed commands.  If it's in the
   * list, returning 0 allows it to execute normally.  We're not allowing
   * souls for now. */

  if( member_array( cmd, allowed_commands ) != -1 /* ||
      SOUL_OBJECT->query_soul_command( cmd ) */ )
    return 0;

  /* Let cres do things players can't, but warn them. */

  if( this_player()->query_creator() ) {
    write( "You attempt aerial feats mere mortals dare not.\n" );
    return 0;
  }

  /* A nice little array of messages, so they don't get bored with the
   * same warning every time. */

  write( ({
      "You don't dare do that while flying so high above the ground.\n",
      "Your broom lurches alarmingly, nearly tossing you to the ground "
          "below.\n",
      "Your concentration wanes and your broom suddenly loses "
          "altitude.\n",
      })[ random( 3 ) ] );
  return 1;

  /* The rest of the function isn't ever used because of the above return,
   * but I left it here in case we want to expand the list of allowed
   * commands later. */

  /* if the first character is ', then it's a say and allowed. */
  if( cmd[0] == 39 ) return 0;

  /* if the first character is ", then it's a say and allowed. */
  if( cmd[0] == 34 ) return 0;

} /* screen_command() */

string *query_people() { return people; }