#include  <bit.h>

inherit "/obj/vessel";

int juice_bit( object ob, string fn );

mapping juice_funs = ([
  "/std/bit" : (: juice_bit :),
]);

void setup() {
  set_name( "juicer" );
  set_short( "juicer" );
  set_main_plural( "juicers" );
  set_long( "This is a device for extracting the juice or liquid from " +
            "objects.  It has a hole at the top for inserting things, " +
            "and a small bowl at the bottom to catch the juice.  You " +
            "could probably \"juice\" something and then \"pour\" " +
            "the juicer into another container.\n" );
  set_value( 4000 );
  set_weight( 100 );
  set_max_volume( 19200 );
} /* setup() */

void init() {
  ::init();
  this_player()->add_command( "juice", this_object(), "%I 'in' %D" );
} /* init() */

int do_juice( object *things ) {
  int i, amount, *weight_unit, percent;
  string medium_short, *types, fn;
  mapping amount_types;

  if ( living( environment() ) ) {
    this_player()->add_failed_mess( this_object(), "You have to put down "+
        "$D to use it.\n", ({ }) );
    return 0;
  }
  for ( i = 0; i < sizeof( things ); i++ )
    if ( environment( things[ i ] ) != this_player() ) {
      things = delete( things, i, 1 );
      i--;
    }
  if ( !sizeof( things ) ) {
    this_player()->add_failed_mess( this_object(), "You should be carrying "+
        "whatever you want to $V in $D", ({ }) );
    return 0;
  }
  if ( sizeof( things ) > 1 ) {
    this_player()->add_failed_mess( this_object(), "You should only $V "+
        "one thing at a time in $D in case they get mixed up.\n", ({ }) );
    return 0;
  }
  if ( !function_exists( "query_continuous", things[ 0 ] ) ) {
    if ( fn = things[ 0 ]->query_property("juice")
        && functionp( juice_funs[ fn ] )) {
      return evaluate( juice_funs[ fn ], things[ 0 ], fn );
    }
    fn = explode(file_name(things[ 0 ]), "#")[ 0 ];
    if (functionp( juice_funs[ fn ] )) {
      return evaluate( juice_funs[ fn ], things[ 0 ], fn );
    }
    this_player()->add_failed_mess( this_object(), "You cannot $V $I "+
        "in $D.  Trust me on this.\n", ({ things[ 0 ] }) );
    return 0;
  }
  if ( !(percent = things[ 0 ]->query_property("%liquid")) ) {
    this_player()->add_failed_mess( this_object(), "$I apparently doesn't " +
        "contain much juice.\n", ({ things[ 0 ] }) );
    things[ 0 ]->move("/room/rubbish");
    return 0;
  }
  things[ 0 ]->add_plural( (string)things[ 0 ]->query_name() );
/* The weight unit has been set or is still the default.  Either is fine. */
  if ((amount = (int)things[ 0 ]->query_amount()) == 0) {
    weight_unit = (int *)things[ 0 ]->query_weight_unit();
    amount = ( weight_unit[ 1 ] * (int)things[ 0 ]->query_weight() ) /
      weight_unit[ 0 ];
    amount = ( ( 50 + random( 21 ) ) * amount ) / 80;
  }
  things[ 0 ]->set_amount( amount*percent/100 );
  things[ 0 ]->set_weight_per_bite( 0 );
  things[ 0 ]->set_bites_gone( 0 );
  medium_short = (string)things[ 0 ]->query_medium_short();
  if ( !medium_short )
    medium_short = (string)things[ 0 ]->query_short();
  if ( !things[ 0 ]->query_medium_alias() )
    things[ 0 ]->set_medium_alias( "Ground"+ capitalize( medium_short ) );
/* In fact, if the continuous medium details are not set and the object
 *     returns ([ ]) to query_static_auto_load() the object will fail
 *     dismally on autoloading next time.  Hope the person who created
 *     the object knows this and set it up properly...
 */
  amount_types = ([ "drop" : ({ 1, "drops" }), "ounce" :
        ({ 120, "ounces" }), "pint" : ({ 2400, "pints" }) ]);
  things[ 0 ]->set_amount_types( amount_types );
  types = m_indices( amount_types );
  for ( i = 0; i < sizeof( types ); i++ )
    things[ 0 ]->add_adjective( ({ types[ i ],
        amount_types[ types[ i ] ][ 1 ] }) );
  things[ 0 ]->add_adjective( "of" );
  if ( !sizeof( (mixed *)things[ 0 ]->query_pile_types() ) )
    things[ 0 ]->set_pile_types( ({ 50, "small", 2400, "medium", 19200,
        "large", "huge" }) );
  things[ 0 ]->add_property( "determinate", "some " );
  things[ 0 ]->grind();
  things[ 0 ]->move( this_object() );
  this_player()->add_succeeded_mess( this_object(), "$N $V $D to make $I.\n",
      ({ things[ 0 ] }) );
  return 1;
} /* do_grind() */

int juice_bit( object ob, string fn ) {
    object juice_ob;
    mixed *bit_data;
    string *m, s;
    int i;
    
    juice_ob = clone_object("/obj/reagents/generic_liquid.ob");
    juice_ob->set_name("juice");
    // Create medium alias
    if (s = ob->query_race_name())
      s = capitalize(s);
    bit_data = ob->query_bit_data();
    m = explode(bit_data[ BIT_NAME ], " ");
    for ( i = 0; i < sizeof(m); i++ )
      s += capitalize(m[i]?m[i]:"");
    juice_ob->set_medium_alias(s+"Juice");
    switch ( bit_data[ BIT_ALIAS ] ) {
      case "eye":
        juice_ob->add_adjective("eye");
        juice_ob->add_alias("ichor");
        juice_ob->set_short("clear liquid");
        juice_ob->set_long("This is a clear liquid, slightly tinged" +
                           "with blood.\n");
        juice_ob->set_amount(10*(int)ob->query_weight() + 10);
        break;
      default:
        // For things I haven't foreseen...
        juice_ob->set_short("liquid");
        juice_ob->set_long("This is a non-descript liquid.  It's probably " +
                           "useless.\n");
        break;
    }
    juice_ob->move(this_object());
    ob->move("/room/rubbish");
    this_player()->add_succeeded_mess( this_object(), "$N $V $D to make $I.\n",
                                      ({ juice_ob }) );
    return 1;
}
