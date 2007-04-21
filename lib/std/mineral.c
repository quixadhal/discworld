#include <mineral.h>

#define HANDLER "/obj/handlers/cmr_handler"
#define PRICE_INDEX "/obj/handlers/cpi_handler"
#define DEFAULT_MARKET "Ankh-Morpork"

inherit "/std/object";

int check_tool( object *tools );

nosave string mineral;

void create() {
   write( "If you've not just logged in and are waiting for your "
         "inventory to be regenerated, please bug report this location, "
         "as it's using the obsolete mineral object.\n" );
   ::create();
} /* create() */

void init() {
   this_player()->add_command( "chip", this_object(), "<direct:object> 'with' <indirect:object>" );
   this_player()->add_command( "chip", this_object(), "<direct:object> 'using' <indirect:object>" );
   this_player()->add_command( "smash", this_object(), "<direct:object> 'with' <indirect:object>" );
   this_player()->add_command( "smash", this_object(), "<direct:object> 'using' <indirect:object>" );
} /* init() */

varargs void make_mineral( string word, int number, string *inputs ) {
   string adjective, material_adjective, colour_code, noun, *args;
   mineral = word;
   material_adjective = (string)HANDLER->query_material_adjective( mineral );
   colour_code = (string)HANDLER->query_material_ansi_colour( mineral );
   set_weight( number );
   switch( number ) {
      case 0..PEBBLE:
         adjective = "small";
         noun = "pebble";
         break;
      case PEBBLE+1..STONE:
         adjective = "medium sized";
         noun = "stone";
         break;
      case STONE+1..ROCK:
         adjective = "large";
         noun = "rock";
         break;
      default:
         adjective = "huge";
         noun = "boulder";
   }
   if ( !pointerp( inputs ) )
      args = allocate( 10 );
   else
      args = inputs + ({ 0, 0, 0, 0, 0 });
   if ( !args[0] ) {
      set_name( noun );
      add_plural( noun +"s" );
   } else {
      set_name( args[0] );
      if ( !args[3] )
         add_plural( args[0] +"s" );
      else
         add_plural( args[3] );
   }
   if ( !args[1] ) {
      set_short( colour_code + material_adjective +" "+ noun +"%^RESET%^" );
      set_main_plural( colour_code + material_adjective +" "+ noun +
            "s%^RESET%^" );
   } else {
      set_short( args[1] );
      if ( !args[4] )
         set_main_plural( args[1] +"s" );
      else
         set_main_plural( args[4] );
   }
   if ( !args[2] )
      set_long( "This is a "+ adjective +" lump of $mineral$.\n" );
   else
      set_long( args[2] );
   add_adjective( ({ adjective }) + explode( material_adjective, " " ) );
} /* make_mineral() */

string long( string str, int dark ) {
   string bit1, bit2, ret;
   sscanf( ::long( str, dark ), "%s$mineral$%s", bit1, bit2 );
   ret = bit1 + (string)HANDLER->identify_material( mineral, this_player(), 1 )
      + bit2;
   return ret;
} /* long() */

string *query_adjectives() {
   string ret;
   ret = HANDLER->identify_material( mineral, this_player(), 0 );
   return ::query_adjectives() + ({ ret });
} /* query_adjectives() */

string *parse_command_adjectiv_id_list() {
   string ret;
   ret = HANDLER->identify_material( mineral, this_player(), 0 );
   return ::parse_command_adjectiv_id_list() + ({ ret });
} /* parse_command_adjectiv_id_list() */

string query_mineral() { return mineral; }

string query_material() { return mineral; }

int query_value() {
  return ( query_weight() * (int)(PRICE_INDEX)->query_price( mineral,
            DEFAULT_MARKET ) );
} /* query_value() */

int query_value_in( string word ) {
  if ( ( !word || ( word == "" ) ) || ( word == "default" ) )
    word = DEFAULT_MARKET;
  return ( query_weight() * (int)(PRICE_INDEX)->query_price( mineral, word ) );
} /* query_value_in() */

int do_chip( object *tools ) {
   object chip;
   if ( !check_tool( tools ) )
      return 0;
   if ( query_weight() == 1 ) {
      write( "This piece of "+ (string)HANDLER->identify_material( mineral,
                this_player(), 0 ) +" is already a chip.\n" );
      return 0;
   }
   set_weight( query_weight() - 1 );
   chip = clone_object( MINERAL );
   chip->make_mineral( mineral, 1 );
   chip->move( environment() );
   this_player()->add_succeeded( tools[0] );
   return 1;
} /* do_chip() */

int do_smash( object *tools ) {
   int i, j, largest, number, size;
   object debris, remains;
   if ( !check_tool( tools ) )
      return 0;
   largest = this_player()->query_max_weight();
   if ( largest >= query_weight() )
      largest = query_weight();
   else {
      remains = clone_object( MINERAL );
      remains->make_mineral( mineral, query_weight() - largest );
      remains->move( environment() );
   }
   tools[ 0 ]->hit_weapon( largest, "blunt" );
   number = 2 + random( 4 );
   for ( i = 0; i < 8; i++ ) {
      largest /= number;
      for ( j = 1; j < number; j++ ) {
         size = ( 3 * largest + random( largest ) ) / 4;
         if ( size ) {
            debris = clone_object( MINERAL );
            debris->make_mineral( mineral, size );
            debris->move( environment() );
          }
       }
   }
   this_player()->add_succeeded( tools[0] );
   move( "/room/rubbish" );
   return 1;
} /* do_smash */

int check_tool( object *tools ) {
   string tool;
   object *held;
   if ( sizeof( tools ) > 1 ) {
      write( "You only need to use one tool at a time.\n" );
      return 0;
   }
   if ( !sizeof( tools ) ) {
      write( "You need to use a tool of some sort.\n" );
      return 0;
   }
   held = this_player()->query_holding();
   if ( member_array( tools[0], held ) == -1 ) {
      write( "You need to use a tool that you are holding.\n" );
      return 0;
   }
   tool = tools[0]->query_name();
   if ( member_array( tool, ({ "pickaxe", "pick" }) ) == -1 ) {
      write( "You need to use a tool appropriate to the job.\n" );
      return 0;
   }
   return 1;
} /* check_tool() */

mixed *stats() {
   return ::stats() + ({
      ({ "mineral", mineral, }),
   });
} /* stats() */

mapping int_query_static_auto_load() {
   return ([
      "::" : ::int_query_static_auto_load(),
      "mineral" : mineral,
   ]);
} /* int_query_static_auto_load() */

void init_static_arg( mapping map ) {
   if ( map["::"] )
      ::init_static_arg( map["::"] );
   if ( !undefinedp( map["mineral"] ) )
      mineral = map["mineral"];
} /* init_static_arg() */

mixed query_static_auto_load() {
   if ( file_name( this_object() )[ 0 .. 11 ] == "/std/mineral" )
      return int_query_static_auto_load();
   return ([ ]);
} /* query_static_auto_load() */
