/*
** Dartboard, originally from LustyMUD, donated by Lusty, 
** patched to work under MudOS by Veronica, and changed 
** a bit by Shadowthief.
**
** 
** - I figured above credits were enough, and that they didn't
**   have to aslo coprise the entire long of the object.
** - Made it choose randomly from all creators instead of having 
**   a hard coded list of some.
** - Fixed the messages going to room and the players when a
**   game starts.
** - Commented out the add_command() that adds the syntax to
**   play for bets, as that feature isn't implemented anyway.
** - Cleaned up the code a bit.
**
** - Still needs improvement, though. -- May 2002, Tilly
*/

#include <player_handler.h>

inherit "/std/object";

string  *creators, name1, name2, target;
object  player1, player2;
int     score1, score2, in_use, thrown, bet;


/* Start of function prototypes */
void setup();
void init();
int abandon( string str );
string find_target();
int play( string str );
int throw_dart();
int score_it( int i );
void heart_beat();
void end_game();
/* End of function prototypes */

void setup() {
  set_name("dartboard");
    add_property( "there", "mounted on the wall" );
  add_adjective("party");
  set_main_plural("dartboards");
  set_short("party dartboard");
  set_long("This is a party dartboard.  It's round, with concentric "
    "circles in black and yellow painted on it.\n");
  set_weight(10000);
  set_value(0);
  reset_get();
  set_read_mess( 
    "This is a game of darts for two players. Before beginning "
    "their throws, the players must both type 'play'. Each player "
    "in turn can then throw three darts at the board (by typing "
    "'throw' three times). Note that your accuracy is affected "
    "by your DEX stat. The winner of the game is the player with the "
    "highest score after all darts have been thrown.\n" );
} /* setup() */


void init() {
  add_command("abandon", "", (:this_object()->abandon(""):));
  add_command("play", "", (:this_object()->play(0):));

  /* Commenting this one out since it obviously doesn't work anyway.
  add_command("play", "<word'bet'>", (:this_object()->play($4[0]):));
  */
  add_command("throw", "", (:this_object()->throw_dart():));
} /* init() */


int abandon( string str ) {
  if (this_player() == player1 || this_player() == player2) {
    write("You call off the game of darts.\n");
    say("The game of darts is called off by "
      + this_player()->query_short()+".\n");
    end_game();
    return 1;
  }
} /* abandon() */


string find_target() {
  string *creators, victim;

  creators = get_dir("/w/");
  victim = (creators[(random(sizeof(creators)))]);

  while (!PLAYER_HANDLER->test_user(victim) ) {
    victim = (creators[(random(sizeof(creators)))]);
}

  return capitalize(victim);
} /* find_target() */


int play( string str ) {
  int val;

  if (in_use) {
    write("The dartboard is in use right now.\n");
    return 1;
  }

  if (!str) str = "0";

  if (!sscanf(str, "%d", val) || val < 0) {
    write("Bets must be for zero or greater coins.\n");
    return 1;
  }

  if (this_player()->query_money() < val) {
    write("Betting is not yet supported... I'm working on it! "
   "Just type \"play\".\n");
    return 1;
  }

  if (player1) {
    if (this_player() == player1) {
      write("You can't play against yourself!\n");
      return 1;
    }

    if (val != bet) {
      write("Bet amounts must match. " + name1 + " bet " + bet + ".\n");
      return 1; 
    }

    player2 = this_player();
    name2 = player2->query_short();
    target = find_target();
    tell_room( environment(), name1 + " and " + name2 + 
      " have started a game of darts, throwing at " + target 
      + ".\n", ({ player1, player2 }) );
    tell_object( player2, "You accept " + name1 + "'s challenge, "
      "and you both prepare to throw darts at " + target + ".\n" );
    tell_object( player1, name2 + " accepts your challenge, and you "
      "both prepare to throw darts at " + target + ".\nThrow your darts.\n");

    in_use = 1;
    return 1;
  }
 
  bet = val;

  player1 = this_player();
  name1 = player1->query_short();

  write("You can 'abandon' the game if no one takes up your challenge.\n");
  say( name1 + " is looking for someone else to play darts with.\n");

  set_heart_beat(1);
  return 1;
} /* play() */


int throw_dart() {
  int hit;
  string start;

  if (!in_use)  {
     return notify_fail( "You are not currently playing.\n ");
  }
  if (this_player() != player1 && this_player() != player2)  {
     return notify_fail( "You are not currently playing.\n" );
  }

  if (in_use == 1 && this_player() == player1) {
    thrown += 1;

    write("You throw a dart.\n");
    say( name1 + " throws a dart.\n" );

    hit = score_it(random(21) + ((player1->query_dex() -13 ) / 2 ));

    if (hit == 500 && bet >= 50) {
      write("You feel a bit more experienced.\n");
      player1->ajdust_xp(10);
    }

    score1 += hit;

    if (thrown == 3) {
      write("That was your third dart. Your score is " + score1 + ".\n");
      say(name1 + " has a score of " + score1 + " after three darts.\n");
      if (player2) tell_object(player2, "Your turn to throw.\n");
      in_use = 2;
    }

    return 1;
  }

  if (in_use == 2 && this_player() == player2) {
    thrown += 1;

    write("You throw a dart.\n");
    say(name2+ " throws a dart.\n" );

    hit = score_it(random(21) + ((player2->query_dex() - 13 ) / 2 ));

    if (hit == 500 && bet >= 50) {
      write("You feel a bit more experienced.\n");
      player2->adjust_xp(10);
    }

    score2 += hit;

    if (thrown == 6) {
      write("That was your third dart.\n");

      if (score1 > score2) {
        start = " with a score of " + score1 + " to " +
          score2 + ".\n";
        write(name1 + " beats you" + start);
        tell_room( environment(), name1 + " beats " + name2 + start,
                ({ player1, player2 }));
        tell_object(player1, "You beat " + name2 + start);
      } else if (score2 > score1) {
        start = " with a score of " + score2 + " to " +
          score1 + ".\n";
        write("You beat " + name1 + start);
        tell_room( environment(), name2 + " beats " + name1 + start,
                    ({ player1, player2 }));
        tell_object(player1, name2 + " beats you" + start);
      } else {
        tell_room(environment(this_object()),
          "It's a tie with a score of " + score1 + "!\n");
      }

      end_game();
    }

    return 1;
  }

  write("It's not your turn to throw a dart!\n");
  return 1;
} /* throw_dart() */


int score_it( int i ) {
  string rest_mess;
  int ret;

  if (i < 2) {
    rest_mess = "the wall. No score.\n";
    ret = 0;
  } else if (i < 19) {
    rest_mess = "number "+i+", scoring "+(i*5)+".\n" ;
    ret = i * 5;
  } else if (i == 19) {
    rest_mess =  target+"'s face! Score of 250!\n";
    ret = 250;
  } else {
    rest_mess = target+"'s nose! Score of 500!\n";
    ret = 500;
  }

  write("You hit " + rest_mess);
  say( this_player()->query_short() + " hit " + rest_mess);

  return ret;
} /* score_it() */


void heart_beat() {
  if (!player1) player1 = find_player(lower_case(name1));
  if (name2 && !player2) player2 = find_player(lower_case(name2));

  if (!player1 || environment() != environment(player1))
    say(name1 + " is no longer here. The game of darts is called off.\n");
  else if (name2 && (!player2 || environment() != environment(player2)))
    say(name2 + " is no longer here. The game of darts is called off.\n");
  else
    return;

  end_game();
} /* heart_beat() */


void end_game() {
  set_heart_beat(0);
 
/*  if (player1) player1->add_money(bet);
  if (player2) player2->add_money(bet); */

  in_use  = 0; bet    = 0; thrown  = 0;
  player1 = 0; player2 = 0;
  name1  = 0; name2  = 0;
  score1  = 0; score2  = 0;
} /* end_game() */

