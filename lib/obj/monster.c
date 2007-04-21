/*  -*- LPC -*-  */
/*
 * $Locker: pinkfish $
 * $Id: monster.c,v 1.85 2003/07/28 21:05:41 ceres Exp pinkfish $
 */

/**
 * This file contains all the information relevant to creating an
 * npc (or monster).
 * @author Pinkfish
 */
#include <config.h>
#include <living.h>
#include <map.h>
#include <monster.h>
#include <move_failures.h>
#include <player.h>
#include <skills.h>
#include <wander.h>
#include <route.h>
#include <combat.h>
#define SPELL_INCLUDE_CLASS
#include <top_ten_tables.h>

#define SOUL_OBJECT "/obj/handlers/soul"

inherit "/std/living/mon_actions";
inherit "/std/living/living";
inherit "/std/living/response_mon";
inherit "/global/events";
inherit "/global/guild-race";

nosave string cap_name;
nosave mixed *chat_string;
nosave mixed *achat_string;
private nosave mixed *_combat_actions;
nosave mixed _move_after;
nosave mixed *throw_out;
nosave int chat_chance;
nosave int achat_chance;
nosave int aggressive;
nosave int join_fight_type;
nosave int follow_speed;
nosave int virtual_move;
nosave int moves;
nosave string race;
nosave string guild;
nosave string join_fight_mess;
nosave string true_location;
nosave mixed *enter_commands;
nosave string *move_zones;
private nosave mixed *_queued_commands;
private nosave mixed *doing_story;
nosave object last_attacked;
private nosave string *following_route;
private nosave int added_language;
private nosave function cmd_func = 0;

// This is used to make sure players cannot change an npc's positon with
// souls.
private nosave int cannot_change_position;
private nosave int always_return_to_default_position;

private nosave mapping _spell_actions;

void do_move_after(int running_away);
void do_route_move();
private void _next_queued_command();
void start_attack( object who );

#if !efun_defined(add_action)
protected mixed _process_input(string);
protected mixed command(string);
void command_override(function func);
int drunk_check(string str);
#endif



void create() {
    do_setup++;
    events::create();
    living::create();
    mon_actions::create();
    response_mon::create();
    //p  command::create();
    do_setup--;
    reset_get();
    follow_speed = 3;
    doing_story = ({ ({ }), ({ }) });
    chat_string = ({ 0, ({ }) });
    achat_string = ({ 0, ({ }) });
    move_zones = ({ });
    _spell_actions = ([ ]);
    _combat_actions = ({ });
    enter_commands = ({ });
    known_commands = ({ });
    _queued_commands = ({ });
    following_route = ({ });
    enable_commands();
    living_commands();
    parser_commands();
    communicate_commands();
    command_commands();
    add_property("npc",1);
    set_rows( 24 );
    set_cols( 200 );
    // Default to 5 seconds.
    always_return_to_default_position = 5;
#ifdef OLD_SOUL
    add_action("*", "soul_commqz", -2);
#endif
    /*
      add_action("init_race", "init_race");
      race_guild_commands();
    */
    set_con( 13 );
    set_dex( 13 );
    set_int( 13 );
    set_str( 13 );
    set_wis( 13 );
    set_max_hp( 10000 );
    set_hp( 100000 );
    set_max_gp( 10000 );
    set_gp( 10000 );
    if ( !do_setup )
        this_object()->setup();
    if(clonep())
        call_out(function() {
              if(!environment()) {
                  move(find_object("/room/rubbish"));
              }
          }, 60);
} /* create() */

/**
 * This method sets up a nationality and sets up a region in the
 * nationality for the npc.
 * @param nationality the nationality to set
 * @param region the region in the nationality
 */
void setup_nationality(string nationality, string region) {
   set_nationality(nationality);
   set_nationality_region(region);
   if (!load_object(nationality)) {
      debug_printf("Bad nationality, %O\n", nationality);
   } else {
      if (!nationality->query_region_description(region)) {
         debug_printf("Bad region %O in nationality, %O\n", region,
                      nationality);
      }
      add_language(nationality->query_language());
      set_language(nationality->query_language());
      set_default_language(nationality->query_language());
   }
}

/** @ignore yes */
void dest_me() {
    living::dest_me();
} /* dest_me() */

/** @ignore yes */
int soul_commqz(string str) {
    string verb, bit;

    if (sscanf(str, "%s %s", verb, bit) == 2)
        return (int)SOUL_OBJECT->soul_command(verb, bit);
    return (int)SOUL_OBJECT->soul_command(str, "");
} /* soul_commqz() */

/** @ignore yes */
int query_sp() { return 50; }
/** @ignore yes */
int adjust_sp( int number ) { return 50; }

/**
 * This method returns the current capitalized name of the npc.
 * @return the current capitalized name
 * @see set_cap_name()
 */
string query_cap_name() {
    return cap_name;
} /* query_cap_name() */

/**
 * This method set the current capitalized name of the npc.
 * @param s the capitalized name of the npc
 * @see query_cap_name()
 */
void set_cap_name(string s) { cap_name = s; }

/** @ignore yes */
int soul_com_force(string str) {
    //  string str1,str2;

    if (file_name(previous_object()) != SOUL_OBJECT)
        return 0;
    command(str);
    return 1;
} /* soul_com_force() */

/** @ignore yes */
void set_name(string n) {
    if (query_name() && query_name() != "object")
        return;
    ::set_name(n);
    cap_name = capitalize(query_name());
    add_plural(pluralize(query_name()));
    set_short( query_name() );
    set_long("You see nothing special.\n");
    set_living_name(n);
} /* set_name() */

/** @ignore yes */
string long(string str, int dark) {
    string s;

    if (dark < -1) {
      s = "You can only make out a rough shape in the glare.\n";
    } else if (dark > 1) {
      s = "You can only make out a rough shape in the gloom.\n";
    } else {
       s = query_long();
    }
    if(!dark) {
        s += capitalize(query_pronoun())+" "+health_string()+".\n";
        s += capitalize(query_pronoun()) + " is " +
            this_object()->query_position_short() + ".\n";
        s += calc_extra_look();
        s += query_living_contents(0);
    }
    return s;
} /* long() */

/**
 * This function is deprecated.  Use basic_setup() instead.
 *
 * This method sets the race of the npc.  The race should be one of
 * the races listed in the /std/race.c object.
 * This is used in conjuction with the
 * guild when set_level is called to setup the default
 * attributes for the npc.  This should only be
 * called *before* set_level() is called.
 * @param str the race to set
 * @return always returns 1
 * @example
 * ob = clone_object("/obj/monster");
 * ob->set_race("fish");
 * @example
 * inherit "/obj/monster";
 *
 * void setup() {
 *    ...
 *    set_race("fish");
 *    ...
 *    set_level(12);
 * } /\* setup() *\/
 * @see query_race()
 * @see query_guild()
 * @see set_guild()
 * @see set_level()
 * @see basic_setup()
 */
int set_race(string str) {
    race = str;
    return 1;
} /* set_race() */

/**
 * This method returns the current race of the npc.
 * @return the current race of the object
 * @see set_race()
 * @see query_guild()
 * @see set_level()
 */
string query_race() { return race; }
/**
 * This method returns the current guild of the npc.
 * @return this current guild of the npc
 * @see query_race()
 * @see set_guild()
 * @see set_level()
 */
string query_guild() { return guild; }
/**
 * @ignore yes
 * This method should not be used.  It is 'depreciated'.
 */
string query_class() { return guild; }
/**
 * @ignore yes
 * This method should not be used.  It is 'depreciated'.
 */
string query_profession() { return guild; }

/**
 * @ignore yes
 * This method should not be used.  It is 'depreciated'.
 */
int set_class(string str) {
    guild = str;
} /* set_guild() */

/**
 * This method is deprecated.  Use basic_setup() instead.
 *
 * This method sets the current guild of the npc to the
 * passed in value.  The guild should be one of
 * the guilds listed in the /std/race.c object.
 * This is used in conjuction with the
 * race when set_level is called to setup the default
 * attributes for the npc.  This should only be
 * called *before* set_level() is called.
 * @param str the new guild for the npc
 * @example
 * ob = clone_object("/obj/monster");
 * ob->set_guild("fighter");
 * @example
 * inherit "/obj/monster";
 *
 * void setup() {
 *    ...
 *    set_race("fighter");
 *    ...
 *    set_level(12);
 * } /\* setup() *\/
 * @see query_race()
 * @see query_guild()
 * @see set_guild()
 * @see set_level()
 */
int set_guild(string str) {
    guild = str;
} /* set_guild() */

/**
 * @ignore yes
 * This method should not be used.  It is 'depreciated'.
 */
int set_profession(string str) {
    guild = str;
} /* set_profession() */

/**
 * This method makes the npc initialise all their equipment, like hold
 * it and stuff.
 * @see do_command()
 */
void init_equip() { command("equip"); }

/**
 * This method allows you to control the npc and get it to do
 * actions.  This can be used for npc control and inteligence.
 *
 * Be very careful with this command! This does not go through any
 * command queue like players have and so NPCs can end up doing
 * tons of commands in very short order.
 *
 * If you're trying to make your NPC act like a player use
 * eue_command() instead
 *
 * @param words the action to preform
 * @see init_equip()
 * @see init_command()
 * @example
 * ob = clone_object(NICE_HAIRY_APE);
 * ob->do_command("'I am a hairy ape!");
 * ob->do_command("emote apes around the room.");
 */
int do_command( string words ) {
    if ( this_object()->query_property( PASSED_OUT_PROP ) ) {
        return -1;
    }
    if ( stringp( words ) ) {
        return command( words );
    }
    printf( "Invalid parameter to do_command: %O for monster %O in %O.\n",
      words, this_object(), environment() );
    return -1;
} /* do_command() */

/**
 * This method throws away any queued commands.
 * It doesn't remove the call_out however if no
 * new commands are added there will be no effect.
 * @see init_equip()
 * @see init_command()
 * @see delay_command()
 * @see do_command()
 * @see queue_command()
 */
void delete_queued_commands() {
  _queued_commands = ({ });
}

/**
 * This method returns the queued command list.
 * @see init_equip()
 * @see init_command()
 * @see delay_command()
 * @see do_command()
 * @see queue_command()
 */
mixed *query_queued_commands() {
  return _queued_commands;
}

/** @ignore yes */
private void _next_queued_command() {
  mixed next;

  if (!sizeof(_queued_commands)) return;

  // No actions while casting.
  if(this_object()->query_casting_spell()){
    call_out( (: _next_queued_command :), 2 );
    return;
  }
  
  next = _queued_commands[0];
  if (intp( next )) {
    _queued_commands = _queued_commands[1..];
    if (!sizeof(_queued_commands)) return;
    next = _queued_commands[0];
  }
  while (stringp( next )) {
    if (this_object()->queue_commands()) {
      call_out( (: _next_queued_command :), 2 );
      return;
    }
    do_command( next );
    _queued_commands = _queued_commands[1..];
    if (!sizeof(_queued_commands)) return;
    next = _queued_commands[0];
  }
  call_out( (: _next_queued_command :), next );
} /* _next_queued_command */

/**
 * This method allows you to control the npc and get it to do
 * actions where they are queued as for players.   The command
 * is always delayed by delay even if there are no commands pending
 * unlike queue_command(). This function
 * is 100% compatible with queue_command() and init_command().
 * @param words the action to perform
 * @param interval to wait before the command.
 * @see queue_command()
 * @see query_queued_commands()
 * @see init_command()
 * @see do_command()
 * @example
 * ob = clone_object(NICE_HAIRY_APE);
 * ob->delay_command("'I am a hairy ape!",10);
 * ob->delay_command("emote apes around the room.",2);
 * ob->queue_command("emote get banana.",3);
 * ob->queue_command("emote get apple.");
 * After 10 seconds it says "I am a hariy ape",
 * 2 seconds after that it apes around the room,
 * immediately following that it gets a banana
 * and 3 seconds after that it gets an apple.
 */
int delay_command( string words, int interval ) {
    if ( this_object()->query_property( PASSED_OUT_PROP ) ) {
        return -1;
    }
    if ( stringp( words ) ) {
      if (!sizeof(_queued_commands)) {
        call_out( (: _next_queued_command :), interval );
      }
      _queued_commands = _queued_commands + ({ interval, words });
      return 1;
    }
    printf( "Invalid parameter to delay_command: %O for monster %O in %O.\n",
      words, this_object(), environment() );
    return -1;
} /* delay_command() */

/**
 * This method allows you to control the npc and get it to do
 * actions where they are queued as for players.  If there are no
 * commands pending the command is executed immediately.  This function
 * is 100% compatible with delay_command() and init_command().
 * @param words the action to perform
 * @param interval to wait before processing another command.
 * If omitted defaults to 2 seconds as per players
 * @see delay_command()
 * @see query_queued_commands()
 * @see init_command()
 * @see do_command()
 * @example
 * ob = clone_object(NICE_HAIRY_APE);
 * ob->queue_command("'I am a hairy ape!");
 * ob->queue_command("emote apes around the room.",5);
 * ob->queue_command("get banana",10);
 * ob->delay_command("emote get apple.",3);
 * Right away it says "I am a hairy ape",
 * 2 seconds later it apes around the room,
 * 5 seconds after that it gets a banana
 * and 13 seconds (10+3) after that it gets an apple.
 */
varargs int queue_command( string words, int interval ) {
    if ( this_object()->query_property( PASSED_OUT_PROP ) ) {
        return -1;
    }
    if ( stringp( words ) ) {
      if (undefinedp(interval)) interval=2;
      if (!sizeof(_queued_commands)) {
        _queued_commands = ({ words, interval });
        _next_queued_command();
        return 1;
      }
      _queued_commands = _queued_commands + ({ words, interval });
      return 1;
    }
    printf( "Invalid parameter to queue_command: %O for monster %O in %O.\n",
      words, this_object(), environment() );
    return -1;
} /* queue_command() */

/**
 * This method allows you submit delayed commands to the npc
 * via a call_out.
 * @see do_command()
 * @see queue_command()
 * @see delay_command()
 */
varargs void init_command(string str, int tim) {
    call_out("do_command", tim, str);
} /* init_command() */

/** @ignore yes */
void init() {
  set_heart_beat( 1 );
  if (environment() && aggressive && this_player() &&
      file_name( environment() )[1..4] != "room" &&
      !environment()->no_attack() &&
      this_player()->query_visible(this_object()))
    start_attack(this_player());
}

/**
 * This method check to see if the npc should start attacking someone
 * when they enter the npcs environment.  It is called from inside
 * init().  It will only attack if the agressive is set and the
 * person is visible to be attacked.  The property
 * <pre>"no attack"</pre> can be set on the npc (or player) to
 * stop them being attacked.
 * @param who the person to potentially start attacking
 * @see set_aggressive()
 */
void start_attack( object who ) {
  if(!who || !aggressive ) {
    return;
  }

  if ( !who->query_visible( this_object() ) ||
       who->query_auto_loading() ||
       file_name(who) == DEATH ||
       who->query_property( "guest" ) ||
       ( userp( who ) && !interactive( who ) ) ||
       who->query_property( "no attack" ) ) {
    return;
  }

  /*
   * This uses a call_other() just in case there are shadows.
   */
  if(((aggressive > 1) || interactive(who)) &&
     (!interactive(who) || !who->query_auto_loading()))
    this_object()->attack_ob( who );
}

/** @ignore yes */
varargs int adjust_hp( int number, object attacker, object weapon,
                         string attack ) {
    set_heart_beat( 1 );
    return ::adjust_hp( number, attacker, weapon, attack );
} /* adjust_hp() */

/** @ignore yes */
int adjust_gp( int number ) {
    set_heart_beat( 1 );
    return ::adjust_gp( number );
} /* adjust_gp() */

/**
 * This method checks to see if there are any players in the environment
 * of the npc.  This should be used to determine when chats should
 * be turned off and other things which should only work in the
 * presence of players.
 * @return 1 if there is a player in the room, 0 otherwise
 */
int check_anyone_here() {
    object thing;

    if ( !environment() ) {
        return 0;
    }
    if ( file_name( environment() )[1..4] == "room" ) {
        return 0;
    }
    if ( environment()->query_linked() ) {
        return 1;
    }
    foreach( thing in all_inventory( environment() ) ) {
        if ( interactive( thing ) || thing->query_slave() ) {
            return 1;
        }
    }
    return 0;
} /* check_anyone_here() */

/**
 * This method is used to determine when to throw people out of a
 * room.  This is what detritus uses to throw people out of the
 * mended drum when it gets a bit rowdy.
 * <p>
 * The hps is the level of hps at which the npc will start throwing
 * people out with the chance of it occuring (chance is a percentage).
 * <p>
 * People will be thrown into a random room, if the property
 * <pre>"no throw out"</pre> is specified on the room then they
 * will not be thrown into that room.
 * @param hps the number of hps at which to start throwing people out
 * @param chance the percentage chance of being thrown out
 * @param their_mess the message to show them
 * @param everyone_mess the message to show everyone else
 * @see set_aggressive()
 * @see query_throw_out()
 * @see set_join_fights()
 * @see expand_string()
 */
void set_throw_out( int hps, int chance, string their_mess,
  string everyone_mess ) {
    throw_out = ({ hps, chance, their_mess, everyone_mess });
} /* set_throw_out() */

/**
 * This method returns the current throw out array.
 * The array consists of
 * <pre>({
 *   hps,
 *   chance,
 *   their_mess,
 *   everyone_mess
 * })
 * </pre>The parameters are the same as used in the set_throw_out
 * function.
 * @see set_throw_out()
 * @return the throw out array
 */
mixed *query_throw_out() { return throw_out; }

/**
 * This method is used to make the npc run away.  This will be
 * called by the combat code for wimpy when the npc is bellow the
 * number of points used to trigger the wimpy action.
 * @return 1 if successfuly ran away
 */
int run_away() {
    if ( query_property( "run away" ) == -1 ) {
        return 0;
    }
    // Just to make npcs a bit trickier...
    do_command("lose all");
    become_flummoxed();
    if ( sizeof( following_route ) ) {
        do_route_move();
        return 1;
    }
    if ( query_property( "run away" ) ) {
        this_object()->do_move_after( 1 );
        return 1;
    }
    return ::run_away();
} /* run_away() */

/**
 * This method is used to expand the message strings used in the
 * npc messages.  It is used for chat strings and such things like
 * that.  The strings it expands are of the form:<br>
 * $lname$, $mname$, $aname$, ...<br>
 * The first letter determines the type of object being referenced.
 * They are:
 * <dl>
 * <dt>m
 * <dd>Me!  The npc itself.
 * <dt>l
 * <dt>A living object, choose a random living object in the npcs
 *     environment.
 * <dt>a
 * <dd> Chooses a random attacker from those attacking the npc.
 * <dt>o
 * <dd>Choose a random object in the inventory of the npc.
 * </dl>
 * After the first letter is a type of information being request.
 * <dl>
 * <dt>name
 * <dd>The name of the selected object.
 * <dt>cname
 * <dd>The capitalised name of the selected object.
 * <dt>gender
 * <dd>The gender string of the selected object (male, female, neuter).
 * <dt>poss
 * <dd>The possessive string of the selected object.
 * <dt>obj
 * <dd>The objective string of the selected object.
 * <dt>pronoun
 * <dd>The pronoun string of the selected object.
 * <dt>gtitle
 * <dd>The guild title of the selected object (only useful on livings).
 * <dt>ashort
 * <dd>The a_short() call.
 * <dt>possshort
 * <dd>The poss_short() call.
 * <dt>theshort
 * <dd>The the_short() call.
 * <dt>oneshort
 * <dd>The one_short() call.
 * </dl>
 * @see set_chat_string()
 * @see expand_mon_string()
 * @param in_str the input string
 * @param on the object to use for the 'o' matching
 */
string expand_string(string in_str, object on) {
    string *str, ret;
    int i, add_dollar;
    object liv, *obs, ob;

    in_str = "/global/events"->convert_message( in_str );
    str = explode(in_str, "$");
    ret = "";
    for ( i = 0; i < sizeof( str ); i++ ) {
        if ( i % 2 == 0 ) {
            if (add_dollar) {
                ret += "$";
            }
            ret += str[i];
            add_dollar = 1;
            ob = 0;
        } else switch (str[i][0]) {
        case 'm' :
            ob = this_object();
        case 'l' :
          if(!ob) {
            if(!liv) { /* Changed to not loop badly up when no environment */
              obs = all_inventory(environment()) - ({ this_object() });
              obs = filter_array(obs, (: living($1) &&
                                       $1->query_visible(this_object()) :));
              if (sizeof(obs))
                liv = obs[random(sizeof(obs))];
            }
            if (!liv) { /* No one here to see us blow up anyway ;) */
              break;
            }
            ob = liv;
          }
        case 'a' :
            if ( !ob ) {
                obs = (object *)this_object()->query_attacker_list();
                if ( !sizeof( obs ) )
                    break;
                ob = obs[ random( sizeof( obs ) ) ];
            }
        case 'o' :
            if (!ob) {
                if (!on) {
                    obs = all_inventory(environment());
                    obs = filter_array( obs, (: !living( $1 ) :) );
                    if (sizeof(obs)) {
                        on = obs[random(sizeof(obs))];
                    }
                }
                ob = on;
            }
            switch (str[ i ][ 1 .. ]) {
            case "theshort" :
                ret += (string)ob->the_short();
                add_dollar = 0;
                break;
            case "ashort" :
                ret += (string)ob->a_short();
                add_dollar = 0;
                break;
            case "oneshort":
                ret += (string)ob->one_short();
                add_dollar = 0;
                break;
            case "possshort" :
                ret += (string)ob->poss_short();
                add_dollar = 0;
                break;
            case "name" :
                ret += (string)ob->query_name();
                add_dollar = 0;
                break;
            case "cname" :
                ret += (string)ob->query_cap_name();
                add_dollar = 0;
                break;
            case "gender" :
                ret += (string)ob->query_gender_string();
                add_dollar = 0;
                break;
            case "poss" :
                ret += (string)ob->query_possessive();
                add_dollar = 0;
                break;
            case "obj" :
                ret += (string)ob->query_objective();
                add_dollar = 0;
                break;
            case "gtitle" :
                ret += (string)ob->query_gender_title();
                add_dollar = 0;
                break;
            case "pronoun" :
                ret += (string)ob->query_pronoun();
                add_dollar = 0;
                break;
            default :
                if (add_dollar) {
                    ret += "$";
                }
                ret += str[i];
                add_dollar = 1;
                break;
            }
            ob = 0;
            break;
        default :
            if (add_dollar) {
                ret += "$";
            }
            ret += str[i];
            add_dollar = 1;
            ob = 0;
            break;
        }
    }
    if (strlen(ret) && ret[strlen(ret)-1] == '$') {
        return ret[0..strlen(ret)-2];
    }
    return ret;
} /* expand_string() */

/**
 * This method executes the string passed in.  It handles all the
 * stuff which is needed from the chat_string stuff.
 *
 * If the input is a function pointer then it is evaluated with one
 * parameter, being the npc.
 *
 * If the input is a string then the first letter determines what will
 * be done with it.  All these are passed through expand_string
 * so that exciting things can be done.
 * <ul>
 * <li># - A call_other will be generated.  The parameters are separated by
 * ':'s, so "#frog:bing:fred:chicken" would call<pre>
 * this_object()->bing("fred", "chicken");
 * </pre>.
 * <ul>', ", : - These will generate a 'say', 'lsay' or 'emote'.
 * <ul>@ - This will run the passed in command.  Eg: "@frog" would cause the
 *     soul command frog to be used.
 * <ul>Anything else will be used as a message to be sent to everyone in the
 *     room.
 * </ul>
 * @param str the thing to execute
 * @see expand_string()
 */
void expand_mon_string( mixed str ) {
    string *args;

    if ( functionp( str ) ) {
        evaluate( str, this_object() );
    } else {
        if( !stringp( str ) && environment( this_object() ) ) {
            tell_room(environment(this_object()),
              "%^RED%^"+ this_object()->the_short()+
              " says: please bugreport me, I have a bad load_chat.%^RESET%^\n",
              ({ }) );
        }
        switch ( str[ 0 ] ) {
        case '#' :
            args = explode(str[ 1..], ":");
            switch (sizeof(args)) {
            case 1 :
                call_other( this_object(), args[0] );
                break;
            case 2 :
                call_other( this_object(), args[0], args[1] );
                break;
            case 3 :
                call_other( this_object(), args[0], args[1], args[2] );
                break;
            case 4 :
                call_other( this_object(), args[0], args[1], args[2],
                  args[3] );
                break;
            default :
                call_other( this_object(), args[0], args[1], args[2],
                  args[3], args[4] );
                break;
            }
            break;
        case ':' :
        case '\'' :
        case '"' :
            init_command( expand_string( str, 0 ), 1 );
            break;
        case '@' :
            init_command( expand_string( str[ 1 .. ], 0 ), 1 );
            break;
        default :
            tell_room( environment(), expand_string( str, 0 ) +"\n" );
        }
    }
} /* expand_mon_string() */

/**
 * This method returns 1 if it is ok to turn of the npc's heart beat.
 * THis can be overridden for times when the heart beat needs to be
 * kept on for some reason.
 * @return 1 if the heart beat should go off, 0 if it should stay on
 */
int query_ok_turn_off_heart_beat() {
   return 1;
} /* query_ok_turn_off_heart_beat() */

/** @ignore yes */
void heart_beat() {
  int i, j;
  
  if(base_name(environment()) == "/room/rubbish") {
    set_heart_beat(0);
    return;
  }
  
  ::heart_beat();
  RACE_OB->monster_heart_beat( race, guild, race_ob, guild_ob );
  
  if ( ( hp == max_hp ) && ( gp == max_gp ) ) {
    if (query_ok_turn_off_heart_beat()) {
      if ( !check_anyone_here() ||
           ( !sizeof( achat_string ) && !sizeof( chat_string ) ) ) {
        set_heart_beat( 0 );
        return;
      }
    }
  }
  
  /* These may be obsolete. */
  remove_property( "done follow" );
  this_object()->do_spell_effects( 0 );
  
  if ( check_anyone_here() ) {
    if ( this_object()->query_fighting() ) {
      if ( sizeof( doing_story[ 1 ] ) ) {
        if( !intp(doing_story[ 1 ][ 0 ] ) ) {
          expand_mon_string( doing_story[ 1 ][ 0 ] );
          doing_story[ 1 ] = doing_story[ 1 ][ 1 .. ];
        } else if( random( 1000 ) < doing_story[ 1 ] [ 0 ]) {
          expand_mon_string( doing_story[ 1 ][ 1 ] );
          if ( sizeof( doing_story[ 1 ] ) == 2 ) {
            doing_story[ 1 ] = ({});
          } else {
            doing_story[ 1 ] = ({ doing_story[ 1 ] [ 0 ] }) +
              doing_story[ 1 ][ 2 .. ];
          }
        }
      } else if ( ( random( 1000 ) < achat_chance )
                  && sizeof( achat_string[ 1 ] ) ) {
        i = random( achat_string[ 0 ] + 1 );
        while ( ( i -= achat_string[ 1 ][ j ] ) > 0 )
          j += 2;
        if ( pointerp( achat_string[ 1 ][ j + 1 ] ) ) {
          if( intp(achat_string[ 1 ][ j + 1 ][ 0 ]) ) {
            if( random(1000) < achat_string[ 1 ][ j + 1 ][ 0 ] ) {
              expand_mon_string( achat_string[ 1 ][ j + 1 ][ 1 ] );
              doing_story[ 1 ] = ({ achat_string[ 1 ][ j + 1 ][ 0 ] })+
                achat_string[ 1 ][ j + 1 ][ 2 .. ];
            } else {
              doing_story[ 1 ] = achat_string[ 1 ][ j + 1 ];
            }
          } else {
            expand_mon_string( achat_string[ 1 ][ j + 1 ][ 0 ] );
            doing_story[ 1 ] = achat_string[ 1 ][ j + 1 ][ 1 .. ];
          }
        } else
          expand_mon_string( achat_string[ 1 ][ j + 1 ] );
      }
    } else {
      if ( sizeof( doing_story[ 0 ] ) ) {
        if( !intp( doing_story[ 0 ][ 0 ] ) ) {
          expand_mon_string( doing_story[ 0 ][ 0 ] );
          doing_story[ 0 ] = doing_story[ 0 ][ 1 .. ];
        } else if( random( 1000 ) < doing_story[ 0 ][ 0 ]) {
          expand_mon_string( doing_story[ 0 ][ 1 ] );
          if ( sizeof( doing_story[ 0 ] ) == 2 ) {
            doing_story[ 0 ] = ({});
          } else {
            doing_story[ 0 ] = ({ doing_story[ 0 ] [ 0 ] }) +
              doing_story[ 0 ][ 2 .. ];
          }
        }
      } else if ( ( random( 1000 ) < chat_chance )
                  && sizeof( chat_string[ 1 ] ) ) {
        i = random( chat_string[ 0 ] + 1 );
        while ( ( i -= chat_string[ 1 ][ j ] ) > 0 )
          j += 2;
        if ( pointerp( chat_string[ 1 ][ j + 1 ] ) ) {
          if( intp( chat_string[ 1 ][ j + 1 ][ 0 ]) ) {
            if(random( 1000) < chat_string[ 1 ][ j + 1 ][ 0 ]) {
              expand_mon_string( chat_string[ 1 ][ j + 1 ][ 1 ] );
              doing_story[ 0 ] = ({ chat_string[ 1 ][ j + 1 ][ 0 ] }) +
                chat_string[ 1 ][ j + 1 ][ 2 .. ];
            } else {
              doing_story[ 0 ] = chat_string[ 1 ][ j + 1 ];
            }
          } else {
            expand_mon_string( chat_string[ 1 ][ j + 1 ][ 0 ] );
            doing_story[ 0 ] = chat_string[ 1 ][ j + 1 ][ 1 .. ];
          }
        } else
          expand_mon_string( chat_string[ 1 ][ j + 1 ] );
      }
    }
  }
} /* heart_beat() */

/** @ignore yes */
int clean_up( int parent ) {
  if(query_property("unique") || check_anyone_here())
    return 1;

  if(parent) {
    log_file("CLEANUP", "%s %s cleaned up.\n", ctime(time()),
             file_name(this_object()));
  }
  move( "/room/rubbish" );
  return 1;
}

/**
 * This method generates a random number.  It used to setup the random
 * stats.
 * @param no the number of times to roll the dice
 * @param type the size of the dice
 * @see set_random_stats()
 * @return the randomly generate number
 */
int rand_num(int no, int type) {
    int i, val;

    for (i=0;i<no;i++)
        val = random(type)+1;
    return val;
} /* rand_num() */

/**
 * This method sets the stats for the npc to some exciting random
 * values.
 * @param no the number of times to roll the dice
 * @param type the size of the dice
 * @see rand_num()
 */
void set_random_stats(int no, int type) {
    set_str(rand_num(no, type));
    set_dex(rand_num(no, type));
    set_int(rand_num(no, type));
    set_con(rand_num(no, type));
    set_wis(rand_num(no, type));
} /* set_random_stats() */

/**
 * This method sets up the basic abilities and race of the critter.  It
 * is equivalent to calling set_race(), set_guild(), and then set_level()
 * with the same parameters.  But those latter three functions are
 * deprecated and shouldn't be used.
 * @see set_race()
 * @see set_guild()
 * @see set_level()
 * @param race this is the race of the character.  It should be a
 * race that's understood by the /std/race.c
 * @param guild this is the guild, class, or profession of the NPC.
 * @param level this is the base skill level of the NPC.  The
 * number is used by the race object to set ability scores, and
 * base skills.
 */
void basic_setup(string race, string guild, int level)
{
    this_object()->set_race(race);
    this_object()->set_guild(guild);
    this_object()->set_level(level);
}

/**
 * This function is deprecated.  Use basic_setup() instead.
 *
 * This method sets the level of the npc.  This should only be called
 * *after* the race and guild are set.  If this is called before that
 * the results will be unexpected.
 * @see set_race()
 * @see set_guild()
 * @see basic_setup()
 * @param i the level to set the npc to
 */
void set_level( int i ) { RACE_OB->set_level( i, race, guild ); }

/**
 * This method is used to give some startup money to the npc.
 * @param base the base amount of money to give (fixed)
 * @param rand the random amount of money to give
 * @param type the type of money to give (default: "copper")
 * @example
 * ob = clone_object(CLUCKY_CHICKEN);
 * // This will give the chicken 10 + random(10) royals.
 * // It is a royal chicken
 * ob->give_money(10, 10, "royal");
 * @return the return value of adjust_money()
 * @see /std/living/money.c
 */
int give_money(int base, int rand, string type) {
    if (!type)
        type = "copper";
    return adjust_money(base+random(rand), type);
} /* give_money() */

/**
 * This method loads up the chat strings for the npc.  This will be
 * an array containing pairs of elements, the first pair is the
 * weighting of the chat and the second is the chat to use.
 * <p>
 * All the weights in the array are added up and then a random
 * number is chosen in the weighting.  Then that element is looked
 * up in the array.  This way you can control a chat and make it
 * rare.
 * <p>
 * If the chat string is an array then this a story, the story will be
 * executed one after another and no other chats will be executed
 * in between.  If the first parameter of the story array is a number it
 * will be used as a 1/1000 chance of the next story line being displayed. 
 * Special strings can be used which will replace with
 * object names, see expand_mon_string() for further information.
 * <p>
 * The chat chance is a chance (in 1000) of the chat occuring.  You
 * will need to play with this yourself to see which frequency of
 * chatting you wish for your npcs.
 *
 * @example
 * load_chat(60,
 *    ({
 *       1, "'I am a chicken!",
 *       // Make this one more likely to occur.
 *       2, ":clucks like a chicken."
 *       1, ":pecks at $lpossshort$ foot."
 *     }));
 * @example
 * load_chat(100,
 *    ({
 *       1, "'I am a simple farmer."
 *       1, ":waves $mposs$ pitchfork around."
 *       // A story, they will always occur in this order
 *       1, ({
 *           500,
 *            "'Once upon a time there was a rabbit.",
 *            "'It was a nice rabbit and hung around in bars.",
 *            "'It sung little songs about fruit.",
 *           }),
 *      }));
 * @see expand_mon_string()
 * @param chance the chance in 1000 of a chat working every 2 seconds
 * @param c_s the chat string to use
 * @see load_a_chat()
 * @see query_chat_chance()
 * @see query_chat_string()
 */
void load_chat(int chance, mixed *c_s) {
    int i;
    chat_string = ({ 0, ({ }) });
    for (i=0;i<sizeof(c_s);i+=2) {
        chat_string[1] += ({ c_s[i], c_s[i+1] });
        chat_string[0] += c_s[i];
    }
    chat_chance = chance;
} /* load_chat() */

/**
 * This method sets the current chat chance for messages on the
 * npc.
 * @param i the chat chance
 * @see load_chat()
 * @see query_chat_chance()
 */
void set_chat_chance(int i) { chat_chance = i; }
/**
 * This method returns the current chat chance for messages on
 * the npc
 * @return the current chat chance
 * @see set_chat_chance()
 * @see load_chat()
 */
int query_chat_chance() { return chat_chance; }
/**
 * This method sets the current chat string for messages on the
 * npc.  See load_chat() for a longer description of how the
 * chat string is formatted.
 * @param chat the new chat strings.
 * @see load_chat()
 * @see query_chat_string()
 */
void set_chat_string(string *chat) { chat_string = chat; }
/**
 * This method queries the current chat string for messages on the
 * npc.  See load_chat() for a longer description of how the
 * chat string is formatted.
 * @return the current chat string
 * @see load_chat()
 * @see query_chat_string()
 */
string *query_chat_string() { return chat_string; }

/**
 * This method adds a single chat string into the current list of
 * chat strings.  See load_chat() for a longer description of
 * the chat string.
 * @param weight the weight of the chat
 * @param chat the new chat string
 * @see load_chat()
 * @see remove_chat_string()
 */
void add_chat_string(mixed weight, mixed chat) {
    int i;

    if (pointerp(weight)) {
        for (i=0;i<sizeof(weight);i+=2) {
            add_chat_string(weight[i], weight[i+1]);
        }
    } else {
        if (member_array(chat, chat_string[1]) == -1) {
            chat_string[1] += ({ weight, chat });
            chat_string[0] += weight;
        }
    }
} /* add_chat_string() */

/**
 * This method attempts to remove the given chat string from the
 * current list of chat strings.  The chat message is checked
 * to see if it exists in the array, the weighting of the
 * string is ignored.
 * @param chat the chat string to remove
 * @see add_chat_string()
 * @see load_chat()
 */
void remove_chat_string(mixed chat) {
    int i;

    if (pointerp(chat)) {
        for (i=0;i<sizeof(chat);i++) {
            remove_chat_string(chat[i]);
        }
    } else {
        if ((i = member_array(chat, chat_string[1])) != -1) {
            chat_string[0] -= chat_string[1][i-1];
            chat_string[1] = delete(chat_string[1], i-1, 2);
        }
    }
}  /* remove_chat_string() */

/**
 * This method loads up the set of chat strings to use while in combat.
 * @param chance the chance of the chat occuring
 * @param c_s the chat string to use
 * @see load_chat()
 * @see query_achat_chance()
 * @see query_achat_string()
 */
void load_a_chat(int chance, mixed *c_s) {
    int i;

    achat_string = ({ 0, ({ }) });
    for (i=0;i<sizeof(c_s);i+=2) {
        achat_string[1] += ({ c_s[i], c_s[i+1] });
        achat_string[0] += c_s[i];
    }
    achat_chance = chance;
} /* load_a_chat() */

/**
 * This method sets the current chat chance for attack messages on the
 * npc.
 * @param i the attack message chat chance
 * @see load_chat()
 * @see load_achat()
 * @see query_achat_chance()
 */
void set_achat_chance(int i) { achat_chance = i; }
/**
 * This method returns the current chat chance for attack messages on
 * the npc.
 * @return the current attack message chat chance
 * @see set_achat_chance()
 * @see load_chat()
 * @see load_achat()
 */
int query_achat_chance() { return achat_chance; }
/**
 * This method sets the current chat string for attack messages on the
 * npc.  See load_chat() for a longer description of how the
 * chat string is formatted.
 * @param chat the new chat attack message strings.
 * @see load_chat()
 * @see load_achat()
 * @see query_achat_string()
 */
void set_achat_string(string *chat) { achat_string = chat; }
/**
 * This method queries the current chat string for attack messages on the
 * npc.  See load_chat() for a longer description of how the
 * chat string is formatted.
 * @return the current attack message chat string
 * @see load_chat()
 * @see load_achat()
 * @see set_chat_string()
 */
string *query_achat_string() { return achat_string; }
/**
 * This method adds a single chat string into the current list of
 * attack message chat strings.  See load_chat() for a longer description of
 * the chat string.
 * @param weight the weight of the chat
 * @param chat the new chat string
 * @see load_chat()
 * @see load_achat()
 * @see remove_achat_string()
 */
void add_achat_string(mixed weight, mixed chat) {
    int i;

    if (pointerp(weight))
        for (i=0;i<sizeof(weight);i+=2)
            add_achat_string(weight[i], weight[i+1]);
    else
    if (member_array(chat, achat_string[1]) == -1) {
        achat_string[1] += ({ chat });
        achat_string[0] += weight;
    }
} /* add_achat_string() */

/**
 * This method attempts to remove the given chat string from the
 * current list of attack message chat strings.  The chat message is checked
 * to see if it exists in the array, the weighting of the
 * string is ignored.
 * @param chat the chat string to remove
 * @see add_achat_string()
 * @see load_chat()
 * @see load_achat()
 */
void remove_achat_string(mixed chat) {
    int i;

    if (pointerp(chat))
        for (i=0;i<sizeof(chat);i++)
            remove_achat_string(chat[i]);
    else
    if ((i = member_array(chat, achat_string[1])) != -1) {
        achat_string[0] -= achat_string[1][i-1];
        achat_string[1] = delete(achat_string[1], i-1, 1);
    }
} /* remove_achat_string() */

/**
 * This method adds a move zone onto the npc.  The move zones control
 * which areas the npcs will wander into, a move zone is set on the
 * room and the npcs will only enter rooms which have a matching
 * move zone.  If there is no move zone, then the npc will enter
 * any room.
 * <p>
 * If the parameter is an array each of the elements of the array
 * will be added as a move zone.
 * @param zone the zone(s) to add
 * @see remove_move_zone()
 * @see query_move_zones()
 * @see set_move_after()
 */
void add_move_zone(mixed zone) {
    int i;

    if (pointerp(zone))
        for (i=0;i<sizeof(zone);i++)
            add_move_zone(zone[i]);
    else if (member_array(zone, move_zones) != -1)
        return;
    else
        move_zones += ({ zone });
} /* add_move_zone() */

/** @ignore why is this here? */
void set_move_zones(string *zones) {
    int i;

    for (i=0;i<sizeof(zones);i++)
        add_move_zone(zones[i]);
} /* set_move_zones() */

/**
 * This method removes a move zone from the npc.
 * @param zone the zone to remove
 * @see add_move_zone()
 * @see query_move_zones()
 * @see set_move_after()
 */
void remove_move_zone(mixed zone) {
    int i;

    if (pointerp(zone))
        for (i=0;i<sizeof(zone);i++)
            remove_move_zone(zone[i]);
    else {
        if ((i=member_array(zone, move_zones)) == -1)
            return ;
        move_zones = delete(move_zones, i, 1);
    }
} /* remove_move_zone() */

/**
 * This method returns the current list of move zones on the npc
 * @return the current list of move zones
 * @see add_move_zone()
 * @see remove_move_zones()
 * @see set_move_after()
 */
string *query_move_zones() { return move_zones; }

/**
 * This method sets the speed at which the npc will randomly
 * wander around.  The npc will wander at the speed:<pre>
 * speed = after + random(rand)
 * </pre>This is called every time the npc sets up for its next move.
 * <p>
 * The move zones control
 * which areas the npcs will wander into, a move zone is set on the
 * room and the npcs will only enter rooms which have a matching
 * move zone.  If there is no move zone, then the npc will enter
 * any room.
 * @param after the fixed amount of time
 * @param rand the random amount of time
 * @see remove_move_zone()
 * @see query_move_zones()
 * @see add_move_zone()
 */
void set_move_after(int after, int rand) {
    _move_after = ({ after, rand });
    if (after != 0 && rand != 0) {
        do_move_after(0);
    }
} /* set_move_after() */

/**
 * This method returns the current move after values.
 * It returns an array of the form:<pre>
 *    ({
 *      after,
 *      rand,
 *     })
 * </pre>
 * @return the move after values
 * @see set_move_after()
 */
mixed query_move_after() {
   return copy( _move_after );
} /* query_move_after() */


/**
 * This method adds a command to be called whenever the npc enters
 * a room.  If the command is a string, then it will be executed
 * as if they had typed it.  If it is a function then the function
 * will be evaluated and one argument (the npc itself) will be passed
 * in.
 * @param str the enter commands to add
 * @see reset_enter_commands()
 * @see query_enter_commands()
 */
int add_enter_commands(mixed str) {
    if (stringp(str)) {
        enter_commands += ({ str });
    } else if (pointerp(str)) {
        enter_commands += str;
    } else if (functionp(str)) {
        enter_commands += ({ str });
    }
    return 1;
} /* add_enter_commands() */

/**
 * This method returns the current array of enter commands.
 * @return the current array of enter commands
 * @see reset_enter_commands()
 * @see add_enter_commands()
 */
string *query_enter_commands() { return enter_commands; }

/**
 * This method resets the array of enter commands back to nothing.
 * @see add_enter_commands()
 * @see add_enter_commands()
 */
void reset_enter_commands() { enter_commands = ({ }); }

/** @ignore yes */
varargs int move( mixed dest, string messin, string messout ) {
    int result;
    object before;
    before = environment();
    result = living::move( dest, messin, messout );
    if ( result == MOVE_OK ) {
        me_moveing( before );
        if ( virtual_move )
            true_location = file_name( environment() );
    }
    return result;
} /* move() */

/** @ignore yes */
void room_look() {
    int i;

    ::room_look();
    for ( i = 0; i < sizeof( enter_commands ); i++ ) {
        if ( functionp( enter_commands[ i ] ) ) {
            call_out( enter_commands[ i ], 2 * i + 1, this_object() );
            continue;
        }
        if ( enter_commands[ i ][ 0 .. 0 ] == "#" )
            call_out( enter_commands[ i ][ 1 .. 99 ], 2 * i + 1 );
        else
            init_command( enter_commands[ i ], 2 * i + 1 );
    }
} /* room_look() */

/**
 * This is called when the npc decides it must continue down
 * a certain route.  This will be called by the wander handler
 * and can be used to force the npc to wander along a route
 * faster.
 * @param running_away this is 1 if the npc is running away
 * @see set_move_after()
 */
void do_move_after (int running_away ) {
    if (sizeof(following_route)) {
        do_route_move();
    } else {
        WANDER_HANDLER->move_after( running_away );
    }
} /* do_move_after() */

/**
 * This event is called when a fight is in progress.  It will
 * be used for things like joining into currently running
 * fights and initiating combat with spell casters.
 * @param me the person initiating the attack
 * @param him the person being attacked
 */
void event_fight_in_progress(object me, object him) {
  mixed action;
  int i;

  // Run the combat actions.
  if(sizeof(this_object()->query_attacker_list())) {
    // Do combat actions.
    for(i=0; i<sizeof(_combat_actions); i+= 3) {
      if(_combat_actions[i] > random(100)) {
        action = _combat_actions[i+2];
        if(stringp(action))
          this_object()->do_command(action);
        else if(functionp(action))
          evaluate(action, me, him);
        else if(pointerp(action) && sizeof(action) == 1 && stringp(action[0]))
          call_other(this_object(), action[0], me, him);
        else if(pointerp(action) && sizeof(action) == 2)
          call_other(action[0], action[1], this_object(), me, him);
      }
    }
  }

  if(him == this_object() && this_object()->query_property("see_caster") &&
     !userp(me) &&
     (random(him->query_property("see_caster"))) < (him->query_int()))
    this_object()->attack_ob(me->query_owner());
  
  if(!join_fight_mess || !me || !him)
    return;

  if(him == this_object())
    return;

  /* only attack npcs if fight_type is not 0. */
  if(!join_fight_type && !interactive(him))
    return;

  if(member_array(him, (object *)this_object()->query_attacker_list()) == -1) {
    if(!him->query_property("no attack")) {
      if(join_fight_mess)
        expand_mon_string(join_fight_mess);
      this_object()->attack_ob(him);
    }
  }
} /* event_fight_in_progress() */

/**
 * This method sets the message to use when joining into fights.
 * @param str the message to print when joining a fight
 * @see query_join_fights()
 * @see set_join_fight_type()
 */
void set_join_fights(string str) { join_fight_mess = str; }
/**
 * This method returns the message to use when joining into fights.
 * @return the message to print when joining a fight
 * @see set_join_fights()
 * @see set_join_fight_type()
 */
string query_join_fights() { return join_fight_mess; }

/**
 * This method sets the flag which allows the npc to join into fights.
 * If this is set to a non-zero value then the npc will join into
 * fights in progress using the fight joining message.
 * @param i 1 if the npc is to join fights, 0 if not
 * @see set_join_fights()
 * @see query_join_fight_type()
 */
void set_join_fight_type(int i) { join_fight_type = i; }
/**
 * This method returns the flag which allows the npc to join into fights.
 * @return 1 if the npc is to join fights, 0 if not
 * @see set_join_fights()
 * @see query_join_fight_type()
 */
int query_fight_type() { return join_fight_type; }

/** @ignore yes */
void event_exit( object me, string mess, object to ) {
  mixed *bing;
  int i, j, k;
  string *zones, fname;
  object *attacker_list, ob;
  
  events::event_exit( me, mess, to );
  living::event_exit( me, mess, to );

  if (!_move_after || sizeof(following_route)) /* we dont move :( */
    return ;
  
  /* If we're not going anywhere... */ 
  if ( !to || !me->query_visible(this_object()))
    return; 

  /* follow the suckers. */
  attacker_list = this_object()->query_attacker_list();
  
  if(attacker_list && sizeof(attacker_list)) {
    // If we're not fighting them then don't follow them.
    if (member_array(me, attacker_list) == -1)
      return;
    
    // Remove us. 
    attacker_list -= ({ me }); 
    
    // If there are people still here who we're fighting then don't
    // follow the person who left.
    foreach(ob in attacker_list) {
      if(ob && environment(ob) == environment())
        return;
    }
  } else { 
    /* Not being attacked, let's not follow. */ 
    return;
  }
    
  bing = (mixed *)environment()->query_dest_dir( this_object() );

  if( !bing ) {
    return ; /* lost cause */
  }

  fname = file_name( to );
    
  /* Check to see they're going a valid direction for this room. */ 
  
  if ((i = member_array(fname, bing)) == -1) {
    return ; /* lost cause */
  }
  if (!this_object()->query_property("unrestricted follow")) {
    zones = (string *)to->query_zones();
    if (move_zones && (j = sizeof(move_zones))) {
      while (j--) {
        if (member_array(move_zones[j], zones) != -1) {
          k = 1; /* If we are suppose to wander in move_zones.. dont go */
          break;  /* where we are not supposed to... get stuck */
        }
      }
      if (!k)
        return;
    } else
      return;
  }
  remove_call_out("do_follow_move");
  call_out("do_follow_move", 4 + random(follow_speed), bing[i-1]);
}

/**
 * This method is used to make the npcs follow after attackers when they
 * leave the room.
 * @param dir the dirction to follow them in
 */
void do_follow_move(string dir) {
  if (sizeof(following_route))
    return ;
  // NPCs are following & attacking instantly since they don't have
  // time_left. So, use up actions instead.
  this_object()->adjust_action_defecit((ACTIONS_PER_HB / (COMBAT_SPEED + 1)));
  do_command(dir);
}

/**
 * This method adds a combat action to the npc.  This is an action which
 * has a chance of occuring during combat.  The name is an identifier
 * which can be used to remove the action with later.  The action
 * itself can be a string, then that command will be executed.  If
 * the action is a function pointer then it will be evaluated with
 * two arguments, the first being the attacker, the second being the
 * target.
 * <p>
 * If the action is an array, if it is one element then the function
 * specified will be called on the attacked with the same arguements
 * as above.  If the size of the array is two then the function will
 * be called on the specified object with the arguments as above.
 * @see remove_combat_action()
 * @see query_combat_actions()
 * @see /std/effects/fighting/combat.c
 */
void add_combat_action( int chance, string name, mixed action ) {
    int i;
    i = member_array( name, _combat_actions );
    if ( i == -1 ) {
        _combat_actions += ({ chance, name, action });
    } else {
        _combat_actions[ i - 1 ] = chance;
        _combat_actions[ i + 1 ] = action;
    }
} /* add_combat_action() */

/**
 * This method will remove the combat action with the specified name.
 * @return 1 if it is found and removed, 0 if not
 * @see add_combat_action()
 * @see query_combat_actions()
 * @see /std/effects/fighting/combat.c
 */
int remove_combat_action( string name ) {
    int i;

    i = member_array( name, _combat_actions );
    if ( i == -1 )
        return 0;
    _combat_actions = delete( _combat_actions, i - 1, 3 );
    return 1;
} /* remove_combat_action() */

/**
 * This method returns the current array of combat actions on the
 * npc.
 * <p>
 * The array will have the format of:
 * <pre>
 *    ({
 *        action1_chance,
 *        action1_name,
 *        action1_action,
 *        ...
 *     })
 * </pre>
 * @return the combat action array
 * @see add_combat_action()
 * @see remove_combat_actions()
 * @see /std/effects/fighting/combat.c
 */
mixed *query_combat_actions() { return copy( _combat_actions ); }

/**
 * This method does a combat action.
 * @param target the target of the action
 * @param thing
 */
void do_combat_action( object player,
                       object target,
                       mixed action ) {
   object place;

   if ( !target ) {
      return;
   }
   place = environment( target );
   if ( place != environment( player ) ) {
      return;
   }
   if ( stringp( action ) ) {
      do_command( action );
      return;
   }
   if (functionp(action)) {
      evaluate(action, player, target);
   }
   if ( pointerp(action) && sizeof(action) == 1 && stringp( action[ 0 ] ) ) {
      call_other( this_object(), action[ 0 ], player, target );
      return;
   }
   if (pointerp(action) && sizeof(action) == 2) {
      call_other( action[ 0 ], action[ 1 ], this_object(), player, target );
   }
} /* do_combat_action() */

/**
 * This is the call back from the combat effect to do something
 * wonderful and wild.
 * @param player the player
 * @param target the target
 */
void combat_actions_call_back( object player, object target ) {
  int i;
  object thing;
  object *things;
  mixed *actions;

  if ( !player || !target ) {
    return;
  }
  thing = environment( player );
  if ( thing != environment( target ) ) {
    return;
  }

  things = filter_array( all_inventory( thing ), (: living( $1 ) ==
                                                  !userp( $1 ) :) );
  foreach ( thing in things ) {
    actions = (mixed *)thing->query_combat_actions();
    for(i=0; i<sizeof(actions); i += 3) {
      if(actions[i] > random(100)) {
        call_out( "do_combat_action", 1, player, target, actions[ i + 2 ] );
      }
    }
  }
} /* combat_actions_call_back() */



/**
 * This method adds an action to the npc that will happen if a specified
 * spell is cast.  This is an action which
 * has a chance of occuring when a spell is being cast.
 * The name is an identifier
 * which can be used to remove the action with later.  The action
 * itself can be a string, then that command will be executed.  If
 * the action is a function pointer then it will be evaluated with
 * two arguments, the first being the caster, the second being the
 * target(s) array and the third being the magic arguments class.
 * <p>
 * If the action is an array, if it is one element then the function
 * specified will be called on the attacked with the same arguements
 * as above.  If the size of the array is two then the function will
 * be called on the specified object with an extra first argument being
 * the npc which the effect is being called from.
 * @param spell_object the spell to respond to
 * @param chance the chance of it working
 * @param name the name of the thing
 * @param action the action to preform
 */
void add_spell_action(string spell_object, int chance,
                     string name, mixed action) {
   if (!_spell_actions[spell_object]) {
      _spell_actions[spell_object] = ([ ]);
   }
   _spell_actions[spell_object][name] = ({ chance, action });
} /* add_spell_action() */

/**
 * This method removes the specified spell action.
 * @param name the name of the spell to remove
 * @return 1 if successful, 0 if not
 */
int remove_spell_action(string name) {
   string spell;
   mapping bits;
   int ret;

   foreach (spell, bits in _spell_actions) {
      if (bits[name]) {
         map_delete(bits, name);
         ret = 1;
      }
   }
   return ret;
} /* remove_spell_action() */

/**
 * This method returns the list of spell actions present on the
 * npc.
 * @return the list of spell actions
 */
mapping query_spell_actions() {
   return _spell_actions;
} /* query_spell_actions() */

/**
 * This method does a combat action.
 * @param caster the caster of the spell
 * @param targets the targets of the spell
 * @param action the action to do
 * @param args the spell arguements
 */
void do_spell_action( object caster,
                      object* targets,
                      mixed action,
                      mixed args ) {
   if ( stringp( action ) ) {
      do_command( action );
      return;
   }
   if (functionp(action)) {
      evaluate(action, caster, targets);
   }
   if ( pointerp(action) && sizeof(action) == 1 && stringp( action[ 0 ] ) ) {
      call_other( this_object(), action[ 0 ], caster, targets, args );
      return;
   }
   if (pointerp(action) && sizeof(action) == 2) {
      call_other( action[ 0 ], action[ 1 ], this_object(), caster, targets,
                  args );
   }
} /* do_spell_action() */


/**
 * This returns the race object associated with the npc.
 * @return the race object associated with the npc
 * @see set_race()
 * @see set_race_ob()
 * @see set_level()
 */
mixed query_race_ob() { return race_ob; }
/**
 * This method sets the race object associated with the npc.
 * This will probably not do what you expect and cannot be used
 * in conjucton with set_level to set the race.  It is called
 * by /std/race.c when set_level() is called.
 * @see set_level()
 * @see set_race()
 * @see /std/race.c
 * @param r the new race object
 */
void set_race_ob(mixed r) { race_ob = r; }

/**
 * This returns the guild object associated with the npc.
 * @return the guild object associated with the npc
 * @see set_guild()
 * @see set_guild_ob()
 * @see set_level()
 */
mixed query_guild_ob() { return guild_ob; }
/**
 * This method sets the guild object associated with the npc.
 * This will probably not do what you expect and cannot be used
 * in conjucton with set_level to set the guild.  It is called
 * by /std/race.c when set_level() is called.
 * @see set_level()
 * @see set_guild()
 * @see /std/race.c
 * @param g the new guild object
 */
void set_guild_ob(mixed g) { guild_ob = g; }

/**
 * This method queries the speed at which the npc will follow
 * after a player when they leave combat.
 * @return the current follow speed
 * @see set_follow_speed()
 */
int query_follow_speed() { return follow_speed; }
/**
 * This method sets the speed at which the npc will follow
 * after a player when they leave combat.
 * @return the current follow speed
 * @see set_follow_speed()
 */
void set_follow_speed(int f) { follow_speed = f; }

/**
 * This method returns the current aggressive level of the npc.
 * If the aggressive is set to 1, then the npc will attack all players
 * that enter its environment.  If the aggressive is set to 2 then
 * the npc will attack everything (including other npcs).
 * <p>
 * See the function start_attack() for information about things you
 * can do to stop aggressive npcs attacking things.
 * @return the aggressive level of the npc
 * @see set_aggressive()
 * @see start_attack()
 */
int query_aggressive() { return aggressive; }
/**
 * This method sets the current aggressive level of the npc.
 * If the aggressive is set to 1, then the npc will attack all players
 * that enter its environment.  If the aggressive is set to 2 then
 * the npc will attack everything (including other npcs).
 * <p>
 * See the function start_attack() for information about things you
 * can do to stop aggressive npcs attacking things.
 * @see query_aggressive()
 * @see start_attack()
 * @see set_join_fights()
 * @see set_throw_out()
 * @param a the new aggressive level
 */
void set_aggressive(int a) {
    aggressive = a;

    // this added to make aggressive npcs join in fights if they arent
    // already set that way. This is needed coz otherwise you can sneak
    // into a room and fight the aggressive npcs one at a time -- Ceres 10/97
    if (a && !join_fight_mess) {
        join_fight_mess = this_object()->one_short()+" joins in the fight.\n";
    }
}

/**
 * This method returns the current guild level of the npc.  This is
 * a pass through call to a function on the guild object associated
 * with this npc.
 * @return the current guild level of the object
 * @see set_guild()
 */
int query_level() {
    if (!guild_ob) return 1;
    return (int)guild_ob->query_level(this_object());
} /* query_level() */

/**
 * This method returns the amount of death experiecne that would be
 * gained by killing the npc.
 * @return the amount of death experience for the npc
 */
int query_death_xp() {
    int amount;

    if ( query_property( "dead" ) || query_property( "unique" ) ) {
        return 0;
    }
    amount = (int)TOP_TEN_HANDLER->calculate_rating( this_object() );
    //amount += (int)this_object()->query_max_hp();
    //amount *= 120;
    return amount / 2;
} /* query_death_xp() */

/** @ignore yes */
mixed *stats() {
    mixed *bing;
    int i;
    bing = ({ });
    for (i=0;i<sizeof(move_zones);i++)
        bing += ({ ({ "move zone "+i, move_zones[i] }) });
    if (!query_move_after())
        return ::stats() + ({
          ({ "guild", query_guild() }),
          ({ "guild ob", query_guild_ob() }),
          ({ "race", query_race() }),
          ({ "race ob", query_race_ob() }),
          ({ "join_fights", query_join_fights() }),
          ({ "follow speed", query_follow_speed() }),
          ({ "level", query_level() }),
          ({ "chat chance", query_chat_chance() }),
          ({ "achat chance", query_achat_chance() }),
          ({ "aggressive", query_aggressive() }),
          ({ "route", (sizeof(following_route)?implode(following_route, ", "):
              "not going anywhere") }),
        }) + bing;
    return ::stats() + ({
      ({ "race", query_race() }),
      ({ "race ob", query_race_ob() }),
      ({ "guild", query_guild() }),
      ({ "guild ob", query_guild_ob() }),
      ({ "join fights", query_join_fights() }),
      ({ "follow_speed", query_follow_speed() }),
      ({ "level", query_level() }),
      ({ "chat chance", query_chat_chance() }),
      ({ "achat chance", query_achat_chance() }),
      ({ "aggressive", query_aggressive() }),
      ({ "move after-fix", query_move_after()[0] }),
      ({ "move after-rand", query_move_after()[1] }),
      ({ "route", (sizeof(following_route)?implode(following_route, ", "):
          "not following anyone") }),
    }) + bing;
} /* stats() */

/** @ignore yes */
string expand_nickname(string str) { return str; }

/** @ignore yes */
void event_enter( object dest, string mess, object from ) {
    /* stop massive spam in /room/rubbish and /room/void */
    if( environment( this_object() ) &&
      file_name( environment( this_object() ) )[ 1 .. 4 ] == "room" )
        return;
    if(check_anyone_here() && (moves > 2)){
        moves = 0;
        do_move_after(0);
    }
    living::event_enter( dest, mess, from );
    events::event_enter( dest, mess, from );
} /* event_enter() */

/** @ignore yes */
void event_person_say( object thing, string start, string mess, string lang,
                       string accent) {
    response_mon::event_person_say(thing, start, mess, lang);
} /* event_person_say() */

/** @ignore yes */
void event_whisper( object thing, string start, string mess, object *obs,
  string lang, object me) {
    response_mon::event_whisper( thing, mess, obs, lang, me);
} /* event_whisper() */

/** @ignore yes */
varargs void event_soul( object thing, string mess, mixed avoid, string verb,
  string last, mixed at ) {
    response_mon::event_soul( thing, mess, avoid, verb, last, at );
    events::event_soul( thing, mess, avoid );
} /* event_soul() */

/**
 * This method moves the npc to room it is really supposed to be in.
 * This is used with the virtual moving
 * @param check_room the place where it thinks we are
 * @see query_virtual_move()
 */
void real_room( string check_room ) {
    if ( check_room == true_location ) {
        move_object( true_location );
    }
} /* real_room() */

/**
 * This method returns true if we are currently using the
 * virtual move system.
 * @return 1 if we are virtual moving, 0 if not
 * @see set_virtual_move()
 */
#if !efun_defined(add_action)
int query_virtual_move() { return virtual_move; }
#else
int query_virtual_move() { return 0; }
#endif
/**
 * This method sets the current virual move ability of the npc.
 * NB: This is currently disabled virtual moving is not possible
 * at all.
 * @param number 1 for virtual moving, 0 for not
 * @see query_virtual_move()
 */
void set_virtual_move( int number ) {
    if(virtual_move && !number && file_name(environment()) == "/room/virtual"){
        object ob = load_object(true_location);
        if(ob)
            move(ob);
    }
    virtual_move = number;
    if ( virtual_move && environment() )
        true_location = file_name( environment() );
} /* set_virtual_move() */

/**
 * This method returns the true location of the npc.  This is
 * the real room it is in, not the room it is currently in.
 * @return the true location of the npc
 * @see set_true_location()
 * @see set_virtual_move()
 */
string query_true_location() { return true_location; }

/**
 * This method sets the true location of the npc.  This is
 * the real room it is in, not the room it is currently in.
 * @param word the new true location of the npc
 * @see set_true_location()
 * @see set_virtual_move()
 */
void set_true_location( string word ) {
    if(word == "/room/virtual"){
        if(true_location)
            return;
        //we have to be somewhere!
        word = file_name(environment());
        if(word == "/room/virtual"){
            //something is totally wrong give up on the npc
            move("/room/rubbish");
            true_location = "/room/rubbish";
        }
    }
    true_location = word;
}

int cleaning_room() {
    if ( virtual_move ) {
        true_location = file_name( environment() );
        "/room/virtual"->force_load();
        move_object( "/room/virtual" );
        return 1;
    }
    return 0;
} /* cleaning_room() */

/**
 * This method causes the npc to move in the given direction.
 * @param move the direction to move
 */
void do_move( string move ) {
    if (1 || check_anyone_here()) {
        moves = 0;
    }
    if (moves++ < 3) {
        string tmp;
        command(move);
        tmp = file_name(environment());
        if(tmp == "/room/virtual")
            tmp = true_location; //room didn't let us in.
        true_location = tmp;
    } else {
        WANDER_HANDLER->delete_move_after(this_object());
    }
} /* do_move() */

/* Move along a nice route thingy... */
/**
 * This method gets the next direction to go in the route which is
 * currently being followed.  It will remove this direction off the
 * array.
 * @return the next direction to go down
 * @see query_last_route_direction()
 * @see query_following_route()
 * @see do_route_move()
 */
string get_next_route_direction() {
    string direc;

    if (!sizeof(following_route)) {
        return 0;
    }

    direc = following_route[0];
    following_route = following_route[1..];
    return direc;
} /* get_next_route_direction() */

/**
 * This method tells us if the npc is currently following a route.
 * @return 1 if there are route directions to follow still
 * @see get_next_route_direction()
 * @see query_following_route()
 * @see do_route_move()
 */
int query_last_route_direction() { return ( sizeof(following_route) ? 1 : 0 ); }

/**
 * This method returns the current array of directions we are following
 * as a route.
 * @see get_next_route_direction()
 * @see query_last_route_direction()
 * @see do_route_move()
 */
string *query_following_route() { return following_route; }

/**
 * This method moves the npc one more location along the route it
 * is following.
 * @see query_last_route_direction()
 * @see query_following_route()
 * @see get_next_route_direction()
 */
void do_route_move() {
    if (!sizeof(following_route)) {
        return ;
    }

    do_command(get_next_route_direction());
} /* do_route_move() */

/**
 * This method is called by the move_me_to function after the
 * route handler has successfuly discovered the route to follow.
 * @param route the route to follow
 * @param delay the delay to follow it with
 * @param dest route destination
 * @see move_me_to()
 */
protected void got_the_route(string *route, int delay, string dest) {
    following_route = route;
    if (sizeof(route)) {
        WANDER_HANDLER->move_me_please(delay, dest);
        do_route_move();
    } else {
        this_object()->stopped_route();
    }
} /* got_the_route() */

/**
 * This method will move the npc to the specified destination.  The
 * npc will walk from where they currently are to the destination using
 * the time delay specified between the movements.
 * <p>
 * If the location is reached then the function "stopped_route" will
 * be called on the npc.  The first arguement to the function will
 * be 0 if the npc did not reach its destination and 1 if it did.
 * @param dest the destination to go to
 * @param delay the time delay between each move
 * @example
 * inherit "/obj/monster";
 *
 * void go_home() {
 *    move_me_to(HOME_LOCATION, 2);
 * } /\* go_home() *\/
 *
 * void stopped_route(int success) {
 *    if (success) {
 *       do_command("emote jumps for joy.");
 *    } else {
 *       do_command("emote looks sad and lost.");
 *    }
 * } /\* stopped_route() *\/
 * @see get_next_route_direction()
 * @see got_the_route()
 * @see query_last_route_direction()
 * @see query_following_route()
 * @see do_route_move()
 */
varargs void move_me_to(string dest, int delay) {
    string *dest_dir, *start_dir;

    if (!environment() || !file_name(environment()))
        return ;
    if(!delay)
      delay = 10;
    
    if (!MAP->static_query_short( dest ) ||
      !MAP->static_query_short( file_name(environment()) ) ) {
        //very likely to have no route
       if (!sizeof( dest_dir = dest->query_dest_dir( this_object() ) ) ||
           !sizeof( start_dir = 
               environment()->query_dest_dir( this_object() ) ) )
       {
            //one of the rooms have no exits
            move( dest, "$N wanders in.\n", "$N wanders off.\n" );
        } else {
            //both rooms have exits
            move( dest,
              replace_string( query_msgin(), "$F",
                dest_dir[random(sizeof(dest_dir)/2)*2]),
              replace_string( query_msgout(), "$T",
                start_dir[random(sizeof(start_dir)/2)*2]));
        }
        call_out( "stopped_route", 1 );
        return;
    }

    ROUTE_HANDLER->get_route(dest, file_name(environment()),
      (: got_the_route($1, $(delay), $(dest)) :));
} /* move_me_to() */

/** @ignore yes */
string identify( object thing, object *places ) {
    do_command( "'Please bug me!  I'm using identify()!" );
    return file_name( thing );
} /* identify() */

/** @ignore yes */
int query_time_left() { return 1; }

/** @ignore yes */
int ignore_identifier() { return 1; }

/**
 * This method adds a language to the npc. 
 * <p>
 * After the sun has died away<br>
 * The stars come out and glow<br>
 * Lighting the embers of good intentions<br>
 * Ghostly white, unhappily bright<br>
 * Time lost, the day done
 * @param str the language to add
 */
void add_language(string str) {
    ::add_language(str);
    added_language = 1;
} /* add_language() */

/**
 * This method sets the value of the unable to change position flag.
 * This flag will be checked by the soul, and by anything else which
 * deliberatly changes someones position.
 * @param flag the unable to change position flag
 * @see /std/living/living->set_default_position()
 */
void set_cannot_change_position(int flag) {
    cannot_change_position = flag;
} /* set_cannot_change_position() */

/**
 * This method returns the current value of the unable to change
 * position flag.
 * @return the unable to change position flag
 * @see /std/living/living->set_default_position()
 */
int query_cannot_change_position() {
    return cannot_change_position;
} /* query_cannot_change_position() */

/**
 * This method overrides the position code so that if the position
 * is changed we are changed back to the default position.
 * @param new_pos the new position
 * @ignore yes
 */
void set_position(string new_pos) {
    if (always_return_to_default_position) {
        if (new_pos != query_position() &&
          this_player() != this_object()) {
          // Please, do NOT use function pointers unless it's necessary.
          // Took me ages to find this "function" given that all
          // call_stack could tell me was that it was "<function>"
            call_out("return_to_default_position",
              always_return_to_default_position, 0);
        }
    }
    ::set_position(new_pos);
} /* set_position() */

/**
 * This method sets the status of the flag that makes the npc return
 * to the default position if its position is changed.  The flag
 * specified the length of time to wait before causing the
 * default position to be restored.
 * @param tim the time to wait before the position is restored
 * @see /std/living/living->return_to_default_position()
 * @see query_always_return_to_default_position()
 */
void set_always_return_to_default_position(int tim) {
    always_return_to_default_position = tim;
} /* set_always_return_to_default_position() */

/**
 * This method returns the status of the flag that makes the npc return
 * to the default position if its position is changed.  The flag
 * specified the length of time to wait before causing the
 * default position to be restored.
 * @return the time to wait before the position is restored
 * @see /std/living/living->return_to_default_position()
 * @see set_always_return_to_default_position()
 */
int query_always_return_to_default_position() {
    return always_return_to_default_position;
} /* query_always_return_to_default_position() */

/** @ignore yes */
mapping int_query_static_auto_load() {
    return ([
      "::" : ::int_query_static_auto_load(),
      "cap name" : cap_name,
      "race" : race,
      "guild" : guild,
    ]);
}

/** @ignore yes */
mixed query_static_auto_load() {
    if ( base_name(this_object()) + ".c" == __FILE__ )
        return int_query_static_auto_load();
    return ([ ]);
}

mapping query_dynamic_auto_load() {
    return ([
      "::" : ::query_dynamic_auto_load(),
      "chat string"                       : chat_string,
      "achat string"                      : achat_string,
      "combat actions"                    : _combat_actions,
      "move after"                        : _move_after,
      "throw out"                         : throw_out,
      "chat chance"                       : chat_chance,
      "achat chance"                      : achat_chance,
      "aggressive"                        : aggressive,
      "join fight type"                   : join_fight_type,
      "join fight mess"                   : join_fight_mess,
      "follow speed"                      : follow_speed,
      "virtual move"                      : virtual_move,
      "moves"                             : moves,
      "true location"                     : true_location,
      "enter commands"                    : enter_commands,
      "move zones"                        : move_zones,
      "doing story"                       : doing_story,
      "last attacked"                     : last_attacked,
      "following route"                   : following_route,
      "added language"                    : added_language,
      "cannot change position"            : cannot_change_position,
      "always return to default position" : always_return_to_default_position,
      "level"                             : query_level(),
    ]);
}

void init_static_arg(mapping args) {
    if (args["::"])
        ::init_static_arg(args["::"]);
    if (!undefinedp(args["cap name"]))
        cap_name = args["cap name"];
    if (!undefinedp(args["race"]))
        race = args["race"];
    if (!undefinedp(args["guild"]))
        guild = args["guild"];
}

void init_dynamic_arg( mapping args, object ob ) {
    if (args["::"])
        ::init_static_arg(args["::"]);
    if (!undefinedp(args["chat string"]))
        chat_string = args["chat string"];
    if (!undefinedp(args["achat string"]))
        achat_string = args["achat string"];
    if (!undefinedp(args["combat actions"]))
        _combat_actions = args["combat actions"];
    if (!undefinedp(args["move after"]))
        _move_after = args["move after"];
    if (!undefinedp(args["throw out"]))
        throw_out = args["throw out"];
    if (!undefinedp(args["chat chance"]))
        chat_chance = args["chat chance"];
    if (!undefinedp(args["achat chance"]))
        achat_chance = args["achat chance"];
    if (!undefinedp(args["aggressive"]))
        aggressive = args["aggressive"];
    if (!undefinedp(args["join fight type"]))
        join_fight_type = args["join fight type"];
    if (!undefinedp(args["join fight mess"]))
        join_fight_mess = args["join fight mess"];
    if (!undefinedp(args["follow speed"]))
        follow_speed = args["follow speed"];
    if (!undefinedp(args["virtual move"]))
        virtual_move = args["virtual move"];
    if (!undefinedp(args["moves"]))
        moves = args["moves"];
    if (!undefinedp(args["true location"]))
        true_location = args["true location"];
    if (!undefinedp(args["enter commands"]))
        enter_commands = args["enter commands"];
    if (!undefinedp(args["move zones"]))
        move_zones = args["move zones"];
    if (!undefinedp(args["doing story"]))
        doing_story = args["doing story"];
    if (!undefinedp(args["last attacked"]))
        last_attacked = args["last attacked"];
    if (!undefinedp(args["following route"]))
        following_route = args["following route"];
    if (!undefinedp(args["added language"]))
        added_language = args["added language"];
    if (!undefinedp(args["cannot change position"]))
        cannot_change_position = args["cannot change position"];
    if (!undefinedp(args["always return to default position"]))
        always_return_to_default_position =
        args["always return to default position"];
    if (!undefinedp(args["level"]))
        set_level(args["level"]);
}
/**
 *
 * attack_permission function, added for use by the allow_attack simul.
 * @param object The person performing the action
 * @param object The object being acted on, this object usually.
 * @param string Attack type, this will be one of "combat", "theft", or "magic",
 * this lets you give your NPCs different responses for different attacks. As well
 * as make them immune to theft and magic (as an example)
 * @return int 1 if the action is denied, 0 is it can go through.
 * @see efun::allow_attack()

 */
int attack_permission( object ob1, object ob2, string stringy ) { return 0; }

#if !efun_defined(add_action)
/** @ignore yes */
protected mixed _process_input(string str) {
    object ob = this_player();
    if(str == "")
        return 0;
    _notify_fail(0);
    efun::set_this_player(this_object());
    if(!this_object()->drunk_check(str))
        if(!this_object()->exit_command(str))
            if(!this_object()->cmdAll(str))
                if(!this_object()->new_parser(str))
                    if(!this_object()->lower_check(str)){
                        efun::set_this_player(ob);
                        return 0;
                    }
    efun::set_this_player(ob);
    return "bing";
} /* _process_input() */

int drunk_check(string str) {
   if(cmd_func){
     object owner = function_owner(cmd_func);
     if(owner && owner == environment(this_player())){
       int res = evaluate(cmd_func, str);
       if(res)
         return res;
     } else cmd_func = 0;
   }

   return 0;
} /* drunk_check() */

/**
 * Use this function to set a function that is called with the NPCs input
 * before command() gets it, return 1 from the function if the
 * input needs no further parsing (ie the command is handled)
 *
 * @param func = function in the players environment to call.
 */

void command_override(function func) {
  if(!functionp(func))
    error("command_override needs a function!");
  cmd_func = func;
}


/** @ignore yes */
protected mixed command(string cmd){
  int time = eval_cost();
  if(_process_input(cmd))
    return eval_cost() - time + 1; // on v22.2 eval_cost runs up, reverse for v22.1
  return 0;
}

#endif

/** @ignore yes */
int _living(){return 1;}


/**
 * This event is triggered when hide_invis is added or removed from an
 * object.  In this case it's used to make the NPC attack if someone
 * sneaks into the room and comes out of hiding.
 * @param hider The person who's hiding/unhiding.
 * @param adding 1 if the person is hiding, 0 if they are coming out.
 * @param type The type of hide invis.
 * @param quiet The quiet flag that's passed into remove_hide_invis.
 */
void event_hide_invis( object hider, int adding, string type, int quiet ) {
   if ( aggressive &&
        !adding &&
        environment() &&
        file_name( environment() )[1..4] != "room" )
   {
      start_attack(hider);
   }
} /* event_hide_invis() */
