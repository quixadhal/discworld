/*  -*- LPC -*-  */
inherit "/obj/cont_medium";

/* One unit = 1 grain of sand.  About.  I'm roughing this
 * heavily.
 */
void setup()
{
  set_name("sand");
  set_short("sand");
  set_main_plural( "sand" );
  add_adjective( ({ "some", "grain", "pinch", "measure", "handful",
    "hatful", "beach", "of" }) );
  set_long("This is some lovely soft sand, the type that you'd find on "
    "a beach.\n");
  add_property("determinate", "some ");
  set_weight_unit( ({ 1, 1000 }) ); /* 1000 grains in 50 grams? */
  set_value_scale( 0.00001 ); /* "cheap as sand" */
  set_medium_alias("BeachAndDesertSand");
  set_pile_name("pile");
  set_continuous();
  set_amount_types( ([
    "grain": ({ 1, "grains" }),
    "pinch": ({ 40, "pinches" }),
    "measure": ({ 200, "measures" }),
    "handful": ({ 10000, "handfuls" }),
    "hatful": ({ 50000, "hatfuls" }),
    "beach": ({ 1000000, "beaches" }),
    ]) );
  set_pile_types( ({
    40, "tiny",
    10000, "small",
    50000, "medium",
    200000, "large",
    "huge"
    }) );
  set_amount(12000);
}
