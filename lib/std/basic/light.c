static int light, int_light;

int query_light() {
  return light + int_light;
}

int query_my_light() { 
  return light;
}


int query_int_light() {
  return int_light;
}

int set_opaque() {
  if (!this_object()->query_property("opaque") && environment())
    environment()->adjust_light(-int_light);
  this_object()->add_property("opaque",1);
  return 1;
}

int reset_opaque() {
  if (this_object()->query_property("opaque") && environment())
    environment()->adjust_light(int_light);
  this_object()->remove_property("opaque");
}

/* this is used by objects being added to the inventory etc. */
int adjust_light(int i) {
  if (i) {
    if (environment() && !this_object()->query_property("opaque"))
      environment()->adjust_light(i);
    event(this_object(), "light_change", i);
  }
  return (int_light += i);
}

int set_light(int i) {
  if (light - i) {
    if (environment())
      environment()->adjust_light(i-light);
    event(this_object(), "light_change", i-light);
  }
  return (light = i);
}

mixed *stats() {
  return ({
    ({ "light", light }),
    ({ "int light", int_light }),
    ({ "opaque", this_object()->query_property("opaque") }),
         });
}

mixed *query_init_data() {
   return ({ "light", light, "set_light/p/" });
} /* query_init_data() */
