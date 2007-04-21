/**
 * This is the effect skelton docs.  This effect
 * has a classification of "magic.talker".
 * <p>
 * Describe the arguments in here.
 * @classification magic.talker
 * @see help::effects
 */
 
#include "path.h"
#include <library.h>
#include <talker.h>
#include <player.h>
#include <playtesters.h>
#include <deity.h>
#include <nomic_system.h>

#define HELP_CMD "/cmds/player/help"

nosave mapping _channel_cache;

// New channels must be added to this list.
nosave string *valid_channels = ({"one", "two", "A'Tuin", "Intermud", "Apex",
    "Adventurers", "Priests", "Wizards",
    "Assassins", "Warriors", "Thieves",
    "Witches", "playerkillers",
    "playtesters", "Catfish", "Fish",
    "Gapp", "Gufnork", "Hat",
    "Pishe", "Sandelfon", "Sek",
      "theAgateanEmpireCouncil",
     "KlatchCouncil", "KlatchCouncilMagistrate",
    "Ankh-MorporkCouncil",
    "Ankh-MorporkCouncilMagistrate" });

/** @ignore yes */
string query_classification() { return "magic.talker"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS +"talker"; }

/** @ignore yes */
int query_indefinite() { return 1; }

/**
 * This method returns the "default" effect arguments. They are used
 * if the existing ones are invalid (or old).
 * @return a talker_args class containing "new" arguments.
 */
protected class talker_args new_talker_args() {
    return new(class talker_args, status : 1, channels : ({ "one" }),
        verbose : 1, local_echo : 0, colour : 0 );
} /* new_talker_args() */

/** @ignore yes */
class talker_args beginning( object thing, mixed args, int id ) {

    if ( environment( thing ) )
        tell_object( environment( thing ), "Somewhere inside " +
            thing->the_short() + " a pair of glittering eyes swirl "
            "into being.\n" );
    
    thing->add_extra_look( this_object() );
    thing->add_alias( "talker" );
    thing->add_plural( "talkers" );
    
    if (arrayp(args)) {
        args = new_talker_args();
        return args;
    }
    
    if (!classp(args)) {
        return new_talker_args();    
    }
} /* beginning() */

/** @ignore yes */
class talker_args merge_effect(object thing, mixed old_args, mixed new_args) {
    class talker_args args;
    
    if (!classp(old_args) || !classp(new_args))
        return new_talker_args();
    
    args = copy(old_args);
    
    args->channels += new_args->channels;
    args->channels = uniq_array(args->channels);
    
    return args;
} /* merge_effect() */

/** @ignore yes */
void restart( object thing, mixed args, int id ) {
    if (!classp(args)) {
        thing->set_arg_of(thing->sid_to_enum(id), new_talker_args());
        tell_object( environment( thing ), "Your talker quivers "
            "uncontrollably and starts to jump around.  You will have "
            "to re-add your talker channels and reconfigure your talker.\n" );
    }
    
    thing->add_extra_look( this_object() );
    thing->add_alias( "talker" );
    thing->add_plural( "talkers" );
} /* restart() */

/** @ignore yes */
void end( object thing, mixed *args, int id ) {
    if ( environment( thing ) )
        tell_object( environment( thing ), "The pair of glittering eyes in "+
          (string)thing->the_short() +" close and fade away forever.\n" );
    thing->remove_extra_look( this_object() );
    thing->remove_alias( "talker" );
    thing->remove_plural( "talkers" );
} /* end() */

/** @ignore yes */
string extra_look( object thing ) {
    int *enums;
    mixed args;

    enums = (int *)thing->effects_matching( "magic.talker" );
    if ( !sizeof( enums ) ) {
        return "";
    }
    args = thing->arg_of( enums[ 0 ] );
    
    if (!classp(args)) {
         return "";
    }
        
    if ( args->status ) {
        return "Just beneath the surface, you can make out what looks like a "
        "pair of glittering eyes.\n";
    }
    return "Just beneath the surface, you can make out what looks like a "
    "pair of closed eyes.\n";
} /* extra_look() */

/**
 * This method is used to determine which talker channels can use.
 * It also caches the data so that it only has to be recalculated 
 * once.
 * <p>
 * @param player the player's channel list to generate
 * @return an array of channel names. (string *)
 * @see clear_cache()
 * @see query_channel_cache()
 */
string *all_channels( object player ) {
    string word, *list;
    string p_name;
    class cache_data settings;
    string* areas;

    if ( player->query_property("gagged") ) {
        return ({ });
    }

    list = ({ "one", "two" });
    word = (string)player->query_guild_ob();

    if ( file_size( word +".c" ) > 0 ) {
        if (word != "disavowed") {
            list += ({ capitalize(word->query_name()) });
        }
    }
    else {
        list += ({ "Adventurers" });
    }

    word = (string)player->query_deity();

    if ( stringp( word ) ) {
        list += ({ capitalize( word ) });
    }

    word = (string)player->query_name();

    p_name = player->query_name();

    /* This stores the channels so that we only have to check the data
       once. */

    if (!_channel_cache) {
        _channel_cache = ([ ]);
    }

    if (undefinedp(_channel_cache[p_name])) {
        _channel_cache[p_name] = new(class cache_data,
          talker_quest : LIBRARY->query_quest_done(p_name, "talker quest"),
          playtester : PLAYTESTER_HAND->query_playtester(p_name),
          apex_member : "/d/am/buildings/apex/admin_office"->query_member(p_name)
        );
    }

    settings = _channel_cache[p_name];

    if (settings->talker_quest) {
        list += ({ "A'Tuin", "Intermud" });
    }

    if (settings->apex_member) {
        list += ({ "Apex" });
    }

    if (settings->playtester) {
        list += ({ "playtesters" });
    }

    if ( player->query_player_killer()) {
        list += ({ "playerkillers" });
    }

    areas = NOMIC_HANDLER->query_citizenship_areas();
    foreach (word in areas) {
       if (NOMIC_HANDLER->is_citizen_of(word, p_name)) {
          list += ({ replace_string(word, " ", "") + "Council" });
       }
       if (NOMIC_HANDLER->is_magistrate_of(word, p_name)) {
          list += ({ replace_string(word, " ", "") + "CouncilMagistrate" });
       }
    }

    return list;
} /* all_channels() */

/**
 * This returns the channels that a player is on in a nifty
 * formatted way.  Used in the channel list and talker shadow.
 * @return a list of channels that this_player() is on.
 */
string channels() {
    return sprintf( "%#-*s\n", (int)this_player()->query_cols(),
      implode( all_channels( this_player() ), "\n" ) );
} /* channels() */

/**
 * This method is used to check whether or not a channel (or a list of
 * channels are valid.
 * @param word if this is a string, then valid() checks to see whether
 * or not the channel can be used.  If it is an array, then it validates
 * all the channels mentioned.
 * @return 1 if the channel (or channels) are valid, 0 if they are not.
 */
varargs int valid( mixed word, object who) {
    
    if (!who) {
        who = this_player();
    }

    if (arrayp(word) && arrayp(valid_channels)) {
        if (sizeof( word - valid_channels ) != 0) {
            return 0;
        }
    } else {
        if (member_array( word, valid_channels ) == -1) {
            return 0;
        }
    }

    if (who->query_lord()) {
       return 1;
    }

    if ( who->query_creator() ) {
        if (stringp(word) &&
            word[<7..] != "Council" &&
            word[<10..] != "Magistrate") {
            return 1;
        } else if (arrayp(word)) {
            if (!sizeof(filter(word, (: $1[<7..] == "Council" &&
                                        $1[<10..] == "Magistrate" :)))) {
                return 1;
            }
        }
    }
    if ( arrayp( word ) ) {
        return sizeof( word - all_channels( who ) ) == 0;
    }
    
    return ( member_array( word, all_channels( who ) ) != -1 );
} /* valid() */

object find_actual_talker( object thing ) {
    object talker;
    
    while ( thing ) {
        talker = thing;
        thing = query_shadowing( thing );
    }
    
    return talker;
} /* find_actual_talker() */
   
/**
 * This method returns a formatted list of which players are using the 
 * talker.
 * @param word the name of a specific channel to get a listing of.
 * @param brief only show the people, not what they are listening to
 * @return a formatted string containing who is on a channel.
 */
string list( string word, int brief ) {
    int space;
    int *enums;
    string sender;
    string table;
    object thing;
    object *ok;
    object *things;
    object holder;
    class talker_args args;

    table = "";

    space = (int)this_player()->query_cols() - 20;
        
    things = children( SHADOWS + "talker" ) - 
        ({ find_object( SHADOWS + "talker" ) });
        
    things = filter( map( things, (: find_actual_talker( $1 ) :) ), 
        (: $1 && environment($1) && living(environment($1)) &&
           (!userp($1) || interactive($1)) :) );
    
    /* Remove duplicates.  If you have a lot of talkers, they won't
       all show up. */
    things = uniq_array(things);
    things = sort_array( things, 
        (: strcmp( environment($1)->query_name(), 
            environment($2)->query_name() ) :) );
    ok = ({ });
    foreach( thing in things ) {
        holder = environment( thing );
        if (userp(holder)) {
            sender = capitalize( holder->query_cap_name() );
        } else {
            sender = capitalize( holder->query_name() );
        }
            
        if ( holder->query_invis() ) {
            continue;
        }
                        
        enums = (int *)thing->effects_matching( "magic.talker" );

        if ( !sizeof( enums ) ) {
            if ( !word ) {
                table += sprintf( LIST_FORMAT, sender, space, "(barfed)" );
            }
            continue;
        }
        
        args = thing->arg_of( enums[ 0 ] );
        
        if (!classp(args)) {
            continue;
        }
            
        if ( !args->status) {
            if ( !word ) {
                table += sprintf( LIST_FORMAT, sender, space, "(asleep)" );
            }
            continue;
        }
        
        if ( !word || ( member_array( word, args->channels ) != -1 ) ) {
            if (brief) {
                ok += ({ sender });
            } else {
                table += sprintf( LIST_FORMAT, sender, space,
                    query_multiple_short( args->channels ) );
            }
        }
    }

    if (brief) {
       return "$I$5=People listing to the channel " + word + " are: " +
              query_multiple_short(ok) + "\n";
    }
    
    if ( table == "" ) {
        return sprintf( LIST_FORMAT, "None", space, "" );
    }
    
    return table;
} /* list() */

string normalise_name( string channel ) {
    int position;
    mixed *details;
    string *lower_names;
    string lower_channel;
    
    lower_channel = lower_case( channel );
    lower_names = map( valid_channels, (: lower_case($1) :) );
    
    
    position = member_array( lower_channel, lower_names );
    
    if ( position == -1 ) {
        details = HELP_CMD->find_match_in_array( lower_channel,
            lower_names );
        
        if ( details[ 1 ] > 70 ) {
            return valid_channels[ details[ 0 ] ];
        }
        
        return channel;
    }
    
    return valid_channels[ position ];
} /* normalise_name() */

/**
 * This method is used for debugging purposes to query the 
 * channel cache.
 * @param person the name of a person to query. If this is not
 * specified, then the entire channel cache is returned.
 * @return a mapping containing channel cache data.
 */ 
varargs mapping query_channel_cache(string person) {
    if (!_channel_cache)
        return ([ ]);

    if (stringp(person) && classp(_channel_cache[person])) {
        return copy(_channel_cache[person]);
    } 

    return copy(_channel_cache);
} /* query_channel_cache() */

/** @ignore yes */
void reset() {
    _channel_cache = ([ ]);
} /* reset() */

/**
 * This method is used to clear a specific person's cache 
 * details.  It is called from the shadow when a person
 * attempts to re-add a channel, so they their details
 * can be updated.
 * @param person the person's name to clear from the cache.
 */
void clear_cache(string person) {
    if (_channel_cache) {
        map_delete(_channel_cache, person);
    }
} /* clear_cache() */
