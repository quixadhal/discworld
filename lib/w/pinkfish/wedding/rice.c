inherit "/std/object";

int amount_left;
#define ME "/w/pinkfish/wedding/rice"

void setup() {
  set_name("rice");
  add_adjective( ({ "some", "bag", "of" }) );
  add_alias("Really stupid alias");
  set_short("bag of rice");
  set_long("A small bag full of rice, it looks like you can throw it at "+
           "people.\n");
  amount_left = 10;
}

void set_amount_left(int left) { amount_left = left; }
int query_amout_left() { return amount_left; }
int adjust_amount_left(int i) { return (amount_left += i); }

void init() {
  this_player()->add_command("throw", this_object(), "%D %p %I");
  this_player()->add_command("chuck", this_object(), "%D %p %I");
}

mixed do_throw(object *obs) {
  int i;
  object new;

  for (i=0;i<sizeof(obs);i++) {
    this_player()->add_succeeded(obs[i]);
    amount_left--;
    new = clone_object(ME);
    new->set_amount_left(1);
    new->move(obs[i]);
    if (!amount_left) {
      call_out("dest_me",0);
      return "some rice";
    }
  }
  return "some rice";
}

move(object dest, string mess_in, string mess_out) {
  int i;
  object rice;

  i = ::move(dest, mess_in, mess_out);
  if (i)
    return i;
  if (!objectp(dest))
    dest = find_object(dest);
  remove_alias("Really stupid alias");
  if ((rice = present("Really stupid alias", dest))) {
    rice->adjust_amount_left(amount_left);
    call_out("dest_me",0);
    set_short(0);
    return i;
  }
  add_alias("Really stupid alias");
  return i;
}

mixed do_chuck(object *obs) {
  return do_throw(obs);
}
