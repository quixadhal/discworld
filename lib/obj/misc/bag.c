inherit "/obj/container";

void setup() {
  set_name("bag");
  set_long("Its a bag, you can put things in it.\n");
  set_value(50);
  set_weight(10);
  set_max_weight(100);
  set_prevent_insert();
}
