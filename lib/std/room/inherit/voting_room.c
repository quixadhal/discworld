/*  -*- LPC -*-  */
/*
 * $Locker: pinkfish $
 * $Id: voting_room.c,v 1.9 2001/10/16 16:53:06 ceres Exp pinkfish $
 *
 */

#include <board.h>

#define DEFAULT_VOTE_DURATION (2600*24*7)

/**
 * Generalised voting room intended to be inherited into rooms in the mud that
 * will setup the parameters and settings for votes.
 * <p>
 * The room does not inherantly support player initiated votes, however it is
 * a fairly simple matter to add this to your own room and have your room add
 * votes when players initiate them.
 *
 * @author Ceres
 */

/**
 *  This class stores the essential information about a vote. It is only used
 *  internally to the voting room.
 * @element type used to setup vote types
 * @element desc the description of the vote
 */
class vote {
  string type;                  /* used to setup vote types */
  string desc;                  /* description of the vote */
  string *choices;              /* options in the vote */
  mixed *votes;
  int ending;                   /* when the vote ends */
  string *voted;                /* people who've voted */
}

/**
 *  This class stores the essential information about an election during the
 *  setup phase. It is only used internally to the voting room.
 */
class election {
  mixed *candidates;            /* who are the candidates */
  int closes;                   /* when does acceptance of nominees close */
}

private int vote_counter;               /* Used to give each vote a unique number */

private mapping votes;                  /* the votes themselves */
private mapping elections;              /* the elections being setup */

private nosave string _vote_file;        /* Controls which vote file to use */
private nosave mixed _vote_cond;
private nosave mixed _comp_action;       /* Completion action */
private nosave int _vote_duration;       /* The normal duration for a vote */
private nosave int _open_voting;         /* Make who voted for what known */
private nosave int _no_elections;
private nosave string _proxy;            /* A proxy room to use for votes. */

private nosave mixed _stand_cond;
private nosave mixed _second_cond;

private nosave mixed _election_announcement; /* The board, person and subject */

int list_votes();
int cast_vote( string words, int num );
int stand_for_election(string position);
int second_candidate(string who, string position);
int list_elections();
int do_create_vote(string choices);

/**
 * @ignore
 */
void create() {
  seteuid("Room");
  votes = ([ ]);
  elections = ([ ]);
}

/**
 * @ignore
 */
void init() {
   // commands for votes
   add_command("vote", "<string> on <number>", (: cast_vote($4[0], $4[1]) :) );
   add_command("list", "votes", (: list_votes :));
   add_command("votes", "", (: list_votes :));

   // commands for elections
   if (!_no_elections) {
      add_command( "stand", "[for] <string>", (: stand_for_election($4[0]) :));
      add_command( "second", "<string> [for] <string>",
                   (: second_candidate($4[0], $4[1]) :) );
      add_command("list", "elections", (: list_elections :));
      add_command("elections", "", (: list_elections :));
   }
} /* init() */

/**
 * This method is used to return the current votes.  This can be overridden
 * in higher functions to get the votes from somewhere else if nessessary.
 * @return the votes
 */
protected mapping query_our_votes() {
   if (_proxy) {
      return _proxy->query_votes();
   }
   if (votes) {
      return votes;
   }
   return ([ ]);
} /* query_our_votes() */

/**
 * This method adds in a vote.
 * @param id the id of the vote
 * @param data the vote data
 */
protected void add_our_vote(int id, class vote data) {
   if (!votes) {
      votes = ([ ]);
   }
   votes[id] = data;
} /* add_our_vote() */

/**
 * This method is used to return the current elections.  This can be
 * overridden in hgiher inherits to control things better.
 * @return the elections
 */
protected mapping query_our_elections() {
   if (_proxy) {
      return _proxy->query_elections();
   }
   if (elections) {
      return elections;
   }
   return ([ ]);
} /* query_our_elections() */

/**
 * This method adds in a election.
 * @param name the name of the election to add
 * @param data the election data
 */
protected void add_our_election(string name, class election data) {
   if (!elections) {
      elections = ([ ]);
   }
   elections[name] = data;
} /* add_our_election() */

/**
 * This is the init() which should be called as well as the default
 * init() if you wish to have player added votes.
 */
void init_add_vote() {
   add_command( "add", "vote with <string'choices'>",
               (: do_create_vote($4[0]) :));
} /* init_add_vote() */

/**
 * This setups all the stuff that  needs to be setup after the room has
 * been loaded.  Makes all the callouts for the right amount of time
 * and so on.
 */
void setup_after_load() {
  int i;
  string election;
  int *vote_ids;

  if (_proxy) {
    return ;
  }
  vote_ids = m_indices( query_our_votes() );

  for ( i = 0; i < sizeof( vote_ids ); i++ ) {
    if ( query_our_votes()[ vote_ids[ i ] ]->ending < time() ) {
      call_out( "end_vote", 10 * ( i + 1 ), vote_ids[ i ] );
    } else {
      call_out( "end_vote", query_our_votes()[ vote_ids[ i ] ]->ending - time(),
                vote_ids[ i ] );
    }
  }

  foreach(election in keys(query_our_elections())) {
    if(query_our_elections()[election]->closes < time()) {
      call_out( "start_election_vote", 30 * random(5), election);
    } else {
      call_out( "start_election_vote", query_our_elections()[election]->closes - time(),
                election);
    }
  }
} /* setup_after_load() */

/**
 * This method is called by the inheriting object to determine which save file
 * to use for votes.
 *
 * @param str The name of the file you want the voting info saved to
 * (without the .o)
 */
void set_save_file(string file) {
  
  _vote_file = file;
  if( file_size( _vote_file + ".o" ) > 0 ) {
    unguarded( (: restore_object, _vote_file :) );
    setup_after_load();
  } else {
    elections = ([ ]);
    votes = ([ ]);
  }
}

/**
 * This method returns the save file currently used for the room.
 * @return the current save file
 */
string query_save_file() { return _vote_file; }

/**
 * Private function to save the rooms data file.
 */
protected void save_room() {
   if (_vote_file) {
      unguarded( (: save_object, _vote_file :) );
   }
}

/**
 * This method sets up a proxy for the room.  A proxy is somewhere else
 * to get all the voting information from.
 * @param proxy the proxy tpo setup
 */
void set_proxy(string proxy) {
   _proxy = proxy;
} /* set_proxy() */

/**
 * This method sets the room to not allow elections and disable all the
 * election commands.
 * @param no_elections the flag
 */
void set_no_elections(int no_elections) {
   _no_elections = no_elections;
} /* set_no_elections() */

/**
 * This function setups if the votes should be open or not.  If a vote is
 * open then you know who voted for each thing.
 * @param open 1 for an open system 0 for closed
 */
void set_open_voting(int open) {
   _open_voting = open;
} /* set_open_voting() */

/**
 * This function queries if the votes should be open or not.  If a vote is
 * open then you know who voted for each thing.
 * @return 1 for an open system 0 for closed
 */
int query_open_voting(int open) {
   return _open_voting;
} /* set_open_voting() */

/**
 * This function defines a function to be called to determine if a player
 * is eligible to vote.
 *
 * @param cond This will usually be a mixed array of an object and a function.
 * It could also be a function pointer.
 *
 * Your function should return 1 if the player is eligible to vote or 0 if
 * they are not.
 */
void set_vote_conditions(mixed cond) { _vote_cond = cond; }

/**
 * This function defines a function to be called when the vote is complete.
 * Typically your function will post the vote results somewhere or somesuch.
 * The parameters passedinto the function are:<br>
 * ( string type, string description, string* choices, mapping votes, string* voted) <br>
 *
 * @param cond This will usually be a mixed array of an object and a function.
 * It could also be a function pointer.
 */
void set_completion_action(mixed cond) { _comp_action = cond; }

/**
 * This function is used to set the normal vote and election duration. It can
 * be overridden when calling add_vote. If it is not set and no duration is
 * given in add_vote then the default value of DEFAULT_VOTE_DURATION is used.
 *
 * @param duraction The number of seconds the vote should be open.
 *
 * @see add_vote()
 */
void set_vote_duration(int duration) { _vote_duration = duration; }

/**
 * This function is used to returns the normal vote and election duration.
 * If it is not set and no duration is
 * given in add_vote then the default value of DEFAULT_VOTE_DURATION is used.
 *
 * @param duraction The number of seconds the vote should be open.
 *
 * @see add_vote()
 */
int query_vote_duration() {
   if (!_vote_duration) {
      return DEFAULT_VOTE_DURATION;
   }
   return _vote_duration;
}

/**
 * This function defines a function to be called to determine if a player
 * is eligible to stand for election to a position.
 *
 * @param cond This will usually be a mixed array of an object and a function.
 * It could also be a function pointer.
 *
 * Your function should return 1 if the player is eligible to stand or 0 if
 * they are not.
 */
void set_stand_conditions(mixed cond) { _stand_cond = cond; }

/**
 * This function defines a function to be called to determine if a player
 * is eligible to second a canditate for election.
 *
 * @param cond This will usually be a mixed array of an object and a function.
 * It could also be a function pointer.
 *
 * Your function should return 1 if the player is eligible to second or 0 if
 * they are not.
 */
void set_second_conditions(mixed cond) { _second_cond = cond; }

/**
 * This function defines the board, person and subject for announcements
 * of elections.
 *
 * @param board This is the board to post to.
 * @param person This is who to post as.
 * @param subject This is the subject line to use.
 * @param prefix The text to preceed the message.  The default is
 *               "All eligible persons are requested to vote for the position "
 *               "of "
 * @param suffix The text to follow the message.
 */
void set_election_announcement(string board, string person, string subject,
                               string prefix, string suffix) {
  _election_announcement = ({ board, person, subject, prefix, suffix });
}

/**
 * This function is called when a player votes. The syntax is
 * "vote <choice> on <vote>".
 *
 * @see set_vote_conditions()
 */
int cast_vote( string which_str, int vote_id ) {
  string pname;
  int which, ok;
  class vote this_vote;
 
  if (_proxy) {
     return _proxy->cast_vote(which_str, vote_id);
  }
 
  if ( undefinedp(query_our_votes()[vote_id])) {
    add_failed_mess("There is no vote " + vote_id + ".\n");
    return 0;
  }

  this_vote = query_our_votes()[vote_id];
  which = member_array(which_str, this_vote->choices);
  if (which == -1) {
    if(strlen(which_str) > 1) {
      add_failed_mess("There is no choice " + which_str + " for vote id " +
           vote_id + ".\n");
      return 0;
    }

     which = upper_case( which_str )[ 0 ] - 65;
  }
  if(which < 0 || which > sizeof(this_vote->choices) -1) {
    add_failed_mess("There is no choice " + which_str + " for vote id " +
                           vote_id + ".\n");
    return 0;
  }
  
  ok = 1;
  if (functionp(_vote_cond)) {
    ok = evaluate(_vote_cond, this_player(), this_vote->desc);
  }
  if(arrayp(_vote_cond)) {
    ok = call_other(_vote_cond[0], _vote_cond[1], this_player(), this_vote->desc);
  }

  if(!intp(ok)) {
    ok = 0;
  }

  if(!ok) {
    add_failed_mess("Sorry, but you are not allowed to vote on this "
                       "subject.\n");
    return 0;
  }

  pname = this_player()->query_name();
  if(member_array(pname, this_vote->voted) != -1) {
    add_succeeded_mess(({"You have already voted on this subject.\n", ""}));
    return 1;
  }

  this_vote->voted += ({ pname });
  if (_open_voting) {
     this_vote->votes[which] += ({ pname });
  } else {
     if (arrayp(this_vote->votes[which])) {
        this_vote->votes[which] = sizeof(this_vote->votes[which]);
     }
     this_vote->votes[which]++;
  }
  save_room();
 
  add_succeeded_mess(({ "You cast your vote for "+which_str + " on " +
                        vote_id+".\n",
                        "$N casts a vote.\n" }));
  
  return 1;
} /* cast_vote() */

/**
 * This function is called when a player types 'list'. It lists the currently
 * open votes.
 */
int list_votes() {
  int i, j, *vote_ids;
  string text;
  class vote this_vote;
  
  if ( !m_sizeof( query_our_votes() ) ) {
    write( "There are no votes in progress.\n" );
    return 1;
  }
  
  vote_ids = m_indices( query_our_votes() );
  if ( sizeof( vote_ids ) > 1 )
    write( "The following votes are in progress:\n" );
  else
    write( "The following vote is in progress:\n" );
  text = "";

  for ( i = 0; i < sizeof( vote_ids ); i++ ) {
    this_vote = query_our_votes()[vote_ids[i]];
    text += "  "+ vote_ids[i] + ". " + this_vote->desc +"\n Choices are :\n";

    for(j=0; j<sizeof(this_vote->choices); j++) {
      text += sprintf("    %c. %s\n", 'A'+j, this_vote->choices[j]);
    }
    text += " Voting closes at "+ ctime( this_vote->ending ) +".\n\n";
  }
  
  this_player()->more_string(sprintf( "%-=*s",
                                      (int)this_player()->query_cols(),
                                      text ));
  return 1;
}

/**
 * This function is called to add a vote to the system.
 *
 * @param type Freeform string giving the type of vote. This is typically used
 * by your completion function so that it can perform different actions for
 * different types of votes.
 *
 * @param description The description of the vote as shown to the player.
 *
 * @param choices An array of vote options eg. ({"Yes", "No" })
 *
 * @param ending An integer time of when the vote should be terminated.
 *
 * @see set_completion_action()
 */
void add_vote(string type, string description, string *choices, int ending) {
  class vote new_vote;
  
  if (_proxy) {
     return _proxy->add_vote(type, description, choices, ending);
  }
  vote_counter++;
  
  new_vote = new(class vote);
  new_vote->type = type;
  new_vote->desc = description;
  new_vote->choices = choices;
  if (_open_voting) {
    new_vote->votes = allocate(sizeof(choices), (: ({ }) :));
  } else {
    new_vote->votes = allocate(sizeof(choices));
  }
  if(ending) {
    new_vote->ending = ending;
  } else if(_vote_duration) {
    new_vote->ending = (_vote_duration + time());
  } else {
    new_vote->ending = (DEFAULT_VOTE_DURATION + time());
  }
  new_vote->voted = ({ });

  add_our_vote(vote_counter, new_vote);
  //query_our_votes()[ vote_counter ] = new_vote;

  call_out( "end_vote", new_vote->ending - time(), vote_counter );

  save_room();
}

/**
 * @ignore
 */
mapping query_votes() { return query_our_votes() + ([ ]); }

/**
 * @ignore
 */
mapping query_elections() { return query_our_elections() + ([ ]); }

/**
 * This function is called to terminate a vote. It calls your completion
 * action.
 *
 * @see set_completion_action()
 */
void end_vote( int which ) {
  if ( !query_our_votes()[ which ] ) {
      return;
  }

  if(functionp(_comp_action)) {
      evaluate(_comp_action, query_our_votes()[which]->type, query_our_votes()[which]->desc,
               query_our_votes()[which]->choices, query_our_votes()[which]->votes,
               query_our_votes()[which]->voted);
  } else if(arrayp(_comp_action)) {
    call_other(_comp_action[0], _comp_action[1], query_our_votes()[which]->type,
               query_our_votes()[which]->desc, query_our_votes()[which]->choices, query_our_votes()[which]->votes,
               query_our_votes()[which]->voted);
  }

  map_delete(query_our_votes(), which);

  save_room();
}

/**
 * This function is provided for convenience to make it easy for your vote
 * end function to post to a board.
 *
 * @param board The name of the board to post to.
 * @param name The name of the person to post as.
 * @param subject The subject line to use.
 * @param message The message to post.
 */
void make_announcement(string board, string name, string subject,
                       string message ) {
  BOARD_HAND->add_message(board, name, subject,
                          sprintf( "%-=*s", 64, message));
}

/**
 * This function is used to initiate an election.
 * It sets up the election class and adds it to the mapping.
 *
 * @param position The name of the position the election is for.
 */
void initiate_election(string position) {
  class election tmp;

  if (_proxy) {
     return _proxy->initiate_election(position);
  }
  if(query_our_elections()[position])
    return;

  tmp = new(class election);
  tmp->candidates = ({});
  if(_vote_duration) {
    tmp->closes = time() + _vote_duration;
  } else {
    tmp->closes = time() + DEFAULT_VOTE_DURATION;
  }

  add_our_election(position, tmp);

  call_out("start_election_vote", tmp->closes + 60, position);
  
  save_room();
} /* initiate_election() */

/**
 * This method determines if there is already an election of the
 * specified type in progress.
 * @param position the name of the election in progress
 * @return 1 if there is an election in progress
 */
int query_election_in_progress(string position) {
   if (classp(query_our_elections()[position])) {
       return 1;
   }
   return 0;
} /* query_election_in_progress() */

/**
 * This function is used by players to stand for election. The syntax is:
 * "stand for <position>".
 *
 * @see set_stand_conditions()
 */
int stand_for_election(string position) {
  int ok, i;
  
  if (_proxy) {
     return _proxy->stand_for_election(position);
  }
  // check if they're eligible.
  ok = 1;
  if(functionp(_stand_cond)) {
    ok = evaluate(_stand_cond, this_player(), position);
  } else if(arrayp(_stand_cond)) {
    ok = call_other(_stand_cond[0], _stand_cond[1], this_player(), position);
  }

  if(!intp(ok))
    ok = 0;

  if(!ok) {
    add_succeeded_mess(({ "Sorry, but you are not allowed to stand for this "
                            "election.\n", ""}));
    return 1;
  }

  // check if there is an election for this position.
  if(!query_our_elections()[position]) {
    add_succeeded_mess(({"There is no election in progress for the post of " +
                    position + ".\n", ""}));
    return 1;
  }

  // check if they're already a candidate for this position.
  for(i=0; i<sizeof(query_our_elections()[position]->candidates); i++) {
    if(query_our_elections()[position]->candidates[i][0] == this_player()->query_name()) {
      add_succeeded_mess(({"You are already standing in this "
                             "election.\n", ""}));
      return 1;
    }
  }
    
  // add them to the list of candidates.
  query_our_elections()[position]->candidates += ({({ this_player()->query_name(), "" })});
  save_room();
  add_succeeded_mess(({"You have been added to the list of candidates for "
                         "the position of " + position + ".  You must now "
                         "find someone to second your candidacy.\n",
                         "$N stands for election.\n"}));
  return 1;
}

/**
 * This function is used by players to second candidates for election. If a
 * candidates isn't seconded he/she won't be in the election. The syntax is:
 * "second <player> for <position>".
 *
 * @see set_second_conditions()
 */
int second_candidate(string who, string position) {
  mixed *candidates;
  int i, found, ok;
  
  if (_proxy) {
     return _proxy->second_candidate(who, position);
  }
  // check if they're eligible.
  ok = 1;
  if(functionp(_second_cond)) {
    ok = evaluate(_second_cond, this_player(), position);
  } else if(arrayp(_second_cond)) {
    ok = call_other(_second_cond[0], _second_cond[1], this_player(), position);
  }

  if(!intp(ok))
    ok = 0;

  if(!ok) {
    add_succeeded_mess(({"Sorry, but you are not allowed to second candidates "
                           "in this election.\n", ""}));
    return 1;
  }

  // check the position is up for election
  if(!query_our_elections()[position]) {
    add_succeeded_mess(({"There is no election in progress for " + position +
                           ".\n", ""}));
    return 1;
  }

  // check the candidate is standing
  candidates = (query_our_elections()[position])->candidates;
  for(i=0; i< sizeof(candidates); i++) {
    if(candidates[i][0] == who)
      found = i+1;
  }

  if(!found) {
    add_succeeded_mess(({who + " is not standing for the position of " +
                           position + ".\n", ""}));
    return 1;
  }

  if(this_player()->query_name() == who) {
    add_succeeded_mess(({"You cannot second yourself.\n", ""}));
    return 1;
  }

  if(candidates[found-1][1] != "") {
    add_succeeded_mess(({candidates[found-1][0] +
                           " has already been seconded by " +
                           candidates[found-1][1] + ".\n", ""}));
    return 1;
  }
  
  // mark the candidate as seconded (supported).
  candidates[found-1][1] = this_player()->query_name();

  save_room();
  
  add_succeeded_mess(({candidates[found-1][0] +
                         " has been seconded in the election for "+
                         position+".\n", ""}));
  return 1;
}

/**
 * Once the candidacy phase is over this function starts the election vote
 * itself by taking all eligible candidates, setting them as choices in the
 * election and then posting an announcemment.
 */
void start_election_vote(string post) {
  string str, *choices;
  int i;
  
  if(!query_our_elections()[post])
    return;


  if(_election_announcement[3])
    str = _election_announcement[3];
  else
    str = "All eligible persons are requested to vote for the position of ";
  
  str += post + "\n\nThe candidates are:\n";
  
  choices = ({ });
  for(i=0; i<sizeof(query_our_elections()[post]->candidates); i++) {
    if(query_our_elections()[post]->candidates[i][1] != "") {
      str += sprintf("  %s seconded by %s.\n", 
                     query_our_elections()[post]->candidates[i][0],
                     query_our_elections()[post]->candidates[i][1]);
      choices += ({ query_our_elections()[post]->candidates[i][0] });
    }
  }

  if(!sizeof(choices)) {
    str = "In the election for the position of " + post +
      " no candidate stood for election therefore the election "
      "is null and void.\n";
  } else if(sizeof(choices) < 2) {
    str = "In the election for the position of " + post +
      " only one candidate stood for election therefore the election "
      "is null and void.\n";
  } else {

    if(_election_announcement[4])
      str += _election_announcement[4];

    add_vote("election", "Election for the post of " + post + "\n", choices,
             0);
  }
  
  make_announcement(_election_announcement[0], _election_announcement[1],
                    _election_announcement[2], str);

  map_delete(query_our_elections(), post);
  save_room();
}

/**
 * This function is called when a player types 'elections'. It lists the
 * elections currently accepting candidates.
 */
int list_elections() {
  class election this_election;
  string *posts, text;
  int i, j;
  
  if ( !m_sizeof( query_our_elections() ) ) {
    write( "There are no elections in progress.\n" );
    return 1;
  }
  
  posts = m_indices( query_our_elections() );
  if ( sizeof( posts ) > 1 )
    write( "The following elections are in progress:\n" );
  else
    write( "The following election is in progress:\n" );
  text = "";

  for ( i = 0; i < sizeof( posts ); i++ ) {
    this_election = query_our_elections()[posts[i]];
    text += "  Election to the post of "+ posts[i] + ".\n";

    if(!sizeof(this_election->candidates)) {
      text += "  No candidates have declared yet.\n";
    } else {
      text += "    Current candidates are :\n";
      
      for(j=0; j<sizeof(this_election->candidates); j++) {
        if(this_election->candidates[j][1] != "")
          text += sprintf("    %c. %s seconded by %s.\n", 'A'+j,
                          this_election->candidates[j][0],
                          this_election->candidates[j][1]);
        else
          text += sprintf("    %c. %s not yet seconded.\n", 'A'+j,
                          this_election->candidates[j][0]);
          
      }
    }
    text += " Candidacies must be declared by "+
      ctime( this_election->closes ) +".\n\n";
  }
  
  this_player()->more_string(sprintf( "%-=*s",
                                      (int)this_player()->query_cols(),
                                      text ));
  return 1;
}

/** @ignore yes */
string strip_spaces(string str) {
   if (!strlen(str)) {
      return str;
   }
   while (str[0] == ' ') {
      str = str[1..];
   }
   if (!strlen(str)) {
      return str;
   }
   while (str[<1] == ' ') {
      str = str[0..<2];
   }
   return str;
} /* strip_spaces() */

/**
 * This is an option function which can be defined in upper level inherits.
 * It allows the players to create their own votes with their own choices.
 */
int do_create_vote(string choices) {
   string* bits;

   bits = map(explode(choices, ","), (: strip_spaces($1) :)) - ({ "" });
   write("Choices: " + query_multiple_short(bits) + ".\n");
   write("What description would you like for your vote?\n");
   this_player()->do_edit("", "create_vote_desc", this_object(), 0, bits);
   add_succeeded_mess(({ "", "$N starts to create a new vote.\n" }));
   return 1;
} /* do_create_vote() */

/** @ignore yes */
void create_vote_desc(string str, string* choices) {
   if (!str) {
      write("Aborting.\n");
      return ;
   }
   write("Are you sure you wish to create a vote with a description of:\n" +
         str + "\nWith vote choices of " + query_multiple_short(choices) +
         ".\n");
   write("Please answer yes or no: ");
   input_to("create_vote_desc_confirm", 0, str, choices);
} /* create_vote_desc() */

/** @ignore yes */
void create_vote_desc_confirm(string str, string desc, string* choices) {
   str = lower_case(str);
   if (!strlen(str) ||
       (str[0] != 'y' && str[0] != 'n' && str[0] != 'q')) {
      write("Please answer yes or no: ");
      input_to("create_vote_desc_confirm", 0, str, choices);
   }

   if (str[0] == 'q' || str[0] == 'n') {
      write("Ok, quitting.\n");
      return ;
   }

   //
   // Now we add the vote.
   //
   add_vote("freeform", desc, choices, 0);
   write("Added in the vote.\n");
} /* create_vote_desc_confirm() */

/**
 * This is an administrative function to allow the removal/cancellation
 * of an election.
 * @param election The name of the election to be cancelled
 * @return Returns 1 for success, 0 for failure.
 */
int delete_election(string election) {
  if(!query_our_elections()[election])
    return 0;
  map_delete(query_our_elections(), election);
  save_room();
  return 1;
}

/**
 * This is an administrative function to allow the removal/cancellation
 * of a vote.
 * @param vote_id The id number of the vote to be cancelled.
 * @return Returns 1 for success, 0 for failure.
 */
int delete_vote(int vote_id) {
  if(!query_our_votes()[vote_id])
    return 0;
  
  map_delete(query_our_votes(), vote_id);
  save_room();
  return 1;
}
