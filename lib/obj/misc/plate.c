/**
* This is an inheritable for a plate, including cutlery.  You can put things
* on it.  You can also eat food directly from the plate.  It was designed
* for the pub_shop.c inheritable, for use with the meals that can be sold
* there.
* @author Lemming
* @started 26/12/1999
*/


inherit "/std/surface";


/** @ignore */
void init() {
  this_player()->add_command( "eat", this_object(),
    "<indirect:object:direct-obs> {from|on} <direct:object:me>" );
  this_player()->add_command( "eat", this_object(),
    "from <direct:object:me>" );
} /* init() */


/** @ignore yes */
void create() {
  do_setup++;
  ::create();
  do_setup--;

/*  I'm commenting these two out, because for reasons beyond me
**  they mess up the main plurals of the objects inheriting
**  this file. And I can't imagine this file being cloned directly
**  anywhere, so they shouldn't be needed.   -- Tilly
**  set_name( "plate" );
**  set_short( "plate" );
*/
  set_long( "This is a nice plate for keeping food on.\n" );


  if ( ! do_setup ) {
    this_object()->setup();
    this_object()->reset();
  }
} /* create */



/** @ignore */
int do_eat( object *obs, string dir, string indir, mixed *args ) {
  object ob;
  object *food;

  if( sizeof( obs ) )
    food = copy( obs );
  else
    food = all_inventory();

  if( sizeof( food ) ) {
    food = filter( food, (: $1->query_edible() :) );
  } else {
    this_player()->add_failed_mess( this_object(),
      "There's nothing on $D\n", ({ }) );
    return 0;
  }

  if( !sizeof( food ) ) {
    if( sizeof( obs ) )
      this_player()->add_failed_mess( this_object(),
        "You can't eat $I on $D!\n", obs );
    else
      this_player()->add_failed_mess( this_object(),
        "There's nothing edible on $D\n", ({ }) );
    return 0;
  }

  foreach( ob in food ) {
    ob->do_eat();
  }

  return 1;
} /* do_eat() */


/** @ignore yes */
mapping query_static_auto_load() {
  return int_query_static_auto_load();
}
