#include "virtual.h"

void dest_me() { destruct(this_object()); }

void create() {
object clone;

  clone = (object)SERVER->create_virtual_object("/std/room.c", 1);
  call_other(clone, "set_long",  "   This is barely a room. It is, infact, a small lump of putty! Deep in the heart of nowhere, you spy a circle of blue wax. This room description is beginning to run out of steam. There! Psfth. " );
  call_other(clone, "set_short",  Wombleroom );
  call_other(clone, "add_item",   { circle, wax } // "   The circle of blue wax is infact bright green." );
  call_other(clone, "add_exit",  north // /players/zellski/virtual/test.r  );
  call_other(clone, "set_light", 100);
  "/global/virtual/compiler"->set_return_ob(clone);
} /* create() */
