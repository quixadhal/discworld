#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/vessel.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/misc/buckets/bucket_small.ob#1" );
  call_other( clone, "set_name",  "bucket"  );
  call_other( clone, "set_short",  "small bucket"  );
  call_other( clone, "set_long",  "This is a small metal bucket.  It is not very shiny.\n"  );
  call_other( clone, "set_main_plural",  "small buckets"  );
  call_other( clone, "add_adjective",  "small"  );
  call_other( clone, "add_plural",  "buckets"  );
  call_other( clone, "set_stuck",  1  );
  call_other( clone, "set_leak_rate",  0  );
  call_other( clone, "set_max_volume",  3000  );
  call_other( clone, "set_max_weight",  30  );
  call_other( clone, "set_full_weight",  30  );
  call_other( clone, "set_value",  85  );
  call_other( clone, "set_weight",  10  );
  call_other( clone, "set_material",  "metal" );
   return clone;
} /* create_virtual_ob() */
