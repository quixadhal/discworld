/* the bits controller.... Doesn't do overly much... */
object *bits;
#define TIME_OUT 60

void create() {
  bits = ({ });
}

void add_bit(object ob) {
  bits += ({ ob });
  call_out("do_update", TIME_OUT);
}

void remove_bit(object ob) {
  int i;

  if ((i = member_array(ob, bits)) == -1)
    return ;
  bits = delete(bits, i, 1);
}

void do_update() {
  mixed *new;
  int i;

  new = unique_array(bits, "do_decay");
  if ((i = member_array(1, new)) == -1) {
    bits = ({ });
    return ;
  }
  bits = new[i+1];
  call_out("do_update", TIME_OUT);
}
