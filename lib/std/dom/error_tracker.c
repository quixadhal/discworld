inherit "/std/dom/error_tracker_master";

void create()
{
  set_domain("klatch");
  set_name("klatchian error tracker");
  set_other_dirs(({ }));
  set_period(604800);
  ::create();
}
