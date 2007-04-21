// for WIZ_PRESENT
#include <creator.h>

inherit "/cmds/base";

int cmd( string words ) {
  string *name = allocate(2);
  string *mess = allocate(2);
  object *person = allocate(2);
  object *tmp;
  object thing;
  mixed *inv = allocate(2);
  mixed *matching = allocate(2);
  int i;
  
  sscanf( words, "%s with %s", name[0], name[1] );
  for( i=0; i<2; i++)
    name[i] = this_player()->expand_nickname( name[i] );
    
  for( i=0; i<2; i++) {
    if( !(person[i] = find_player( name[i] )) )
         return notify_fail("No such player - " + name[i] + ".\n");
    inv[i] = all_inventory( person[i] );
    tmp = ({ });
    foreach( thing in inv[i] )
      if( thing->query_cloned_by() == name[!i] ) tmp += ({ thing });
    if(tmp) {
      switch( arrayp(tmp) ) {
        case 1:  matching[i] = tmp; break;
        default: matching[1] = ({ tmp });
      }
    }
    switch( sizeof(matching[i]) ) {
      case 0:
        mess[i] = "No items in " + name[i] + " were cloned by " + name[!i] + ".\n";
        break;
      case 1:
        mess[i] = "One item in " + name[i] + " was cloned by " + name[!i] + ".\n";
        mess[i] += "  " + WIZ_PRESENT->desc_f_object(matching[i][0]) + "\n";
        break;
      default:
        mess[i] = sizeof(matching[i]) + " items in " + name[i] + 
                                       " were cloned by " + name[!i] + ".\n";
        foreach( thing in matching[i] )
          mess[i] += "  " + WIZ_PRESENT->desc_f_object(thing) + "\n";
    }
  }
  write(mess[1] + "\n" + mess[0]);
  //tell_creator("mansarde", "\nwords: %s\nmatching: %O\n", words, matching );
  
  return 1;
}