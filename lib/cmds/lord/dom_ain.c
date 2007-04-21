/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: dom_ain.c,v 1.1 2003/03/21 02:01:51 ceres Exp $
 * 
 */
#include <parser.h>
#include <access.h>
#include <player_handler.h>

inherit "/cmds/base";

#define MASTER "/secure/master"

string get_last_log( int last_log_on );

string master;

int list(string arg) {
  int i, num, cols;
  string *members, *domains, mast;

  if (this_player() != this_player(1))
    return 0;
  domains = get_dir("/d/");
  if(!sizeof(domains)) {
    notify_fail("No domains exist.\n");
    return 0;
  }
  if(arg) {
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
                               mast->query_project(members[i]));
      return 1;
    }
  }
  write("Current domains:\n");
  for(i=0; i<sizeof(domains); i++) {
    mast = "/d/" + domains[i] + "/master";
    members = (string *)mast->query_members(arg);
    printf( "%15s has %2d %-7s and is owned by %s.\n", domains[ i ],
            num = sizeof( (string *)mast->query_members() ),
            ( ( num == 1 ) ? "member" : "members" ),
            capitalize( (string)mast->query_lord() ) );
  }
  return 1;
} /* list() */

int add_creator(string cre, string dom) {
  string mast;
  
  if (this_player() != this_player(1))
    return 0;

  if(file_size("/d/" + dom) != -2) {
    notify_fail("No such domain as " + dom + ".\n");
    return 0;
  }
  cre = (string)this_player()->expand_nickname(cre);
  mast = "/d/" + dom + "/master";
  if((string)mast->query_lord() != geteuid(previous_object()) &&
     !"/secure/master"->query_player_trustee(this_player()->query_name())) {
    notify_fail("You are not the Lord of that domain.\n");
    return 0;
  }
  if(!mast->add_member(cre)) {
    notify_fail("Failed to add " + capitalize(cre) + " to domain " +
      dom + ".\n");
    return 0;
  }
  write("Creator " + capitalize(cre) + " now added to domain " + dom + ".\n");
  return 1;
} /* add_creator() */

int delete_creator(string cre, string dom) {
  string mast;

  if (this_player() != this_player(1))
    return 0;

  if(file_size("/d/" + dom) != -2) {
    notify_fail("No such domain as " + dom + ".\n");
    return 0;
  }
  cre = (string)this_player()->expand_nickname(cre);
  mast = "d/" + dom + "/master";
  if((string)mast->query_lord() != geteuid(previous_object()) &&
     !"/secure/master"->query_player_trustee(this_player()->query_name())) {
    notify_fail("You are not the Lord of that domain.\n");
    return 0;
  }
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
  mast = "d/" + dom + "/master";
  if((string)mast->query_lord() != geteuid(previous_object()) &&
     !"/secure/master"->query_player_trustee(this_player()->query_name())) {
    notify_fail("You are not the Lord of that domain.\n");
    return 0;
  }
  if (!mast->set_project(cre, project)) {
    notify_fail("Failed to set the project " + project + " for " +
                capitalize(cre) + " in the domain " + dom + "\n");
    return 0;
  }
  write("Project for " + capitalize(cre) + " set to " + project + 
        " in the domain " + dom + ".\n");
  return 1;
} /* do_project() */

#ifdef DISABLED
// This is disabled since it can freeze the mud for 10+ seconds
int do_info() {
  string *creators,
         *domains,
         *mem,
         *cre_not,
         bit;
  mapping no_dir, not_creator;
  int i, j, cols;

  cols = (int)this_player()->query_cols();
  creators = get_dir("/w/");
  domains = "/secure/master"->query_domains();
  no_dir = ([ ]);
  not_creator = ([ ]);
  cre_not = ({ });
  for (i=0;i<sizeof(creators);i++)
    if (file_size("/w/"+creators[i]) != -2) {
      creators = delete(creators, i, 1);
      i--;
    } else if (!PLAYER_HANDLER->test_creator(creators[i])) {
      cre_not += ({ creators[i] });
      creators = delete(creators, i, 1);
      i--;
    }
  for (i=0;i<sizeof(domains);i++) {
    mem = (string *)("/d/"+domains[i]+"/master")->query_members() +
          ({ (string *)("/d/"+domains[i]+"/master")->query_lord() });
/* remove the members from the creators domain. */
    creators -= mem;
/* Check each one... */
    for (j=0;j<sizeof(mem);j++) {
      if (file_size("/w/"+mem[j]) != -2)
        no_dir[mem[j]] = domains[i];
      if (!PLAYER_HANDLER->test_creator(mem[j]))
        not_creator[mem[j]] = domains[i];
    }
/* Ok...  print the membership list. */
    bit = "Members of "+domains[i]+" are: ";
    printf("%s%-=*s\n", bit, cols-strlen(bit), implode(mem, ", ")+".");
  }
  if (sizeof(no_dir)) {
    write("These creators are in domains but do not have a directory:\n");
    printf("    %-=*s", cols-5, implode(keys(no_dir), ", ")+".\n");
  }
  if (sizeof(not_creator)) {
    write("These creators are in domains but are not actually creators:\n");
    printf("    %-=*s", cols-5, implode(keys(not_creator), ", ")+".\n");
  }
  if (sizeof(creators)) {
    write("These creators are not in any domain:\n");
    printf("    %-=*s", cols-5, implode(creators, ", ")+".\n");
  }
  cre_not -= ({ ".dead_ed_files", ".old_creators" });
  if (sizeof(cre_not)) {
    write("These creators have a directory, but are not creators...:\n");
    printf("    %-=*s", cols-5, implode(cre_not, ", ")+".\n");
  }
  return 1;
} /* do_info() */
#endif

int detailed_info( string name, string option ) {
  string ret, *members;
  int i, cols;

  if(member_array(name, "/secure/master"->query_domains()) == -1)
    return notify_fail("No such domain: " + name + "\n");

  cols = this_player()->query_cols();
  
  master = "/d/"+name+"/master";
  write(master+"\n");
  members = master->query_members();
  switch (option[0..0]) {
  case "n":
    members = sort_array(members, 1);
    break;
  case "p":
    members = sort_array(members,
                         (: strcmp(master->query_project($1),
                                   master->query_project($2)) :));
    break;
  case "l":
    members = sort_array(members,
                         (:PLAYER_HANDLER->test_last($2) -
                          PLAYER_HANDLER->test_last($1):));
    break;
  }
  ret = "The current members of this domain are:";
  
  for (i=0;i<sizeof(members);i++) {
    ret += sprintf("\n%-12s: Last login: %s\n",
                   capitalize(members[i]),
                   find_player(members[i])?
                     "%^GREEN%^%^BOLD%^Now%^RESET%^"
                     :get_last_log((int)PLAYER_HANDLER->test_last(members[i])),
                   );
    ret += sprintf("              Project   : %-=*s\n", cols-27,
                   master->query_project(members[i]));
  }
  this_player()->more_string(ret, "Domain Info");
  return 1;
}

string get_last_log(int last_log_on) {
    string retval;
    int tmp_time, sec, min, hour, day;

/* Should be a nice number.... */
    tmp_time = time()-last_log_on;
    if (!tmp_time) {
      sec = min = hour = day = 0;
    }
    if(tmp_time > 24*60*60) {
      retval = (day = tmp_time/(24*60*60))+" day"+
                (day<2?"":"s")+" ago.";
      if(day > 14)
        retval = "%^RED%^"+retval+"%^RESET%^";
      else if(day > 7)
        retval = "%^YELLOW%^"+retval+"%^RESET%^";
      else
        retval = "%^GREEN%^"+retval+"%^RESET%^";
    } else
      retval = "%^GREEN%^Today%^RESET%^";
    
    return retval;
}

mixed *query_patterns() {
  return ({
    "list <string'list'>", (: list($4[0]) :),
    "add <word'creator'> <word'domain'>", (: add_creator($4[0], $4[1]) :),
    "delete <word'creator'> <word'domain'>",
      (: delete_creator($4[0], $4[1]) :),
    "project <word'creator'> <word'domain'> <string'project'>",
      (: do_project($4[0], $4[1], implode($4[2..], " ")) :),
#ifdef DISABLED     
    "info", (: do_info() :),
#endif      
    "info <string'domain'>", (: detailed_info($4[0], " ") :),
    "info <string'domain'> [by] {n|name|p|project|l|login}",
      (: detailed_info($4[0], $4[1]) :) });
} /* query_patterns() */
