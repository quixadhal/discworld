/**
 * Generic piece of writing paper
 * @author unknown, lost in the rivers of time
 */

#include <playtesters.h>

#define AUTO_LOAD_TAG "paper"

inherit "/std/object";

/* Function prototypes */
int do_uncrumple();
int do_crumple();
int do_eat();

/* Global variables */

private int _crumpled;
private string _old_short;

void create() {
   do_setup++;
   ::create();
   do_setup--;

   set_name( "paper" );
   set_short( "piece of writing paper" );
   add_adjective( ({ "piece of", "writing" }) );
   set_main_plural( "pieces of writing paper" );
   add_plural( "pieces of writing paper" );
   add_adjective(({ "piece", "pieces", "of", "writing" }));
   set_long( "This is a nice sheet of writing paper.\n" );
   set_value( 25 );
   set_weight( 0 );
   add_property( "writeable", 1 );
   add_property( "paper", 1 );
   set_max_size( 1000 );

   if ( !do_setup ) {
      this_object()->setup();
   }

} /* create() */

void init() {
   if ( !this_player() ) {
       return;
   }

   add_command( "eat", "<direct:object>", (: do_eat :) );
   add_command( "crumple", "<direct:object>", (: do_crumple :) );
   add_command( "untangle", "<direct:object>", (: do_uncrumple :) );
   add_command( "uncrumple", "<direct:object>", (: do_uncrumple :) );

/*
   if ( PLAYTESTER_HAND->query_tester( this_player() ) ) {
      ORIGAMI_HANDLER->add_origami_commands( this_player(), this_object() );
   }*/

} /* init() */

/**
 * This method checks to see whether or not this piece of paper is
 * edible.
 * @return 1 if the object is edible.
 */
public int query_edible() {
    return 1;
} /* query_edible() */

/**
 * This method checks to see whether or not this item is a piece of
 * paper.
 * @return 1 if the item is a piece of paper
 */
public int query_paper() {
   return 1;
} /* query_paper() */

/** @ignore yes */
int do_crumple() {
   if (_crumpled) {
      add_failed_mess("$D is already crumpled.\n");
      return 0;
   }
   _old_short = query_short();

   call_out( (: set_short :), 1, "crumpled up " + query_short() );
   _crumpled = 1;
   return 1;
} /* do_crumple() */

/** @ignore yes */
int do_uncrumple() {
    if (!_crumpled) {
        add_failed_mess("$D is already uncrumpled.\n");
        return 0;
    }

    // We do this in the call_out to make sure that the messages are
    // set properly.

    call_out( (: set_short :), 1, _old_short );
   _crumpled = 0;
   return 1;
} /* do_untangle() */

int do_eat() {
   string *flavours = ({ "red ink", "green ink", "black ink",
       "blue ink", "orange", "apple", "cinnamon", "banana",
       "monkey", "cabbage", "hat" });

   add_succeeded_mess( ({ "You open your mouth up widely and pop $D into "
      "your mouth.  After a moment's deliberation, you chew and consume "
      "$D.\n", "$N $V $D.\n" }) );

   call_out( (: tell_object( $1, $2 ) :), 1,
       this_player(), "Mmm! " +
       capitalize( flavours[random( sizeof( flavours ) ) ] ) +
       " flavoured!\n" );

   move( "/room/rubbish" );
   return 1;
} /* do_eat() */

/** @ignore yes */
string query_readable_message(object player, int ignore_labels) {
   if (!_crumpled) {
      return ::query_readable_message(player, ignore_labels);
   }
   return "Only bits of ink can be seen on the crumpled paper.\n";
} /* query_readable_message() */

/** @ignore yes */
string *parse_command_adjectiv_id_list() {
   if (_crumpled) {
      return ::parse_command_adjectiv_id_list() + ({ "crumpled", "up" });
   }
   return ::parse_command_adjectiv_id_list();
} /* parse_command_adjectiv_id_list() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
    mapping map;

    map = ::query_dynamic_auto_load();

    add_auto_load_value( map, AUTO_LOAD_TAG, "crumpled", _crumpled );
    add_auto_load_value( map, AUTO_LOAD_TAG, "old short", _old_short );

    return map;
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg( mapping map, object ) {
    ::init_dynamic_arg(map);

    if (map) {
        _crumpled = query_auto_load_value( map, AUTO_LOAD_TAG, "crumpled" );
        _old_short = query_auto_load_value( map, AUTO_LOAD_TAG, "old short" );

        // These can be removed at a later date.

        if ( map[ "paper - crumpled" ] ) {
            _crumpled = map[ "paper - crumpled" ];
        }

        if ( map[ "paper - short" ] ) {
            _old_short = map[ "paper - short" ];
        }
    }
} /* init_dynamic_arg() */

/** @ignore yes */
mapping query_static_auto_load() {
   if ( explode( file_name( this_object() ), "#" )[ 0 ] != "/obj/misc/paper" )
      return ([ ]);
   return int_query_static_auto_load();
} /* query_static_auto_load() */

