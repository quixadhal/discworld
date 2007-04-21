inherit "/cmds/base";

string idle_time(object person) {
  int hours, mins, secs;
  string s, m, h;

  secs = query_idle(person);
  mins = secs / 60;
  secs %= 60;
  hours = mins / 60;
  mins %= 60;
  s = ("0"+secs)[<2..<1];
  m = ("0"+mins)[<2..<1];
  if ( !hours )  {
    return " (idle: "+m+":"+s+")";
  }
  h = ("0"+hours)[<2..<1];
  return " (idle: "+h+":"+m+":"+s+")";
} /* idle_time() */

int cmd( int show_idle ) {
  object *people, person;
  string *cre, *invis, *director, *trustee, bing, obtyp;

  cre = ({ });
  invis = ({ });
  director = ({ });
  trustee = ({ });

  people = filter_array( users(), (: $1->query_creator() :) );
  people = people - this_player()->query_ignoring( people );
  people = sort_array( people,
    (: strcmp( $1->query_name(), $2->query_name() ) :) );

  foreach( person in people )  {
    bing = person->query_cap_name();
    if ( show_idle && query_idle( person ) > 120 )  {
      bing += idle_time( person );
    }
    if (!this_player()->query_property("nod")) {
      obtyp = person->query_object_type();
        switch(obtyp) {
        case "X":
           break;
        case " ":
           break;
        case "C":
           bing += "(%^CYAN%^C%^RESET%^)";
           if ( "/d/liaison/master"->query_member((string)person->query_name()))
              bing += "(%^YELLOW%^l%^RESET%^)";
           break;
        case "S":
           bing += "(%^BOLD%^%^CYAN%^S%^RESET%^)";
           if ( "/d/liaison/master"->query_member((string)person->query_name()))
             bing += "(%^YELLOW%^l%^RESET%^)";
           break;
        case "T":
           bing += "(%^BOLD%^%^RED%^T%^RESET%^)";
           if ( "/d/liaison/master"->query_member((string)person->query_name()))
             bing += "(%^YELLOW%^l%^RESET%^)";
           break;
        case "D":
           bing += "(%^RED%^D%^RESET%^)";
           if ( "/d/liaison/master"->query_member((string)person->query_name()))
             bing += "(%^YELLOW%^l%^RESET%^)";
           break;
        default:
           break;
      }
    }

    switch( person->query_invis()  )  {
      case 0:
        cre += ({ bing });
        break;
      case 1:
        invis += ({ bing });
        break;
      case 2:
        director += ({ bing });
        break;
      default:
        trustee += ({ bing });
        break;
    }
  }

  if(sizeof(cre)) {
    tell_object(this_player(),
      "/global/events"->convert_message("$I$2=%^GREEN%^"
      "Visible%^RESET%^: "+query_multiple_short(cre)+".\n"));
  }
  if(sizeof(invis)) {
    tell_object(this_player(),
      "/global/events"->convert_message("$I$2=%^CYAN%^"
      "Invisible%^RESET%^: "+query_multiple_short(invis)+".\n"));
  }
  if(sizeof(director)) {
    tell_object(this_player(),
      "/global/events"->convert_message("$I$2=%^YELLOW%^Director "
      "Invisible%^RESET%^: "+query_multiple_short(director)+".\n"));
  }
  if(sizeof(trustee)) {
    tell_object(this_player(),
      "/global/events"->convert_message("$I$2=%^RED%^Trustee "
      "Invisible%^RESET%^: "+query_multiple_short(trustee)+".\n"));
  }
  return 1;
} /* cmd() */

mixed *query_patterns() { 
    return ({ "", (: cmd(1) :),
              "noidle", (: cmd(0) :) });
} /* query_patterns() */ 
