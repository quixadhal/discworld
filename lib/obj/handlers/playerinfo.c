/*
 * $Locker:  $
 * $Id: playerinfo.c,v 1.63 2003/05/31 18:40:26 ceres Exp $
 */

/**
 * Playerinfo database handler.
 *
 * This handler collects all the fascinating information about players and
 * their sins. At the moment, the following events are supported:
 * replace (all replacements, added by Presto's wand),
 * gag (gagging and ungagging, added by the gagger),
 * suspend (player suspension, added by the "suspend" command),
 * meteor (meteoring a player, added by the "meteor" command),
 * multiplay (various events added by the multiplayer handler),
 * harassment (comments about cases of harassment, added via 'addevent'),
 * misc (misc. comments, added via 'addevent'),
 * cheat (currently unused)
  *
 * The "replace" and "multiplay" events are debounced (merged). In addition,
 * the "replace" events expire in 30 days.
 *
 * @author Fiona
 */
#include <playerinfo.h>
#include <player_handler.h>
#include <refresh.h>
#include <applications.h>
#include <newbiehelpers.h>

// This is where our save files go
#define SAVE_DIR         "/save/playerinfo"
#define SAVE_FILE        ( SAVE_DIR + "/handler_data" )
#define NO_ACCESS_LOG    6

// Originators of function calls. Checked for security reasons
#define LIAISON_WAND       "/d/liaison/items/wand"
#define LIAISON_SCEPTRE    "/d/liaison/items/mort_sceptre"
#define LIAISON_PEN        "/d/liaison/items/quota_pen"
#define LIAISON_INTERVIEW  "/d/liaison/utils/interview"
#define MULTIPLAY          "/obj/handlers/multiplayer"
#define GAG                "/cmds/creator/gag"
#define UNGAG              "/cmds/creator/ungag"
#define METEOR             "/cmds/creator/meteor"
#define FRY                "/cmds/creator/fry"
#define REPORT_COMMAND     "/cmds/creator/playerinfo"
#define ADD_COMMAND        "/cmds/creator/addevent"
#define DELETE_COMMAND     "/cmds/creator/delevent"
#define ADDALT_COMMAND     "/cmds/creator/addalt"
#define DELALT_COMMAND     "/cmds/creator/delalt"
#define FAMILY_COMMAND     "/cmds/creator/family"
#define SUSPENDER          "/secure/bastards"
#define SHOWHELP_COMMAND   "/cmds/creator/show_help"
#define PATRICIAN_PALACE   "/d/am/patrician/patrician"
#define REFRESH_HANDLER    "/obj/handlers/refresh"
#define PRISON             "/d/sur/beta/prison/dungeon"
#define FETCH_COMMAND      "/cmds/creator/fetch"
#define REARRANGE_COMMAND  "/cmds/guild-race/rearrange"
#define PATRICIAN_PT       "/d/playtesters/handlers/applications"
#define PATRICIAN_COMPLAIN "/d/am/patrician/pat_complaints"
#define RUN_HANDLER        "/obj/handlers/guild_things/run"
#define BANISH_COMMAND     "/secure/cmds/creator/banish"
#define FAIRY_GODMOTHER    "/obj/handlers/fairy_godmothers"

#define CACHE_SIZE 150

// These don't really belong here... and we have /include/colour.h...
// ...oh well.
#define CL_CREATOR      "%^CYAN%^"
#define CL_EVENT        "%^RED%^"
#define CL_RESET        "%^RESET%^"
#define CL_HEADER       "%^RED%^"

// This is the interval at which debounced events are saved.
#define DEBOUNCE_PERIOD (60*30)

// This is the interval between consecutive checks for timeouts.
// One day is suffucuent.
#define TIMEOUT_PERIOD  (60*60*24)


//This is one week.
#define ONE_WEEK           ( 60 * 60 * 24 * 7 )

nosave inherit "/std/object";

// Function prototypes
mapping          query_timeouts();
protected string filename(string name);
string *         query_events();
protected int    query_debounced(string event);
protected int    query_lord_only(string event);
int              query_source_ok(string event, string source);
protected int    query_deleter_ok(string event, object deleter);
protected int    query_can_add(string e, object p);
protected int    query_can_delete(string e, object p);
protected int    query_can_handle_alts(object p);
protected void   do_debouncing(string player, class dbentry entry);
protected void   do_timeout();
protected void   load_player(string player);
protected void   save_player(string player);
void             player_remove(string player);
int              add_entry(object creator, string player, string event,
                   string comment, mixed *extra);
int              delete_entry(object creator, string player, string event,
                   int n);
protected string query_header(string player);
protected string query_entry(int idx, class dbentry e,
                   string display_name);
string           query_access_log(object source, string player, int lastn);
string           query_event(object source, string player, string event);
string           add_alt(object creator, string player, string alt);
string           delete_alt(object creator, string player, string alt);
mapping          query_alerts();
int              query_alerts_for( string player );
int              is_alert( string player, int idx );
int              acknowledge_alert( object creator, string player,
                   string event, int idx, string update, mixed * extra );
void             clear_alerts_for( string player );
int              increment_alerts_for( string player );
int              decrement_alerts_for( string player );
void             correct_alts_for( string player );
void player_deleted(mixed player, int deleted);
void fix_alts(mixed player);

class source {
   string *add;
   string *delete;
}

// This is where the current player's info resides.
private class playerinfo    dossier;
// Event timeout information (in seconds)
private nosave mapping      timeouts    = ([ "replace" : (60*60*24*30) ]);

// Ok guys, NEVER do things like this, it is highly poisonous...
private int local_time;
private nosave mapping _sources;

nosave mapping _alerts; // format: ([ "playername1":n, "playername2":n ... ])
                 // where n is the number of alerts in their dossier.
nosave string * _lordonly;

private nosave int correcting_alts; // used to control the alt correction
                                    // process so we don't loop infinitely.
private nosave int no_recurse;

private nosave mapping _dossier_cache;
private nosave int _dossier_cache_hits;
private nosave int _dossier_total;

void save_handler_data() {
   mapping vars = ([ ]);
   string tmp;

   vars["alerts"] = _alerts;

   tmp = save_variable( vars );
   
   unguarded( (: write_file, SAVE_FILE, tmp, 1 :) );
} /* save_handler_data() */

void load_handler_data() {
   mapping vars;
   string tmp;

   if( file_size( SAVE_FILE ) <= 0 )
      return;

   tmp = unguarded( (: read_file, SAVE_FILE :) );
   vars = restore_variable( tmp );

   _alerts = vars["alerts"];
} /* load_handler_data() */

void create() {
    ::create();
    _sources = ([
        "cheat":        new(class source,
                            add : ({ }),
                            delete : ({ DELETE_COMMAND })),
        "discipline":   new(class source,
                            add : ({ ADD_COMMAND, PRISON, REPORT_COMMAND,
                                     BANISH_COMMAND }),
                            delete : ({ DELETE_COMMAND })),
        "email":        new(class source,
                            add : ({ }),
                            delete : ({ DELETE_COMMAND })),
        "family":       new(class source,
                            add : ({ FAMILY_COMMAND }),
                            delete : ({ DELETE_COMMAND })),
        "gag":          new(class source,
                            add : ({ GAG, UNGAG,
                                     ADD_COMMAND, REPORT_COMMAND }),
                            delete : ({ DELETE_COMMAND })),
        "harassment":   new(class source,
                            add : ({ ADD_COMMAND, REPORT_COMMAND }),
                            delete : ({ DELETE_COMMAND })),
        "meteor":       new(class source,
                            add : ({ METEOR, FRY }),
                            delete : ({ DELETE_COMMAND })),
        "misc":         new(class source,
                            add : ({ ADD_COMMAND, APPLICATIONS_HANDLER,
                                     REARRANGE_COMMAND, PATRICIAN_PT,
                                     PATRICIAN_COMPLAIN, RUN_HANDLER,
                                     REPORT_COMMAND, NEWBIEHELPERS_HANDLER, 
                                     LIAISON_SCEPTRE, 
                                     LIAISON_PEN, FAIRY_GODMOTHER,
                                     LIAISON_INTERVIEW }),
                            delete : ({ DELETE_COMMAND })),
        "multiplay":    new(class source,
                            add : ({ MULTIPLAY, ADD_COMMAND,
                                     REPORT_COMMAND }),
                            delete : ({ DELETE_COMMAND })),
        "replace":      new(class source,
                            add : ({ LIAISON_WAND, 
                                     ADD_COMMAND, FETCH_COMMAND, 
                                     REPORT_COMMAND }),
                            delete : ({ DELETE_COMMAND })),
        "showhelp":     new(class source,
                            add : ({ SHOWHELP_COMMAND }),
                            delete : ({ DELETE_COMMAND })),
        "suspend":      new(class source,
                            add : ({ SUSPENDER }),
                            delete : ({ })),
        "alert":        new(class source,
                            add : ({ ADD_COMMAND, REPORT_COMMAND }),
                            delete : ({ DELETE_COMMAND })),
        "refresh":      new(class source,
                            add : ({ REFRESH_HANDLER }),
                            delete : ({ DELETE_COMMAND })),
        "delete":      new(class source,
                           add : ({ REFRESH_HANDLER }),
                           delete : ({ DELETE_COMMAND }))
    ]);

    _alerts = ([ ]);
    _lordonly = ({ });
    _dossier_cache = ([ ]);
    _dossier_cache_hits = 0;
    _dossier_total = 0;

    seteuid("Room");
    load_handler_data();


} /* create() */

// Return the mapping of event timeouts
mapping query_timeouts() {
    return timeouts;
} /* query_timeouts() */

string query_name() {
    return "playerinfo handler";
}

/**
 * Give the filename for the player's savefile.
 * @param name the name of the player
 * @return the name of the playerinfo file
 * @ignore
 */
protected string filename(string name) {
    string p = lower_case(name);
    return sprintf("%s/%c/%s.o",SAVE_DIR,p[0],p);
} /* filename() */

/**
 * Answer the list of all possible events.
 * @return array of all event types recognized by the playerinfo handler
 */
string *query_events() {
    return keys(_sources);
} /* query_events() */

/**
 * Check if the event should be debounced.
 * @param event the name of the event
 * @return nonzero if the given event is to be debounced
 * @ignore
 */
protected int query_debounced(string event) {
    return (event == "replace" || event == "multiplay" ||
            event == "gag" || event == "misc" ||
            event == "discipline" || event == "showhelp" ||
            event == "suspend");
} /* query_debounced() */

/**
 * Check if the event is lords-only.
 * @param event the name of the event
 * @return nonzero if the event can only be added by lords
 * @ignore
 */
protected int query_lord_only(string event) {
    return ( member_array( event, _lordonly ) >= 0 ) ? 1 : 0;
} /* query_lord_only() */

/**
 * Check if the event has come from the valid source.
 * @param event the name of the event
 * @param source the object trying to add the event
 * @return nonzero if the event can be added by the given source
 */
int query_source_ok(string event, string source) {
    // 0 means no check should be done; ({ }) means nobody can add it
    string *reqd;

    reqd = _sources[event]->add;

    if(reqd == 0) {
        return 1;
    }
    if(sizeof(reqd) == 0) {
        return 0;
    }
    return member_array(source,reqd) >= 0;
} /* query_source_ok() */

/**
 * Check if the request to delete an event came from the valid source.
 * @param event the name of the event
 * @param remover the object that tries to remove the event
 */
protected int query_deleter_ok(string event, object deleter) {
    // 0 means no check should be done; ({ }) means nobody can delete it
    string *reqd;

    reqd = _sources[event]->delete;

    if (reqd == 0) {
        return 1;
    }
    if (sizeof(reqd) == 0) {
        return 0;
    }
    return member_array(base_name(deleter),reqd) >= 0;
} /* query_deleter_ok() */

/**
 * Check if the player is allowed to add the event.
 * @param e the name of the event
 * @param p the player who is trying to add it
 * @return nonzero of the player is allowed to add the event
 * @ignore
 */
protected int query_can_add(string e, object p) {
    if(member_array(e,query_events()) < 0)
        return 0;
    if(!interactive(p))
        return 1;
    return !(query_lord_only(e) && !p->query_lord());
} /* query_can_add() */

/**
 * Check if the player is allowed to perform delete operations.
 * @param p the player who is trying to delete something
 * @ignore
 */
protected int query_can_delete(string e, object p) {
    if( !interactive(p) ) {
        return 1;
    }
    if( e == "misc" || e == "replace" ) {
        return master()->query_senior( p->query_name() ) ||
               "/d/liaison/master"->query_deputy( p->query_name() );
    }
    return p->query_lord();
} /* query_can_delete() */

/**
 * Check if the player is allowed to add and delete alt characters.
 * @param p the player who is trying to add or delete alt(s)
 * @ignore
 */
protected int query_can_handle_alts(object p) {
    if( !interactive(p) ) {
        return 1;
    }
    return master()->query_senior( p->query_name() ) ||
           "/d/liaison/master"->query_deputy( p->query_name() );
} /* query_can_handle_alts() */

/**
 * Check if we can debounce the event. Add a new event or modify
 * the last event depending on whether it's debounceable or not.
 * @param player the name of the player for which the database entry is added
 * @param entry the database entry to be added
 * @ignore
 */
protected void do_debouncing(string player, class dbentry entry) {
    int             n;
    class dbentry   last;

    if (query_debounced(entry->event)) {
        //tell_creator("pinkfish", "[playerinfo] Debouncing: %O.\n",entry);
        n = sizeof(dossier->data);
        if ( n ) {
            last = dossier->data[ n - 1 ];
            //tell_creator("pinkfish", "[playerinfo] Last: %O.\n",last);
            if( entry->event == last->event &&
                entry->creator == last->creator &&
                entry->time - last->time <= DEBOUNCE_PERIOD)
            {
                // Merge the two events
                // tell_creator("pinkfish", "[playerinfo] Merging events.\n");
                last->comment += entry->comment;
                last->time = entry->time;
                if(last->extra != 0) {
                    if(entry->extra == 0) {
                        entry->extra = ({ });
                    }
                    last->extra += entry->extra;
                }
                //tell_creator("pinkfish", "[playerinfo] Result: %O.\n",last);
                return;
            }
        }
    }
    //tell_creator("pinkfish", "[playerinfo] Not merging events.\n");
    dossier->data += ({ entry });
    return;
} /* do_debouncing() */

/**
 * Check the currently loaded data for timed out entries and remove them.
 * @ignore
 */
protected void do_timeout() {
    function not_timed_out = function(class dbentry p)
    {
        int life = timeouts[p->event];

        if(life == 0)          // This event cannot be timed out
            return 1;
        // Time it out if its life period has expired
        return local_time <= (p->time + life);
    };

    local_time = time();

    dossier->data = filter(dossier->data, not_timed_out);
    dossier->last_check = time();
} /* do_timeout() */

/**
 * Load the data of the playerinfo object from its save file. If there's no
 * file, create an empty dossier. Don't load anything if the data is
 * already loaded.
 * @param player the name of the player whose data is to be loaded
 * @ignore
 */
protected void load_player(string player) {
    string  p = lower_case(player);
    string  fn = filename(p);
    class playerinfo tmp;
    mixed result;

    _dossier_total++;

    if ( !undefinedp(_dossier_cache[ player ] ) ) {
        dossier = _dossier_cache[ player ]; 
        _dossier_cache_hits++;
        return; 
    }

    if( dossier != 0 && dossier->name == p ) {
        return;                // Already have it here
    }

    if(file_size(fn) > 0) {
        result = unguarded( (: restore_object, fn, 0 :) );
    } else {
        dossier = new(class playerinfo,
            name:       p,
            last_check: time(),
            alts:       ({ }),
            data:       ({ }));
    }

    if (!classp(dossier)) {
        dossier = new(class playerinfo,
            name:       p,
            last_check: time(),
            alts:       ({ }),
            data:       ({ }));
    }

    if(sizeof(dossier) == 5) {
      tmp = new(class playerinfo,
                name: dossier->name,
                last_check: dossier->last_check,
                alts: copy(dossier->alts),
                data: copy(dossier->data),
                main_alt: dossier->main_alt,
                old_alts: ({ }));
      dossier = tmp;
    }
    if(!dossier->old_alts)
      dossier->old_alts = ({ });
    
    // Try and correct some of the damage caused by the last batch
    // of changes which screwed up the alt system.
    if(!correcting_alts) {
        correct_alts_for( dossier->name );
    }

    _dossier_cache[ player ] = dossier;
} /* load_player() */

/**
 * Save the data of the playerinfo object to its save file.
 * @param player the name of the player whose data is to be saved
 * @ignore
 */
protected void save_player(string player) {
    if( time() - dossier->last_check >= TIMEOUT_PERIOD ) {
        do_timeout();
    }

    unguarded( (: save_object, filename(player) :) );
} /* save_player() */

/**
 * Remove the player's data file.
 * @param player the name of the player
 */
void player_remove(string player) {
    string alt, new_main;
    string * alts;

    if ( !player ) {
        return;
    }
    player = lower_case( player );

    // If this is a main alt, pick one of their alts and make them the
    // main.
    correcting_alts = 1;
    load_player( player );
    if ( sizeof( dossier->alts ) ) {
        // Grab a list of alts from this dossier...
        alts = copy( dossier->alts );
        new_main = alts[0];

        alts -= ({ new_main });
        // ... Set up the new main player...
        load_player( new_main );
        dossier->main_alt = 0;
        dossier->alts = uniq_array( alts + ({ player }) );
        save_player( new_main );

        // ... And make all the other alts point to the new main.
        foreach( alt in alts ) {
            load_player( alt );
            dossier->main_alt = new_main;
            save_player( alt );
        }
    }
    correcting_alts = 0;

    unguarded( (: rm, filename(player) :) );
    clear_alerts_for(player);
} /* player_remove() */

/**
 * Add a new entry to the player's database.
 * @param source the creator or another object trying to add the event
 * @param player the name of the player
 * @param event event the name of the event to be added
 * @param comment arbitrary comment text (more than one line is OK)
 * @param extra arbitrary array of arbitrary objects (can be 0)
 * @return nonzero if the entry was successfully added to the database
 */
int add_entry(object creator, string player, string event, string comment,
  mixed *extra) {
    class dbentry   new_entry;

    if(!query_can_add(event,creator))
        return 0;              // No permission to add this event
    if(!query_source_ok(event,base_name(previous_object())))
        return 0;              // Wrong object trying to add this event
    if(!PLAYER_HANDLER->test_user(lower_case(player)))
        return 0;              // No such player
    if(comment[<1..<1] != "\n") {
        comment += "\n";
    }

    load_player(player);
    new_entry = new(class dbentry,
            time:       time(),
            creator:    capitalize(creator->query_name()),
            event:      event,
            comment:    (comment == 0 ? "" : comment),
            extra:      extra);
    do_debouncing(player, new_entry);
    save_player(player);
    // Add the player to the alerts mapping (to be checked by the login
    // handler which will dispatch warnings to currently online creators
    // the next time the player logs in, until the event is acknowledged)
    if( event == "alert" )
        increment_alerts_for(player);
    return 1;
} /* add_entry() */

/**
 * Delete an entry from the playerinfo database.
 * @param source the creator or another object trying to add the event
 * @param player the name of the player
 * @param event the name of the event of the entry being deleted
 * @param n the index of the entry being deleted
 * @return nonzero if the entry was successfully deleted
 */
int delete_entry(object creator, string player, string event, int n) {
   int idx = n - 1;
   class dbentry * data;
   class dbentry fluff;

   if( !query_can_delete( event, creator ) )
      return 0;
   if( !query_deleter_ok( event, previous_object() ) )
      return 0;      // Wrong object trying to delete this event
   load_player(player);
   if( ( idx < 0 ) || ( idx >= sizeof( dossier->data ) ) )
      return 0;
   fluff =  dossier->data[idx];
   if( fluff->event != event )
      return 0;

   data = copy( dossier->data );
   data = data[0 .. (idx - 1)] + data[(idx + 1) .. <1];
   dossier->data = data;
   save_player(player);
   log_file("DELETE", ctime(time()) + ": " + event + " added by " +
            fluff->creator + "\n");

   if( event == "alert" )
      decrement_alerts_for(player);

   return 1;
} /* delete_entry() */

/**
 * Print the header of of the database report.
 * @param source the creator who requested the report
 * @param player the name of the player
 * @ignore
 */
protected string query_header( string player ) {
  string aka, alts, str;
  string *tmp, *tmp2;
  
  aka = alts = str = "";
  
  if( sizeof( dossier->alts ) > 0 )
    alts = " aka " +
      query_multiple_short(map(dossier->alts,
                               (: CL_HEADER+capitalize($1)+CL_RESET :)));
  if(sizeof(dossier->old_alts) > 0)
    alts += " (and was " +
      query_multiple_short(map(dossier->old_alts,
                               (: CL_HEADER+capitalize($1)+CL_RESET :))) + ")";
    
  if( dossier->main_alt ) {
    // Cleanup their alt list if the main_alt is deleted.
    if(!PLAYER_HANDLER->test_user(dossier->main_alt)) {
      fix_alts(player);
      load_player(player);
    }
        
    aka = " (alt of " + CL_HEADER + capitalize(dossier->main_alt) + CL_RESET;

    // Load up the 'main' player file and grab a list of all their alts,
    // then reload this player's file.
    load_player(dossier->main_alt);
    tmp = copy(dossier->alts);
    tmp2 = copy(dossier->old_alts);
    
    if(!tmp) {
      tmp = ({ });
      dossier->alts = ({ });
      save_player( dossier->name );
    }
    load_player(player);

    tmp -= ({ player });
    if(sizeof(tmp)) 
      aka += ", aka " +
        query_multiple_short(map(tmp, (: CL_HEADER+capitalize($1)+CL_RESET :)));

    if( arrayp( tmp2 ) ) {
      tmp2 -= ({ player });
      if(sizeof(tmp2)) {
        aka += " and was " +
          query_multiple_short(map(tmp2, (: CL_HEADER+capitalize($1)+CL_RESET:)));
      }
    }
    aka += ")";
  }
  return sprintf( "Report for: %s%s%s\n\n",
                  CL_HEADER + capitalize(player) + CL_RESET, alts, aka );
}

/**
 * Print one entry of the dossier.
 * @param source the creator who requested the report
 * @param idx the index of the database entry to print
 * @param e the database entry to print
 * @ignore
 */
protected string query_entry( int idx, class dbentry e,
                              string display_name ) {
   string  date      = ctime( e->time );
   string  creator   = e->creator;
   string  event     = e->event;
   string  *comments = explode( e->comment, "\n" );
   string  line, str;
   int     lines = 0;

   if( display_name )
      display_name = sprintf( "%-31s ",
        "(" + CL_CREATOR + display_name + CL_RESET + ")" );
   else
      display_name = "";
   str = sprintf( "%2d. %s%s %s%|14s%s (by %s%s%s)\n",
     idx + 1, display_name, date, CL_EVENT, event, CL_RESET,
     CL_CREATOR, creator, CL_RESET );
   foreach( line in comments ) {
      if( sizeof(line) != 0 ) {
         str += sprintf( "    %s\n", line );
         lines++;
      }
   }
   if( !lines ) {
      str += sprintf( "    (no comments)\n" );
   }
   return str;
}

/**
 * Print all entries from the given player's dossier.
 * @param source the creator who requested the report
 * @param player the name of the player
 */
varargs string query_dossier( object source, string player, int lastn ) {
   int             i;
   class dbentry   * list;
   string          msg, str;
   mapping         log;

   load_player(player);
   str = query_header(player);
   list = dossier->data;

   //Backwards compatability.
   if ( sizeof( dossier ) == NO_ACCESS_LOG ) {
    dossier = new ( class playerinfo, 
        name: dossier->name,
        last_check: dossier->last_check,
        alts: dossier->alts,
        data: dossier->data,
        main_alt: dossier->main_alt,
        old_alts: dossier->old_alts,
        access_log: ([ ]) );
   }

   if ( mapp( dossier->access_log ) ) 
     log = dossier->access_log;
   else
     log = ([ ]);

   if( !lastn ||
       sizeof(list) <= lastn ||
       source->query_property(VERBOSE_PI) )
   {
      i = 0;
   } else {
      i = sizeof(list) - lastn;
      msg = "%^RED%^NOTE:%^RESET%^ Only displaying this player's most "
        "recent " + lastn + " entries.  Use 'playerinfo "
        + player + " verbose' to see their entire dossier.\n";
      str += msg + "\n";
   }

   for( ; i < sizeof( list ); i++ ) {
      str += query_entry( i, list[i], 0 );
   }

   if( msg ) {
      str += "\n" + msg;
   }

   //Filter week old entries.
   log = filter( log, (: $2 > time() - ONE_WEEK :) );

   //Add new ones!
   if ( interactive( source ) ) 
     log[ source->query_name() ] = time();

  //Restore the access log.
  dossier->access_log = log;
  save_player( player );
  return str;
}

/**
 * Print all entries from the given player's dossier with the given event
 * type.
 * @param source the creator who requested the report
 * @param player the name of the player
 * @param event the name of the event
 */
string query_event( object source, string player, string event ) {
   int             i;
   class dbentry   *list;
   string          str;

   load_player(player);
   str = query_header(player);
   list = dossier->data;
   for( i = 0; i < sizeof(list); i++ ) {
      if( list[i]->event == event )
         str += query_entry( i, list[i], 0 );
   }
   return str;
}

/**
 * Print all entries for this player and all alts, in chronological order.
 * @param source the creator who requested the report
 * @param player the name of the player
 */
string query_interleaved( object source, string player, string event ) {
   class playerinfo * dossiers;
   class dbentry * stuff;
   string str;
   string * alts;
   int i, size, done, earliest, earliestt, count;
   int * earliests;

   load_player(player);
   if( dossier->main_alt ) {
      player = dossier->main_alt;
      load_player(player);
   }
   str = query_header(player);

   // Find all this player's alts
   alts = ({ player }) + copy( dossier->alts );
   size = sizeof(alts);

   // If the player has no alts, just use the standard dossier display code
   if( size == 1 ) {
      if(event) {
        str = query_event( source, player, event );
      } else {
        str = query_dossier( source, player );
      }
      return str;
   }

   // load up all of the dossiers locally
   dossiers = allocate(size);
   earliests = allocate(size);
   for( i = 0; i < size; i++ ) {
     load_player( alts[i] );
     if( sizeof( dossier->data ) ) {
       dossiers[i] = copy(dossier);
     } else {
       dossiers[i] = 0;
     }
     earliests[i] = 0;
   }

   // loop until there are no entries left to display.
   count = 0;
   while( !done ) {
      done = 1;
      earliestt = 0;
      for( i = 0; i < size; i++ ) {
         if( !dossiers[i] ) {
            continue;
         }
         done = 0;
         stuff = dossiers[i]->data;
         if( !earliestt || stuff[ earliests[i] ]->time < earliestt ) {
            earliest = i;
            earliestt = stuff[ earliests[i] ]->time;
         }
      }
      if(done) {
         continue;
      }
      stuff = dossiers[earliest]->data;
      if( !event ||
          stuff[ earliests[earliest] ]->event == event )
      {
         str += query_entry( count, stuff[ earliests[earliest] ],
                             alts[earliest] );
         count++;
      }
      earliests[earliest]++;
      if( earliests[earliest] >= sizeof(stuff) ) {
         dossiers[earliest] = 0;
      }
   }
   return str;
}

/**
 * Return which player this player is an alt of (if any).
 * param player A players name
 * return the name of the players main alt.
 */
string query_alt_of(string player) {
  if(!PLAYER_HANDLER->test_user(player))
    return 0;
  
  load_player(player);
  return dossier->main_alt;
}

string *query_alts(string player) {
  if(!PLAYER_HANDLER->test_user(player))
    return ({ });

  load_player(player);
  return dossier->alts;
}

/**
 * Add an alt character name to this player's dossier. This function succeeds
 * if both characters are not "main", or only one if them is "main". Both
 * players will have their dossiers modified.
 * @param player the name of the player
 * @param alts the names of the alt characters to add
 * @return a string describing the outcome of the function call
 */
string add_alt(object creator, string player, string alt) {
  class dbentry new_entry;
  int is_deleted;
  string *alts, tmp;
  
  if((base_name(previous_object()) != "/cmds/player/register") &&
     !query_can_handle_alts(this_player()))
    return "You are not allowed to add players' alts.\n";

  alt = lower_case(alt);

  if(!find_player(alt) && !PLAYER_HANDLER->test_user(alt)) {
    if(!creator)
      return "No such player: " + capitalize(alt) + ".\n";
    if(file_size(filename(alt)) == -1)
      return "No such player and no record for: " + capitalize(alt) + ".\n";
    is_deleted = 1;
  }
  
  load_player(alt);
  if(dossier->main_alt && PLAYER_HANDLER->test_user(dossier->main_alt) &&
     !is_deleted)
    return capitalize(alt) + " is already an alt of " +
      capitalize(dossier->main_alt) + ".\n";

  if(sizeof(dossier->alts)) {
    if(is_deleted)
      return capitalize(alt) + " already has alts.\n";
    else {
      // Since this player is deleted we'll add these alts & old_alts to
      // the newly registered player.
      alts = dossier->alts + dossier->old_alts;
    }
  }
  
  player = lower_case(player);
  if(!find_player(player) && !PLAYER_HANDLER->test_user(player))
    return "No such player: "+ capitalize(player) +".\n";

  correcting_alts = 1;

  load_player(player);
  if(dossier->main_alt == alt) {
    correcting_alts = 0;
    return capitalize(player) + " is already an alt of " +
      capitalize(dossier->main_alt) + ".\n";
  }
  
  if(dossier->alts && member_array(alt, dossier->alts) != -1) {
    correcting_alts = 0;
    return capitalize(alt) + " is already an alt of " +
      capitalize(player) + ".\n";
  }

  if(!dossier->alts)
    dossier->alts = ({ });
  
  dossier->alts += ({ alt });
  if(alts) {
    foreach(tmp in alts) {
      if(PLAYER_HANDLER->test_user(tmp))
        dossier->alts += ({ tmp });
      else
        dossier->old_alts += ({ tmp });
    }
  }
  
  new_entry = new(class dbentry,
                  time:       time(),
                  creator:    creator ? capitalize(creator->query_name()) :
                              player,
                  event:      "register",
                  comment:    "Registered " + capitalize(alt) +
                              " as an alt.\n",
                  extra:      0);

  do_debouncing(player, new_entry);
  save_player(player);

  load_player(alt);
  dossier->main_alt = player;
  dossier->alts = ({ });
  dossier->old_alts = ({ });
  new_entry = new(class dbentry,
                  time:       time(),
                  creator:    creator ? capitalize(creator->query_name()) :
                              player,
                  event:      "register",
                  comment:    "Registered as an alt of " + player + ".\n",
                  extra:      0);
  do_debouncing(alt, new_entry);
  save_player(alt);

  correcting_alts = 0;
  
  return "Added " + capitalize(alt) + " as an alt of " + capitalize(player) +
    ".\n";
}

/**
 * Delete an alt character name from this player's dossier. Note that both
 * players have their dossier modified.
 * @param player the name of the player
 * @param alts the name of the alt characters to delete
 * @return a string describing the outcome of the function call
 */
string delete_alt(object creator, string player, string alt) {
  string ret;
  class dbentry new_entry;

  if(!query_can_handle_alts(this_player())) 
    return "You are not allowed to delete players' alts.\n";

  player = lower_case(player);
  alt = lower_case(alt);

  load_player(player);
  if(!dossier->alts || member_array(alt, dossier->alts) == -1)
    ret = capitalize(alt) + " was not an alt of " + capitalize(player);
  else {
    dossier->alts -= ({ alt });
    new_entry = new(class dbentry,
                    time:       time(),
                    creator:    capitalize(creator->query_name()),
                    event:      "register",
                    comment:    "Removed " + capitalize(alt) + " as an alt.\n",
                    extra:      0);
    do_debouncing(alt, new_entry);
    save_player(player);
  }
  
  load_player(alt);

  if(dossier->main_alt != player) {
    if(ret)
      return capitalize(alt) + " is not an alt of " + capitalize(player) +
        ".\n";
    else
      ret = capitalize(player) + " was not the main player for " +
        capitalize(alt);
  } else {
    dossier->main_alt = 0;
    dossier->alts = ({ });
    dossier->old_alts = ({ });
    new_entry = new(class dbentry,
                    time:       time(),
                    creator:    capitalize(creator->query_name()),
                    event:      "register",
                    comment:    "Removed as an alt of " + capitalize(player) +
                    ".\n",
                    extra:      0);
    do_debouncing(alt, new_entry);
    save_player(alt);
  }

  if(ret)
    return "Deleted " + capitalize(alt) + " from " + capitalize(player) +
      "'s list of alts (" + ret + ").\n";
  else
    return "Deleted " + capitalize(alt) + " from " + capitalize(player) +
      "'s list of alts.\n";
}

/**
 *  @return The alerts mapping.
 */
mapping query_alerts() {
   if( !_alerts ) {
      _alerts = ([ ]);
   }
   return _alerts;
} /* query_alerts() */

/**
 * @param player Name of the player to query
 * @return The number of alerts for that player
 */
int query_alerts_for( string player ) {
   player = lower_case(player);

   if( !_alerts ) {
      _alerts = ([ ]);
   }
   return _alerts[player];
} /* query_alerts_for() */

/**
 * @param player The name of the player
 * @param idx The number of the event to check for alert status.
 * @return 0 if the event is not an alert, 1 if it is.
 */
int is_alert( string player, int idx ) {
//   if( !PLAYER_HANDLER->test_user( lower_case( player ) ) ) {
//      return 0;              // No such player
//   }
   load_player( player );
   if( sizeof(dossier->data) < idx ) {
      return 0;
   }
   return ( dossier->data[ idx - 1 ] )->event == "alert";
} /* is_alert() */

/*
 * @param player The name of the player.
 * @param event The type of event to change the alert to.
 * @param idx The number of the event to acknowledge.
 * @param update The event description
 * @return 1 for success or 0 for failure.
 */
int acknowledge_alert( object creator, string player, string event,
   int idx, string update, mixed * extra ) {
   class dbentry entry;
   string previnfo;

   player = lower_case(player);

   if( !query_can_add( event, creator ) )
      return 0;   // No permission to add this event
   if( !query_source_ok( "alert", base_name( previous_object() ) ) )
      return 0;   // Wrong object trying to add this event
   if( !query_source_ok( event,  base_name( previous_object() ) ) )
      return 0;   // This object is not allowed to add this type of event.
   if( !PLAYER_HANDLER->test_user(player) )
      return 0;   // No such player
   if( member_array( event, keys(_sources) ) < 0 )
      return 0;   // No such event type

   // Update the entry
   idx--;
   load_player( player );
   entry = dossier->data[idx];

   previnfo = sprintf( "Originally added by %s%s%s at %s:\n%s\n---\n",
     CL_CREATOR, entry->creator, CL_RESET, ctime( entry->time ),
     entry->comment );

   entry->time    = time();
   entry->creator = capitalize( creator->query_name() );
   entry->event   = lower_case( event );
   entry->comment = previnfo + update;

   dossier->data[idx] = entry;
   if( event != "alert" ) {
      decrement_alerts_for(player);
   }

   save_player( player );
   return 1;
} /* acknowledge_alert() */

/**
 * @param player The name of the player.
 */
void clear_alerts_for( string player ) {
   player = lower_case(player);

   if( !_alerts ) {
      _alerts = ([ ]);
   }

   map_delete( _alerts, player );
   save_handler_data();
} /* clear_alerts_for() */

/**
 * @param player The name of the player.
 * @return The updated number of alerts for that player.
 */
int increment_alerts_for( string player ) {
   player = lower_case(player);

   if( !_alerts ) {
      _alerts = ([ ]);
   }

   if( !PLAYER_HANDLER->test_user(player) ) {
      return 0;
   }

   if( undefinedp( _alerts[player] ) ) {
      _alerts[player] = 1;
   } else {
      _alerts[player] = _alerts[player] + 1;
   }

   save_handler_data();
   return _alerts[player];
} /* increment_alerts_for() */

/**
 * @param player The name of the player.
 * @return The updated number of alerts for that player.
 */
int decrement_alerts_for( string player ) {
   player = lower_case(player);

   if( !_alerts ) {
      _alerts = ([ ]);
   }

   if( undefinedp( _alerts[player] ) ) {
      return 0;
   }

   _alerts[player] = _alerts[player] - 1; 

   if(_alerts[player] <= 0 ) {
      map_delete( _alerts, player );
   }

   save_handler_data();
   return _alerts[player];
} /* int decrement_alerts_for() */

/**
 * @return A list of the currently-online players who have unacknowledged alerts
 */
string * query_online_alerts() {
   if ( !_alerts ) {
      _alerts = ([ ]);
      return ({ });
   }

   return filter( keys(_alerts), (: find_player($1) :) );
} /* query_online_alerts() */

/**
 * @ignore yes
 * Added by Shrike, extended by Ceres. It fixes up the handling of alts.
 */
private void correct_alts_for(string player) {
  string main;
  string *alts, *tmp, alt;
  
  if(!player)
    return;
  
  player = lower_case(player);

  correcting_alts = 1;
  
  // Load the player's dossier...
  load_player(player);
  main = dossier->main_alt;
  alts = copy(dossier->alts);

  //  if(this_player()->query_name("ceres"))
  // write("Processing: " + player + " (" + main + ")\n");
  
  if(main && !no_recurse) {
    // non-main characters should not have alts.
    if(sizeof(alts)) {
      dossier->alts = ({ });
      save_player(player);
    }

    // The alt & the main char must exist.
    if((!PLAYER_HANDLER->test_user(player) ||
        !PLAYER_HANDLER->test_user(main))) {
      correcting_alts = 0;
      no_recurse = 1;
      correct_alts_for(main);
      load_player(player); // reload the original
    }
    return;
  }

  // This is the main alt and it nolonger exists.
  if(!PLAYER_HANDLER->test_user(player)) {
    // No alts so we're done.
    if(!sizeof(alts)) {
      if(this_player()->query_name("ceres"))
        write("No alts for : " + player + "\n");

      correcting_alts = 0;
      return;
    }

    // Need to pick a new main alt.
    tmp = filter(alts, (: PLAYER_HANDLER->test_user($1) :));
    if(sizeof(tmp)) {
      main = tmp[0];
    }
  }

  // We've picked a new main so clear our alts list & mark as as
  // having been an alt of the new main alt.
  if(main) {
    dossier->main_alt = main;
    dossier->alts = ({ });
    save_player(player);
  }
  
  // ... Load up the new main player and add to their alts as required...
  if(main)
    load_player(main);
  
  if(sizeof(alts)) {
    tmp = filter(alts, (: !find_player($1) &&
                        !PLAYER_HANDLER->test_user($1) :));
    tmp = uniq_array(dossier->old_alts + tmp);
  } else
    alts = ({ });

  alts = uniq_array(alts + dossier->alts) - ({ main });
  alts = filter(alts, (: find_player($1) || PLAYER_HANDLER->test_user($1) :));

  // If we have a new main char, the alts or old alts have changed
  // then we have some tidying up to do.
  if(main && (alts != dossier->alts || tmp != dossier->old_alts)) {
    dossier->alts = copy(alts);
    dossier->old_alts = copy(tmp);
    save_player(main);

    // ... Make sure all of this guy's alts point to them as the main...
    foreach(alt in alts) {
      if(dossier->main_alt != main) {
        load_player(alt);
        dossier->main_alt = main;
        save_player(alt);
      }
    }
    // ... Make sure all of this guy's alts point to them as the main...
    foreach(alt in tmp) {
      if(dossier->main_alt != main) {
        load_player(alt);
        dossier->main_alt = main;
        save_player(alt);
      }
    }
  }

  // ... and restore the original player.
  load_player(player);
  correcting_alts = 0;
}

void reregister_parent( string player ) {
   string main;
   string * alts;

   if ( !player ) {
      return;
   }
   player = lower_case(player);

   correcting_alts = 1;

   // Load the player's dossier...
   load_player( player );
   main = dossier->main_alt;

   if ( main ) {
      load_player(main);
      alts = dossier->alts;
      if ( !alts || !sizeof( alts ) ) {
         dossier->alts = ({ player });
      } else if ( member_array( player, alts ) == -1 ) {
         dossier->alts = alts + ({ player });
      } else {
         return;
      }
      save_player( main );
   }
} /* reregister_parent() */

/**
 * @ignore yes
 *
 * Register a players refresh. This is called by the refresh handler.
 */
void player_refreshed(mixed player, int totally) {
  class dbentry new_entry;
  string comment;
  
  if(objectp(player))
    player = player->query_name();

  switch (totally) {
  case PARTIAL_REFRESH:
    comment = "They did a partial refresh.";
    break;
  case TOTAL_REFRESH:
    comment = "They did a total and complete refresh.  (Even refreshed their "
      "breath)";
    break;
  default:
    comment = "They did some weird sort of unknown refresh.";
    break;
  }
  
  //
  // Slip an event into their playerinfo
  //
  load_player(player);
  new_entry = new(class dbentry,
                  time:       time(),
                  creator:    "Refresh Handler",
                  event:      "refresh",
                  comment:    comment,
                  extra:      0);
  do_debouncing(player, new_entry);
  save_player(player);
}

/**
 * @ignore yes
 *
 * Register a players deletion. This is called by the refresh handler.
 */
void player_deleted(mixed player, int deleted) {
  class dbentry new_entry;
  string *tmp, *tmp2, alt, main;
  
  if(objectp(player))
    player = player->query_name();

  // No record, then lets not create one eh?
  if(file_size(filename(player)) == -1)
    return;

  clear_alerts_for(player);
  load_player(player);
  new_entry = new(class dbentry,
                  time:       time(),
                  creator:    "Refresh Handler",
                  event:      "delete",
                  comment:    "Player deleted",
                  extra:      0);
  do_debouncing(player, new_entry);
  save_player(player);
  
  // move us from the alts list to the old_alts list for our main char.
  if(dossier->main_alt) {
    main = dossier->main_alt;
    dossier->main_alt = 0;
    save_player(player);
    load_player(main);
    dossier->alts -= ({ player });
    dossier->old_alts += ({ player });
    save_player(main);
    return;
  }

  // This was the main char. So we must pick a new main, transfer
  // all alts into it and then update the main_alt for all the other alts.
  if(dossier->alts && sizeof(dossier->alts)) {

    // choose a new main.
    tmp = filter(dossier->alts, (: PLAYER_HANDLER->test_user($1) :));
    if(sizeof(tmp))
      main = tmp[0];
    tmp2 = dossier->old_alts;

    // load and setup the new main.
    load_player(main);
    dossier->main_alt = 0;
    if(sizeof(tmp) > 1)
      dossier->alts = copy(tmp[1..]);
    dossier->old_alts = copy(tmp2 + ({ player }));
    save_player(main);

    // change all the other alts over to use this new main.
    foreach(alt in dossier->alts) {
      load_player(alt);
      dossier->main_alt = main;
      save_player(alt);
    }

    return;
  }
}

/** @ignore yes */
void player_created(string player) {
  if(file_size(filename(player)) == -1)
    return;
  
  load_player(player);
  if(dossier->main_alt) {
    dossier->main_alt = 0;
    dossier->alts = ({ });
    save_player(player);
  }
  return;
}

/** @ignore yes */
void fix_alts(mixed player) {
  string *alts, alt, main;

  if(objectp(player))
    player = player->query_name();
  
  // No record, not much we can do.
  if(file_size(filename(player)) == -1)
    return;

  load_player(player);

  if(dossier->main_alt) {
    main = dossier->main_alt;
    
    load_player(dossier->main_alt);
    if(!PLAYER_HANDLER->test_user(dossier->main_alt)) {
      if(sizeof(alts))
        main = alts[0];
    }
  } else {
    main = player;
  }

  // Make sure we're in the alts list and main isn't.
  // Put in some stricter type checking to deal with the situation
  // when ->alts or ->old_alts is 0
  alts = ({});
  
  if( dossier->alts ) {
     alts = alts + dossier->alts;
  }
  
  if( dossier->old_alts ) {
     alts = alts + dossier->old_alts;
  }  
  
  alts = uniq_array( alts + ({ player }));
  alts -= ({ main });
    
  // load and setup the new main.
  dossier->main_alt = 0;
  dossier->alts = filter(alts, (: PLAYER_HANDLER->test_user($1) :));
  dossier->old_alts = filter(alts, (: !PLAYER_HANDLER->test_user($1) :));
  
  // change all the other alts over to use this new main.
  foreach(alt in alts) {
    load_player(alt);
    dossier->main_alt = main;
    dossier->alts = 0;
    dossier->old_alts = 0;
    save_player(alt);
  }
}

void reset() {
    if ( sizeof( _dossier_cache ) > CACHE_SIZE ) {
        _dossier_cache = ([ ]);
    }
} /* reset() */ 

string query_access_history( string player ) {
  mapping history;
  string ret = "";

  load_player(player);
  
  //   if ( sizeof( dossier ) == NO_ACCESS_LOG ) 
  //     return "This player record has not been accessed since Tue May 27 01:29:16 2003.\n";
  
  history = dossier->access_log;
  
  ret += sprintf( "Access log for player %s:\n", capitalize( player ) );
  
  foreach( string name in sort_array( keys( history ), 
                                      (: $(history)[$2] - $(history)[$1] :) )) 
    ret += sprintf( "%12s:\t%s.\n", capitalize(name), 
                    query_time_string( time() - history[name], 2 ) + " ago");
  return ret;
}
mixed *stats() {
    return ({ 
        ({ "cache hits", _dossier_cache_hits }),
        ({ "total requests", _dossier_total }),
        ({ "cache hit %", to_int(_dossier_cache_hits * 100.0 /
                                 _dossier_total) }),
        ({ "current cache size", sizeof( _dossier_cache ) })
    });
} /* stats() */ 

