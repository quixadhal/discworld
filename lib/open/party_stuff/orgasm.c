/* Created by Geolin 21/2/93 */
inherit "/std/room";
#include "path.h"
object player;
string p;
void setup() {
		         set_light(100);
      set_short("Orgasm Chamber.\n");
         set_long("You are standing in a Orgasm Chamber, it is cylinder in shape and seems to "+
               "be whining at you strangely.  A Sign hangs here.\n");
add_sign("The sign reads 'Type orgasm for your umm...orgasm..\n");
         add_item("walls","The walls appear to be created out of some sort of metal, they are also quite "+
                  "warm to touch..\n");
}
void init() {
::init();
add_action("orgasm","orgasm");
}
orgasm() {
player=this_player();
p=(string)this_player()->query_short();
tell_object(player,"The machine begins to whirr lightly....\n");
call_out("org2",5);
return(1);
}
org2() {
tell_object(player,"You begin to feel strange....\n");
call_out("org3",5); }
org3() {
tell_object(player,"You sigh heavily, quietly moaning.\n");
call_out("org4",5); }
org4() {
tell_object(player,"You shake your head from side to side, gripping the walls of the chamber.\n");
call_out("org5",3); }
org5() {
tell_object(player,"You begin to shiver excitedly.\n");
shout(p+" begins to moan in the distance.\n");
}
