/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: top_ten_handler.c,v 1.23 2003/03/21 01:25:05 ceres Exp $
 * 
*/
/**
 * Top ten player lists - overall and for each guild.
 *<p>
 * archive and top_ten is of the format:
 *<code><p>
 *       ({ ({ name (guild), rating, level, age }), ... })
 *</code><p>
 * guild_top_tens is of the format:
 *<code><p>
 *       ([ guild: ({ ({ name, rating, level, age }), ... }), ... ])
 * </code>
 * @author Deutha
 */

#include <library.h>
#include <skills.h>
#include <top_ten_tables.h>
#include <clubs.h>
#include <player.h>
#include <player_handler.h>

#define SAVE_FILE "/save/top_ten_tables"
#define TOP_TEN_SIZE 15
#define GUILD_TOP_TEN_SIZE 10
#define ARCHIVE_TIME 50 * 24 * 60 * 60
#define ARCHIVE_RATING 30000

/* The Apex Club accepts members with a rating of 10k or greater */
#define APEX_LIMIT 10000

/* Age in days = age in seconds / 86400 */
#define AGE_DIVIDER 100
// #define AGE ( -thing->query_time_on() / 86400 )
#define AGE calculate_age_modifier( thing, 0 )
// #define AGE ( -( thing->query_time_on() - ( thing->query_refresh_time() ? ( time() - thing->query_refresh_time() ) + thing->query_time_on() : 0 ) ) / 86400 )

mapping guild_top_tens;
mixed *archive, *top_ten;
nosave int average;
nosave mixed *skills;
string _family_largest;
int _family_largest_num;
string _family_qp;
int _family_qp_num;
string _family_age;
int _family_age_num;
mapping _family_guilds;
mapping _family_guilds_num;
string _family_pk;
int _family_pk_num;
string _family_single_gender;
int _family_single_gender_num;
string _family_start_time;
int _family_start_time_num;
string _family_relationships;
int _family_relationships_num;

string _club_largest;
int _club_largest_num;
string _club_qp;
int _club_qp_num;
string _club_age;
int _club_age_num;
mapping _club_guilds;
mapping _club_guilds_num;
string _club_pk;
int _club_pk_num;
string _club_single_gender;
int _club_single_gender_num;
string _club_start_time;
int _club_start_time_num;

void calculate_family_and_club_info();

void create() {
    seteuid( (string)"/secure/master"->
      creator_file( file_name( this_object() ) ) );
    guild_top_tens = ([ ]);
    _family_guilds = ([ ]);
    _club_guilds = ([ ]);
    _family_guilds_num = ([ ]);
    _club_guilds_num = ([ ]);
    archive = ({ });
    top_ten = ({ });
    call_out( "check_tables", 5 );
    call_out( "check_family_and_club_info", 10 );
    if ( file_size( SAVE_FILE +".o" ) > 0 ) {
        unguarded( (: restore_object, SAVE_FILE :) );
        if (!_family_guilds_num) {
            _family_guilds_num = ([ ]);
        }
        if (!_club_guilds_num) {
            _club_guilds_num = ([ ]);
        }
    }
} /* setup() */

private void save_me() {
    unguarded( (: save_object, SAVE_FILE :) );
} /* save_me() */

/**
 * Some sort of weight average used in the rating calulation.
 * @return the weighted average
 */
int query_average() { return average; }
/**
 * The skils list with weights.  This is used to help generate the
 * rating.
 * @return the skils list with ratings
 */
mixed *query_skills() { return skills; }

/**
 * The mapping which is the guild top ten tables.
 * @return the guild top ten mapping
 */
mapping query_guild_top_tens() { return guild_top_tens; }

/** @ignore yes */
void set_guild_top_tens( mapping map ) {
    guild_top_tens = map;
    save_me();
} /* set_guild_top_tens() */

/**
 * Add in a new table.  This should be used when a new guild is created
 * and their table needs to be defined.
 * @param word the name of the table to create
 */
void add_guild_top_ten( string word ) {
    if ( guild_top_tens[ word ] ) {
        return;
    }
    guild_top_tens[ word ] = ({ });
    save_me();
} /* add_guild_top_ten() */

/**
 * The complete archived top ten table.
 * @return the archived top ten table
 */
mixed *query_archive() { return archive; }

/** @ignore yes */
void set_archive( mixed *args ) {
    archive = args;
    save_me();
} /* set_archive() */

/**
 * Return the complete list of top ten tables.
 * @return the array of arrays being the top ten tab;les
 */
mixed *query_top_ten() { return top_ten; }

/** @ignore yes */
void set_top_ten( mixed *args ) {
    top_ten = args;
    save_me();
} /* set_top_ten() */

/** @ignore yes */
int check_person( mixed *args, string guild ) {
    int i;
    string word;

    if ( !guild ) {
        sscanf( args[ 0 ], "%s (%s)", word, guild );
    } else {
        word = args[ 0 ];
    }

    if ( !PLAYER_HANDLER->test_user( word ) ||
      !PLAYER_HANDLER->test_active( word ) ||
      PLAYER_HANDLER->test_creator( word ) ||
      PLAYER_HANDLER->test_property( word, "no_score" ) ||
      PLAYER_HANDLER->test_property( word, "no score" ) ) {
        return 0;
    }

    if ( "/obj/handlers/player_handler"->test_guild( word, guild ) != "/std/guilds/"+ guild )
        return 0;
    if ( ( (int)PLAYER_HANDLER->test_last( word ) < time() - ARCHIVE_TIME ) ) {
        if ( args[ 1 ] > ARCHIVE_RATING ) {
            args[ 0 ] = word +" ("+ guild +")";
            for ( i = sizeof( archive ) - 1; i > -1; i-- ) {
                if ( archive[ i ][ 0 ] == args[ 0 ] ) {
                    if ( archive[ i ][ 1 ] > args[ 1 ] )
                        archive[ i ] = args + ({ time() });
                    break;
                }
            }
            if ( i == -1 ) {
                archive += ({ args + ({ time() }) });
            }
        }
        return 0;
    }
    return 1;
} /* check_person() */

/**
 * Wander over the top ten tables checking to see if everyone
 * still exists.
 */
void check_tables() {
    int i;
    string word;

    for ( i = sizeof( top_ten ) - 1; i > -1; i-- ) {
        if ( !check_person( copy( top_ten[ i ] ), 0 ) ) {
            top_ten = delete( top_ten, i, 1 );
        }
    }
    foreach ( word in keys( guild_top_tens ) ) {
        for ( i = sizeof( guild_top_tens[ word ] ) - 1; i > -1; i-- ) {
            if ( !check_person( copy( guild_top_tens[ word ][ i ] ), word ) ) {
                guild_top_tens[ word ] = delete( guild_top_tens[ word ], i, 1 );
            }
        }
    }
    save_me();
} /* check_tables() */

/**
 * Returns the ordered list of people on the top ten list.  If the table
 * name is 0 or it is "main" the main table is checked.  If the 
 * table name is "archive" then the archive is used.
 * @param table_name the name of the tanble to check
 * @return the array of arrays of the top ten information
 * @see /include/top_ten_tables.h
 */
mixed *query_ordered_table( string table_name ) {
    int i, highest_loc, highest_num;
    mixed *ret, *args;

    if ( !table_name || table_name == "main") {
        args = top_ten;
    } else {
        if ( table_name == "archive" ) {
            args = archive;
        } else {
            if ( !guild_top_tens[ table_name ] ) {
                return ({ });
            }
            args = guild_top_tens[ table_name ];
        }
    }
    ret = ({ });
    while ( sizeof( args ) ) {
        highest_loc = highest_num = 0;
        for ( i = 0; i < sizeof( args ); i++ ) {
            if ( args[ i ][ TOP_TEN_RATING ] > highest_num ) {
                highest_num = args[ i ][ TOP_TEN_RATING ];
                highest_loc = i;
            }
        }
        ret += ({ args[ highest_loc] });
        args = args[ 0 .. highest_loc - 1 ] + args[ highest_loc + 1 .. ];
    }
    return ret;
} /* query_orderd_table() */

/** @ignore yes */
mixed *remove_name( string word, mixed *args ) {
    int i;

    if ( !sizeof( args ) ) {
        return ({ });
    }
    for ( i = sizeof( args ) - 1; i > -1; i-- ) {
        if ( word == explode( args[ i ][ TOP_TEN_NAME ], " " )[ 0 ] ) {
            args = delete( args, i, 1 );
        }
    }
    return args;
} /* remove_name() */

/** @ignore yes */
int *find_lowest( mixed *args ) {
    int i, lowest_loc, lowest_num;

    if ( !sizeof( args ) ) {
        return ({ 0, 0 });
    }
    lowest_loc = 0;
    lowest_num = args[ 0 ][ TOP_TEN_RATING ];
    for ( i = 1; i < sizeof( args ); i++ ) {
        if ( lowest_num > args[ i ][ TOP_TEN_RATING ] ) {
            lowest_loc = i;
            lowest_num = args[ i ][ TOP_TEN_RATING ];
        }
    }
    return ({ lowest_loc, lowest_num });
} /* find_lowest() */

/** @ignore yes */
int query_skill_weight( string skill ) {
    int total;
    string *next;

    next = (string *)SKILL_OB->query_immediate_children( skill );
    if ( !sizeof( next ) ) {
        return 1;
    }
    foreach ( skill in next ) {
        total += query_skill_weight( skill );
    }
    return total;
} /* query_skill_weight() */

int calculate_age_modifier( object thing, int algorithm ) {

    if ( algorithm ) { 
        return ( -( thing->query_time_on() - ( thing->query_refresh_time() ? ( time() - thing->query_refresh_time() ) + thing->query_time_on() : 0 ) ) / 86400 );
    }
    return ( -thing->query_time_on() / 86400 );
} /* calculate_start_time() */ 

/**
 * Figure out the rating for the player.
 * @param thing the object to get the ratingof
 * @return the current rating
 */
int calculate_rating( object thing ) {
    int i, j, rating, *bonuses;

    rating = (int)thing->query_level();
    if ( !skills ) {
        skills = ({ });
        for ( i = 0; i < sizeof( STD_SKILLS ); i += SKILL_ARR_SIZE ) {
            skills += ({
              (string *)SKILL_OB->query_immediate_children( STD_SKILLS[ i ] )
            });
            skills += ({
              sizeof( skills[ <1 ] ),
              query_skill_weight( STD_SKILLS[ i ] )
            });
            average += skills[ <1 ];
        }
        average /= sizeof( skills ) / 3;
    }
    bonuses = allocate( sizeof( skills ) / 3 );
    for ( i = sizeof( bonuses ) - 1; i > -1; i-- ) {
        for ( j = skills[ 3 * i + 1 ] - 3; j > -1; j-- ) {
            bonuses[ i ] +=
            (int)thing->query_skill_bonus( skills[ 3 * i ][ j ], 1 );
        }
        bonuses[ i ] = ( ( bonuses[ i ] / skills[ 3 * i + 1 ] ) *
          skills[ 3 * i + 2 ] ) / average;
    }
    bonuses = sort_array( bonuses, -1 );
    j = sizeof( bonuses );
    for ( i = 0; i < j; i++ ) {
        rating = 2 * rating + bonuses[ i ];
        if ( !i && userp( thing ) ) {
            rating += (int)LIBRARY->
            query_quest_points( (string)thing->query_name() );
        }
    }

    if( rating > APEX_LIMIT && userp( thing ) ) {
        rating = AGE * (rating - APEX_LIMIT) / AGE_DIVIDER + APEX_LIMIT;
    }

    return rating;
} /* calculate_rating() */

/**
 * Called when a player advances their skills.
 * @param word the skill being advanced
 * @param thing the player which advanced their skils
 */
void player_skill_advance( string word, object thing ) {
    int rating, *lowest;
    string name;
    mixed *guild_top_ten;

    if ( thing->query_creator() ) {
        return;
    }
    if ( thing->query_property( "guest" ) ) {
        return;
    }
    if ( thing->query_property( "no_score" ) ) {
        return;
    }
    if ( thing->query_property( "no score" ) ) {
        return;
    }
    guild_top_ten = guild_top_tens[ word ];
    if ( !guild_top_ten ) {
        return;
    }
    name = (string)thing->query_name();
    rating = calculate_rating( thing );
    guild_top_ten = remove_name( name, guild_top_ten );
    lowest = find_lowest( guild_top_ten );
    if ( sizeof( guild_top_ten ) < GUILD_TOP_TEN_SIZE ) {
        guild_top_ten += ({
          ({ name, rating, (int)thing->query_level(),
            -(int)thing->query_time_on() }) });
    } else {
        if ( rating > lowest[ 1 ] ) {
            guild_top_ten[ lowest[ 0 ] ] =
            ({ name, rating, (int)thing->query_level(),
              -(int)thing->query_time_on() });
        }
    }
    guild_top_tens[ word ] = guild_top_ten;
    //   save_me();
    top_ten = remove_name( name, top_ten );
    lowest = find_lowest( top_ten );
    if ( sizeof( top_ten ) < TOP_TEN_SIZE ) {
        top_ten += ({
          ({ name +" ("+ word +")", rating,
            (int)thing->query_level(), -(int)thing->query_time_on() }) });
    } else {
        if ( rating > lowest[ 1 ] ) {
            top_ten[ lowest[ 0 ] ] =
            ({ name +" ("+ word +")", rating,
              (int)thing->query_level(), -(int)thing->query_time_on() });
        }
    }
    //   save_me();
} /* player_skill_advance() */

/**
 * Remove the named player from the named table.
 * @param word1 the name of the player
 * @param word2 the name of the table, 0 for the main table
 */
varargs void excise_name( string word1, string word2 ) {
    if ( !word2 ) {
        top_ten = remove_name( word1, top_ten );
    } else {
        if ( guild_top_tens[ word2 ] ) {
            guild_top_tens[ word2 ] = remove_name( word1,
              guild_top_tens[ word2 ] );
        }
    }
    save_me();
} /* excise_name() */

/**
 * Stuff to make sure that all the clubs still exist.
 */
void check_family_and_club_info() {
    int offset;
    string bing;
    string guild;

    _family_qp_num = 0;
    _family_age_num = 0;
    _family_largest_num = 0;
    if (!mapp(_family_guilds)) {
        _family_guilds = ([ ]);
    }

    if (!mapp(_club_guilds)) {
        _club_guilds = ([ ]);
    }

    // Do these first so that the table is not stuffed up.
    if (_family_qp && !CLUB_HANDLER->is_family(_family_qp)) {
        _family_qp = 0;
        _family_qp_num = 0;
    }
    if (_family_age && !CLUB_HANDLER->is_family(_family_age)) {
        _family_age = 0;
        _family_age_num = 0;
    }
    if (_family_largest && !CLUB_HANDLER->is_family(_family_largest)) {
        _family_largest = 0;
        _family_largest_num = 0;
    }
    if (_family_single_gender && !CLUB_HANDLER->is_family(_family_single_gender)) {
        _family_single_gender = 0;
        _family_single_gender_num = 0;
    }
    if (_family_pk && !CLUB_HANDLER->is_family(_family_pk)) {
        _family_pk = 0;
        _family_pk_num = 0;
    }

    if (_club_qp && !CLUB_HANDLER->is_club(_club_qp)) {
        _club_qp = 0;
        _club_qp_num = 0;
    }
    if (_club_age && !CLUB_HANDLER->is_club(_club_age)) {
        _club_age = 0;
        _club_age_num = 0;
    }
    if (_club_largest && !CLUB_HANDLER->is_club(_club_largest)) {
        _club_largest = 0;
        _club_largest_num = 0;
    }
    if (_club_single_gender && !CLUB_HANDLER->is_club(_club_single_gender)) {
        _club_single_gender = 0;
        _club_single_gender_num = 0;
    }
    if (_club_pk && !CLUB_HANDLER->is_club(_club_pk)) {
        _club_pk = 0;
        _club_pk_num = 0;
    }

    offset = 3;
    foreach (guild, bing in _family_guilds) {
        if (stringp(bing)) {
            if (!CLUB_HANDLER->is_family(bing)) {
                map_delete(_family_guilds, guild);
                map_delete(_family_guilds_num, guild);
            }
        } else {
            map_delete(_family_guilds, guild);
            map_delete(_family_guilds_num, guild);
        }
    }

    foreach (guild, bing in _club_guilds) {
        if (stringp(bing)) {
            if (!CLUB_HANDLER->is_club(bing)) {
                map_delete(_club_guilds, guild);
                map_delete(_club_guilds_num, guild);
            }
        } else {
            map_delete(_club_guilds, guild);
            map_delete(_club_guilds_num, guild);
        }
    }
    save_me();
} /* calculate_family_and_club_info() */

void inform_of_club(string club,
  int family,
  int type,
  mixed num) {
    string str;
    int bing;

    if (family) {
        switch (type) {
        case TOP_TEN_LARGEST_FAMILY:
            if (_family_largest_num < num ||
              _family_largest == club) {
                _family_largest_num = num;
                _family_largest = club;
                save_me();
            }
            break;
        case TOP_TEN_OLDEST_FAMILY:
            if (_family_age_num < num ||
              _family_age == club) {
                _family_age_num = num;
                _family_age = club;
                save_me();
            }
            break;
        case TOP_TEN_MOST_QUEST_POINTS:
            if (_family_qp_num < num ||
              _family_qp == club) {
                _family_qp_num = num;
                _family_qp = club;
                save_me();
            }
            break;
        case TOP_TEN_MOST_GUILD:
            foreach (str, bing in num) {
                if (_family_guilds_num[str] < bing ||
                  _family_guilds[str] == club) {
                    _family_guilds_num[str] = bing;
                    _family_guilds[str] = club;
                    save_me();
                }
            }
            break;
        case TOP_TEN_MOST_PKS:
            if (_family_pk_num < num ||
              _family_pk == club) {
                _family_pk_num = num;
                _family_pk = club;
                save_me();
            }
            break;
        case TOP_TEN_SINGLE_GENDER:
            if (_family_single_gender_num < num ||
              _family_single_gender == club) {
                _family_single_gender_num = num;
                _family_single_gender = club;
                save_me();
            }
            break;
        case TOP_TEN_OLDEST_LOGONS:
            if (_family_start_time_num < num ||
              _family_start_time == club) {
                _family_start_time_num = num;
                _family_start_time = club;
                save_me();
            }
            break;
        case TOP_TEN_MOST_RELATIONSHIPS:
            if (_family_relationships_num < num ||
              _family_relationships == club) {
                _family_relationships_num = num;
                _family_relationships = club;
                save_me();
            }
            break;
        }
    } else {
        switch (type) {
        case TOP_TEN_LARGEST_FAMILY:
            if (_club_largest_num < num ||
              _club_largest == club) {
                _club_largest_num = num;
                _club_largest = club;
                save_me();
            }
            break;
        case TOP_TEN_OLDEST_FAMILY:
            if (_club_age_num < num ||
              _club_age == club) {
                _club_age_num = num;
                _club_age = club;
                save_me();
            }
            break;
        case TOP_TEN_MOST_QUEST_POINTS:
            if (_club_qp_num < num ||
              _club_qp == club) {
                _club_qp_num = num;
                _club_qp = club;
                save_me();
            }
            break;
        case TOP_TEN_MOST_GUILD:
            foreach (str, bing in num) {
                if (_club_guilds_num[str] < bing ||
                  _club_guilds[str] == club ||
                  !_club_guilds[str]) {
                    _club_guilds_num[str] = bing;
                    _club_guilds[str] = club;
                    save_me();
                }
            }
            break;
        case TOP_TEN_MOST_PKS:
            if (_club_pk_num < num ||
              _club_pk == club) {
                _club_pk_num = num;
                _club_pk = club;
                save_me();
            }
            break;
        case TOP_TEN_SINGLE_GENDER:
            if (_club_single_gender_num < num ||
              _club_single_gender == club) {
                _club_single_gender_num = num;
                _club_single_gender = club;
                save_me();
            }
            break;
        case TOP_TEN_OLDEST_LOGONS:
            if (_club_start_time_num < num ||
              _club_start_time == club) {
                _club_start_time_num = num;
                _club_start_time = club;
                save_me();
            }
            break;
        }
    }
} /* inform_of_club() */

mixed *query_family_info() {
    return ({ _family_largest,
      _family_age,
      _family_qp,
      _family_guilds,
      _family_pk,
      _family_single_gender,
      _family_start_time,
      _family_relationships });
} /* query_family_info() */

mixed *query_club_info() {
    return ({ _club_largest,
      _club_age,
      _club_qp,
      _club_guilds,
      _club_pk,
      _club_single_gender,
      _club_start_time });
} /* query_club_info() */

/** @ignore yes */
void dest_me() {
    save_me();
    destruct(this_object());
} /* dest_me() */

/** @ignore yes */
void reset() {
    save_me();
} /* reset() */
