#include "path.h"
inherit "std/room";
 
#define OUTSIDE "d/am/am/short2"
 
string ban;
static string message, apply_dom, *doms;
static int in_use;
static object sign;

string query_cap_name() { return "A wandering spirit"; }

void setup() {
  set_light(60);
  set_short("Patrician's Office");
  set_long(
"This is a small sparsely funished outer waiting room in the Patrician's "+
"Office.  Several busy looking clerks are rushing around.  You can see a "+
"small sign hanging on the wall.\n");
 
  add_exit("east", OUTSIDE, "door");
  add_exit("west", ROOM+"patrician2", "corridor");

  sign = clone_object(ITEM+"pat_sign");
  hidden_objects += ({ sign });

  add_item("busy looking clerk",
           "There are several of them pretending to look busy here.\n");

  set_zone("patricians office");
}
 
void init() {
  ::init();
  add_action("banish", "banish");
  add_action("start", "start");
  add_action("vote", "vote");
  add_action("apply", "apply");
  add_action("complain", "complain");
}

/*
int read(string str) {
  int plevel;

  if(str != "sign") {
    notify_fail("You can only see a sign here to read.\n");
    return 0;
  }
  if(this_player()->query_creator()) {
    write("As a creator you can :-\n"+
          "  banish <name> <reason>\n");
  }
  if(this_player()->query_property("guest")) {
    write("As a guest you can :-\n"+
          "  start     - remove guest property.\n");
  }
  if(!this_player()->query_property("guest")) {
    write("As a player you can :-\n"+
          "  vote\n"+
          "  apply     - apply to a house to become a creator.\n"+
          "  complain\n");
  }
  return 1;
}
*/

int banish(string str) {
  string str1, temp1;

  if(!this_player()->query_creator())
    return 0;
  notify_fail("Usage : banish <name> <reason>\n");
  if(!str)
    return 0;
  if(sscanf(str, "%s %s", str, str1) != 2)
    return 0;
  str = lower_case(str);
  if(sscanf(str,"%s.",temp1))
    return 0;
  if(sscanf(str, "%s/", temp1))
    return 0;
  if(file_size("/banish/"+str+".o") >= 0) {
    notify_fail("That name is already banished.\n");
    return 0;
  }
  if("secure/login"->test_user(str)) {
    notify_fail("That is a player.  You must rm or mv the player file first.\n");
    return 0;
  }
  ban = "Banished by : "+(string)this_player()->query_name()+"\n";
  ban += ctime(time())+"\n";
  ban += "Banish Reason : " + str1 + "\n";
  seteuid((string)"secure/master"->get_root_uid());
  write_file("/banish/"+str+".o", ban);
  seteuid("Room");
  write(str + " banished.\n");
  return 1;
}

int start() {
  if(!this_player()->query_property("guest")) {
    return 0;
  }
  this_player()->remove_property("guest");
  this_player()->save_me();
  write("You are no longer a guest.  Happy adventuring.\n");
  write("Please set your password too using password.\n");
  this_player()->do_force_on_me("password");
  return 1;
}

int complain() {
  if(this_player()->query_property("guest")) {
    return 0;
  }
  if(in_use) {
    write("Sorry, a complaint is currently being posted.\n"+
          "Please try again soon or fetch a creator if nobody is here.\n");
    return 1;
  }
  message = this_player()->query_cap_name() + " " + ctime(time()) + "\n";
  in_use = 1;
  write("You are lodging an official complaint about something or someone.\n"+
        "Do not take this action lightly.\n"+
        "Please provide as much information as possible and a Lord or God "+
        "will mail you regarding the problem as soon as possible.  Only "+
        "problems that you feel affect the entire mud should be complained "+
        "about.  Feedback for the general populous will probably end up on "+
        "bulletin boards around the place.\n");
  write("Enter complaint.  ** to end or ~q to abort.\n");
  write("] ");
  input_to("get_complaint");
  return 1;
}

void get_complaint(string str) {
  if(str == "~q") {
    write("Complaint aborted.\n");
    in_use = 0;
    message = "";
    return;
  }
  if(str == "**") {
    write("Complaint Posted\n");
    message += "\n\n";
    log_file("COMPLAINTS", message);
    message = "";
    in_use = 0;
    return;
  }
  message += str + "\n";
  write("] ");
  input_to("get_complaint");
}

int apply() {
  if(this_player()->query_property("guest")) {
    return 0;
  }
  if(this_player()->query_creator()) {
    notify_fail("You are a creator already stupid.\n");
    return 0;
  }
  if(!"secure/master"->eligible_creator(this_player())) {
    notify_fail("You have not fulfilled the requirements yet.\n");
    return 0;
  }
  write("To which domain would you like to apply (? for list) : ");
  input_to("apply2");
  return 1;
}

void apply3();

void apply2(string str) {
  doms = get_dir("/d/*");
  doms = delete(doms,0,2);
  if(str == "?") {
    int loop;

    for(loop = 0; loop < sizeof(doms); loop++) {
      write(doms[loop] + "\n");
    }
    apply();
    return;
  }
  if(member_array(str, doms) == -1) {
    write("That domain does not exist.\n");
    return;
  }
  apply_dom = str;
  apply3();
}

void apply3() {
  write("The lord of that domain will get back to you at the earliest "+
        "convenience to discuss the details with you.\n");
  log_file("APPLICATIONS", this_player()->query_name() + " " + 
           ctime(time()) + apply_dom + "\n\n");
}

int vote() {
  write("No issues up for vote.\n");
  return 1;
}
