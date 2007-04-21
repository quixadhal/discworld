/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: playtesters.c,v 1.29 2002/08/11 16:26:47 drakkos Exp $
 * $Log: playtesters.c,v $
 * Revision 1.29  2002/08/11 16:26:47  drakkos
 * Various updates
 *
 * Revision 1.28  2002/07/19 02:16:35  drakkos
 * Adjust quota fix
 *
 * Revision 1.27  2002/07/16 22:35:49  pinkfish
 * Fix up the remove stuff to actually delete empty lists.
 *
 * Revision 1.26  2002/07/16 21:39:13  pinkfish
 * make it handle multiple destinations from the current location.
 *
 * Revision 1.25  2002/07/16 21:00:24  pinkfish
 * Add in jump points.
 *
 * Revision 1.24  2002/07/16 20:49:18  drakkos
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.23  2002/07/04 10:40:21  pinkfish
 * Fix up the logging.
 *
 * Revision 1.22  2002/07/04 10:35:22  drakkos
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.21  2002/02/21 21:27:20  arienne
 * Every IDEA report, every tenth BUG report and no TYPO reports are
 * now posted to the "playtesters" board.
 *
 *
 * Revision 1.20  2001/11/03 10:29:54  arienne
 * Added check that they actually are a playtester before putting them on
 * leave.  Ditto with off leave.
 *
 * Revision 1.17  2001/10/23 18:47:20  arienne
 * Added query_show_list_colour() -- like query_show_list(), except it Burns
 * your Eyes with Fluorescent Goodness.
 *
 * Revision 1.16  2001/05/13 16:59:42  arienne
 * Fixed up monthly roll-over so that it adds the new month to 'months'.
 *
 * Revision 1.11  2001/03/25 01:33:45  ceres
 * Stopped the 'ptesters' command loaded the player files of every playtesters
 *
 * Revision 1.9  2000/11/15 16:41:42  pinkfish
 * Make it wrap the other way.
 *
 * Revision 1.8  2000/11/14 20:28:51  pinkfish
 * Make it post wrapped note to the board.
 * .
 *
 * Revision 1.7  2000/11/14 20:22:53  terano
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.6  2000/03/29 06:05:21  ceres
 * rcsi /obj/handlers/playtesters.c
 * Modified to use the player handler directly.
 *
 * Revision 1.5  1999/12/01 09:28:21  taffyd
 * Added super new functions! Bonus functionality for no cost.
 *
 * Revision 1.4  1999/03/17 01:42:53  gruper
 * Added query_tester()
 *
 * Revision 1.3  1998/06/04 01:53:57  presto
 * Made it check the date every time a report is filed so that reports at the
 * beginning of a month go onto the correct total.  Long uptimes were causing
 * most of Jun 1st bugs to be filed under May, etc.
 *
 * Revision 1.2  1998/02/04 05:01:37  sin
 * test_age() sometimes returns a negative value (when the player is still logged in).  I added a wrapper that takes the absolute value.
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/**
 * Keeps track of all the current playtesters.  This keeps track of all
 * the current playtesters.
 */
#include <board.h>
#include <project_management.h>
#include <player_handler.h>
#include <playtesters.h>
#include <login.h>

#define SAVE_FILE "/save/playtesters"
#define LEVEL 150

#define NORMAL_PLAYTESTER 1
#define SENIOR_PLAYTESTER 2

// every IDEA report is being posted

// post every tenth BUG report
#define BUG_SKIP 10

// not posting any TYPO reports, or other sorts of report

// this is the counter for BUG reports, nothing else
private int count = 0;

private nosave int updating = 0;

private string* months = ({});
private mapping pts = ([]);
private mapping playtesters = ([]);
private mapping ages = ([]);
private mapping guilds = ([]);
private mapping bugs = ([]);
private mapping leave = ([]);
private mapping tmp_pts = ([ ]);
private mapping _jump_points;
private string *executives = ({ });

int query_leave (string);

/** @ignore yes */
void save_file() {
    unguarded( (: save_object, SAVE_FILE :) );
} /* save_file() */


void create() {
   seteuid( master()->creator_file( file_name( this_object() ) ) );

   months = ({ });
   pts = ([ ]);
   ages = ([ ]);
   guilds = ([ ]);
   bugs = ([ ]);
   playtesters = ([ ]); /// XXX temporary
   leave = ([ ]);
   tmp_pts = ([ ]);
   _jump_points = ([ ]);
   executives = ({ });
   
   if ( file_size( SAVE_FILE + ".o" ) > 0 )
      unguarded( (: restore_object, SAVE_FILE, 1 :) );
 
     
   call_out( "reset", 2 );
} /* create() */


/**
 * This method returns a list of the current play testers.  This is
 * a list of string names in an array.
 * @return the list of current playtesters
 */
string* query_playtesters() {
    return copy( keys( pts ) );
} /* query_playtesters() */

/**
 * This method tests to see if the given name is a playtester.
 * @return 1 if they are a playtester, 0 if they are not.
 * @see add_playtester()
 * @see remove_playtester()
 * @see query_senior_playtester()
 */
int query_playtester( string name ) {
  return !undefinedp( pts[ name ] );
} /* query_playtester() */

/**
 * This method should never ever be used.  Use of this call is meant to be
 * broken.  Old code that uses check_playtester() probably meant to use
 * query_playtester().  Use this instead (or better, query_tester()).  If you
 * wish to hire a playtester using this call (which is what it actually did!),
 * please look at hire_playtester() and add_playtester().
 * @see query_playtester()
 * @see query_tester()
 * @see hire_playtester()
 * @see add_playtester()
 * @return Always returns 0.
 */
int check_playtester( string ) {
    log_file( "PLAYTESTERS", file_name( previous_object() ) + " using "
              "deprecated check_playtester() call\n" );
    return 0;
} /* check_playtester() */

/**
 * This method tests to see if "person" is an authorized tester,
 * i.e. a playtester, creator or has the "test character" property.
 * @param person A string or object to determine tester status for.
 * @return 1 if they are a tester, 0 if they are not.
 * @see query_playtester()
 */
int query_tester( mixed person ) {
    object ob;
    string str;

    if ( stringp( person ) ) {
        ob = find_player( person );
        if ( !ob ) {
            return 0;
        }
        str = person;
    } else if ( objectp( person ) ) {
        ob = person;
        str = person->query_name();
    } else {
        return 0;
    }

    return ( ob->query_creator() ||
             ob->query_property( "test character" ) ||
             query_playtester( str ) );
} /* query_tester() */

/**
 * This method finds the age of the playtester from the player handler.  NOTE:
 * This causes the playerfile to be loaded if it hasn't been already - this
 * will cause LOTS OF LAG if done on a lot of playerfiles.
 * @param name the name of the playtester to check
 * @return an integer representing the age of the playtester in seconds.
 * Always >= 0.
 */ 
protected int get_age_raw( string name ) {
    int fu = PLAYER_HANDLER->test_age( name );
    if ( fu < 0 )
        fu = -fu;
    return fu;
} /* get_age_raw() */

/**
 * This method will get the age of a playtester from this handlers' cache.
 * @param name the name of the playtester to check
 * @return an integer representing the age of the playtester in
 * seconds.  Always >= 0.
 */
int get_age_cached( string name ) {
    if ( ages[ name ] < 0 )
        ages[ name ] = -ages[ name ];
    return ages[ name ];
} /* get_age_cached() */

/**
 * This method will update the playtesters' age if they are online and return
 * the value from the cache.
 * @param name the name of the playtester to check
 * @return an integer representing the age of the playtester in seconds.
 * Always >= 0.
 */
int get_age_uncached( string name ) {
    if ( find_player( name ) )
        ages[ name ] = find_player( name )->query_time_on();
    return get_age_cached( name );
} /* get_age_uncached() */

/**
 * This method will update the playtesters' age from the player handler.
 * NOTE: This causes the playerfile to be loaded if it hasn't been already -
 * this will cause LOTS OF LAG if done on a log of playerfiles.
 * @param name the name of the playtester to check
 * @return an integer representing the age of the playtester in seconds.
 * Always >= 0.
 */
int get_age_uncached_offline( string name ) {
    ages[ name ] = get_age_raw( name );
    return get_age_cached( name );
} /* get_age_uncached_offline() */

/**
 * This method determines if they are a senior playtester.
 * @param name the name of the playtester to check
 * @return 1 if they are a senior playteser, 0 if they are not
 * @see add_senior_playtester()
 * @see remove_senior_playtester()
 * @see query_playtester()
 */
int query_senior_playtester( string name ) {
    return ( pts[ name ] == SENIOR_PLAYTESTER );
} /* query_senior_playtester() */


/**
 * This method adds the playtester to the array of current play
 * testers.  NOTE: This uses get_age_raw(), which will cause lag if called on
 * lots of playerfiles.
 * @param name the name of the player to add
 * @return 1 if successful, 0 on failure
 * @see remove_playtester()
 * @see query_playtester()
 * @see add_senior_playtester()
 * @see valid_playtester()
 */
int add_playtester( string name ) {
    if ( pts[ name ] )
        return 0;
    log_file( "PLAYTESTERS", ctime( time() ) + ": " + name +
              " added as a playtester by %s\n", this_player()->query_name() );
    pts[ name ] = NORMAL_PLAYTESTER;
    ages[ name ] = get_age_uncached_offline( name );
    guilds[ name ] = PLAYER_HANDLER->test_guild( name );
    bugs[ name ] = ([ ]);
    if (tmp_pts[name]) {
      if (tmp_pts[name] == SENIOR_PLAYTESTER) {
        pts[name] = SENIOR_PLAYTESTER;
      }
      map_delete (tmp_pts, name);
    }
    PT_APPLICATION_HANDLER->delete_applicant (name);
    save_file();
    return 1;
} /* add_playtester() */

/**
 * This method adds the player as a senior playtester.  The playtester
 * needs to already be added as a normal playtester before becoming a
 * senior playtester.
 * @param name the name of the player to add
 * @return 1 on success, 0 on failure
 * @see remove_senior_playtester()
 * @see add_playtester()
 * @see query_senior_playtester()
 */
int add_senior_playtester( string name ) {
    if ( pts[ name ] != NORMAL_PLAYTESTER )
        return 0;
    log_file( "PLAYTESTERS", ctime( time() ) + ": " + name +
              " promoted to senior playtester by %s\n",
              this_player()->query_name() );
    pts[ name ] = SENIOR_PLAYTESTER;
    save_file();
    return 1;
} /* add_senior_playtester() */

/**
 * This method adds the player as a senior playtester.  The playtester
 * needs to already be added to become a senior playtester.
 * @param name the name of the player to remove
 * @return 1 on success, 0 on failure
 * @see add_senior_playtester()
 * @see remove_playtester()
 * @see query_senior_playtester()
 */
int remove_senior_playtester( string name, string reason ) {
    if ( pts[ name ] != SENIOR_PLAYTESTER )
        return 0;
    log_file( "PLAYTESTERS", ctime( time() ) + ": " + name +
              " demoted from senior playtester; Reason: " + reason +
              " by "+ this_player()->query_name() + "\n" );
    pts[ name ] = NORMAL_PLAYTESTER;
    save_file();
    return 1;
} /* remove_senior_playtester() */

/**
 * This method will remove the playtester from the current list of
 * play testers.
 * @param name the name of the player to remove
 * @param reason the reason for the removal
 * @return 1 on success, 0 on failure
 * @see add_playtester()
 * @see remove_senior_playtester()
 * @see query_playtester()
 */
int remove_playtester( string name, string reason ) {
    string str;
    if ( !query_playtester( name ) )
        return 0;
    str = ctime( time() ) +": "+ name + " removed as a playtester";
    if ( reason ) {
        str += "; Reason: " + reason;
    }
    str += " by "+ this_player()->query_name() + "\n" ;
    log_file( "PLAYTESTERS", str + "\n" );
    map_delete( pts, name );
    map_delete( ages, name );
    map_delete( guilds, name );
    map_delete( bugs, name );
    PROJECT_HANDLER->clear_playtesting_projects (({name }));
    save_file();
    return 1;
} /* remove_playtester() */

/**
 * THis method returns a string reason why they are an invalid play
 * tester.  NOTE: This uses the player_handler, which will cause lag if
 * called on lots of playerfiles.
 * @param name the player to check
 * @return the string reason, 0 if none
 * @see valid_playtester()
 */
string reason_invalid_playtester(string name) {
    if ( !PLAYER_HANDLER->test_user( name ) ) {
        return "not a user";
    }
    if ( PLAYER_HANDLER->test_creator( name ) ) {
        return "currently a creator";
    }
    if ( PLAYER_HANDLER->test_level( name ) < LEVEL ) {
        return "level is too low";
    }
    return 0; 
} /* reason_invalid_playtester() */

/**
 * This method determins if they are a valid playtester or not.  NOTE: This
 * uses reason_invalid_playtester(), which will cause lag if called on lots of
 * playerfiles.
 * @param name the name of the player to test
 * @return 1 if they are, 0 if they are not
 * @see reason_invalid_playtester()
 */
int valid_playtester( string name ) {
    return ( !stringp( reason_invalid_playtester( name ) ) );
} /* valid_playtester() */

/**
 * This method checks to make sure an object can be a playtester, and if so,
 * adds them.
 * @param thing The player object to add as a playtester
 * @return 1 on success, 0 on failure.
 */
int hire_playtester( object thing ) {
    string name;

    if ( !objectp( thing ) )
        return 0;
    name = thing->query_name();
    if ( !stringp( name ) )
        return 0;
    if ( !valid_playtester( name ) )
        return 0;
    return add_playtester( name );
} /* check_playtester() */

/**
 * This adjusts the quota of a playtester.
 *
 * @param name The query_name() of the playtester.
 * @param amount A integer to adjust by.
 */
void adjust_quota( string name, int amount ) {
    string* bits;
    string right_now;
    int* bing;
    int age;
    
    bits = explode( ctime( time() ), " " ) - ({ "" });
    right_now = bits[ 1 ] + " " + bits[ 4 ];

    if (!bugs[name][right_now]) {
      bugs[name][right_now] = 0;
    }
    bing = bugs[ name ][ right_now ];
    age = get_age_uncached( name );
    
    if ( arrayp( bing ) && ( sizeof( bing ) == 3 ) ) {
        bing[ 0 ] += amount;
        bing[ 1 ] += age - bing[ 2 ];
        bing[ 2 ] = age;
    } else {
        bing = ({ amount, 0, age });
    }
    bugs[ name ][ right_now ] = bing;
    save_file();
} /* adjust_quota() */

/**
 * This method determines if a playtester is valid or not, and if not will
 * automagically remove them (does not remove senior playtesters
 * automatically).  NOTE: This uses valid_playtester() and
 * reason_invalid_playtester(), which will cause lag if called on lots of
 * playerfiles.
 * @param name the name of the player to test
 * @see reason_invalid_playtester()
 */
protected void check_playtesters_one( string name ) {
    if ( !valid_playtester( name ) ) {
        remove_playtester( name, reason_invalid_playtester( name ) +
                           " [automatic]" );
    } else {
        adjust_quota( name, 0 );
    }
} /* check_playtesters_one() */

/**
 * This goes through the list of playtesters and checks to make
 * sure they are all still valid.  It does so slowly, using a call_out until
 * it gets to the end.
 * @param who A string* of the query_name()'s of the playtesters to check.
 * @see valid_playtester()
 * @see remove_playtester()
 */
void check_playtesters( string* who ) {
    string* bits;
    string right_now;
    
    if ( sizeof( who ) ) {
        check_playtesters_one( who[ 0 ] );
        if ( sizeof( who ) > 1 ) {
            call_out( "check_playtesters", random( 10 ) + 5, who[ 1.. ] );
        } else {
            bits = explode( ctime( time() ), " " ) - ({ "" });
            right_now = bits[ 1 ] + " " + bits[ 4 ];
            if ( member_array( right_now, months ) < 0 ) {
                months += ({ right_now });
                save_file();
            }
        }            
    }
} /* check_playtesters() */


/**
 * @ignore yes
 * This just fixes up the quotas to contain zeroes for months that they are
 * present but do not file reports.  It does this every 12 resets.
 */
void reset() {
    if ( !updating )
        call_out( "check_playtesters", 30, copy( keys( pts ) ) );
    
    updating++;
    updating %= 12;
} /* reset() */

/**
 * This is called when a playtester makes a bug report.
 * @param name the name of the playtester
 * @param type the type of report
 * @param file the file the report is on
 * @param text the text associated with the report
 */
void report_made( string name, string type, string file, string text ) {
    int post = 0;
   
    if ( !query_playtester( name ) )
        return;

    if ( regexp( type, "IDEA" ) ) {
        post = 1;
    } else if ( regexp( type, "BUG" ) ) {
        count = ( count + 1 ) % BUG_SKIP;
        post = ( count == 0 );
    }

    if ( post ) {
        BOARD_HAND->add_message( "playtester_bugs", capitalize( name ),
                                 type + " for " + file,
                                 sprintf( "%75-=s", text ) );
    }
    
    adjust_quota( name, 1 );
} /* report_made() */

/**
 * This is a nice way of showing a list of the current play testers.
 * It prints a table of the previous four and current months, with columns
 * of bug reports made and number of hours online spent.
 */
string query_show_list() {
    string result;

    result = "              ";
    foreach ( string month in months[ <5.. ] ) {
        result += "    " + month;
    }
    result += "\n";
   
    foreach ( string name in sort_array( query_playtesters(), 1 ) ) {
        if ( query_senior_playtester( name ) ) {
            result += "S ";
        } else {
            result += "  ";
        }
        
        if ( stringp( leave[ name ] ) )
            result += sprintf( "%-14s", "[" + name + "]" );
        else
            result += sprintf( " %-12s ", name );
        
        foreach ( string month in months[ <5.. ] ) {
            if ( sizeof( bugs[ name ][ month ] ) >= 2 )
                result += sprintf( "   %4d %4d", bugs[ name ][ month ][ 0 ],
                                   ( bugs[ name ][ month ][ 1 ] / 3600 ) );
            else
                result += "   ---- ----";
        }
        result += "\n";
    }
    return result;
} /* show_list() */

/**
 * This is a nice way of showing a list of the current play testers.
 * It prints a table of the previous four and current months, with columns
 * of bug reports made and number of hours online spent.
 */
string query_show_list_colour() {
    string result;
    string colour;
    int online, reports;
    string temp;

    result = "              ";
    foreach ( string month in months[ <5.. ] ) {
        result += "    " + month;
    }
    result += "\n";
   
    foreach ( string name in sort_array( query_playtesters(), 1 ) ) {
        reports = 0;
        online = 0;

        if ( query_senior_playtester( name ) ) {
            temp = "S ";
        } else {
            temp = "  ";
        }
        temp += sprintf( "%%s%-12s%%%%^RESET%%%%^", name );
        foreach ( string month in months[ <5.. ] ) {
	          if (query_leave (name)) {
                    colour = "%%^WHITE%%^";	            
	          }        			
            else if ( sizeof( bugs[ name ][ month ] ) >= 2 ) {

                reports += bugs[ name ][ month ][ 0 ];
                online += bugs[ name ][ month ][ 1 ];
                
                if ( bugs[ name ][ month ][ 1 ] == 0 )
                    colour = ( stringp( leave[ name ] ) ?
                               "%%^MAGENTA%%^" :
                               "%%^BOLD%%^%%^MAGENTA%%^" );
                else if ( bugs[ name ][ month ][ 0 ] >=
                     ( bugs[ name ][ month ][ 1 ] / 7200 ) )
                    colour = ( stringp( leave[ name ] ) ?
                               "%%^GREEN%%^" :
                               "%%^BOLD%%^%%^GREEN%%^" );
                else if ( bugs[ name ][ month ][ 0 ] <
                          ( bugs[ name ][ month ][ 1 ] / 24000 ) )
                    colour = ( stringp( leave[ name ] ) ?
                               "%%^RED%%^" :
                               "%%^BOLD%%^%%^RED%%^" );
                else
                    colour = ( stringp( leave[ name ] ) ?
                               "%%^ORANGE%%^" :
                               "%%^YELLOW%%^" );
                
                temp += sprintf( "   %s%4d %4d%%%%^RESET%%%%^",
                                 colour,
                                 bugs[ name ][ month ][ 0 ],
                                 ( bugs[ name ][ month ][ 1 ] / 3600 ) );
            } else
                temp += "   ---- ----";
        }
        temp += "\n";
        
        if ( online == 0 )
            colour = ( stringp( leave[ name ] ) ?
                       "%^BOLD%^%^WHITE%^" :
                       "%^BOLD%^%^MAGENTA%^" );
        else if ( reports >= ( online / 7200 ) )
            colour = ( stringp( leave[ name ] ) ?
                       "%^GREEN%^" :
                       "%^BOLD%^%^GREEN%^" );
        else if ( reports < ( online / 24000 ) )
            colour = ( stringp( leave[ name ] ) ?
                       "%^RED%^" :
                       "%^BOLD%^%^RED%^" );
        else
            colour = ( stringp( leave[ name ] ) ?
                       "%^ORANGE%^" :
                       "%^YELLOW%^" );
        
        result += sprintf( temp, colour );
    }
    return result;
} /* query_show_list_colour() */

/**
 * This method returns a mapping which contains which Guilds
 * the playtesters belong to.
 * @return a lovely mapping
 */
mapping query_playtester_guilds() {
    mapping r = ([]);
    foreach ( string pt, string guild in guilds ) {
        if ( arrayp( r[ guild ] ) )
            r[ guild ] += ({ pt });
        else
            r[ guild ] = ({ pt });
    }
    return r;
} /* query_playtester_guilds() */

/**
 * This method returns a mapping which contains which the playtesters mapped
 * to their guild objects.
 * @return a lovely mapping
 */
mapping query_guilds() {
    return copy( guilds );
} /* query_guilds() */

/** 
 * @ignore yes
 * This small utility function is used to update the guilds...
*/ 
void update_guilds() {
  string guild;
    foreach( string player, mapping data in playtesters ) { 
        if ( !sizeof (data[ "guild" ]) ) { 
            guild = PLAYER_HANDLER->test_guild( player );
            if (!sizeof (guild)) {
              guild = "/std/guilds/standard";
            }
            
            data[ "guild" ] = guild;
        }
    }

    save_file(); 
} /* update_guilds() */ 

/**
 * Returns the raw playtester data for processing!  DEPRECATED AND WILL NOT
 * RETURN UP TO DATE INFORMATION!
 * @see query_bugs()
 * @see query_months()
 */
mixed query_pt_data( string name ) {
    if ( !undefinedp( playtesters[ name ] ) )
        return copy( playtesters[ name ] );
    return 0;
} /* query_pt_data() */

/**
 * This will return bug information for a given playtester.  The mapping
 * returned will map months to arrays of ints.  The format of these arrays is:
 * ({ bugs, online_that_month, age_at_end_of_month }).
 * @see query_months()
 */
mapping query_bugs( string name ) {
    return copy( bugs[ name ] );
} /* query_bugs() */

/**
 * This returns a sorted list of months, earliest month first.
 */
string* query_months() {
    return copy( months );
} /* query_months() */

/**
 * This marks a playtester as "on leave", logs it, with a reason.
 */
int set_leave( string name, string reason ) {
    if ( pts[ name ] ) {
        leave[ name ] = reason;
        log_file( "PLAYTESTERS", ctime( time() ) + ": " + name + " marked as "
                  "on leave: " + reason + "\n" );
        save_file();
        return 1;
    } else {
        return 0;
    }
} /* set_leave */

/**
 * This marks a playtester as no longer "on leave", and logs it.
 */
int reset_leave( string name ) {
    if ( leave[ name ] ) {
        map_delete( leave, name );
        log_file( "PLAYTESTERS", ctime( time() ) + ": " + name + " no longer "
                  "marked as on leave\n" );
        save_file();
        return 1;
    } else {
        return 0;
    }
} /* reset_leave */

/**
 * This tests whether a playtester is "on leave".
 */
int query_leave( string name ) {
    return stringp( leave[ name ] );
} /* query_leave */



/* What follows is a load of spam for converting from the old playtesters'
   handler save file to the new one.  You should never ever ever need to call
   this lot.  That's why there's a dodgy #ifdef around it.  Cool?  Cool.
   -- Arienne */

#ifdef PLAYTESTERS_HANDLER_OLD_STYLE_CONVERSION

/**
 * @ignore yes
 * This is a utility to transfer over the data from the old-type (buggy and
 * inefficient handler) to the new type.
 */
void transfer_data_callout( string* pts ) {
    string pt;
    mixed bing;
    int lastage;
    int nowage;
    
    if ( sizeof( pts ) ) {
        pt = pts[ 0 ];
        
        nowage = get_age_uncached_offline( pt );
        lastage = nowage - playtesters[ pt ][ "age" ];

        guilds[ pt ] = PLAYER_HANDLER->test_guild( pt );
        ages[ pt ] = nowage;
        bugs[ pt ] = ([ ]);
        
        foreach ( string s in keys( playtesters[ pt ] ) -
                  ({ "senior", "age", "current age", "guild" }) ) {

            bing = playtesters[ pt ][ s ];

            if ( arrayp( bing ) ) {
                bugs[ pt ][ s ] = ({ bing[ 0 ], bing[ 1 ], nowage });
            } else if ( intp( bing ) ) {
                bugs[ pt ][ s ] = ({ bing, lastage, nowage });
            }
        }

        tell_creator( "arienne", "Done " + pt + "!\n" );
        
        if ( sizeof( pts ) > 1 ) {
            call_out( "transfer_data_callout", 5, pts[ 1.. ] );
        } else {
            save_file();
            tell_creator( "arienne", "Done the transfer!\n" );
        }
    }
}

/**
 * @ignore yes
 * This is a utility to transfer over the data from the old-type (buggy and
 * inefficient handler) to the new type.
 */
void transfer_data_to_new_system() {
    foreach ( string pt in keys( playtesters ) ) {
        if ( playtesters[ pt ][ "senior" ] )
            pts[ pt ] = SENIOR_PLAYTESTER;
        else
            pts[ pt ] = NORMAL_PLAYTESTER;
    }
    call_out( "transfer_data_callout", 5, copy( keys( playtesters ) ) );
} /* transfer_data_to_new_system() */

#endif

/**
 * This method returns the current mapping of jump points.
 * @return the jump points
 */
mapping query_jump_points() {
   if (!_jump_points) {
      _jump_points = ([ ]);
   }
   return copy(_jump_points);
}

/**
 * This method adds a jump point to the list.
 * @param from the from location
 * @param to the to location
 */
void add_jump_point(string from, string to) {
   if (!_jump_points[from]) {
      _jump_points[from] = ({ });
   }
   _jump_points[from] += ({ to });
   save_file();
}

/**
 * This method removes a specific jump point from the list.
 * @param from the specific jump point to remove
 */
void remove_jump_point_from(string from) {
   map_delete(_jump_points, from);
   save_file();
}

/**
 * This method removes a specific jump point from the list.
 * @param from the from location to remove
 * @param to the to location to remove
 */
void remove_jump_point(string from, string to) {
   if (_jump_points[from]) {
      _jump_points[from] -= ({ to });
      if (!sizeof(_jump_points[from])) {
         map_delete(_jump_points, from);
      }
      save_file();
   }
}

/**
 * This method finds the destination from the start location.
 * @param from the location to find the jump point too
 */
string* query_jump_destination(string from) {
   return _jump_points[from];
}

/**
 * This method adds the player as a playtester executive.  The playtester
 * needs to already be added as a playtester before becoming a
 * playtester executive.
 * @param name the name of the player to add
 * @return 1 on success, 0 on failure
 * @see remove_pt_exec()
 * @see add_playtester()
 * @see query_pt_exec()
 */
int add_pt_exec( string name ) {
    if ( undefinedp (pts[ name ]))
        return 0;
    log_file( "PLAYTESTERS", ctime( time() ) + ": " + name +
              " promoted to playtester executive by %s\n",
              this_player()->query_name() );
    executives += ({ name });
    save_file();
    return 1;
} /* add_senior_playtester() */

/**
 * This method removes the player as a playtester executive.  
 * @param name the name of the player to add
 * @return 1 on success, 0 on failure
 * @see add_pt_exec()
 * @see add_playtester()
 * @see query_pt_exec()
 */
int remove_pt_exec( string name ) {
    if ( !undefinedp (pts[ name ]))
        return 0;
    if (member_array (name, executives) == -1) {
      return 0;
    }
    
    log_file( "PLAYTESTERS", ctime( time() ) + ": " + name +
              " removed from playtester executive by %s\n",
              this_player()->query_name() );
    executives -= ({ name });
    save_file();
    return 1;
} /* add_senior_playtester() */

int query_pt_exec(string name) {
  if (member_array (name, executives) == -1) {
    return 0;
  }
  return 1;
}

int query_exec_access (string name) {
  if (master()->query_senior(name)) {
    return 1;
  }
  if (query_pt_exec (name)) {
    return 1;
  }
  if (query_senior_playtester (name)) {
    return 1;
  }
  if (load_object ("/d/playtesters/master")->query_deputy (name)) {
    return 1;
  }  

  return 0;
}
