/*	button.c

    Change log:

	??/??/?? - Created/ported by unknown person.
	01/01/93 - Modified by Godot to dest living objects too.  Also made
			it not gettable.  Converted it to my verbose style.
			Changed push to add_action(), as it always got 
			superceded by the soul command.

*/
inherit "std/object";

/*
    Function prototypes.
*/
int	do_press();
int	do_push( string what );
void	init();
void	setup();

/*
    Functions.
*/
int
do_press()
{
    int		i;
    object	*inv;
    object	room;
    object	tp;

    room = environment( this_object() );
    inv  = all_inventory( room );
    tp   = this_player();

    for ( i=0 ; i<sizeof( inv ) ; i++ )
	if ( !interactive( inv[i] ) && inv[i] != this_object() )
	    inv[i]->dest_me();

    write( "You clean the room up a bit.\n" );
    tell_room( room , tp->query_cap_name() + " cleans up the rooms a bit.\n"
	     , tp );

    return 1;
}

status
do_push( string what )
{
    if ( what != "button" && what != "red button" )
    {
	notify_fail( "Push what?\n" );
	return 0;
    }
    do_press();
    write( "You push a red button.\n" );
    return 1;
}

void
init()
{
    this_player()->add_command( "press" , this_object() );
    add_action( "do_push" , "push" );
}

void
setup()
{
    add_adjective( "red" );
    set_name( "button" );
    set_short( "red button" );
    set_long( "This is a red button that destructs all non-player objects in "
	    + "the room.  USE WITH EXTREME CAUTION.\n");
    reset_drop();
    reset_get();
}

/* EOF */
