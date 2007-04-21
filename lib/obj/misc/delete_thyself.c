/**
 * Make an object the will automaticly delete itself.  Useful for getting rid
 * of things out of vaults and so on.
 * @author Pinkfish
 */
inherit "/std/object";

void create() {
   ::create();
   if (clonep(this_object())) {
      call_out((: move("/room/rubbish") :), 3);
   }
} /* create() */

void init_dynamic_arg(mapping map) {
} /* init_dynamic_arg() */

void init_static_arg() {
} /* init_static_arg() */
