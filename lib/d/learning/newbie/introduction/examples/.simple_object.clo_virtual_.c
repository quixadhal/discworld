#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/clothing.c", 1 );
  clone->add_property( "virtual name", "/d/learning/newbie/introduction/examples/simple_object.clo#1" );
  call_other( clone, "set_name",  "dungarees"  );
  call_other( clone, "set_short",  "pair of waterproof dungarees"  );
  call_other( clone, "set_long",  "This is a pair of waterproof dungarees.  Luckily they "     "are also ooze proof.\n"  );
  call_other( clone, "add_adjective",  ({"pair of", "waterproof"})  );
  call_other( clone, "set_weight",  45  );
  call_other( clone, "set_value",  4000  );
  call_other( clone, "setup_clothing",  50000  );
  call_other( clone, "set_type",  "robe" );
   return clone;
} /* create_virtual_ob() */
