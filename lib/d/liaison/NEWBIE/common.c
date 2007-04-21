
void dest_money(object coins) {
  if(coins->query_number_coins() > 1)
    tell_room(this_object(), "As the coins hit the ground they "
              "slip between the cracks and disappear.\n");
  else
    tell_room(this_object(), "As the coin hits the ground it "
              "slips between the cracks and disappears.\n");

  coins->move("/room/rubbish");
}
#ifdef 0
void event_enter(object ob, string mess, object from) {
  if(base_name(ob) == "/obj/money") {
    call_out("dest_money", 0, ob);
  }
  return;
}
#endif

