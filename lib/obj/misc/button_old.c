inherit "std/object";

void setup() {
  add_adjective("red");
  reset_drop();
  set_name("button");
  set_short("red button");
  set_long(
"This is a red button that destructs all non-living objects in the room.  "+
"USE WITH EXTREME CAUTION.\n");
}

void init() {
  this_player()->add_command("push", this_object());
  this_player()->add_command("press", this_object());
}
 
int do_push() {
  object ob, temp;
  int changed;
 
  changed = 1;
  while (changed) {
    changed = 0;
    ob = first_inventory(environment(this_object()));
    while (ob) {
      if (ob != this_object() && !query_ip_number(ob)) {
        changed = 1;
        temp = ob;
      }
      ob = next_inventory(ob);
      if (temp) {
        temp->dest_me();
      }
    }
  }
  return 1;
}
 

int do_press() {
  return do_push();
}
