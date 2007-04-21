string *find_stuff(object ob, string varname){
  string *variables = variables(ob);
  string *ret = ({});
  if(member_array(varname, variables) > -1){
    object *inhobs = map(inherit_list(ob), (:find_object:));
    object obj;
    foreach(obj in inhobs){
      ret += find_stuff(obj, varname);
    }
    
    if(ret == ({})){
      ret = ({file_name(ob)});
    }
  }
  return ret;
}

int cmd(object ob, string var){
  string *files = find_stuff(ob, var);
  if(sizeof(files) > 1)
    printf("%s is defined in %s and %s.\n", var, implode(files[0..<2], ", "),
           files[<1]);
  else
    write("No redefinition found.\n");
  return 1;
}

string *query_patterns(){
  return ({"<indirect:wiz-present> <word'global'>", (:cmd($1[0], $4[1]):)});
}
