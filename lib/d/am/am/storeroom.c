inherit "/std/storeroom";
#include "path.h"

setup() {
  set_short("The storeroom");
  set_long("Big empty green.\n");
  add_exit("south", ROOM+"filgreeshop", "door");
  set_light(100);
}

void reset() {
  call_out("af_reset", 0);
}

void af_reset() {
  int i, j;
  object ob;
  i = sizeof(find_match("buckets", this_object()));
  for (;i<3;i++) {
    ob = clone_object("/obj/container");
    j = random(3);
    ob->set_leak_rate(0);
    ob->set_max_volume(1000+j*2000);
    ob->set_max_weight(10+j*20);
    ob->set_full_weight(100+j*200);
    ob->set_stuck(1);
    ob->set_name("bucket");
    ob->set_short(({ "small", "medium", "large" })[j]+" bucket");
    ob->set_main_plural(({ "small", "medium", "large" })[j]+" buckets");
    ob->add_adjective( ({ "small", "medium", "large" })[j]);
    ob->add_plural("buckets");
    ob->set_value(60+j*23);
    ob->set_long("A metal bucket, it is not very shiny.\n");
    ob->move(this_object());
    ob->set_weight(4+j*3);
  }
}
