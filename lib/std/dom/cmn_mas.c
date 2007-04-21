/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: cmn_mas.c,v 1.5 2000/12/19 07:21:36 drakkos Exp $
*/
/* inherited by the common rooms in the domains*/
inherit "/std/room/basic_room";
private nosave string _dom_name;

string query_dom() { return _dom_name; }
void set_dom(string str) { _dom_name = str; }

int do_list(string);
int add_creator(string);
int delete_creator(string);
int set_project(string, string);

void create() {
  do_setup++;
  ::create();
  do_setup--;

  add_property ("no scry", 1);
  add_property ("no teleport", 1);
  add_help_file ("/doc/creator/room/commonroom");
         
  if ( !do_setup ) {
    this_object()->setup();
    this_object()->reset();
  }
}

void init() {
  ::init();
  add_command("list", "<word'domain'>", (:do_list($4[0]):));
  add_command("list", "", (:do_list(0):));
  add_command("add", "<word'creator'>", (:add_creator($4[0]):));
  add_command("delete", "<word'creator'>" ,(:delete_creator($4[0]):));
  add_command("project", "<word'name'> <string'project'>", 
    (:set_project($4[0], $4[1]):));
}

int do_list(string arg) {
  string *domains, mast, s = "";
  string *members;
  int i, j;

  if(arg == "all")
    domains = get_dir("/d/");
  else if(!arg)
    domains = ({ _dom_name });
  else {
    domains = ({ arg });
  }

  if(sizeof(domains) > 1) {
    write("All domains:\n");
  }  
  else {
    write("This domain:\n");
  }
    
  for(i=0; i<sizeof(domains); i++) {
    mast = "/d/" + domains[i] + "/master";
    if (!load_object (mast)) {
      continue;
    }
    
    members = mast->query_members();
    
    if(!members) {
      members = ({ });
    }
    
    s += sprintf("%s has %s members and is owned by %s.\n", capitalize (domains[i]),
      query_num (sizeof(members)), capitalize((string)mast->query_lord()));

    for(j=0; j<sizeof(members); j++)
      s +=sprintf("* %s's project: %-=*s\n", capitalize(members[j]),
                       this_player()->query_cols()-12-strlen(members[j]),
                       mast->query_project(members[j]));
      s += "\n\n";
  }
    
  this_player()->set_finish_func(0);
  this_player()->more_string(s);
  return 1;
}

int add_creator(string arg) {
  string mast;
  
  arg = (string)this_player()->expand_nickname(arg);
  mast = "/d/" + _dom_name + "/master";
  if((string)mast->query_lord() != geteuid(this_player())) {
    this_player()->add_failed_mess(this_object(), "You are not the Lord of "
      "this Domain.\n", ({ }));
    return 0;
  }
  if(!mast->add_member(arg)) {
    this_player()->add_failed_mess(this_object(), sprintf ("%s is already a "
      "member of %s.\n", capitalize (arg), capitalize (query_dom())), 
      ({ }));
    return 0;
  }
  write("Creator " + capitalize(arg) + " now added to " + _dom_name + ".\n");
  return 1;
}

int delete_creator(string arg) {
  string mast;

  arg = (string)this_player()->expand_nickname(arg);
  mast = "d/" + _dom_name + "/master";
  if((string)mast->query_lord() != geteuid(this_player())) {
    this_player()->add_failed_mess(this_object(), "You are not the Lord of "
      "this Domain.\n", ({ }));    
    return 0;
  }
  if (!mast->remove_member(arg)) {
    this_player()->add_failed_mess(this_object(), sprintf ("%s is not a "
      "member of %s.\n", capitalize (arg), capitalize (query_dom())), 
      ({ }));
    return 0;
  }
  write("Member " + capitalize(arg) + " removed from " + _dom_name + ".\n");
  return 1;
}

int set_project(string arg, string project)
{
  string mast;
    
  mast = "d/" + _dom_name + "/master";

  if((string)mast->query_lord() != geteuid(this_player())) {
    this_player()->add_failed_mess(this_object(), "You are not the Lord of "
      "this Domain.\n", ({ }));   
    return 0;
  }
  arg = (string)this_player()->expand_nickname(arg);
  if(!mast->set_project(arg, project)) {
    this_player()->add_failed_mess(this_object(), sprintf ("%s is not a "
      "member of %s.\n", capitalize (arg), capitalize (query_dom())), 
      ({ }));
    return 0;
  }
  write(capitalize(arg) + "'s project set to : " + project + ".\n");
  return 1;
}
