inherit "/std/outside";
#include "path.h"
 
make_citymonster(num,monster)
{
        int count;
 
        for(count=0;count<num;count++) {
                if (!monster[count]) {
                        write("Scream: "+CHAR+"city");
                        monster[count]=clone_object(CHAR+"city");
                        monster[count]->move(this_object());
                }
        }
}
