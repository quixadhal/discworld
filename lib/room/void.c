/*	/std/void.c

    Change log:

	12/12/92 - Modified by Godot to put in a red button because one of 
			his storerooms filled the place up one day.

*/
inherit "/std/room";

/*
    Global variables.
*/
object	button;

/*
    Function prototypes.
*/
void	after_reset();
void	dest_me();
void	reset();
void	setup();

/*
    Functions.
*/
void
after_reset()
{
     if ( !button )
     {
	button = clone_object( "/obj/misc/button" );
	button->move( this_object() );
     }
}

void
dest_me()
{
    if ( button )  button->dest_me();

    ::dest_me();
}

void
reset()
{
    call_out( "after_reset" , 1 );
}

void setup() {
     set_light(100);
     set_short("Convention in the void");
     set_long("This is where you come when you fall out a room and have " +
               "nowhere to go. You are inside a large igloo. Penguin " +
               "waiters (in tuxedos of course) are serving drinks to the " +
               "assembled crowd. A large wombat sits at the end of a " +
               "banquet table, his head resting peacefully in a bowl of " +
               "soup.\n");
     add_alias("bowl", "soup");
     add_alias("igloo", "around");
     add_alias("void", "igloo");
     add_alias("penguins", "penguin");
     add_alias("crowd", "penguin");
     add_alias("waiter", "penguin");
     add_item("soup", "It is pink and comfortable-looking\n");
     add_item("wombat", "Perhaps you should tell him to use his spoon.\n");
     add_item("igloo", "It is nice and warm despite the icy decor.\n");
     add_item("penguin", "The penguins are all rather formally dressed.\n");
     add_exit("drum", "/d/am/am/mendeddrum", "door");
    co_ord = 0;
}

/* EOF */

void init() {
  ::init();
  co_ord = 0;
} /* init() */

int *query_co_ord() { return 0; }
