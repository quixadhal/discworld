inherit "/std/room";
#include "path.h"
#include "money.h"
#include "login_handler.h"

#define SAY(y,x) tell_room(this_object(), "Bernita says"+y+": "+x+"\n")
#define COST 100
#define COST_STRING "10 copper coins"
#define DELIVER 300
#define DELIVER_STRING "30 copper coins"
#define DELAY_COST 600
#define DELAY_STRING "60 copper coins"
#define MY_FILE "/d/am/save/flower_shop"

static mapping doing_flower;
mapping delayed_delivery;
static string *flower_names,
              *flower_longs;

void add_flower(string short, string long);

void setup() {
  set_short("flower shop");
  set_light(60);
  set_long(
"A small shop filled with flowers of all shapes and sizes.  You are "+
"surprised that they are still living in the stodgey atmosphere of "+
"Ankh Morpork.  There is a small sign on the wall and a lady standing "+
"behind the counter.\n");

  add_exit("south", ROOM+"filgree5", "door");

  add_item("colourfull colorfull color full lady",
           "A lady wearing brightly coloured clothes with a genuinely "+
           "happy look on her face.  You are surprised that she can "+
           "keep such an expresion in the city.\n");
  add_item("flower",
           "There are flowers everywhere!  Covering the walls, in small "+
           "baskets... You name it, if it can have a flower in it, there "+
           "is one there.\n");
  add_item("counter",
           "A small wooden counter.  It has deep cuts it in from where "+
           "someone has used a knife many times to cut something up.  It "+
           "looks well used.\n");

  add_sign("A small sign painted bright red.\n",
           "The sign has written on it, \"Welcome to Bernita's flower shop, "+
           "you may 'order' some flowers for your favourite person or "+
           "just for your self.  Enquire about our special delivery services"+
           "\"\n");

  add_flower("red rose", "A beautiful looking red rose.  It looks almost "+
                         "as though it was frozen in place.  It is picture "+
                         "perfect.\n");
  add_flower("yellow carnation",
"A lovely looking carnation.  It has lovely yellow petals that curl away "+
"from the center in a lovely roseta pattern.\n");
  add_flower("ultraviolet weeds",
"A nice bunch of slowly glowing weeds.  The person who sent this to you "+
"obviously has a warped sense of humour.\n");
  delayed_delivery = ([ ]);
  restore_object(MY_FILE);
}

void init() {
  ::init();
  add_action("order", "order");
}

void add_flower(string sh, string ln) {
  if (!flower_names) {
    flower_names = ({ });
    flower_longs = ({ });
    doing_flower = ([ ]);
  }
  flower_names += ({ sh });
  flower_longs += ({ ln });
}

int order(string str) {
  int flower,i;
  string s1, *st;
  object ob;

  if (!str) {
    SAY(" asking ", "Ahhh so "+this_player()->query_cap_name()+
        " would like to order some flowers?");
    SAY(" asking ", "What sort of flowers are you interested in buying?");
    SAY("", "We have "+implode(flower_names, ", ")+" available.");
    SAY("", "All my flowers cost "+COST_STRING+".\n");
    return 1;
  }
  if (this_player()->query_value() < COST) {
    SAY("", "I am sorry but "+this_player()->query_name()+
            " does not have enough money to buy a flower.\n");
    return 1;
  }
  if ((flower = member_array(str, flower_names)) == -1) {
    for (i=0;i<sizeof(flower_names);i++)
      if (sscanf(flower_names[i], "%s"+str+"%s", s1, s1) == 2) {
        flower = i;
        break;
      }
    if (i == sizeof(flower_names)) {
      SAY("", "I am sorry we do not appear to have a "+str+" here.\n");
      return 1;
    }
  }
  ob = clone_object("/std/object");
  ob->set_short(flower_names[i]);
  ob->set_name("flower");
  ob->set_long(flower_longs[i]);
  st = explode(flower_names[i], " ");
  ob->add_alias(st[sizeof(st)-1]);
  ob->add_plural(pluralize(st[sizeof(st)-1]));
  ob->add_adjective(st[0..sizeof(st)-2]);
  ob->add_property("read id", "note");
  ob->add_property("dest on quit", 1);
  if (ob->move(this_player()))
    ob->move(this_object());
  this_player()->pay_money(MONEY_HAND->create_money_array(COST));
  doing_flower[this_player()] = ob;
  write("Bertina asks you: Would you like to add a note to it? ");
  say("Bertina asks "+this_player()->query_cap_name()+" something.\n");
  input_to("get_choice");
  return 1;
}

int get_choice(string str) {
  str = lower_case(str);
  if (str[0] != 'y' && str[0] != 'n') {
    write("Bertina says: I don't understand do you want to write a note? ");
    say("Bertina asks "+this_player()->query_cap_name()+" something.\n");
    input_to("get_choice");
    return 1;
  }
  if (str[0] == 'y') {
    this_player()->do_edit("", "fix_flower");
    return 1;
  }
  if ((int)this_player()->query_value() < DELIVER)
    write("Bertian says: You cannot afford to have them delivered, sorry.\n");
  else {
    write(
"Bertina says: It will cost you "+DELIVER_STRING+" to send it to soemone.\n"+
"Bertina asks you: Do you wish to have your flower delivered to someone? ");
    input_to("test_deliver");
    say("Bertina asks "+this_player()->query_cap_name()+" something.\n");
  }
  return 1;
}

int fix_flower(string str) {
  doing_flower[this_player()]->set_read_mess(str+"\n");
  write(
"Bertina says: It will cost you "+DELIVER_STRING+" to send it to soemone.\n"+
"Bertina asks you: Do you wish to have your flower delivered to someone? ");
  say("Bertina asks "+this_player()->query_cap_name()+" something.\n");
  input_to("test_deliver");
  return 1;
}

int test_deliver(string str) {
  str = lower_case(str);
  if (str[0] != 'y' && str[0] != 'n') {
    write("Bertina says: I don't understand do you want the flower delivered? ");
    say("Bertina asks "+this_player()->query_cap_name()+" something.\n");
    input_to("test_deliver");
    return 1;
  }
  if (str[0] == 'y') {
    if ((int)this_player()->query_value() < DELIVER) {
      write("Bertaina says: Opps, you appear to have lost all your money.\n");
      return 1;
    }
    write("Bertina asks: Who do you wish to deliver it to? ");
    say("Bertina asks "+this_player()->query_cap_name()+" something.\n");
    input_to("test_recipient");
    return 1;
  }
  return 1;
}

int test_recipient(string str) {
  object dest;

  str = (string)this_player()->expand_nickname(str);
  str = lower_case(str);
  if ((dest = find_living(str))) {
    if ((int)this_player()->query_value() < DELIVER) {
      write("Bertaina says: Opps, you appear to have lost all your money.\n");
      return 1;
    }
    this_player()->pay_money(MONEY_HAND->create_money_array(DELIVER));
    write("Ok, delivering to "+str+"\n");
    tell_room(this_object(),
       "A small womble scurries out from under the bench looks apologeticaly "+
       "around the room.  Takes the flower from Bertina and wombles out of "+
       "the room.\n");
    tell_room(environment(dest),
       "Womble wombles into the room.\n"+
       "Womble gives a "+doing_flower[this_player()]->short()+
               " to "+dest->query_cap_name()+".\n"+
       "Wombles mumble something about it being from "+
           this_player()->query_cap_name()+".\n"+
       "Womble looks appolgetic and wombles out of the room.\n");
    if (doing_flower[this_player()]->move(dest))
      doing_flower[this_player()]->move(environment(dest));
    doing_flower = m_delete(doing_flower, this_player());
    return 1;
  }
  if ("/secure/login"->test_user(str) &&
      (int)this_player()->query_value() >= DELAY_COST &&
      !delayed_delivery[str]) {
    write("Bertina says: I am sorry but "+str+" is not logged in right now.\n");
    write("Bertine says: It will cost you another "+DELAY_STRING+
          " to have it delivered when they log in.\n");
    write("Bertina says: Would you like to have it delivered? ");
    say("Bertina asks "+this_player()->query_cap_name()+" something.\n");
    delayed_delivery[doing_flower[this_player()]] = str;
    input_to("delay_deliver");
    return 1;
  }
  write("Bertina says: I am sorry but I cannot find "+str+".\n");
  write("Bertina says: Would you like to send it to someone else? ");
  say("Bertina asks "+this_player()->query_cap_name()+" something.\n");
  input_to("test_deliver");
  return 1;
}

int delay_deliver(string str) {
  string bit;

  str = lower_case(str);
  if (str[0] != 'y' && str[0] != 'n') {
    write("Bertina says: I don't understand do you want the flower delivered? ");
    say("Bertina asks "+this_player()->query_cap_name()+" something.\n");
    input_to("delay_deliver");
    return 1;
  }
  if (str[0] == 'n') {
    write("Bertina says: Would you like to deliver it to someone else instead? ");
    say("Bertina asks "+this_player()->query_cap_name()+" something.\n");
    input_to("test_deliver");
    delayed_delivery = m_delete(delayed_delivery, doing_flower[this_player()]);
    return 1;
  }
  if (this_player()->query_value() < DELAY_COST) {
    write("Opps,  You appear to have lost all your money.\n");
    return 1;
  }
  write("Bertina says: Ok, ready to be sent when they log in.\n");
  say("Bertina asks "+this_player()->query_cap_name()+" something.\n");
  this_player()->pay_money(MONEY_HAND->create_money_array(DELIVER));
  bit = delayed_delivery[doing_flower[this_player()]];
  delayed_delivery = m_delete(delayed_delivery, doing_flower[this_player()]);
  delayed_delivery[bit] = ({ doing_flower[this_player()]->query_short(), 
                             doing_flower[this_player()]->query_long(),
                             doing_flower[this_player()]->query_read_mess(),
                             this_player()->query_cap_name() });
  doing_flower[this_player()]->dest_me();
  save_object(MY_FILE);
  return 1;
}

void event_person_say(object ob, string start, string mess) {
  string s1, s2;

  if (sscanf(mess, "%sdelivery%s", s1, s2))
    SAY("", "The delivery costs for our flowers is "+DELIVER_STRING+".");
}

void entering(string name, string type) {
  object ob, pl;
  string *st, sh;

  if(type != LOGIN)
    return;
  if (!delayed_delivery[name])
    return ;
  if(!pointerp(delayed_delivery[name])) { 
    map_delete(delayed_delivery,name);
    return;
  }
  pl = find_player(name);
  ob = clone_object("/std/object");
  ob->set_short((sh=delayed_delivery[name][0]));
  ob->set_name("flower");
  ob->set_long(delayed_delivery[name][1]);
  st = explode(sh, " ");
  ob->add_alias(st[sizeof(st)-1]);
  ob->add_plural(pluralize(st[sizeof(st)-1]));
  ob->add_adjective(st[0..sizeof(st)-2]);
  ob->set_read_mess(delayed_delivery[name][2]);
  ob->add_property("read id", "note");
  ob->add_property("dest on quit",1);
  if (ob->move(pl))
    ob->move(environment(this_object()));
  tell_room(environment(pl),
"A very bored looking womble appears out of a corner you were not "+
"previously aware of.  It looks bordly at "+capitalize(name)+" gives "+
pl->query_pronoun()+" a bunch of "+
"flowers profunctionaly and says something about them being from "+
delayed_delivery[name][3]+" and vanishes back into the corner.\n", ({ pl }));
  tell_object(pl,
"A very bored looking womble appears out of a corner you were not "+
"previously aware of.  It looks bordly at you gives you a bunch of "+
"flowers profunctionaly and says something about them being from "+
delayed_delivery[name][3]+" and vanishes back into the corner.\n");
  delayed_delivery = m_delete(delayed_delivery, name);
  save_object(MY_FILE);
}
