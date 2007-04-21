/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: vr_hand.c,v 1.1 1998/01/06 04:55:54 ceres Exp $
 * $Log: vr_hand.c,v $
 * Revision 1.1  1998/01/06 04:55:54  ceres
 * Initial revision
 * 
*/
/*
   Limitations;
     No support for defines etc, to be added.
     plenty of others I haven't thought of yet ..

*/     




//Function Prototypes
object compile_virtual_room(string filename);
void parse_object(object room, string command);

void create()
{
  seteuid(getuid(this_object()));
} /* create */

object compile_virtual_room(string filen)
{
  string tmp, tmp2;
  object new_room;
  int i;
  i=3;
  tmp=read_file(filen, 2, 1);
  tmp2=tmp[1..strlen(tmp)-2];
  new_room=clone_object("/"+tmp2);
  while (read_file(filen, i, 1))
  {
    tmp=read_file(filen, i, 1);
    if(tmp=="") break;
    parse_object(new_room, tmp);
    i++;
  }
  return new_room;
} /* compile_virtual_room */

/* you want a directive for a particular function - add it to the list below */

void parse_object(object room, string command)
{
  string *func_call,s1,s2,s3;
  int i1,i2,i3,i4;
  if(!command) return;
  func_call=explode(command, "#");
  if((string)func_call[0][0]==";") return;
  switch (func_call[0])
  {
    case "SHORT" :
      room->set_short(func_call[1]);
      break;
    case "LONG" :
      room->set_long(func_call[1]);
      break;
    case "EXIT" :
      room->add_exit(func_call[1], func_call[2]);
      break;
    case "LIGHT" :
      i1=sscanf(func_call[1], "%d", i2);
      room->set_light(i2);
      break;
    case "ITEM" :
      room->add_item(explode(func_call[1], ","), explode(func_call[2], ","));
      break;
    case "TASTE" :
      room->add_taste(func_call[1], func_call[2]);
      break;
    case "SMELL" :
      room->add_smell(func_call[1], func_call[2]);
      break;
    case "SOUND" :
      room->add_sound(func_call[1], func_call[2]);
      break;
    case "TOUCH" :
      room->add_feel(func_call[1], func_call[2]);
      break;
    case "OBJECT" :
      room->add_object(func_call[1]);
      break;
    case "ACTION" :
      i2=0;
      catch(i1=sscanf(func_call[3], "%d", i2));
      room->action_add(func_call[1], func_call[2], i2);
      break;
    case "CODE" :
      room->set_code_module(func_call[1]);
      break;
    case "PROP" :
      room->add_property(func_call[1], func_call[2]);
      break;
    case "COORD" :
      i1=sscanf(func_call[1], "%d,%d,%d", i2,i3,i4);
      room->set_co_ord(({i2,i3,i4}));
      break;    
  }
  return;
} /* parse_object */

