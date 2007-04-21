/* domain administration room */
#include "access.h"
#include "path.h"
inherit "/std/room/basic_room";
#define MASTER "/secure/master"

int do_list(string arg);
int do_create(string dom, string lord);
int do_list_all();
int do_deputy(string cre, string dom);
int do_undeputy(string cre, string dom);
int do_create(string dom, string lord);
int do_project(string cre, string dom, string project);
int do_add(string cre, string dom);
int do_delete(string cre, string dom);

void setup() {
//  string *doms, com;
//  int i;
  
  set_light(100);
  set_short("domain-control room");
  add_property("determinate", "the ");
  set_long("You float in nothingness, before a vast turtle, its shell pocked "
    "by asteroid craters. In all directions stars twinkle in the black " 
    "curtain of space.\n" 
"Available commands:\n"
"  list                         : list all domains.\n"
"  list <domain>                : list members of a domain.\n"
"  create <domain>              : create a domain (you are lord).\n"
"  create <domain> <lord>       : create a domain for someone else.\n" 
"  add <creator> <domain>       : add a creator to a domain you own.\n" 
"  delete <creator> <domain>    : remove a creator from a domain you own.\n" 
"  project <cre> <dom> <proj>   : Set the creators project.\n"
"  deputy <cre> <dom>           : Appoint creator as a deputy.\n"
"  undeputy <cre> <dom>         : Remove creator as a deputy.\n"
);

  add_item("turtle", "On its back you can see four elephants, and they bear " +
    "the weight of the disc upon their wide backs.\n");
  add_item("elephants", "The four great elephants labour endlessly in the " +
    "task of turning the disc upon their backs.\n");
  add_item("disc", "The whole discworld, from hub to rim, rides upon the " +
    "elephant's backs.\n");
  add_alias("elephant", "elephants");
  add_exit("north", ROOM+"development", "corridor");
  add_exit("south", ROOM+"site_control", "corridor");
  add_exit("west", ROOM+"access_control", "corridor");
  seteuid("Admin");
} /* setup() */

void init() {
  ::init();
/*
  add_action("list", "list", 10);
  add_action("create_domain", "create", 10);
  add_action("add_creator", "add", 10);
  add_action("delete_creator", "delete", 10);
  add_action("do_project", "project", 10);
 */
  add_command("list", "", (: do_list_all() :) );
  add_command("list", "<word>", (: do_list($4[0]) :) );
  add_command("create", "<word>", (: do_create($4[0], 0) :) );
  add_command("create", "<word> <word>", (: do_create($4[0], $4[1]) :) );
  add_command("add", "<word> <word>", (: do_add($4[0], $4[1]) :) );
  add_command("delete", "<word> <word>", (: do_delete($4[0], $4[1]) :) );
  add_command("project", "<word> <word> <string>",
                         (: do_project($4[0], $4[1], $4[2]) :) );
  add_command("deputy", "<word> <word>", (: do_deputy($4[0], $4[1]) :) );
  add_command("undeputy", "<word> <word>", (: do_undeputy($4[0], $4[1]) :) );
} /* init() */

int do_list_all() {
  int i, num;
  string *members, *domains, mast;

  if (this_player() != this_player(1))
    return 0;

  domains = "/secure/master"->query_domains();
  if(!sizeof(domains)) {
    notify_fail("No domains exist.\n");
    return 0;
  }

  write("Current domains:\n");
  for(i=0; i<sizeof(domains); i++) {
    mast = "/d/" + domains[i] + "/master";
    members = (string *)mast->query_members();
    printf( "%15s has %2d %-7s and is owned by %s.\n", domains[ i ],
      num = sizeof( (string *)mast->query_members() ),
      ( ( num == 1 ) ? "member" : "members" ),
      capitalize( (string)mast->query_lord() ) );
  }
  return 1;
} /* list() */

int do_list(string arg) {
  int i, cols;
  string *members, *domains, mast;

  if (this_player() != this_player(1))
    return 0;

  domains = "/secure/master"->query_domains();
  if(!sizeof(domains)) {
    notify_fail("No domains exist.\n");
    return 0;
  }

  if(member_array(arg, domains) == -1) {
    notify_fail("No such domain as " + arg + ".\n");
    return 0;
  } else {
    mast = "/d/" + arg + "/master";
    write(capitalize((string)mast->query_lord()) + " is the lord of " +
      arg + ".\nMembers         Project\n");
    members = (string *)mast->query_members();
    cols = (int)this_player()->query_cols();
    for (i=0;i<sizeof(members);i++)
      printf("%-15s %*-=s\n", members[i], cols-16,
                             mast->query_project(members[i]) +
             (mast->query_deputy(members[i])?" (deputy)":""));
  }
  return 1;
} /* do_list() */

int do_create(string dom, string lord) {
  if (this_player() != this_player(1))
    return 0;

  if(!MASTER->high_programmer(geteuid(previous_object()))) {
    notify_fail("You lack the Power of Change on this scale!\n");
    return 0;
  }
  if (!lord) {
    lord = (string)this_player()->query_name();
  }

  if (!MASTER->create_domain(dom, lord)) {
    notify_fail("Failed to create domain " + dom + " with the lord " + lord
                + ".\n");
    return 0;
  }

  write("You create the domain " + dom + " with " + capitalize(lord) +
    " as its lord.\n");
  return 1;
} /* do_create() */

int do_add(string cre, string dom) {
  string mast;
  
  if (this_player() != this_player(1))
    return 0;

  if(file_size("/d/" + dom) != -2) {
    notify_fail("No such domain as " + dom + ".\n");
    return 0;
  }

  cre = (string)this_player()->expand_nickname(cre);
  mast = "/d/" + dom + "/master";
/* Move this checking into the master object of the domain... */
/*
  if ( !"/secure/master"->query_lord( geteuid( previous_object() ) ) ) {
    notify_fail( "You are not a Lord.\n" );
    return 0;
  }
 */

  if (!mast->add_member(cre)) {
    notify_fail("Failed to add " + capitalize(cre) + " to domain " +
      dom + ".\n");
    return 0;
  }

  write("Creator " + capitalize(cre) + " now added to domain " + dom + ".\n");
  return 1;
} /* do_add() */

int do_delete(string cre, string dom) {
  string mast;

  if (this_player() != this_player(1))
    return 0;

  if(file_size("/d/" + dom) != -2) {
    notify_fail("No such domain as " + dom + ".\n");
    return 0;
  }

  cre = (string)this_player()->expand_nickname(cre);
  mast = "d/" + dom + "/master";
/* Move this checking into the master object of the domain... */
/*
  if ( !"/secure/master"->query_lord( geteuid( previous_object() ) ) ) {
    notify_fail( "You are not a Lord.\n" );
    return 0;
  }
 */

  if (!mast->remove_member(cre)) {
    notify_fail("Failed to remove " + capitalize(cre) + " from domain " +
       dom + "\n");
    return 0;
  }

  write("Member " + capitalize(cre) + " removed from domain " + dom + ".\n");
  return 1;
} /* delete_creator() */

int do_project(string cre, string dom, string project) {
  string mast;

  if (this_player() != this_player(1))
    return 0;

  if(file_size("/d/" + dom) != -2) {
    notify_fail("No such domain as " + dom + ".\n");
    return 0;
  }
  cre = (string)this_player()->expand_nickname(cre);
  mast = "/d/" + dom + "/master";
/*
  if ( !"/secure/master"->query_lord( geteuid( previous_object() ) ) ) {
    notify_fail( "You are not a Lord.\n" );
    return 0;
  }
 */

  if (!mast->set_project(cre, project)) {
    notify_fail("Failed to set the project " + project + " for " +
                capitalize(cre) + " in the domain " + dom + "\n");
    return 0;
  }

  write("Project for " + capitalize(cre) + " set to " + project + 
        " in the domain " + dom + ".\n");
  return 1;
} /* do_project() */

int do_deputy(string cre, string dom) {
  string mast;

  if (this_player() != this_player(1))
    return 0;

  if(file_size("/d/" + dom) != -2) {
    notify_fail("No such domain as " + dom + ".\n");
    return 0;
  }
  cre = (string)this_player()->expand_nickname(cre);
  mast = "d/" + dom + "/master";

  if (!mast->add_deputy(cre)) {
    notify_fail("Failed to add the deputy " +
                capitalize(cre) + " to the domain " + dom + "\n");
    return 0;
  }

  write("Deputy " + capitalize(cre) + " added to "
        "the domain " + dom + ".\n");
  return 1;
} /* do_deputy() */

int do_undeputy(string cre, string dom) {
  string mast;

  if (this_player() != this_player(1))
    return 0;

  if(file_size("/d/" + dom) != -2) {
    notify_fail("No such domain as " + dom + ".\n");
    return 0;
  }
  cre = (string)this_player()->expand_nickname(cre);
  mast = "d/" + dom + "/master";

  if (!mast->remove_deputy(cre)) {
    notify_fail("Failed to remove the deputy " +
                capitalize(cre) + " from the domain " + dom + "\n");
    return 0;
  }

  write("Deputy " + capitalize(cre) + " removed from "
        "the domain " + dom + ".\n");
  return 1;
} /* do_undeputy() */
