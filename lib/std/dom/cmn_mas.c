/* inherited by the common rooms in the domains*/
inherit "/std/room";
string dom_name;

string query_dom() { return dom_name; }
void set_dom(string str) { dom_name = str; }

void init() {
  ::init();
  add_action("do_list", "list");
  add_action("add_creator", "add");
  add_action("delete_creator", "del*ete");
  add_action("set_project", "pro*ject");
}

int do_list(string arg) {
  string *domains, mast, s, *ppl;
  mapping members;
  int i, j;

  if(arg == "all")
    domains = get_dir("/d/");
  else if(!arg)
    domains = ({ dom_name });
  else {
    notify_fail("Usage: list\n       list all\n");
    return 0;
  }

  if(sizeof(domains) > 1) write("All domains:\n");
  else write("This domain:\n");
  s = "";
  for(i=0; i<sizeof(domains); i++) {
    mast = "/d/" + domains[i] + "/master";
    members = (mapping)mast->query_members();
    if(!members) members = ([ ]);
    printf("%8s has %2d members and is owned by %s.\n", domains[i],
      m_sizeof(members), capitalize((string)mast->query_lord()));
    ppl = m_indices(members);
    for(j=0; j<sizeof(ppl); j++)
      s += sprintf("%s : %-*=s", capitalize(ppl[j]),
        (int)this_player()->query_cols(), members[ppl[j]] + ".\n");
    s += "\n\n";
  }
  this_player()->set_finish_func(0);
  this_player()->more_string(s);
  return 1;
}

int add_creator(string arg) {
  string mast;
  
  if (!arg) {
    notify_fail("Usage: add <creator>\n");
    return 0;
  }
  arg = (string)this_player()->expand_nickname(arg);
  mast = "/d/" + dom_name + "/master";
  if((string)mast->query_lord() != geteuid(this_player())) {
    notify_fail("You are not the Lord of this Domain.\n");
    return 0;
  }
  if(!mast->add_member(arg)) {
    notify_fail(capitalize(arg) + " is already a member of "+dom_name + ".\n");
    return 0;
  }
  write("Creator " + capitalize(arg) + " now added to " + dom_name + ".\n");
  return 1;
}

int delete_creator(string arg) {
  string mast;

  if (!arg) {
    notify_fail("Usage: delete <creator>\n");
    return 0;
  }
  arg = (string)this_player()->expand_nickname(arg);
  mast = "d/" + dom_name + "/master";
  if((string)mast->query_lord() != geteuid(this_player())) {
    notify_fail("You are not the Lord of this Domain.\n");
    return 0;
  }
  if (!mast->remove_member(arg)) {
    notify_fail(capitalize(arg)+" is not a member of "+dom_name+".\n");
    return 0;
  }
  write("Member " + capitalize(arg) + " removed from " + dom_name + ".\n");
  return 1;
}

int set_project(string str)
{
  string name, project, mast;
  
  if(sscanf(str, "%s %s", name, project) != 2) {
    notify_fail("Usage: project <creator> <project>\n");
    return 0;
  }
  mast = "d/" + dom_name + "/master";
  if((string)mast->query_lord() != geteuid(this_player())) {
    notify_fail("You are not the lord of this domain.\n");
    return 0;
  }
  name = (string)this_player()->expand_nickname(name);
  if(!mast->set_project(name, project)) {
    notify_fail(capitalize(name) + " is not a member of this domain.\n");
    return 0;
  }
  write(capitalize(name) + "'s project set to : " + project + ".\n");
  return 1;
}
