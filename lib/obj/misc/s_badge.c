#include <mail.h>

#define DOMAIN explode( theres[ i ], "/" )[ 1 ]
#define LORD capitalize( (string)( "/d/"+ DOMAIN +"/master" )->query_lord() )
#define MASTER "/secure/master"
#define HANDLER "/obj/handlers/playtesters"

#define PROTECTION_EFFECT "/d/playtesters/effects/pt_protection"

inherit "/obj/armour";

mapping routes;

int access_mail(string str);
int list_destinations();
int list_transits();
int do_goto(string destination);

void setup() {
   set_name( "badge" );
   set_short( "bejewelled gold badge" );
   add_adjective( "gold" );
   set_long( "This bejewelled gold badge is the mark of a Senior Playtester."
           "  It has a drawing of a womble on the front; it seems to be using"
           " a magnifying glass to examine something.  On the back is "
           "engraved a scroll with some words on it.\n" );
   setup_armour( 100 );
   set_damage_chance( 0 );
   set_type( "badge" );
   reset_drop();

   // Put this in a call_out to make sure the PTs get it.
   call_out( (: add_wear_effect :), 0, PROTECTION_EFFECT );
   
   /*
    * Please make sure that routes are placed correctly.
    * This is a _mapping_ !
    */
   routes = ([
     /* "/d/am/undercellar/upper_a" : ({
         "/d/am/short/short11",
      }), */ /* til I find out what the heck happened */
      "/d/klatch/djel/city/port/riverwalk09" : ({
         "/d/klatch/djel/city/port/pier1",
       }),
      "/d/klatch/djel/city/port/pier1" : ({
        "/d/klatch/djel/city/port/riverwalk09",
       }),
       "/d/am/isle_gods/siege/siege4" : ({
           "/d/am/isle_gods/buildings/parlour/parlor",
       }),
       "/d/am/ninth/ninth1" : ({
          "/d/am/blood/blood01",
       }),
       "/d/ram/ohulan/market/market_c" : ({
         "/d/ram/rambling_trail",
       }),
      "/d/klatch/djel/city/roads/widder35" : ({
         "/d/klatch/ephebe/desert/roads/road5",
       }),
      "/d/am/gates/hubwards" : ({
         "/d/sur/beta/topography/Sto_Plains/s_caravan/entrance",
      }),

    ]);
   set_read_mess( "This badge allows the bearer to move into certain areas "
         "to be playtested from certain entry locations (near to where the "
         "areas will be eventually connected).  To see the possible "
         "destinations from your current room, use the command "
         "\"destinations\", and to move into one of them, use the command "
         "\"goto <label>\" where <label> will be A, B, C, etc..  You can "
         "get a list of all transit points with \"transits\", although no "
         "guarantee is made that each item on this list will be "
         "recognisable.  Please report any problems with rooms not "
         "loading to the relevant domain Lord or Liaison(s)." );
} /* setup() */

void init() {
   string word;
   if ( !environment() )
      return;
   if ( !living( environment() ) )
      return;
   if ( !environment()->query_creator() ) {
      word = (string)environment()->query_name();
      if ( !HANDLER->query_playtester( word ) ) {
         tell_object( environment(), "You are not a playtester.  "
               "The playtester's badge disappears in a flurry of fluff.\n" );
         set_drop();
         move( "/room/rubbish" );
         return;
      }
      if ( HANDLER->query_senior_playtester( word ) ) {
         add_command( "mail", "", (: access_mail(0) :) );
         add_command( "mail", "<string>", (: access_mail($4[0]) :) );
      }
   }
   add_command("destinations", "", (: list_destinations() :) );
   add_command("transits", "", (: list_transits() :) );
   add_command("goto", "<string>", (: do_goto($4[0]) :) );
/* creator "goto" should take precidence */
} /* init() */

int access_mail( string words ) {
   return (int)MAIL_TRACK->mail( words );
} /* access_mail() */

int list_destinations() {
  int i;
  string here, *theres;
  object there;
  here = file_name( environment( this_player() ) );
  if ( !sizeof ( theres = routes[ here ] ) )
    return notify_fail( "You cannot use the badge to move from here.\n" );
  write( "From here you can use the badge to move to:\n" );
  for ( i = 0; i < sizeof( theres ); i++ ) {
    if ( !( there = find_object( theres[ i ] ) ) ) {
      if ( file_size( theres[ i ] +".c" ) < 0 ) {
/* might need to be check with MASTER using file_exists */
        write( sprintf( "%c: %s cannot be found; please contact %s.\n", 65 + i,
            theres[ i ], LORD ) );
        continue;
      }
      catch( theres[ i ]->force_load() );
      if ( !( there = find_object( theres[ i ] ) ) ) {
        write( sprintf( "%c: %s will not load, please contact %s.\n", 65 + i,
            theres[ i ], LORD ) );
        continue;
      }
    }
    write( sprintf( "%c: %s\n", 65 + i, (string)there->a_short() ) );
  }
  return 1;
} /* list_destinations() */

int list_transits() {
   int i;
   string *theres;
   object there;
   theres = keys( routes );
   if ( !sizeof( theres ) )
      return notify_fail( "There are no transit points at the moment.\n" );
   write( "You can use the badge to move from:\n" );
   for ( i = 0; i < sizeof( theres ); i++ ) {
    if ( !( there = find_object( theres[ i ] ) ) ) {
      if ( file_size( theres[ i ] +".c" ) < 0 ) {
/* might need to be check with MASTER using file_exists */
        write( sprintf( "%s cannot be found; please contact %s.\n",
            theres[ i ], LORD ) );
        continue;
      }
      catch( theres[ i ]->force_load() );
      if ( !( there = find_object( theres[ i ] ) ) ) {
        write( sprintf( "%s will not load, please contact %s.\n",
            theres[ i ], LORD ) );
        continue;
      }
    }
    write( sprintf( "   %s\n", (string)there->a_short() ) );
  }
  return 1;
} /* list_transits() */

int do_goto( string destination ) {
  int i;
  string here, *theres;
  object there;

  i = destination[ 0 ] - 65;
  if ( ( i < 0 ) || ( i > 25 ) ) {
    notify_fail( "The destination label needs to be a capital letter "+
        "between A and Z.\n" );
    return 0;
  }
  here = file_name( environment( this_player() ) );
  if ( !sizeof ( theres = routes[ here ] ) ) {
    notify_fail( "You cannot use the badge to move from here.\n" );
    return 0;
  }
   if ( i >= sizeof( theres ) )
      return notify_fail( "That is not a valid label from here.\n" );   
  if ( !( there = find_object( theres[ i ] ) ) ) {
    if ( file_size( theres[ i ] +".c" ) < 0 ) {
/* might need to be check with MASTER using file_exists */
      write( theres[ i ] +" cannot be found; please contact "+ LORD +".\n" );
      return 1;
    }
    catch( theres[ i ]->force_load() );
    if ( !( there = find_object( theres[ i ] ) ) ) {
      write( theres[ i ] +" will not load; please contact "+ LORD +".\n" );
      return 1;
    }
  }
  write( "Moving you to "+ (string)there->the_short() +"...\n" );
  this_player()->move_with_look( there, "$N appear$s in a flurry of fluff "
             "that only $N could muster.", "$N disappear$s in a flurry of fluff." );
  return 1;
} /* do_goto() */

