#include <creator.h>
#define COUNT 10

int cmd(string arg){
  mapping m = ([]);
  string *obs, ob, ret = "";

  if(!master()->query_senior(this_player()->query_name()))
    return 0;

  switch(arg){
  case "all":
    objects((:$(m)[base_name($1)]++,reset_eval_cost():)); 
    break;

  case "inside":
    objects((:environment($1) && $(m)[base_name($1)]++:));
    break;

  default:
    objects((:environment($1) && living($1) && 
       $(m)[base_name($1)]++:));
  }

  obs = keys(filter(m, (:$2 > COUNT:)));

  foreach(ob in obs){
    object *things, thing;
    
    m = ([]);
    
    reset_eval_cost();

    if(arg != "all")
      things = filter(children(ob), (:environment($1):));
    else
      things = children(ob);
    
    foreach(thing in things)
      m[environment(thing)]++;
    
    m = filter(m, (:$2 > COUNT:));
    if(sizeof(m)){
      object *rooms = keys(m);
      
      foreach(thing in rooms){
        ret += sprintf("%3d %s in %s\n", m[thing], 
                       WIZ_PRESENT->desc_f_object(find_object(ob)),
                       WIZ_PRESENT->desc_f_object(thing));
      }
    }
  }
  

  ob = sprintf("/w/%s/mobs", this_player()->query_name());
  printf("Saving to %s.\n", ob);
  //this_player()->more_string(ret);
  seteuid(geteuid(this_player()));
  unguarded((:rm($(ob)),write_file($(ob), $(ret)):));
  return 1;
}

mixed *query_patterns() {
  return ({ "", (: cmd(0) :),
      "{all|inside}", (: cmd($4[0]) :) });
}

string help() {
  return "Shows groups of NPCs of one kind that are in the same room.";
}


