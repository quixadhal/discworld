#include "player.h"
#include "living.h"

/* The effects of being drunk.
 * It is not recomended for a good party :)
 */
void do_wander(int bing);

static int drunkness, missing;

int drunk_heart_beat(int intox) {
  int lvl, nxt, i, con;
  object *obs;

  if (intox <= 0)
    return 1;
  if (missing) {
    missing --;
    return 0;
  }
  if (this_object()->query_property(PASSED_OUT_PROP))
    return 0;
  if (--drunkness > 0 && !missing)
    return 1;
  con = (int)this_player()->query_con();
  if (random((50-con)*10) > intox)
    return 1;
  drunkness = 20+random(30);
  lvl = random(intox)/1500;
  if (lvl > 3)
    lvl = 3;
  switch (lvl) {
    case 0 :
             switch (random(5)) {
               case 0 :
                        command("hiccup");
                        break;
               case 1 :
                        command("burp");
                        break;
               case 2 :
                        command("puke");
                        break;
               case 3 :
                        command("trip");
                        break;
               case 4 :
                        command("stagger");
                        break;
             }
             break;
    case 1 :
             switch (random(1)) {
               case 0 :
                       write("You trip over and fall on your face.\n");
                       say(this_player()->query_cap_name()+" trips over "+
                         "and falls on " +
                         (string)this_object()->query_possessive() +
                         " face.\n");
                       missing = 10+random(10);
                       return 0;
                       break;
             }
             break;
    case 2 :
             switch (random(2)) {
               case 0 :
                      obs = all_inventory();
                      while (sizeof(obs) &&
                             obs[(i=random(sizeof(obs)))]->move(environment())&&
                             !obs[i]->short())
                        obs = delete(obs, i, 1);
                      if (!sizeof(obs))
                        break;
                      write("Opps butterfingers, you have droped "+
                             obs[i]->short()+".\n");
                      say(this_player()->query_cap_name()+" stumbles a bit "+
                          "and drops "+obs[i]->short()+".\n");
                      break;
               case 1 :
                      write("Now how did you get that bruise?\n");
                      say(this_player()->query_cap_name()+" falls heavily "+
                           "on the ground.  You wince in sympathy.\n");
                      this_player()->adjust_hp(-random(100));
                      break;
             }
             break;
    case 3 :
             switch (random(2)) {
               case 0 :
                        this_player()->add_property(PASSED_OUT_PROP,1);
                        call_out("remove_property",20+random(30),
                                     PASSED_OUT_PROP);
                        write("The world goes black.  You have passed out.\n");
                        say(this_object()->query_cap_name()+
                             " looks ill the falls on the ground and lies "+
                             "there with a blank look on "+
                             this_object()->query_objective()+" face.\n");
                        return 0;
                        break;
               case 1 : /* wandering fits */
                       do_wander(5+random(10));
                       break;
             }
             break;
  }
  return 1;
}

void do_wander(int num) {
  string *direcs;
  int i, bong;

  if (num > 0)
    call_out("do_wander", 2+random(5), --num);

  direcs = (mixed *)environment()->query_dest_dir();
  while (!bong && sizeof(direcs)) {
    i = random(sizeof(direcs)/2)*2;
    this_object()->add_property(UNKNOWN_MOVE_PROP, 1);
    bong = command(direcs[i]);
    this_object()->remove_property(UNKNOWN_MOVE_PROP);
    if (!bong)
      direcs = delete(direcs, i, 2);
  }
}
