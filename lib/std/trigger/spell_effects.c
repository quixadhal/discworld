void do_spell_effects(object ob) {
  mixed *bit;
  int i;

  bit = (mixed *)this_object()->query_actions("spell_effects");
  for (i=0;i<sizeof(bit);i+=2)
    call_other(bit[i+1][0], bit[i+1][1], ob);
  previous_object()->do_spell_effects();
}
