// Aquilo //

inherit "/std/object";

void setup(){
  set_name("flint");
  set_short("flint stone");
  set_long("This is a flint stone.  It looks slightly worn and chipped "
    "but it's still keen enough to serve its purpose.\n");
  add_property("no recycling", 1);
  add_alias("stone");
  set_value( 600 );
}

