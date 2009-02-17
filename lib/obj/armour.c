/*  -*- LPC -*-  */
/*
 * $Id: armour.c,v 1.20 2003/01/09 10:51:14 taffyd Exp $
 */
#include <move_failures.h>
#include <virtual.h>

#define AC_LIMIT 100

#undef INFORM

inherit "/std/object";
inherit "/std/basic/wearable";
inherit "/std/armour_logic";


void create() {
   do_setup++;
   object::create();
   armour_logic::create();
   wearable::create();
   add_alias( "armour" );
   add_plural( "armours" );
   add_help_file("armour");
   if ( !query_property( "shop type" ) )
      add_property( "shop type", "armoury" );
   do_setup--;
   if ( !do_setup )
      this_object()->setup();
} /* create() */

int query_armour() { return 1; }

string long( string word, int dark ) {
   return object::long( word, dark ) + wearable::long( word, dark );
} /* long() */

varargs int query_ac( string type, int amount, string zone ) {
   int a_class, tmp;

#ifdef INFORM
   string message;
#endif
   a_class = armour_logic::query_ac( type, amount, zone );

#ifdef INFORM
   message = query_short() +": striking " + zone + ", basic value "+ a_class;
#endif

   if ( a_class > AC_LIMIT )
      a_class = AC_LIMIT;
   a_class += ( a_class * query_enchant() ) / query_max_enchant() + query_enchant();

#ifdef INFORM
   message += "; after enchant "+ a_class;
   if ( objectp( worn_by ) )
      event( environment( worn_by ), "inform", message, "combat" );
#endif

   do_damage( type, amount );
   a_class = ( a_class * query_cond() ) / query_max_cond();

   // damage may get through depending on how good the armour is.
   switch(random(a_class)) {
   case 0:
     return 0;
   case 1:
     return a_class / 2;
   case 2..3:
     return a_class * 2 / 3;
   default:
     // they'll always take a small amount of damage.
     if(a_class > amount)
       tmp = amount - (amount / (5 + random(10)));
     else
       tmp = a_class - (amount / (5 + random(10)));

     if(tmp < a_class / 2)
       tmp = a_class / 2;
     return tmp;
   }
} /* query_ac() */

void setup_armour( int number ) {
   set_max_cond( number );
   set_cond( number );
   set_lowest_cond( number );
} /* setup_armour() */

int query_value() {
   return modify_value( object::query_value() );
} /* query_value() */

int query_full_value() { return object::query_value(); }

/**
 * This function is called on an object and causes it to be worn
 * if it can be by it's environment(), i.e the player carrying it.
 */
void player_wear() {
   if ( !environment() )
      return;
   wearable::player_wear();
} /* player_wear() */

/**
 * @ignore yes
 */
int drop(mixed dest) {
   if ( worn_by && living(worn_by))
      return 1;
   return object::drop(dest);
} /* drop() */

/**
 * @ignore yes
 */
varargs int move( mixed dest, string messin, string messout ) {
   int flag;
   flag = object::move( dest, messin, messout );
   if ( ( flag == MOVE_OK ) && worn_by )
      set_worn_by( 0 );
   return flag;
} /* move() */

/**
 * @ignore yes
 */
void dest_me() {
   set_worn_by( 0 );
   object::dest_me();
} /* dest_me() */

/**
 * @ignore yes
 */
mixed *stats() {
   return object::stats() + armour_logic::stats() + wearable::stats() +
          ({ ({ "max ac", AC_LIMIT }) });
} /* stat() */

/**
 * @ignore yes
 */
mapping int_query_static_auto_load() {
   return ([
      "::" : object::int_query_static_auto_load(),
      "ac" : ac,
      "armour types" : armour_types,
   ]) + wearable::query_static_auto_load();
} /* query_static_auto_load() */

/**
 * @ignore yes
 */
mapping query_static_auto_load() {
   if ( base_name(this_object()) != __FILE__[0..<3]) {
      return ([ ]);
   }
   return int_query_static_auto_load();
} /* query_static_auto_load() */

/**
 * @ignore yes
 */
mapping query_dynamic_auto_load() {
   mapping map;
   map = ([
      "::" : object::query_dynamic_auto_load(),
   ]) + wearable::query_dynamic_auto_load();
   return map;
} /* query_dynamic_auto_load() */

/**
 * @ignore yes
 */
void init_static_arg( mapping map ) {
   if ( !mapp( map ) )
      return;
   if ( map[ "::" ] )
      object::init_static_arg( map[ "::" ] );
   wearable::init_static_arg( map );
   if ( map[ "ac" ] )
      ac = map[ "ac" ];

   if (pointerp(ac)) {
     int i;
     mixed *tmp;

     /* Change them to the new system */
     tmp = (mixed *)ac;
     ac = ([ ]);
     for (i=0;i<sizeof(tmp);i+=2)
       add_ac(tmp[0], tmp[1][1], tmp[1][0]);
   } else if ( map[ "armour types" ] )
      armour_types = map[ "armour types" ];

   /* Set up the max ac levels correctly. */
   set_type( query_type() );
} /* init_static_arg() */

void replace_me(){
  object receipt;

  receipt = clone_object( "/std/object" );
  receipt->set_name( "receipt" );
  receipt->set_short( "destructed item receipt" );
  receipt->add_adjective( ({ "destructed", "item" }) );
  receipt->set_long( "This seems to be a small piece of paper.\n" );
  receipt->set_read_mess( "According to our sources, your "+query_short()+" was not "
                          "allowed to exist.  Have a nice day." );
  receipt->move( environment() );
  receipt->set_weight( 1 );
  destruct( this_object() );
}

/**
 * @ignore yes
 */
void init_dynamic_arg( mapping map, object ) {
   mapping stat_temp;
   string virt_name, new_name;

   if ( map[ "::" ] )
      object::init_dynamic_arg( map[ "::" ] );
   wearable::init_dynamic_arg( map );

   if( virt_name = query_property( VIRTUAL_NAME_PROP ) ) {
     if( file_size( virt_name ) == -1 ) {
       new_name = ( CLONER )->other_file( virt_name );
       if( stringp( new_name ) && ( new_name != virt_name ) ) {
         add_property( VIRTUAL_NAME_PROP, new_name );
         virt_name = new_name;
       } else {
         if( VIRTUAL_HANDLER->query_forbidden( virt_name ) ) {
           call_out( "replace_me", 1 );
         } else {
           VIRTUAL_HANDLER->add_missing( virt_name );
         }
       }
     }
     if( file_size( virt_name ) != -1 &&
         query_property( "virtual time" ) < stat( virt_name )[1] ) {
       stat_temp = ( VIRTUAL_HANDLER )->new_data( virt_name );
       if( mapp( stat_temp ) ) {
         init_static_arg( stat_temp );
         add_property( "virtual time", time() );
       }
     }
   }

} /* init_dynamic_arg() */

