
void alt_move(mixed dest, object ob){
  if(!ob)
    return;
  evaluate(bind((:move_object, dest:), ob));
}
