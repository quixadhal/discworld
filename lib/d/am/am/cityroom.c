inherit "std/outside";
#include "path.h"
 
#define HOSPITAL "d/am/hospital"

int num;
object *monster;
string type;
 
void make_citymonster()
{
  int count;
 
  for(count=0;count<num;count++) {
    if (!monster[count]) {
      monster[count] = (object)HOSPITAL->get_monster(type);
      monster[count]->add_property("start location", this_object());
      monster[count]->move(this_object());
    }
  }
}
 
void set_monster(int number, string filename) {
  num = random(4);
  monster = allocate(num);
  type = filename;
}
 
void create() {
  ::create();
  num = 0;
  type = CHAR+"citymonster";
}
 
void reset() {
  make_citymonster();
}

void add_monster(object rep, object me) {
  int i;

  if ((i = member_array(rep, monster)) == -1)
    return ;
  monster[i] = me;
}
 
void dest_me() {
  int loop;

  if(monster) {
    for(loop = 0; loop < sizeof(monster); loop++) {
      if(monster[loop]) {
        monster[loop]->dest_me();
      }
    }
  }
  ::dest_me();
}
