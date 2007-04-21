/*
** The way this used to be, if the player trying to work the quest 
** suddenly buggered off in the middle of it, the quest related 
** weights would still be there, but if another player came along and 
** moved one of the weights, the quest related one would vanish. This 
** has puzzled players, and cased at least one bug report.  I've changed 
** it so that the quest related weight hangs around until a player resets 
** the weights or actually starts the quest using the figure command. 
**
** Also removed some code that has been commented out for two years, and 
** some other bits that were redundant because of this piece of unused 
** code. And did some general Spring Cleaning. It seems to be working fine, 
** but feel free to revert it if it breaks horribly.  --Tilly  
*/ 


#include <library.h>

#define DEFAULT_PANS ([ \
  "left" : ([ ]), \
  "middle" : ([ \
  "very small" : ({ "1/9", 1 }), \
  "small" : ({ "1/3", 3 }), \
  "medium" : ({ "1", 9 }), \
  "large" : ({ "3", 27 }), \
  "very large" : ({ "9", 81 }) ]), \
  "right" : ([ ]) ])

inherit "/std/object";

private nosave int balance;
private nosave string *doing;
private nosave mapping pans;


/* Start of function prototypes */
void setup();
void init();
string *query_doing();
mapping query_pans();
string long( string words, int dark );
string pans_look();
void recalculate_balance();
int reset_weights();
string weight_string( int weight );
int weigh_something( object* obs );
int figure_it_out();
int move_a_specific_weight( string weight_size, string pan_to );
int move_a_weight( string weight_size, string pan_from, string pan_to );
void it_is_balanced( object person );
/* End of function prototypes */


void setup() {
  set_name( "balance" );
  set_short( "weighing balance" );
  add_adjective( "weighing" );
  add_alias( ({ "pan", "pans", "weight", "weights" }) );
  set_weight( 200 );
  reset_get();
  doing = ({ });
  pans = DEFAULT_PANS;
} /* setup() */


void init() {
  add_command("weigh", "<indirect:object:me'thing(s)'> on <direct:object>",
    (:this_object()->weigh_something($1):));
  add_command("reset", "weights on balance", (: reset_weights() :));
  add_command("figure", "", (:this_object()->figure_it_out():));
  add_command("move",
          "<string'size'> weight to <string'position'> pan",
          (:this_object()->move_a_specific_weight($4[0], $4[1]):));
} /* init() */


string *query_doing() { return doing; }

mapping query_pans() { return pans; }


string long( string words, int dark ) {
  int i, j;
  string long;
  string *bits;
  string *places;
  string *weights;

  if (!words) {
    words = "balance";
  }
  bits = explode( words, " " );
  switch ( bits[ sizeof( bits ) - 1 ] ) {
    case "balance" :
      return "This is a largish bronze balance, securely bolted in "
        "place.  The main part of the balance is a long arm which "
        "pivots at its centre.  There is a pan hanging from each "
        "end of the arm such that it will be level when the weights "
        "in the pans are equal.  A third pan is fixed to a stationary "
        "part of the balance where the weights can be held when not "
        "in use.\n" + pans_look() + "You could probably use the balance "
        "to weigh something.\nThere appears to be something "
        "written on it.\n";
    case "pan" :
    case "pans" :
      return "There are three pans.  One pan hangs from the left end of "
        "the arm, one from the right end and there is a third pan "
        "in the middle.\n"+ pans_look();
    case "weight" :
    case "weights" :
      long = "There are weights of many different sizes in the pans.  "
        "They are:\n";
      bits = ({ });
      places = m_indices( pans );
      for ( i = 0; i < sizeof( places ); i++ ) {
        if ( !m_sizeof( pans[ places[ i ] ] ) ) {
          continue;
        }
        weights = m_indices( pans[ places[ i ] ] );
        for ( j = 0; j < sizeof( weights ); j++ ) {
          if ( sizeof( doing ) ) {
            if ( doing[ 1 ] == weights[ j ] ) {
              continue;
            }
          }
          bits += ({ add_a( weights[ j ] ) + " weight marked with \""
            + pans[ places[ i ] ][ weights[ j ] ][ 0 ] +" lb\"" });
        }
      }
      long += "    " + implode( bits[ 0 .. sizeof( bits ) - 2 ],
        ",\n    " ) +",\n  and "+ bits[ sizeof( bits ) - 1 ] + ".\n";
      long += "The weights can be moved from one pan to another and "
        "reset back to their starting positions.\n";
      return long;
  }
  return "You're not quite sure what you're looking at.\n";
} /* long() */


string pans_look() {
  int i;
  int j;
  string pans_status;
  string *places;
  string *weights;

  pans_status = "";
  places = m_indices( pans );
  for ( i = 0; i < sizeof( places ); i++ ) {
    pans_status += "The "+ places[ i ] +" pan ";
    if ( !m_sizeof( pans[ places[ i ] ] ) )
      pans_status += "is empty.\n";
    else {
      weights = m_indices( pans[ places[ i ] ] );
      for ( j = 0; j < sizeof( weights ); j++ )
        weights[ j ] = add_a( weights[ j ] ) +" weight";
      pans_status += "holds "+ query_multiple_short( weights ) +".\n";
    }
  }
  switch ( balance ) {
    case -1 :
      pans_status += "The left pan hangs lower than the right pan.\n";
      break;
    case 0 :
      pans_status += "The left pan hangs level with the right pan.\n";
      break;
    case 1 :
      pans_status += "The left pan hangs higher than the right pan.\n";
      break;
  }
  return pans_status;
} /* pans_look() */


void recalculate_balance() {
  int i;
  int j;
  int old_balance;
  string *places;
  string *weights;
  mapping pan_weights;

  pan_weights = ([ ]);
  places = m_indices( pans );
  for ( i = 0; i < sizeof( places ); i++ ) {
    pan_weights[ places[ i ] ] = 0;
    if ( !m_sizeof( pans[ places[ i ] ] ) ) {
      continue;
    }
    weights = m_indices( pans[ places[ i ] ] );
    for ( j = 0; j < sizeof( weights ); j++ ) {
      pan_weights[ places[ i ] ] +=
        pans[ places[ i ] ][ weights[ j ] ][ 1 ];
    }
  }
  old_balance = balance;
  if ( pan_weights[ "left" ] > pan_weights[ "right" ] ) {
    balance = -1;
  } else {
    if ( pan_weights[ "left" ] == pan_weights[ "right" ] ) {
      balance = 0;
    } else {
      balance = 1;
    }
  }
  if ( old_balance == balance ) {
    switch ( balance ) {
      case -1 :
        tell_room( environment(), "The arm rocks a bit but steadies, "
          "with the left pan still hanging lowest.\n" );
        break;
      case 1 :
        tell_room( environment(), "The arm rocks a bit but steadies, "
          "with the right pan still hanging lowest.\n" );
        break;
    }
    return;
  }
  switch ( balance ) {
    case -1 :
      tell_room( environment(), "The arm of the balance tips and the "
        "left pan ends up hanging lowest.\n" );
      break;
    case 0 :
      tell_room( environment(), "The arm of the balance levels out, "
        "with the left and right pans hanging level.\n" );
      break;
    case 1 :
      tell_room( environment(), "The arm of the balance tips and the "
        "right pan ends up hanging lowest.\n" );
      break;
  }
} /* recalculate_balance() */


int reset_weights() {
  pans = DEFAULT_PANS;
  if (sizeof(doing)) {
    pans[ "left" ][ doing[ 1 ] ] = ({ "?", 1 + random( 121 ) });
  }
  add_succeeded_mess("$N $V the weights on $D.\n");
  recalculate_balance();
  return 1;
} /* reset_weights() */


string weight_string( int weight ) {
  int wholes;
  int ninths;

  wholes = weight / 9;
  ninths = weight % 9;
  if ( wholes && ninths ) {
    return wholes + " " + ninths + "/9 lb";
  }
  if ( wholes ) {
    return wholes + " lb";
  }
  return ninths + "/9 lb";
} /* weight_string() */


int weigh_something( object* obs ) {
  int i;
  int info;
  int weight;
  object person;

  if ( sizeof( doing ) ) {
    person = find_player( doing[ 0 ] );
    if ( person == this_player() ) {
      add_failed_mess( "Hold your horses, you're trying to get the hang "
        "of it still.\n" );
      return 0;
    }
    if ( person && ( environment( person ) == environment() ) ) {
      add_failed_mess( (string)person->one_short() + " is using "
        "the balance at the moment.  Come back when "
        + (string)person->query_pronoun() + " has finished.\n" );
      return 0;
    }
    doing = ({ });
    pans = DEFAULT_PANS;
  }
  info = (int)LIBRARY->query_player_quest_info(
    (string)this_player()->query_name(), "balance" );
  switch ( info ) {
    case 0 :
      add_failed_mess( "You don't know how the balance works to weigh "
        "anything.\nTry looking at the balance, the pans and "
        "the weights, and then maybe you can figure it out.\n" );
      return 0;
    case 1 :
      add_failed_mess( "You're still not too sure how the balance works "
        "to weigh anything.\nTry looking at the balance, the pans and "
        "the weights, and then maybe you can figure it out.\n" );
      return 0;
    case 2 :
      add_failed_mess( "You're very nearly sure how the balance works, "
        "but maybe you should try to figure it out once more before "
        "you weigh anything.\n" );
      return 0;
  }
  for ( i = 0; i < sizeof( obs ); i++ ) {
    weight = obs[ i ]->query_complete_weight();
    if ( !weight ) {
      write( obs[ i ]->the_short() + " doesn't weigh anything.\n" );
      continue;
    }
    if ( weight > 121 ) {
      write( obs[ i ]->the_short() +
        " is heavier than all the weights available put together.\n" );
      continue;
    }
    write( obs[ i ]->the_short() +
      " weighs "+ weight_string( weight ) +".\n" );
  }
  add_succeeded_mess(({ "", "$N $V $I on $D.\n" }), obs);
  return 1;
} /* weigh_something() */


int figure_it_out() {
  int info;
  object person;

  if ( sizeof( doing ) ) {
    person = find_player( doing[ 0 ] );
    if ( person == this_player() ) {
      notify_fail( "You're already engaged in figuring out how the "
        "balance can be used to weigh something.\n" );
      return 0;
    }
    if ( person && ( environment( person ) == environment() ) ) {
      notify_fail( (string)person->one_short() + " is using "
        "the balance at the moment.  Come back when "
        + (string)person->query_pronoun() + " has finished.\n" );
      return 0;
    }
    doing = ({ });
    pans = DEFAULT_PANS;
  }
  info = (int)LIBRARY->query_player_quest_info(
    (string)this_player()->query_name(), "balance" );
  if ( info > 2 ) {
    write( "You already know how the balance works.\n" );
    return 1;
  }
  doing = ({ (string)this_player()->query_name(), ({ "red", "green",
    "blue" })[ info ] });
  write( "You see " + add_a( doing[ 1 ] ) + " weight in the middle pan "
    "that you hadn't noticed before.  Maybe you could use this to "
    "experiment, so you place it in the left pan.\n" );
  say( (string)this_player()->one_short() + " moves "
    + add_a( doing[ 1 ] ) + " weight from the middle pan to the "
    "left pan.\n" );
  pans[ "left" ][ doing[ 1 ] ] = ({ "?", 1 + random( 121 ) });
  recalculate_balance();
  return 1;
} /* figure_it_out() */


int move_a_specific_weight( string weight_size, string pan_to ) {
  object person;
  string pan_from;

  if ( sizeof( doing ) ) {
    person = find_player( doing[ 0 ] );
    if ( !person ) {
      doing = ({ });
    } else {
      if ( person != this_player() ) {
        if ( environment( person ) == environment() ) {
          add_failed_mess( (string)person->one_short() + " is "
            "using the balance at the moment.  Come back when "
            + (string)person->query_pronoun() + " has finished.\n" );
          return 0;
        } else {
          doing = ({ });
          pans = DEFAULT_PANS;
        }
      }
    }
  }

  if ( !pans[ pan_to ] ) {
    add_failed_mess( "There is a left pan, a middle pan and a right pan, "
      "but no " + pan_to + " pan.\n" );
    return 0;
  }
  foreach (pan_from in keys(pans)) {
    if ( pans[ pan_from ][ weight_size ] ) {
      return move_a_weight(weight_size, pan_from, pan_to);
    }
  }
  add_failed_mess("Unable to find the " + weight_size + " weight.\n");
  return 0;
} /* move_a_specific_weight() */


int move_a_weight( string weight_size, string pan_from, string pan_to ) {

  if ( pan_from == pan_to ) {
    add_failed_mess( "The " + weight_size + " weight is already in the "
      + pan_to + " pan.\n" );
    return 0;
  }
  if ( sizeof( doing ) ) {
    if ( weight_size == doing[ 1 ] ) {
      notify_fail( "You don't feel like moving the " + doing[ 1 ]
        + " weight since that's what you're trying to weigh.\n" );
      return 0;
    }
  }
  pans[ pan_to ][ weight_size ] = pans[ pan_from ][ weight_size ];
  pans[ pan_from ] = m_delete( pans[ pan_from ], weight_size );
  write( "You move the "+ weight_size +" weight from the "+ pan_from
    + " pan to the "+ pan_to + " pan.\n" );
  say( (string)this_player()->one_short() + " moves "
    + add_a( weight_size ) +" weight from the "+ pan_from + " pan "
    "of the balance to the " + pan_to + " pan.\n" );
  recalculate_balance();
  if ( sizeof( doing ) && !balance ) {
    call_out( "it_is_balanced", 0, this_player() );
  }
  return 1;
} /* move_a_weight() */


void it_is_balanced( object person ) {
  int info;

  tell_object( person, "You feel a small surge of self-esteem to have found "
    "that the "+ doing[ 1 ] +" weight weighs "
    + weight_string( pans[ "left" ][ doing[ 1 ] ][ 1 ] ) +".\n" );
  info = (int)LIBRARY->query_player_quest_info( (string)person->query_name(),
    "balance" );
  info++;
  switch ( info ) {
    case 1 :
      tell_object( person, "You've made a good start at working out "
        "how the balance operates.  You think you should practice "
        "with it a couple of times more to get the hang of it, "
        "though.\n" );
      person->adjust_xp( 5000 );
      break;
    case 2 :
      tell_object( person, "You're definitely getting to understand how "
        "the balance operates.  You think you should practice with "
        "it once more to get used to it completely, though.\n" );
      person->adjust_xp( 10000 );
      break;
    case 3 :
      tell_object( person, "You're now adept at using the balance and "
        "can use it to weigh anything.\n" );
/* Quest!
 * Name: balance quest
 * Title: Expert Balancer
 * Story: discovered how to weigh things using powers of three
 * Level: 3
 */
      if ( interactive( person ) ) {
        if ( !LIBRARY->query_quest_done( (string)person->query_name(),
          "balance quest" ) ) {
          LIBRARY->set_quest( (string)person->query_name(),
            "balance quest" );
        }
      }
      break;
    case 4:
      info--;
      break; //if people want to keep moving the weights, let them.
    default :
      tell_object( person, "Something has gone wrong with the balance.  "
        "Please contact Wodan about it.\n" );
  }
  LIBRARY->set_player_quest_info( (string)person->query_name(), "balance",
    info );
  tell_object( person, "You put all the weights back into the middle "
    "pan.\n" );
  tell_room( environment(), (string)person->the_short() +
     " seems satisfied with "+ (string)person->query_objective() +"self, "
    "and returns all of the weights to the middle pan.\n", person );
  doing = ({ });
  pans = DEFAULT_PANS;
} /* it_is_balanced() */
