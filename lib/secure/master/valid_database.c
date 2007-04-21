mixed valid_database(object ob, string action, mixed *info){
  string obname = base_name(ob);
  switch (obname){
  case "/obj/handlers/clusters":
  case "/obj/handlers/map":
  case "/cmds/creator/osql":
  case "/cmds/errors_base":
    return -1;
  default:
    return 0;
  }
}
