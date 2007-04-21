/**
 * Library inherit. Allows players to take out books on loan. If
 * set to be player contributable, then books (most likely player 
 * written, but any kind) can be added/removed by designated 
 * librarian players.
 *
 * @author Aquilo
 */

// ToDo 
// Functions to be called when a damaged/lost book is reported for a player
// Prevent returning of books w/ pages ripped out.
// Gving status of a book, who has it on loan, ue back etc...

#include <book_handler.h>
#include <money.h>
#include <language.h>
#include <player.h>
#include <player_handler.h>
#include <am_time.h>

#define BASE_PATHS ({ "/std/book", "/std/book_dir", "/std/leaflet", \
                      "/obj/misc/paper", "/obj/misc/nroff_paper" })

#define TITLE_PROP          "title"
#define AUTHOR_PROP         "author"
#define FROM_LIBRARY_PROP   "from library"
#define BORROWED_BY_PROP    "borrowed by"
#define DUE_BACK_PROP       "due back"
#define REFERENCE_ONLY_PROP "reference only"

#define USER_ACCESS      1
#define LIBRARIAN_ACCESS 2

inherit "/std/room/basic_room";

nosave int       _player_contributable;
nosave string    _library_name;
nosave string    _save_file;
nosave function  _borrow_func;
nosave function  _test_allowed;

int       _loan_length;
int       _fine_per_day;
int       _lost_damaged_fine;
int       _max_loans;

mapping _catalog;    // ([ librarybook id : class _book ])
mapping _accounts;   // ([ playername     : class _account ])
mapping _fines;      // ([ playername     : int fine ])
mapping _access;     // ([ playername     : int access ])

nosave mapping _catalog_by_title;    // ([ "title" : ({ library id }) ])
nosave mapping _catalog_by_author;   // ([ "author" : ({ library id }) ])

/**
 * This is all the info on one of our stored books.
 * @element path the path to the item, if applicable
 * @element auto_load if no path, we store the object whole in this string
 * @element cap_title the capitalized title of the book
 * @element cap_author the capitalized author of the book
 * @element borrowable 1 if this book can be borrowed, 0 if not
 * @element copies Total number of copies owned by library
 * @element loaned An array of due back dates for loaned out copies
 * @element total_borrowed total no of times this book has been borrowed
 */
class _book { 
    string     path;     
    string     auto_load;
    string     cap_title;
    string     cap_author;
    mapping    loaned;
    int        borrowable;
    int        copies;
    int        total_borrowed;
}

/**
 * This holds info on a book a player has borrowed
 * @element id the librarybook id num
 * @element due_back the rl time due back, in secs from 1970
 */
class _loan {
    int       id;
    int       due_back;
}

/**
 * Holds info on a player.
 * @element total_fines_paid how much this blokie has paid in fines in total
 * @element damaged_books total number of books he has damaged
 * @element lost_books total number of books he has lost
 * @element loans info on what  books he currently has out
 */ 
class _account {
    int           total_fines_paid;
    int           lost_damaged;
    class _loan   *loans;
}

int do_add( object *obs, string title, string author );
int do_remove_book( int id );
int do_borrow( mixed arg );
int do_return( object *obs );
int do_list( string arg, string to, string from, string specific );
int do_status( string player );
int do_book_status( int id );
int do_set( string player, string type );
int do_mark( int id, int flag );
int do_set_fine( int i );
int do_set_loan_length( int days );
int do_set_max_loans( int i );
int do_lost_damaged_fine( int i );
int do_report( int id, string arg, string player );

mapping query_loans( string name );
mapping query_borrowed_by( int id );
int query_fine( string name );

void clear_loan( mixed player, int id, int calc_fines );
varargs int add_book_to_library( mixed thing, int copies, string cap_title, 
                                 string cap_author, int borrowable );
void set_access( string player, int access );
int query_access( mixed player );
int do_set( string player, string type );
void save_me();

protected string make_pretty_catalog( string arg, string from, string to, string specific );
protected int compare_widgets( object new_ob, int existing_id );

/** @ignore yes */
protected void build_catalogs(){
  int id;
  class _book book;

  foreach( id, book in _catalog ){
    if( _catalog_by_author[ lower_case(book->cap_author) ] )
      _catalog_by_author[ lower_case(book->cap_author) ] += ({ id });
    else
      _catalog_by_author[ lower_case(book->cap_author) ] = ({ id });

    if( _catalog_by_title[ lower_case(book->cap_title) ] )
      _catalog_by_title[ lower_case(book->cap_title) ] += ({ id });
    else
      _catalog_by_title[ lower_case(book->cap_title) ] = ({ id });
  }

}


/** @ignore yes */
void create(){
  object sign;
  ::create();

  _catalog_by_author = ([ ]);
  _catalog_by_title = ([ ]);

  /* Some default settings */
  _player_contributable = 1;
  _library_name = "The illustrious library of fluff";
  _save_file = base_name( this_object() ) + ".o";

  if( master()->file_exists(_save_file) ){
    unguarded( (: restore_object, _save_file :) );
  } else {
    _catalog = ([ ]);
    _accounts = ([ ]);
    _fines = ([ ]);
    _access = ([ ]);
    _loan_length = 3 * 24 * 60 * 60;
    _fine_per_day = 400;
    _lost_damaged_fine = 1050;
    _max_loans = 5;
  }

  build_catalogs();

  add_help_file("p_library_user");
  add_help_file("p_library_librarian");

  sign = add_sign( 
      "The plaque is a traditional brass plate, set on top a piece "
      "of cherry oak. It looks very formal.\n",
      "For some reason, reading this sign doesn't give you as much "
      "information as looking at it would.\n",
      "brass plaque", "plaque", "common" );
  sign->add_extra_look( this_object() );
  sign->set_read_mess( 0, 0, 0 );

/*
  if ( !do_setup ) { 
    this_object()->setup();
  }
*/

}


/*
void setup(){
  set_short("small library");
  set_long("A library. Lots of books everywhere.\n");
  set_light(70);
}
*/

/** @ignore yes */
int query_library(){  return 1;  }

/** @ignore yes */
void init(){
  if( _player_contributable ){
    add_command( "add", "<indirect:object:me'book'> with "
        "title <string'title'> by author <word'author'>",
        (: do_add( $1, $4[1], $4[2] ) :) );
    add_command( "remove", "<number'book id'>", 
        (: do_remove_book( $4[0] ) :));
  }

  add_command( "borrow", "<number'book id'>", (: do_borrow( $4[0] ) :) );
  add_command( "return", "<indirect:object:me-here>" );
  add_command( "pay", "[fine]"  );

  add_command( "view", "catalogue by {author|title}", (: do_list( $4[0], "a", "z", 0 ) :) );
  add_command( "view", "catalogue by {author|title} from "
      "<word'letter'> to <word'letter'>", (: do_list( $4[0], $4[1], $4[2], 0 ) :) );

  add_command( "view", "works by <string'author'>", 
      (: do_list( "author", 0, 0, $4[0] ) :) );
  add_command( "view", "works titled <string'title'>",
      (: do_list( "title", 0, 0, $4[0] ) :) );

  add_command( "status", "", (: do_status(0) :) );

  add_command( "status", "book <number'book id'>", 
               (: do_book_status( $4[0] ) :) );  

  add_command( "report", "<number'book id'> as {damaged|lost}",
               (: do_report( $4[0], $4[1], 0 ) :) );

  /* Librarian only commands */

  add_command( "set", "access for <word'player'> to {allowed|disallowed}", 
               (: do_set( $4[0], $4[1] ) :) );

  add_command( "set", "fine per day to <number'amount'>", 
               (: do_set_fine( $4[0] ) :) );

  add_command( "set", "loan length to <number'number of days'>", 
               (: do_set_loan_length( $4[0] ) :) );

  add_command( "set", "maximum number of loans to <number'number'>", 
               (: do_set_max_loans( $4[0] ) :) );

  add_command( "set", "lost or damaged fine to <number'amount'>", 
               (: do_lost_damaged_fine( $4[0] ) :) );

  add_command( "mark", "<number'book id'> as reference only",
    (: do_mark( $4[0], 1) :) );

  add_command( "mark", "<number'book id'> as not reference only", 
    (: do_mark( $4[0], 0) :) );

  add_command( "status", "<word'player'>", (: do_status($4[0]) :) );

  add_command( "report", "<number'book id'> as {damaged|lost} for <word'player'>",
               (: do_report( $4[0], $4[1], $4[2] ) :) );



}

/** @ignore yes */
protected string id_to_name( int id ){
  if( !_catalog[id] )
    return 0;
  return "'" +_catalog[id]->cap_title+ "' by " + _catalog[id]->cap_author;
}

/** @ignore yes */
protected int valid_media( object ob ){
  if( ob->query_book() || ob->query_paper() || ob->query_leaflet() )
    return 1;
}

/** @ignore yes */
int do_add( object *obs, string title, string author ){
  object t_p, t_o;
  int num, id, *ids;

  t_p = this_player();
  t_o = this_object();

  debug_printf( "Obs: %O\n", obs );

  if( query_access(this_player()) < LIBRARIAN_ACCESS ){
    add_failed_mess("Only librians can do this.\n");
    return 0;
  }

  if( strlen(author) > 14 ){
    add_failed_mess("The author's name is too long. It has to be less than 13 characters.\n");
    return 0;
  }

  if( strlen(title) > 26 ){
    add_failed_mess("The title is too long. It can only use less than 27 characters.\n");
    return 0;
  }

  if( sizeof(obs) > 1 ){
    add_failed_mess("You can only add one item at a time.\n");
    return 0;
  }

  if( !t_o->valid_media(obs[0]) ){
    add_failed_mess( "The library does not lend items like $I.\n", obs);
    return 0;
  }

  if( obs[0]->query_magic_scroll() || obs[0]->query_spell_book() ){
    add_failed_mess( "This library does not deal in magical works.\n" );
    return 0;
  }

  if( obs[0]->query_property(FROM_LIBRARY_PROP) ){
    add_failed_mess( "This item belongs to a library and cannot be added.\n" );
    return 0;
  }


  if( (num = obs[0]->query_book_num()) ){   // then its a player written book
    if( lower_case(author) != BOOK_HANDLER->query_book_owner(num) ){
      add_failed_mess( "The book has been authored by " + 
        capitalize(BOOK_HANDLER->query_book_owner(num)) + " and not " + 
        author + ".\n");
      return 0;
    }
  }



  if( (ids = _catalog_by_title[ lower_case(title) ]) ){
    foreach( id in ids ){
      if( lower_case(_catalog[id]->cap_author) == lower_case(author) ){
        // Then we already have a book of the same title by the same author
        // Make sure that they are infact the same...
        if( compare_widgets( obs[0], id ) ){
          add_book_to_library( id, 1 );
          add_succeeded_mess( "$N add$s another copy of "
              +id_to_name(id)+ " to the library.\n" );
          obs[0]->move( "/room/rubbish" );
          return 1;
        } else {
          add_failed_mess("$I doesn't seem to be the same as the copy held "
            "in the library. Catalogue it under a different title or author.\n", obs );
          return 0;
        }
      }
    }
  }
        
  if( member_array(base_name(obs[0]), BASE_PATHS) == -1 )
    add_book_to_library( base_name(obs[0]), 1, title, author, 1 );
  else
    add_book_to_library( obs[0], 1, title, author, 1 );    

  add_succeeded_mess( "$N $V an item to the library.\n" );
  obs[0]->move( "/room/rubbish" );
  return 1;
}

/** @ignore yes */
void remove_book( int id ){
  object ob;
  class _book book;
  int i;

  if( !_catalog[id] )
    return;

  book = _catalog[id];

  for( i = 0; i < (book->copies - sizeof(book->loaned)); i++ ){
    if( book->path )
      ob = clone_object( book->path );
    if( book->auto_load )
      ob = PLAYER_OB->load_auto_load_to_array( book->auto_load, this_player() )[0];

    ob->move( this_object() );
  }

  _catalog_by_title[lower_case(book->cap_title)] -= ({ id });
  _catalog_by_author[lower_case(book->cap_author)] -= ({ id });

  if( !sizeof( _catalog_by_title[lower_case(book->cap_title)] ) )
    map_delete( _catalog_by_title, lower_case(book->cap_title) );
  if( !sizeof( _catalog_by_author[lower_case(book->cap_author)] ) )
    map_delete( _catalog_by_author, lower_case(book->cap_author) );

  map_delete( _catalog, id );
  save_me();
}

/** @ignore yes */
int do_remove_book( int id ){
  int out;
  string player, bit;
  class _account account;
  class _loan loan;

  if( query_access(this_player()) < LIBRARIAN_ACCESS ){
    add_failed_mess("Only librians can do this.\n");
    return 0;
  }

  id = to_int(id);
  if( !_catalog[id] ){
    add_failed_mess("There is no item with that id.\n");
    return 0;
  }

  foreach( player, account in _accounts ){
    foreach( loan in account->loans ){
      if( loan->id == id )
        out++;
    }
  }

  if( out ){
    if( out > 1 ) 
      bit = "are " +query_num(out)+ " ";
    else 
      bit = "is one"+" ";
    add_failed_mess( "There " +bit+ id_to_name(id) + 
        ", out on loan already. You can only remove a book if all "
        "copies are in the library.\n" );
    return 0;
  } else {
    add_succeeded_mess("$N $V " +id_to_name(id) + " from the library.\n" );
    remove_book(id);
    tell_object( this_player(), "Any remaining copies will be placed on the floor.\n");
    return 1;
  } 

}

/** @ignore yes */
int do_borrow( mixed arg ){
  int id, ret, t;
  object ob;
  class _book book;
  class _loan loan;
  string name;
  
  name = this_player()->query_name();
  id = to_int(arg);

  if( query_access(this_player()) < USER_ACCESS ){
    add_failed_mess("You do not have access to this library.\n");
    return 0;
  }

  if( _borrow_func ){
    ret = evaluate( _borrow_func, this_player(), id );
    if( ret != 1 )
      return ret;
  }

  if( _fines[name] ){
    add_failed_mess("You can't borrow a book whilst having outstanding fines.\n" );
    return 0;
  }

  if( !_catalog[id] ){
    add_failed_mess("There is no book with an id of " +id+ ".\n" );
    return 0;
  } else {
    book = _catalog[id];
  }

  if( _accounts[name] && sizeof(_accounts[name]->loans) >= _max_loans ){
    add_failed_mess("You have already borrowed the maximum number of books.\n");
    return 0;
  }

  if( (book->copies - sizeof(book->loaned)) < 1 ){
    add_failed_mess("Unfortunately, all copies of " +
        id_to_name(id) + " are out on loan.\n");
    return 0;
  }

  if( _accounts[name] && sizeof( _accounts[name]->loans ) ){
    foreach( loan in _accounts[name]->loans ){
      if( loan->id == id ){
        add_failed_mess("You have already borrowed a copy of " + 
            id_to_name(id) + ".\n");
        return 0;
      }
    }
  }

  if( book->path )
    ob = clone_object( book->path );
  if( book->auto_load )
    ob = PLAYER_OB->load_auto_load_to_array( book->auto_load, this_player() )[0];

  if( !ob ){
    add_failed_mess("Uh oh, something buggered with book id: " + id + 
      ". Please tell a creator.\n" );
    return 0;
  } 

  t = time() + _loan_length;

  ob->add_property( TITLE_PROP, book->cap_title );
  ob->add_property( AUTHOR_PROP, book->cap_author );
  ob->add_property( FROM_LIBRARY_PROP, _library_name );
  ob->add_property( BORROWED_BY_PROP, name );
  ob->add_property( DUE_BACK_PROP, t );

  book->loaned[name] = t;
  book->total_borrowed++;
  _catalog[id] = book;

  loan = new( class _loan, id : id, due_back : t );

  if( _accounts[name] )
    _accounts[name]->loans += ({ loan });
  else {
    // Start them up an account
    _accounts[name] = new( class _account, total_fines_paid : 0,
                           lost_damaged : 0,
                           loans : ({ loan }) );
  }


  if( !book->borrowable ){
    tell_object( this_player(), id_to_name(id)+ " is a reference only book. "
      "It will be placed on "
      "a lecturn for you to read and cannot be removed from the library. Please return it "
      "in the normal way after use.\n");
    ob->move( this_object() );
    ob->reset_get();
    ob->add_property( "there", "sitting on a lecturn" );
    ob->add_property( REFERENCE_ONLY_PROP, 1 );
  } else {
    tell_object( this_player(), "You have until "+ am_time(t) + 
        " to return it.\n");
    ob->move( this_player() );
  }  
  save_me();
  add_succeeded_mess( "$N $V " +id_to_name(id)+ ".\n");
  return 1;
}

/** @ignore yes */
int do_return( object *obs ){
  string name, bit;
  int id, *owed, i, *ours, due;
  class _loan loan;
  class _book book;
  object ob, *not_ours, *wrong_player, *returned;

  name = this_player()->query_name();
  owed = ({ }); 
  ours = not_ours = wrong_player = returned = ({ });


  if( !sizeof(_accounts[name]->loans) ){
    add_failed_mess( "You don't have any books out on loan.\n" );
    return 0;
  }

  foreach( loan in _accounts[name]->loans ){
    owed += ({ loan->id });
  }

  foreach( ob in obs ){
    if( ob->query_property( FROM_LIBRARY_PROP ) == _library_name ){
      if( ob->query_property( BORROWED_BY_PROP ) == name ){     
        foreach( id in owed ){
          if( (book = _catalog[id]) ){
            if( ob->query_property( TITLE_PROP ) == book->cap_title && 
                ob->query_property( AUTHOR_PROP ) == book->cap_author ){

              /* Make sure it has same pages in here... */

              map_delete( _catalog[id]->loaned, name );
              ob->move("/room/rubbish");
              returned += ({ ob });
              ours += ({ id });
            }  
          } else {
            // Book no longer matches id....What to do?
            // Take book from player, clear loan then dest?
            ob->move("/room/rubbish");
            returned += ({ ob });
          }
        }
      } else {
        wrong_player += ({ ob });
      }
    } else {
      not_ours += ({ ob });
    }
  }

  if( sizeof(returned) ){
    foreach( i in ours )
      clear_loan( this_player(), i, 1 );
    if( _fines[name] )
      tell_object(this_player(), "Note: You have fines that are due.\n");
    add_succeeded_mess( "$N $V $I.\n", returned );
    return 1;
  }

  if( (i=sizeof(not_ours)) ){
    if( i > 1 ) bit = "do not"; else bit = "does not";
    add_failed_mess( "$I " +bit+ " belong to this library.\n", not_ours );
    return 0;
  }
 
  if( (i=sizeof(wrong_player)) ){
    if( i > 1 ) bit = "were not"; else bit = "was not";
    add_failed_mess( "$I " +bit+ " were not loaned to you. Only the borrower may "
        "return items.\n", wrong_player );
    return 0;
  }

}

/** @ignore yes */
int do_list( string arg, string from, string to, string specific ){

  if( (to && !stringp(to)) || (from && !stringp(from)) ){
    add_failed_mess( "The ranges must be letters.\n");
    return 0;
  }

  if( specific ){
    tell_object( this_player(), make_pretty_catalog( arg, 0, 0, lower_case(specific) ) );
    return 1;
  } else {
    tell_object( this_player(), make_pretty_catalog( arg, from, to, 0 ) );
    return 1;
  }

}

/** @ignore yes */
int do_pay(){
  int cash, fine;
  string place, str;

  place = query_property( "place" );
  if ( !place || ( place == "" ) )
    place = "default";

  cash = this_player()->query_value_in( place );
  if ( place != "default" )
    cash += this_player()->query_value_in( "default" );

  if( !cash ){
    add_failed_mess("You have money!.\n");
    return 0;
  }

  fine = _fines[this_player()->query_name()];
  if( fine <= 0 ){
    add_failed_mess("You have no outstanding fines to pay.\n");
    return 0;
  }

  if( cash >= fine )
    cash = fine;
  
  this_player()->pay_money( MONEY_HAND->create_money_array(cash, place), place );
  _fines[this_player()->query_name()] -= cash;
  _accounts[this_player()->query_name()]->total_fines_paid += cash;

  if( _fines[this_player()->query_name()] <= 0 ){
    str = MONEY_HAND->money_value_string( cash, place );
    add_succeeded_mess("$N pay$s all $p fines ("+str+").\n");
    map_delete( _fines, this_player()->query_name() );
    save_me();
    return 1;
  } else {
    str = MONEY_HAND->money_value_string( fine - cash, place );
    add_succeeded_mess("$N pay$s some of $p fine, but sill owe$s ("+str+").\n");
    save_me();
    return 1;
  }

}

/** @ignore yes */
int do_status( string player ){
  int fine;
  string place, str, bit;
  class _loan loan;

  if( !player )
    player = this_player()->query_name();

  player = lower_case(player);

  if( player != this_player()->query_name() &&
      query_access(this_player()) < LIBRARIAN_ACCESS ){
    add_failed_mess("You do not have access to see other accounts.\n");
    return 0;
  }

  str = "Account status for " + PLAYER_HANDLER->query_cap_name(player) +".\n";

  if( _accounts[player] && sizeof(_accounts[player]->loans)  ){
    str += "Loaned items:\n";
    str += sprintf( "%|4s%|=30s%|=20s%|9s\n", "Id", "Item", "Due back", "Overdue" );
    foreach( loan in _accounts[player]->loans ){
      if( loan->due_back - time() > 0 )
        bit = "";
      else 
        bit = "*Yes*";
      str += sprintf( "%|4d%|=30s%|=20s%|9s\n", loan->id,
          id_to_name(loan->id), am_time( loan->due_back ), bit );
    }
  } else {
    str += "No items currently on loan.\n";
  }
  

  place = query_property( "place" );
  if ( !place || ( place == "" ) )
    place = "default";

  fine = _fines[player];
  if( fine >= 0 ){
    str += sprintf( "%-15s%-6s\n", "Fines Due:", 
           MONEY_HAND->money_value_string( fine, place ) );
  } else {
    str += "No fines due.\n";
  }
  if( _accounts[player] ){
    str += "Total previously paid fines: " +
      MONEY_HAND->money_value_string( _accounts[player]->total_fines_paid, place );
    str += ".\nBooks lost or damaged: " + _accounts[player]->lost_damaged + ".\n";
  }


  tell_object( this_player(), str );
  add_succeeded_mess("");
  return 1;
}


/** @ignore yes */
int do_book_status( int id ){
  string str, name, date;
  class _book book;

  id = to_int(id);

  if( !_catalog[id] ){
    add_failed_mess("The id " +id+ ", does not point to an item.\n");
    return 0;
  }
  book = _catalog[id];
  str = "Status for id: " +id+ " - " + id_to_name(id) + ".\n";
 
  if( sizeof(book->loaned) ){
    str += sprintf( "%|=14s%|=30s\n", "On loan to", "Date Due Back " );
    foreach( name, date in book->loaned ){
      str += sprintf( "%|=12s%|=30s\n", 
             PLAYER_HANDLER->query_cap_name(name), am_time(date) );
    }
  } else {
    str += "Currently, no copies of the item are on loan.";
  }

  str += "The library holds " + (string)book->copies; 
  if( book->copies == 1 ) str += " copy "; else str += " copies ";
  str += "in total, " + (string)( book->copies - sizeof(book->loaned) ) +
         " of which are available.\n";

  str += "It has been borrowed a total of " + book->total_borrowed;
  if( book->total_borrowed == 1 ) str += " time.\n"; else str += " times.\n";
 
  if( !book->borrowable )
    str += "It is a reference only item.\n";
  tell_object( this_player(), str);
  add_succeeded_mess("$N look$s at the status of a library item.\n");
  return 1;
}


/** @ignore yes */
int do_set( string player, string type ){

  player = lower_case(player);

  if( query_access(this_object()) <= query_access(player) ){
    add_failed_mess( "You do not have permission to do this.\n");
    return -1;
  }

  if( type == "allowed" ){
    if( query_access(player) < USER_ACCESS ){
      set_access( player, USER_ACCESS );
      add_succeeded_mess("$N allow$s " + PLAYER_HANDLER->query_cap_name(player) + 
        " to use the library.\n");
      save_me();
      return 1;
    }
  }

  if( type == "disallowed" ){
    if( query_access(player) == USER_ACCESS ){
      set_access( player, 0 );
      add_succeeded_mess("$N disallow$s " + PLAYER_HANDLER->query_cap_name(player) + 
        " from using the library.\n");
      save_me();
      return 1;
    }
  }
}


int do_mark( int id, int flag ){

  if( query_access(this_player()) < LIBRARIAN_ACCESS ){
    add_failed_mess("You do not have permission to do this.\n");
    return 0;
  }

  if( !_catalog[id] ){
    add_failed_mess("There is no item with that id.\n");
    return 0;
  }

  if( flag ){
    add_succeeded_mess("$N $V " + id_to_name(id) + " to reference only.\n");
    _catalog[id]->borrowable = 0;
    return 1;
  } else {
    add_succeeded_mess("$N $V " + id_to_name(id) + " to not reference only.\n");
    _catalog[id]->borrowable = 1;
    return 1;
  }
  save_me();
}

int do_set_fine( int i ){
  string place;

  if( query_access(this_player()) < LIBRARIAN_ACCESS ){
    add_failed_mess("You do not have permission to do this.\n");
    return 0;
  }

  place = query_property( "place" );
  if( !place || ( place == "" ) )
    place = "default";
   
  i = to_int(i);

  if( i < 0 ){
    add_failed_mess("Don't be silly, thats a negative value!\n");
    return 0;
  }

  if( i > 4000 ){
    add_failed_mess("The maximum fine you can set per day is 4000 units (" +
        MONEY_HAND->money_value_string( 4000, place ) + ").\n" );
    return 0;
  }

  add_succeeded_mess("$N set$s the fine per day to " + i +
    " ("+ MONEY_HAND->money_value_string( i, place ) + ").\n" );
  _fine_per_day = i;
  save_me();
  return 1;
}


int do_set_loan_length( int days ){
  string str;

  if( query_access(this_player()) < LIBRARIAN_ACCESS ){
    add_failed_mess("You do not have permission to do this.\n");
    return 0;
  }
  days = to_int(days);

  if( days > 10 ){
    add_failed_mess("The maximum loan length is 10 days.\n");
    return 0;
  }

  if( days < 1 ){
    add_failed_mess("The minimum loan length is 1 day.\n");
    return 0;
  }
  if( days == 1 )  str = " day"; else str = " days";

  add_succeeded_mess("$N set$s the loan length to " 
     + query_num(days) + str + ".\n" );
  _loan_length = days * AM_SECONDS_PER_DAY;
  save_me();
  return 1;
}

/** @ignore yes */
int do_set_max_loans( int i ){

  if( query_access(this_player()) < LIBRARIAN_ACCESS ){
    add_failed_mess("You do not have permission to do this.\n");
    return 0;
  }
  i = to_int(i);

  if( i < 1 ){
    add_failed_mess("The minimum amount of loans is one.\n");
    return 0;
  }

  if( i > 10 ){
    add_failed_mess("The maximum amount of loans a player can have is ten.\n");
    return 0;
  }

  add_succeeded_mess("$N set$s the maximum number of loans to " 
     + query_num(i) + " items.\n" );
  _max_loans = i;
  save_me();
  return 1;

}

int do_lost_damaged_fine( int i ){
  string place;
 
  if( query_access(this_player()) < LIBRARIAN_ACCESS ){
    add_failed_mess("You do not have permission to do this.\n");
    return 0;
  }
  i = to_int(i);
  place = query_property( "place" );
  if( !place || ( place == "" ) )
    place = "default";
   
  if( i < 0 ){
    add_failed_mess("Don't be silly, thats a negative value!\n");
    return 0;
  }

  if( i > 20000 ){
    add_failed_mess("The maximum fine you can set per day is 20000 units (" +
        MONEY_HAND->money_value_string( 20000, place ) + ").\n" );
    return 0;
  }

  add_succeeded_mess("$N set$s the lost or damaged fine to " + i +
    " ("+ MONEY_HAND->money_value_string( i, place ) + ").\n" );
  _lost_damaged_fine = i;
  save_me();
  return 1;


}

int do_report( int id, string arg, string player ){
  string str;
  class _loan loan, match;

  if( player && query_access(this_player()) < LIBRARIAN_ACCESS ){
    add_failed_mess("You can only report your damaged or lost books.\n");
    return 0;
  }
 
  if( player ){
    str = PLAYER_HANDLER->query_cap_name(player)+ " does";
    player = lower_case(player);
  } else {
    str = "You do";
    player = this_player()->query_name();
  }

  if( !_accounts[player] ){
    add_failed_mess(str + " not have any items on loan.\n");
    return 0;
  }

  if( !_catalog[id] ){
    add_failed_mess("The id " + id + " does not exist.\n");
    return 0;
  }


  foreach( loan in _accounts[player]->loans ){
    if( loan->id == id ){
      match = loan;
    }
  }
  if( !match ){
    add_failed_mess(str + " not have " + id_to_name(id) + " out on loan.\n");
    return 0;
  }

  _accounts[player]->loans -= ({ match });
  map_delete( _catalog[id]->loaned, player );
  _catalog[id]->copies--;
  
  if( player == this_player()->query_name() ) 
    str = "";
  else 
    str = " for " + PLAYER_HANDLER->query_cap_name(player);

  if( _fines[player] )
    _fines[player] += _lost_damaged_fine;
  else
    _fines[player] = _lost_damaged_fine;

  if( arg == "lost" ){
    add_succeeded_mess("You report the loss of " + id_to_name(id) + str + ".\n");
    _accounts[player]->lost_damaged++;
    save_me();
    return 1;
  } 
  if( arg == "damaged" ){
    add_succeeded_mess("You report the damaging of " +id_to_name(id) + str + ".\n");
    _accounts[player]->lost_damaged++;
    save_me();
    return 1;
  }

}


/** @ignore yes */
protected string make_pretty_catalog( string arg, string from, string to, string specific ){
  string widget, c_widget, str, *list;
  int id, *ids;

  if( !from )
    from = "a";
  else
    from = lower_case(from);

  if( !to )
    to = "z";
  else 
    to = lower_case(to);

  if( from > to ){ 
    /* Then they have said between z and a, rather than a to z,
       silly buggers */
    widget = to;
    to = from;
    from = widget;
  }

  if( specific ){
    if( _catalog_by_title[specific] || _catalog_by_author[specific] )
      list = ({ specific });
    else
      list = ({ });
  }

  if( arg == "title" ){

    str = sprintf( "%|=4s%|=26s%|=12s%|=9s%|=8s\n", 
            "Id", "Title", "Author", "Copies", "On Loan" );

    if( !list )
      list = sort_array( keys(_catalog_by_title), 0 );

    foreach( widget in list ){
      ids = _catalog_by_title[ widget ];
      if( widget && sizeof(ids) && (widget[0..0] >= from) && (widget[0..0] <= to) ){
        foreach( id in ids ){
          c_widget = _catalog[ id ]->cap_title;
          str += sprintf( "%|=4d%|=26s%|=12s%|=9d%|=8d\n", 
                   id, c_widget, _catalog[id]->cap_author, 
                   _catalog[id]->copies, sizeof(_catalog[id]->loaned) );
        }
      }
    }
    return str;
  } 


  if( arg == "author" ){

    str = sprintf( "%|=4s%|=12s%|=26s%|=9s%|=8s\n", 
            "Id", "Author", "Title", "Copies", "On Loan" );

    if( !list )
      list = sort_array( keys(_catalog_by_author), 0 );

    foreach( widget in list ){
      ids = _catalog_by_author[ widget ];
      if( widget && sizeof(ids) && (widget[0..0] >= from) && (widget[0..0] <= to) ){
        foreach( id in ids ){
          c_widget = _catalog[ id ]->cap_title;
          str += sprintf( "%|=4d%|=12s%|=26s%|=9d%|=8d\n", 
                   id, _catalog[id]->cap_author, c_widget,
                   _catalog[id]->copies, sizeof(_catalog[id]->loaned) );
        }
      }
    }
    return str;
  }


}

/**
 * This clears the loan of library book id the specified player.
 * If its overdue it sorts out adding a fine to his name.
 * @param player Either the player name or object
 * @param id the library book id
 * @param calc_fines if this is 1, then we add fines onto the player
 * if appropriate, if 0 then we ignore fines and just clear the loan
 */
void clear_loan( mixed player, int id, int calc_fines ){
  int fine;
  class _loan loan;

  if( objectp(player) && userp(player) )
    player = player->query_name();

  if( !stringp(player) )  return;
  if( !_accounts[player] || !sizeof(_accounts[player]->loans) )  return;

  foreach( loan in _accounts[player]->loans ){
    if( loan->id == id ){
      if( calc_fines && (loan->due_back - time() < 0) ){   // Naughty!
        fine = (((time() - loan->due_back) / AM_SECONDS_PER_DAY) + 1) * _fine_per_day;
        if( _fines[player] )
          _fines[player] += fine;
        else
          _fines[player] = fine;
      }
      _accounts[player]->loans -= ({ loan });
    }
  }  
  save_me();
}


/** @ignore yes */
protected int find_blank_id(){
  int *ids;
  int unused_id, i;

  ids = sort_array( keys(_catalog), -1 );
  if( !sizeof(ids) ){
    unused_id = 1;
  } else {
    if( ids[0] > sizeof(ids) ){
      for( i = 0; i < sizeof(ids); i++ ){
        if( undefinedp(_catalog[i]) ){
          unused_id = i;
          break;
        }
      }
    } else {
      unused_id = sizeof(ids) + 1;
    }
  }
  if( unused_id == 0 )
    unused_id = 1;

  return unused_id;
}

/** @ignore yes */
protected void add_book_to_catalogs( class _book new_book, int id ){

  _catalog[id] = new_book;
  save_me();

  if( _catalog_by_author[lower_case(new_book->cap_author)] )
    _catalog_by_author[lower_case(new_book->cap_author)] += ({ id });
  else
    _catalog_by_author[lower_case(new_book->cap_author)] = ({ id });

  if( _catalog_by_title[lower_case(new_book->cap_title)] )
    _catalog_by_title[lower_case(new_book->cap_title)] += ({ id });
  else
    _catalog_by_title[lower_case(new_book->cap_title)] = ({ id });

}


/**
 * This function adds the specified object to the library 
 * @param thing Either a filename, or object. If its an object we record its autoload info
 * @param copies how many copies to add
 * @param cap_title The capitalized title its cataloged under
 * @param cap_author The capitalized author of the work
 * @param borrowable 1 if it can be borrowed, 0 if its reference only
 */
varargs int add_book_to_library( mixed thing, int copies, string cap_title, 
                                 string cap_author, int borrowable ){
  class _book book;
  int id, *ids;

  if( intp(thing) ){
    if( _catalog[thing] ){
      _catalog[thing]->copies += copies;
      return 1;
    } else {
      return 0;
    }
  }

  if( !cap_title || !cap_author || cap_title == "" || cap_author == "" )
    return 0;

  if( (ids = _catalog_by_title[ lower_case(cap_title) ]) ){
    foreach( id in ids ){
      if( lower_case(_catalog[id]->cap_author) == lower_case(cap_author) )
          /*= Then we already have a book of the same title by the same author =*/
          return add_book_to_library( id, 1 );
    }
  }

  if( stringp(thing) ){    
    book = new( class _book );
    book->path = thing;
    book->cap_author = cap_author;
    book->cap_title = cap_title;
    book->borrowable = 1;
    book->copies = copies;
    book->loaned = ([ ]);
    book->total_borrowed = 0;

    id = find_blank_id();
    add_book_to_catalogs( book, id );
    return 1;
  }
  
  if( objectp(thing) ){
    book = new( class _book );
    book->auto_load = AUTO_LOAD_OB->create_auto_load( ({ thing }) );
    book->cap_author = cap_author;
    book->cap_title = cap_title;
    book->borrowable = 1;
    book->copies = copies;
    book->loaned = ([ ]);
    book->total_borrowed = 0;

    id = find_blank_id();
    add_book_to_catalogs( book, id );
    return 1;
  }

}


/**
 * Returns 1, if we think that the new object is the same as the existing one we
 * have in stock.
 * @ignore yes
 */
protected int compare_widgets( object new_ob, int existing_id ){
  class _book book;
  object ob;
  mixed info;
  int perc;

  book = _catalog[existing_id];
  if( !book )  return 0;

  if( book->path )
    ob = clone_object( book->path );
  if( book->auto_load )
    ob = PLAYER_OB->load_auto_load_to_array( book->auto_load, this_player() )[0];

  if( !ob ){
    // Shouldn't happen...
    return 0;
  }

  if( base_name(ob) != base_name(new_ob) )
    return 0; // Then they can't be the same surely...

  if( inherits("/std/book.c", new_ob) ){
    perc = BOOK_HANDLER->compare_pages(
             BOOK_HANDLER->query_pages( new_ob ),
             BOOK_HANDLER->query_pages( ob )
           );
  }

  if( inherits("/std/leaflet.c", new_ob) ){
    perc = BOOK_HANDLER->compare_pages(
             map( new_ob->query_pages(), (: $1[0][0] :) ),
             map( ob->query_pages(), (: $1[0][0] :) )
           );
  }

  if( inherits("/obj/misc/paper", new_ob) ||
      inherits("/obj/misc/nroff_paper", new_ob) ){
    perc = BOOK_HANDLER->compare_pages(
      map( new_ob->query_read_mess(), (: $1[0] :) ),
      map( ob->query_read_mess(), (: $1[0] :) )
    );
  }
  ob->move( "/room/rubbish" );

  if( perc > 97 )
    return 1;
  else
    return 0;
}



/**
 * This is used to sort out the look msg of the sign.
 * @ignore yes
 */
string extra_look( object ob ){
  string *libs, str, place;

  str = "Welcome to " + _library_name + ".\n";

  libs = filter( keys(_access), (: _access[$1] == LIBRARIAN_ACCESS :) );
  if( !sizeof(libs) )
    str += "Currently there are no caretakers of the books.\n";
  if( sizeof(libs) == 1 )
    str += PLAYER_HANDLER->query_cap_name(libs[0]) + " is the sole caretaker "
          " of the books.\n";
  if( sizeof(libs) > 1 ){
    libs = map( libs, (: PLAYER_HANDLER->query_cap_name($1) :) );
    str += query_multiple_short(libs) + " are caretakers of the books.\n";
  }

  place = query_property("place");
  if( !place || place == "" )
    place = "default";
  
  str += "Books may be borrowed for a maximum of " +
         (_loan_length / AM_SECONDS_PER_DAY) +
         " Disc days, before fines are levied.\n";
  str += "Fines currently stand at " +
         MONEY_HAND->money_value_string( _fine_per_day, place ) + 
         " per day the book is overdue.\n";
  str += "A charge of " + MONEY_HAND->money_value_string( _lost_damaged_fine, place ) +
         " will be incurred if you lose or damage a book.\n";
  str += "The maximum number of items you can borrow at one time currently stands at "+
         query_num(_max_loans) + ".\n";

  return str;
}

/** @ignore yes */
protected void save_it(){
  unguarded( (: save_object, _save_file :) );
}

/** @ignore yes */
void save_me(){
  if( find_call_out("save_it") == -1 )
    call_out( "save_it", 2 );
}

/*
void dest_me(){
  object ob, *ref;

  ref = filter( all_inventory(), (: $1->query_property(REFERENCE_ONLY_PROP) :) ) );
  foreach( ob in ref ){

  }
}
*/

/**
 * Returns a mapping of book ids to due back dates for the given player.
 * @param name the player
 * @return a mapping in the form ([ book id : due back ])
 */
mapping query_loans( string name ){
  class _loan loan;
  mapping m = ([ ]);

  if( !_accounts[name] )
    return ([ ]);

  foreach( loan in _accounts[name]->loans ){
    m[ loan->id ] = loan->due_back;
  }  

  return m;

}

/**
 * Returns a mapping of players who borrowed the given book id, mapped to their 
 * due back dates.
 * @param id valid book id
 */
mapping query_borrowed_by( int id ){
  if( _catalog[id] )
    return _catalog[id]->loaned;
  else
    return ([ ]);
}

/**
 * @param name the string name of the person to check
 * @return How much of a fine this person has
 */
int query_fine( string name ){  return _fines[name];  }

/**
 * Debug only
 * @ignore yes
 */
mapping query_catalog(){
  return _catalog;
}

/** @ignore yes */
mapping query_catalog_by_title(){
  return _catalog_by_title;
}

/** @ignore yes */
mapping query_catalog_by_author(){
  return _catalog_by_author;
}

/** @ignore yes */
mapping query_all_accounts(){
  return _accounts;
}


/***** ******/

void set_library_name( string str ){  _library_name = str;  }

string query_library_name(){  return _library_name;  }

void set_player_contributable( int i ){  _player_contributable = i;  }

int query_player_contributable(){  return _player_contributable;  }

void set_max_loans( int i ){  _max_loans = i;  }

int query_max_loans(){  return _max_loans;  }

void set_loan_length( int i ){  _loan_length = i;  }

int query_loan_length(){  return _loan_length;  }

void set_fine_per_day( int i ){  _fine_per_day = i;  }

int query_fine_per_day(){  return _fine_per_day;  }

void set_lost_damaged_fine( int i ){ _lost_damaged_fine = i;  }

int query_lost_damaged_fine(){ return _lost_damaged_fine;  }

void set_save_file( string s ){  _save_file = s;  }

string query_save_file(){  return _save_file;  }

void set_borrow_func( function f ){  _borrow_func = f;  }

function query_borrow_func(){  return _borrow_func;  }


void set_access( string player, int access ){ 
  if( !access ){
    map_delete( _access, player );
  } else {
    _access[player] = access;
  }
  save_me();
}

int query_access( mixed player ){
/*
  if( userp(player) )
    player = player->query_name();
 
  if( stringp(player) )
    player = lower_case(player);
  else 
    return 0;

  return _access[player];
*/
  return LIBRARIAN_ACCESS;
}


