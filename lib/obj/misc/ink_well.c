inherit "/obj/container";
 
void create() {
  ::create();
  set_name("well");
  add_adjective("ink");
  set_short("ink well");
  set_long("A nice, small ink well.\n");
  set_value( 500 );
  set_leak_rate(0);
  set_max_volume(250);
  set_volume(200);
  set_water_volume(200);
  set_ps_coord_quantity((int *)"/std/effects/other/ink"->query_coordinate(), 200);
}
