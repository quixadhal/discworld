object *labels;

void create() {
   labels = ({ });
}

object *query_labels() { return labels; }

/* Labels shouldnt weigh anything.... */
int add_weight(int n) {
  return !n;
}

/* Once it is in us...  we let it go :) */
int test_remove(object ob, int flag) { return 1; }
int test_add(object ob, int flag) { 
  return ob->query_property("label") && member_array(ob, labels) != -1;
}

/* Note, this should be called BEFORE the label is stuck on the object. */
void add_label(object ob) {
  if (environment()) {
    filter_array(all_inventory(environment()), "add_read_this", this_object());
    environment()->add_command("read", this_object());
  }
  labels += ({ ob });
}

/*
 * Make sure this is called when we leave the objects environ,  could do
 * bad things...
 */
void remove_label(object ob) {
  labels = labels - ({ ob });
}

/* std/basic/label: */
mixed *query_init_data() {
   return ({ "labels", labels, "add_label/P*/" });
}
