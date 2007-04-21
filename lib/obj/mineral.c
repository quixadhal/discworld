/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mineral.c,v 1.9 2003/07/09 10:50:11 carmine Exp $
 * $Log: mineral.c,v $
 * Revision 1.9  2003/07/09 10:50:11  carmine
 * Prevented decay level being set
 *
 * Revision 1.8  2001/06/01 21:59:28  ceres
 * Added helpfile
 *
 * Revision 1.6  2000/11/21 03:15:42  ceres
 * Removed ansi colour sequences, they break in too many places.
 *
 * Revision 1.5  2000/05/24 02:33:26  pinkfish
 * Fix up the mineral code.
 *
 * Revision 1.4  2000/04/26 08:15:14  taffyd
 * Multiple bug fixes.
 *
 * Revision 1.3  2000/04/17 10:30:18  shaggy
 *  Forcibly unlocked by taffyd
 *
 * Revision 1.2  1998/10/30 08:36:08  pinkfish
 * Fix up to use set_name refernece.
 *
 * Revision 1.1  1998/01/06 04:58:17  ceres
 * Initial revision
 * 
*/
#include <mineral.h>

#define HANDLER "/obj/handlers/cmr_handler"
#define PRICE_INDEX "/obj/handlers/cpi_handler"
#define DEFAULT_MARKET "Ankh-Morpork"

inherit "/obj/food";

nosave string mineral;

void create() {
   ::create();
   set_decay_speed(0);
   remove_help_file("food");
   add_help_file("mineral");
} /* create() */

varargs void make_mineral( string word, int number, string *inputs ) {
   string adjective, material_adjective, noun, *args;

   mineral = word;
   set_value_info( "material", 1 );
   material_adjective = (string)HANDLER->query_material_adjective( mineral );
   set_weight( number );
   switch( number ) {
      case 0 .. PEBBLE :
         adjective = "small";
         noun = "pebble";
         break;
      case PEBBLE + 1 .. STONE :
         adjective = "medium sized";
         noun = "stone";
         break;
      case STONE + 1 .. ROCK :
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
   if ( !args[ 0 ] ) {
      set_name( noun );
      add_plural( noun +"s" );
   } else {
      set_name( args[ 0 ] );
      if ( !args[ 3 ] )
         add_plural( pluralize( args[ 0 ] ) );
      else
         add_plural( args[ 3 ] );
   }
   if ( !args[ 1 ] ) {
      set_short(material_adjective +" "+ noun);
      set_main_plural(material_adjective +" "+ pluralize( noun ));
   } else {
      set_short( args[ 1 ] );
      if ( !args[ 4 ] )
         set_main_plural( pluralize( args[ 1 ] ) );
      else
         set_main_plural( args[ 4 ] );
   }
   if ( !args[ 2 ] )
      set_long( "This is "+ add_a( adjective ) +" lump of $mineral$.\n" );
   else
      set_long( args[ 2 ] );
   add_adjective( ({ adjective }) + explode( material_adjective, " " ) );
} /* make_mineral() */

string long( string words, int dark ) {
   return replace( ::long( words, dark ), "$mineral$",
         (string)HANDLER->identify_material( mineral, this_player(), 1 ) );
} /* long() */

string *parse_command_adjectiv_id_list() {
   return explode( (string)HANDLER->identify_material( mineral,
         this_player(), 0 ), " " ) + ::parse_command_adjectiv_id_list();
} /* parse_command_adjectiv_id_list() */

string query_mineral() { return mineral; }

string query_material() { return mineral; }

void init() {
   ::init();
   this_player()->add_command( "pulverise", this_object(),
         "<direct:object> {with|using} <indirect:object>" );
   this_player()->add_command( "chip", this_object(),
         "<direct:object> {with|using} <indirect:object>" );
   this_player()->add_command( "smash", this_object(),
         "<direct:object> {with|using} <indirect:object>" );
   this_player()->add_command( "sprinkle", this_object(),
         "<direct:object:me> [on] <indirect:object>" );
   this_player()->add_command( "sprinkle", this_object(),
         "<direct:object:me>" );
} /* init() */

int do_eat() {
   if ( (string)this_player()->query_race_ob() != "/std/races/troll" ) {
      this_player()->add_failed_mess( this_object(),
            "What do you think you are, a troll?\n", ({ }) );
      return 0;
   }
   return ::do_eat();
} /* do_eat() */

int check_tool( object *tools, string *needed ) {
   string word;

   if ( sizeof( tools ) > 1 ) {
      write( "You only need to use one tool at a time.\n" );
      return 0;
   }
   if ( !sizeof( tools ) ) {
      write( "You need to use a tool of some sort.\n" );
      return 0;
   }
   if ( member_array( tools[ 0 ],
         (object *)this_player()->query_holding() ) == -1 ) {
      write( "You need to use a tool that you are holding.\n" );
      return 0;
   }
   foreach ( word in needed ) {
      if ( tools[ 0 ]->id( word ) )
         return 1;
   }
   write( "You need to use a tool appropriate to the job.\n" );
   return 0;
} /* check_tool() */

int do_pulverise( object *tools ) {
   string material_adjective;

   if ( !check_tool( tools, ({ "hammer" }) ) )
      return 0;
   if ( query_continuous() ) {
      write( "The "+ (string)HANDLER->identify_material( mineral,
            this_player(), 0 ) +" has already been pulverised.\n" );
      return 0;
   }
   if ( query_weight() > PEBBLE ) {
      write( "This piece of "+ (string)HANDLER->identify_material( mineral,
            this_player(), 0 ) +" is too large to pulverise.\n" );
      return 0;
   }
   material_adjective = (string)HANDLER->query_material_adjective( mineral );
   /* Hmmm... Can't use set_name() and an alias would be confusing, so... */
   set_name("powder");
   set_short(material_adjective +" powder");
   set_main_plural(material_adjective +" powder" );
   add_plural( "powder" );
   add_property( "determinate", "some " );
   add_adjective( ({ "some", "pinch", "pinches", "handful",
         "handfuls", "of" }) );
   set_long( "This is $pile_size$ of "+ material_adjective +
         " powder, $amount_size$.\n" );
   set_weight_unit( ({ 1, 50 }) );
   set_medium_alias( "Powdered" + capitalize(mineral) );
   set_amount_types( ([ "pinch": ({ 1, "pinches" }),
         "handful": ({ 50, "handfuls" }) ]) );
   set_pile_types( ({ 50, "small", 100, "medium", "large" }) );
   set_continuous();
   set_amount( roll_MdN( 5, 10 ) * query_weight() );
   remove_property("encoded destination");
   move( environment() );
   this_player()->add_succeeded( tools[ 0 ] );
   return 1;
} /* do_pulverise() */

int do_chip( object *tools ) {
   object chip;

   if ( query_continuous() )
      return 0;
   if ( !check_tool( tools, ({ "pick", "pickaxe", "hammer" }) ) )
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
   this_player()->add_succeeded( tools[ 0 ] );
   return 1;
} /* do_chip() */

int do_smash( object *tools ) {
   int i, j, largest, number, size;
   object debris, remains;

   if ( query_continuous() )
      return 0;
   if ( !check_tool( tools, ({ "pick", "pickaxe" }) ) )
      return 0;
   if ( query_weight() < ROCK / 2 ) {
      write( "This piece of "+ (string)HANDLER->identify_material( mineral,
            this_player(), 0 ) +" is too small to smash.\n" );
      return 0;
   }
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

int do_sprinkle(object *obs) {
    if (!query_continuous())
      return notify_fail(
        "You should pulverise the stuff before you sprinkle it.\n");

   if (!sizeof(obs)) {
     obs=({ environment(this_player()) });
     this_player()->add_succeeded_mess(this_object(),
       "$N $V $D everywhere.\n",({ }));
   } else {   
     this_player()->add_succeeded_mess(this_object(),
       "$N $V $D on $I.\n",({ obs[0] }));
   }

   obs[0]->add_effect("/std/effects/other/powdered", 
                      (string)HANDLER->query_material_adjective( mineral ) +
                      " powder" );
   call_out("move",0,"/room/rubbish");
   return 1;
} /* do_sprinkle() */

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
   if ( map[ "::" ] )
      ::init_static_arg( map[ "::" ] );
   if ( !undefinedp( map[ "mineral" ] ) )
      mineral = map[ "mineral" ];
   set_decay_speed(0);
} /* init_static_arg() */

mapping query_static_auto_load() {
   if ( ( explode( file_name( this_object() ), "#" )[ 0 ] == "/obj/mineral" )
         || query_continuous() )
      return int_query_static_auto_load();
   return ([ ]);
} /* query_static_auto_load() */


/*
 * This stops the decay level being set - just in case
*/
void set_decay_level( int level ) {
   ::set_decay_level( 0 );
   return;
} /* set_decay_level */

