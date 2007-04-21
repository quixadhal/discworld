/* The singing birthday demon */
#define SONG "/obj/b_day/happy"

inherit "/obj/monster";

int line;
object owner;
string owner_name;
string* act_lines = ({
  ":takes a deep breath...",
  "sing \"Happy Birthday to you\"",
  "sing \"Happy Birthday to you\"",
  "sing \"Happy Birthday, dear $N\"",
  ":pauses for breath...",
  "sing \"Happy Birthday to yoooou.\"",
  "sing \"The End!\"",
  "hug $n in a way that only a tiny paper "
    "demon could",
  ":waves a minute pointed hand.",
  ":frowns, then folds itself into nothingness..."
});

void setup()
{
  set_name("demon");

  set_short("origami demon");

  add_adjective("tiny");
  add_adjective("pink");
  add_adjective("paper");
  add_adjective("origami");
  set_main_plural("origami demons");
  add_plural("demons");

  set_long("This is a tiny, pink, paper demon. Looking closely you see "
           "that it has been made by someone with a black belt in "
           "Origami!\n"); 
  set_race("demon");
  set_level(1);
  line = 1;
}

void set_owner(string person_name)
{
  owner = find_living(lower_case(person_name));
  owner_name = person_name;
}


void do_the_act(int i)
{
  string line_to_act;
  
  if(i >= sizeof(act_lines)) {
    dest_me();
    return;
  }

  if(environment(owner) != environment()) {
    tell_room(environment(), "The origami demon rustles after "
              + owner_name + ".\n");
    move(environment(owner));
    tell_room(environment(), "The origami demon rustles in.\n" );
    do_command("'Hey "+owner_name+"! I hadn't finished!\n"); 
    return;
  }   
  line_to_act = act_lines[i];
  line_to_act = replace(line_to_act, ({"$N", owner_name, 
                                       "$n", lower_case(owner_name) }));
  if(!strlen(line_to_act)) {
    set_heart_beat(0);
    dest_me();
    return;
  }
  do_command(line_to_act);
  i++;
  if(owner) {
    call_out("do_the_act", 2, i);
  }
  return; 
}


/*Let's keep this fellow unkillable, thankyouverymuch! Siel*/
void attack_by( object thing ){
  
  if( !thing) return;
  if( environment( thing ) != environment() ) return;
  this_object()->stop_fight (thing); 
  thing->stop_fight (this_object());
  init_command("say Meanie!  Shame on you!  I shall finish what I "
               "started and then I shall be off.  You don't really "
               "deserve this, you know.", 1);
  return;
  
} /* attack_by() */
