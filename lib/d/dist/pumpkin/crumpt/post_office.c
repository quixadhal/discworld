#include "path.h"
#include <mail.h>

inherit "/std/room/basic_room";

int do_mail( string words );

private nosave object _dictionary;

void setup() {
   add_help_file("mail_room");
   set_short("Post Office");
   set_light(70);
   set_long(
"Black and orange colours seem to dominate the room, you can see a counter "
"or something in amongst all the clashing colours.\n");

   add_item("counter", ({ "long",
            "There are some envelopes and bits of paper strewn "
            "about on the counter.",
            "position", "the counter" }));

   add_exit("southeast", CRUMPT + "crumpt2", "door");
   add_exit("north", CRUMPT + "parcel_post", "corridor");

   add_property("place", "Pumpkin");
} /* setup() */

void reset() {
   if ( !_dictionary ) {
      _dictionary = clone_object( "/net/obj/dict" );
      _dictionary->add_property( "there", "attached to the counter" );
      _dictionary->reset_get();
      _dictionary->move( this_object() );
   }
} /* reset() */

void init() {
   ::init();
   add_command("mail", "<string>", (: do_mail($4[0]) :));
   add_command("mail", "", (: do_mail(0) :));
} /* init() */

int do_mail( string words ) {
   return (int)MAIL_TRACK->mail( words );
} /* do_mail() */

int send_message( string from, int parcel ) {
#if 0
   object thing;

   thing = clone_object( CHARS +"post_et" );
   thing->set_home( file_name( this_object() ) );
   thing->move( this_object(), "$N pop$s out of the counter and grab$s the mail." );
   if ( find_player( recipient ) ) {
      thing->goto_player( recipient, from );
   } else {
      thing->goto_dest( COMM_ROOM, from );
   }
   return 1;
#endif
} /* send_message() */

