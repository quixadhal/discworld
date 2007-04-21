#define CLONEFILE "/global/virtual/setup_compiler/Room.c"

mixed add_item(mixed a1, mixed a2)
 { MyRoom->add_item(a1,a2); }

mixed set_name(mixed a1)
 { MyRoom->set_name(a1); }

mixed set_long(mixed a1)
 { MyRoom->set_long(a1); }

mixed set_light(mixed a1)
 { MyRoom->set_light(a1); }

mixed add_property(mixed a1, mixed a2)
 { MyRoom->add_property(a1,a2); }

mixed set_short(mixed a1)
 { MyRoom->set_short(a1); }

mixed add_exit(string a1, string a2, string a3)
{
	if( file_size(a2) > 0 )
		{ MyRoom->add_exit(a1,a2,a3); return; }
	if( file_size(a2+".c") > 0 )
		{ MyRoom->add_exit(a1,a2+".c",a3); return ; }
	if( file_size(a2+".vrc") > 0 )
		{ MyRoom->add_exit(a1,a2+".vrc",a3); return; }
	if( file_size(a2+".r") > 0 )
		{ MyRoom->add_exit(a1,a2+".r",a3); return; }
	write("Error: Bad arg 2 to add_exit in virtual room");
	return;
}

mixed modify_exit(string m1, mixed *m2)
 { MyRoom->modify_exit(m1,m2); }

mixed set_dark_mess(mixed a1)
 { MyRoom->set_dark_mess(a1); }

mixed set_co_ord(mixed a1)
 { MyRoom->set_co_ord(a1); }

mixed set_zone(mixed a1)
 { MyRoom->set_zone(a1); }

/************** Virtual Room Functions Not found in /std/room.c **********/

void add_reset_func( mixed ob, string func )
{
  MyRoom->add_reset_func( ob, func );
}
void add_dest_func( mixed ob, string func )
{
  MyRoom->add_dest_func( ob, func );
}
void add_enter_func( mixed ob, string func )
{
  MyRoom->add_enter_func( ob, func );
}
void add_exit_func( mixed ob, string func )
{
  MyRoom->add_exit_func( ob, func );
}
void add_death_func( mixed ob, string func )
{
  MyRoom->add_death_func( ob, func );
}

void add_clone_on_reset( string fname )
{
  MyRoom->add_clone_on_reset( fname );
}


#include "/w/mithal/UTIL/use_comments.h"

