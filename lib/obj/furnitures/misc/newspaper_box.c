/*
 *
 *$Id: newspaper_box.c,v 1.5 2003/03/21 01:44:16 ceres Exp $
 *
 */

/**
 * Newspaper box furniture object.
 * This object has to be set to a specific paper and only works for a certain
 * number of issues.  It only works for the owner of the house it is placed in,
 * and only when it is actually placed.
 * It checks for a new release every hour until it's depleted.
 * @author Wirble
 * @started August 2001
 */


inherit "/std/room/furniture/storage";


/* includes */
#include <room/newspaper.h>
#include <move_failures.h>
#include <housing.h>
#include <player_handler.h>

/* functions */
void check_for_new_papers();
void set_paper( string _paper );
void set_issues_left( int _issues );
void set_international( int value );
void set_last_issue( int _issue );

mapping query_dynamic_auto_load();
varargs void init_dynamic_arg( mapping map, object thing );

int query_last_issue();
int query_international();
int query_issues_left();
string query_paper();


/* variables */
private string _paper = "a given newspaper";
private int _last_issue = 0;
private int _issues_left = 1;
private int _international = 0;


/** @ignore */
void setup() {

   set_name( "box" );
   set_short( "newspaper box" );
   add_adjective( "newspaper" );
   set_long( "It's a large box impregnated with an oil designed to attract "
             "delivery-imps.\n" );
   set_allowed_positions( ({ "hanging", "standing" }) );
   set_allowed_room_verbs( ([ "standing": "stands", "hanging": "hangs" ]) );
   add_extra_look( this_object() );
   set_weight( 100 );
   set_max_weight( 300 );
   set_value( 10000 );
   set_open();
   set_stuck( 1 );
   call_out( "check_for_new_papers", 5 );

} /* void setup() */


/** @ignore */
string extra_look() {
   if( !_paper || _paper == "a given newspaper" ) {
      return "The variables of this newspaper box are broken.  Please ask a "
             "creator to re-set them.\n";
   };
   return "This particular one works for " + _paper + ".\n";
} /* string query_box_long() */


/**
 * This function is called after the box loads and then again every hour.
 * If any new issues of the newspaper are available while the box is placed,
 * the necessary property is added to the house's owner and the
 * newspaper-object is put into the box.
 * Once all issues are delivered, the call-out won't reoccur.
 */
void check_for_new_papers() {

   object newspaper;
   object room = environment( this_object() );
   int i, x, latest_issue;
   int* editions_bought;
   string name;

   if( !is_placed() && _issues_left > 0 ) {
      call_out( "check_for_new_papers", 3600 );
      return;
   };
   /* If the box isn't international and the is placed within the newspaper's
    * area OR
    * if it is international and the newspaper exists... */
   if( ( _international == 0 &&
         member_array( _paper, NEWSPAPER_HANDLER->query_papers_in_area(
           HOUSING->query_region( file_name( room ) ) ) ) != -1 ) ||
       ( _international == 1 &&
         member_array( _paper, NEWSPAPER_HANDLER->query_all_papers() ) != -1 ) ){

      latest_issue = NEWSPAPER_HANDLER->query_last_edition_num( _paper );
      name = environment( this_object() )->query_owner();
      if( latest_issue > _last_issue ) {
         tell_room( room,
                    "A " + ({"red", "green", "purple", "blue", "yellow",
                             "drunk", "smelly", "large", "tiny"})[random(9)] +
                    " imp rushes in with some newpapers in its hands and "
                      "disappears in the box.\n" );
         x = latest_issue - _last_issue;
         /* ...all undelivered issues are created and moved to the box... */
         for( i = 0; i < x; i++ ) {
            newspaper = clone_object( NEWSPAPER_OB );
            newspaper->set_paper( _paper );
            newspaper->set_edition( _last_issue + 1 );
            if( !newspaper->move( this_object() ) == MOVE_OK ) {
               tell_room( room,
                          "Something in the box curses and a newspaper is "
                          "thrown to the floor from inside it.\n" );
               newspaper->move( room );
            };
            _last_issue++;
            _issues_left--;
            editions_bought = PLAYER_HANDLER->test_property( name, "Paper " +
                                                              _paper );
            if( !editions_bought ) {
               editions_bought = ({ });
            };
            /* ... and the paper-bought property is added to the player so
             * the web-newspaper-pages become available.
             * Also the newspaper gets paid. */
            if( member_array( _last_issue + 1, editions_bought ) == -1 ) {
               editions_bought += ({ _last_issue });
               PLAYER_HANDLER->special_add_property( name,
                 "Paper " + _paper, editions_bought );
               NEWSPAPER_HANDLER->add_edition_paper_sold( _paper,
                  _last_issue + 1,
                  NEWSPAPER_HANDLER->query_paper_cost( _paper ) );
            } else {
               NEWSPAPER_HANDLER->add_edition_revenue( _paper, _last_issue + 1,
                  NEWSPAPER_HANDLER->query_paper_cost( _paper ) );
            };
            tell_room( room,
                       "You hear some " + ({"giggling", "ranting", "quacking",
                                            "thumping", "cursing", "swearing",
                                            "snickering"})[random(7)] +
                       " emerging, the imp jumps out and runs off again.\n" );
         };
      };
   };

   if( _issues_left == 0 ) {
      set_long( query_long() +
                "It appears the imp-bait is no longer working.\n" );
   } else {
      call_out( "check_for_new_papers", 3600 );
   };

} /* void check_for_new_papers() */


/**
 * This sets the newspaper the box is dedicated to.
 * @param new_paper The newspaper
 */
void set_paper( string new_paper ) {

   _paper = new_paper;

} /* void set_paper() */


/**
 * This sets the number of issues the box will handle.  This shouldn't be
 * used after the initial creation of the box.
 * @param _issues The new number of issues
 */
void set_issues_left( int _issues ) {

   _issues_left = _issues;

} /* void set_issues_left() */


/**
 * This sets the last issue that was delivered.  Since it should only be called
 * when the box is created, it makes sure that the player doesn't get issues
 * that have been published before the box was purchased.
 * @param _issue The latest issue at the time of purchase
 */
void set_last_issue( int _issue ) {

   _last_issue = _issue;

} /* void set_last_issue() */


/** @ignore */
mapping query_dynamic_auto_load() {

   return ([
            "::": ::query_dynamic_auto_load(),
            "_paper": _paper,
            "_issues_left": _issues_left,
            "_last_issue": _last_issue,
            "_international": _international
        ]);
} /* mapping query_dynamic_auto_load() */


/** @ignore */
varargs void init_dynamic_arg( mapping map, object thing ) {

   if( map[ "::" ] && thing ) {
      ::init_dynamic_arg( map[ "::" ], thing );
   } else if( map[ "::" ] ) {
      ::init_dynamic_arg( map[ "::" ] );
   };
   if( map[ "_paper" ] ) {
      set_paper( map[ "_paper" ] );
   };
   if( map[ "_issues_left" ] ) {
      set_issues_left( map[ "_issues_left" ] );
   };
      if( map[ "_last_issue" ] ) {
      set_last_issue( map[ "_last_issue" ] );
   };
      if( map[ "_international" ] ) {
      set_international( map[ "_international" ] );
   };

} /* varargs void init_dynamic_arg() */


/**
 * This queries the last issue of the paper.
 * @return the number of the last delivered issue
 */
int query_last_issue() {
   return _last_issue;
} /* int query_last_issue() */


/**
 * This queries how many issues are left before the box is used up.
 * @return the number of issues left
 */
int query_issues_left() {
   return _issues_left;
} /* int query_issues_left() */


/**
 * This queries if the box will work if it's in a house outside the newspaper's
 * region.
 * @return 1 if international, 0 if not
 */
int query_international() {
   return _international;
} /* int query_international() */


/**
 * This queries what paper the box is set to.
 * @return the name of the paper
 */
string query_paper() {
   return _paper;
} /* string query_paper() */


/**
 * This sets the box to international or not.
 * @param i 1 for international, 0 for not.
 */
void set_international( int i ) {
   _international = i;
} /* void set_international() */


