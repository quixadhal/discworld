/* inherit this file anything that wants to be holdable */
inherit "/std/object";

static object holder;

int query_holdable() { return 1; }

int set_holder(object ob) {
  holder = ob;
  return 1;
}

object query_holder() { return holder; }

int drop() {
  holder = 0;
  return ::drop();
}

move(dest, arrive, leave) {
  if (holder && dest != holder) {
      holder->unhold_ob(this_object());
      holder = 0;
  }
  return ::move(dest, arrive, leave);
}
