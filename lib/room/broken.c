/*	/std/broken.c

    Change log:

	12/12/92 - Modified by Godot to put in a red button because one of 
			his storerooms filled the place up one day.

        30-Oct-93 - created by ember as a place to record drop-out broken
                    auto load objects
*/
inherit "/std/room/basic_room";

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
     set_short("Graveyard");
  set_long( "This room is full of broken bits and pieces of objects which "+
      "decided not to work or behave properly.\n" );
}

/* EOF */

void init() {
  ::init();
  co_ord = 0;
} /* init() */

int *query_co_ord() { return 0; }
