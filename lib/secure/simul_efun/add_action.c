#define LIV "/obj/handlers/livings"
private nosave string _nf;

varargs void log_file(string name, string fmt, mixed *args ...);

void enable_commands(){
  efun::set_this_player(previous_object());
  LIV->enable_commands(previous_object());
}

void set_living_name(string name){
  LIV->set_living_name(name, previous_object());
}

int living(object ob){
  if(!ob)
    return 0;
  
  return ob->_living(); //LIV->living(ob);
}

int _notify_fail(string mes){
  _nf = mes;
  return 0;
}

string query_notify_fail(){
  return _nf;
}

mixed command(string cmd){
  int time = eval_cost();
  if(evaluate(bind((:call_other:), previous_object()),previous_object(),
           "_process_input", cmd))
    return eval_cost() - time + 1; // on v22.2 eval_cost runs up, reverse for v22.1
  return 0;
}

/** @ignore yes */

mixed actions_defined(mixed, mixed, mixed){ return ({}); }

void set_this_player(object ob){
  error("Illegal use of set_this_player.");
}
