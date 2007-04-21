/*
 * $Locker:  $
 * $Id: graffbase.c,v 1.2 2001/07/25 00:39:17 olorin Exp $
 *
 * This is the temporary and hacked version of Taniwha's graffiti base code.
 * The original files can be found in /w/hobbes/misc/graffiti/
 * Based on /obj/misc/quill
 * Taniwha, 1997
 *
 * $Log: graffbase.c,v $
 * Revision 1.2  2001/07/25 00:39:17  olorin
 * Removed a spurious definition of the type var, which conflicted
 * with a global var, and wasn't used anyway.
 * Fixed a silly sentence construct in a fail message as well.
 *
 * Revision 1.1  1998/01/15 01:37:46  ceres
 * Initial revision
 *
 */

#include <language.h>
// Property to add to an object to allow scrawling

#define GRAFFITI "graffiti"

#define ETP environment(this_player())
#define TO this_object()


object writing_on;
 /* string type; */
object old;
string olds;

/* Functions to override in inheriting objects */
string query_drawingwith(); // name of thing doing the drawing
int do_check();  // do we have the "pen and paper"
int done_check(); // Clean uo routine, break chalk etc

string query_drawingwith() { return "something"; }
int do_check() { return 0; } // fail
int done_check() { return 0; }


void init() {
 this_player()->add_command( "scrawl", this_object(), "with charcoal on wall");
 this_player()->add_command( "scrawl", this_object(), "on wall with charcoal");
} /* init() */

int do_scrawl( object *things ) {
    string language;
   /* if(!this_object()->do_check() ) return 0;*/
    if ( !ETP->query_property( GRAFFITI) ) {
        this_player()->add_failed_mess( this_object(), "You can't scrawl on "+
          "this wall.\n");
        return 0;
    }
    language = (string)this_player()->query_current_language();
    if ( !LANGUAGE_HAND->query_language_written( language ) ) {
        this_player()->add_failed_mess( this_object(), "You can't scrawl on "
           "a wall when you're not using a language that can be used for "
          "scrawling on a wall with.\n",({writing_on}));
        return 0;
    }

    if(( old = present("graffiti",ETP)) && !interactive(old))
    {
        olds = old->query_property("graffiti");
    }
    if ( writing_on ) {
        this_player()->add_failed_mess( this_object(), "The writing "+
          "spot seems to be busy at the moment.\n",
          ({writing_on}));
        return 0;
    }
    this_player()->add_succeeded_mess( this_object(), ({"You start scrawling "+
      "on a wall with some charcoal.\n", "$N starts scrawling on a wall with "
      "some charcoal.\n"}) );
    writing_on = ETP;
    call_out( "begin_writing", 0, this_player() );
    return 1;
} /* do_write() */

void begin_writing( object writer ) {
    if ( !writer ) {
        writing_on = 0;
        return;
    }
    writer->do_edit( 0, "end_writing" );
} /* begin_writing() */

void end_writing( string words ) {
    string language;
    string extra;
    if ( !words || ( words == "" ) ) {
        write( "You don't scrawl anything on "+
         "the wall.\n");
        say( (string)this_player()->the_short() +" doesn't "+
          "scrawl anything after all.\n" );
        writing_on = 0;
        return;
    }
    language = (string)this_player()->query_current_language();
    if ( !LANGUAGE_HAND->query_language_written( language ) ) {
        write( "You seem to have changed to using a non-written language.\n" );
        say( (string)this_player()->the_short() +" seems "+
          "linguistically-challenged.\n" );
        writing_on = 0;
        return;
    }
    if ( !writing_on ) {
        write( "You seem to have lost what you were scrawling on.\n" );
        say( (string)this_player()->the_short() +" looks around "+
          "in puzzlement.\n" );
        return;
    }
    if (  writing_on != ETP ) {
        write("The wall seems to have run away from you.\n" );
        say( (string)this_player()->the_short() +" looks like "+
          (string)this_player()->query_pronoun() +"'s lost something.\n" );
        writing_on = 0;
        return;
    }
    /*   
       writing_on->add_read_mess( words, type, language, 0 );
    */
    if(!stringp(olds)) olds = "";
    words = olds +"\n"+"%^MAGENTA%^Scrawled with rough charcoal, you "
             "read the words:%^RESET%^\n\n"+
    words;
    if(old) old->move("/room/rubbish");
    extra = TO->extra_stuff();
    if(!strlen(extra))extra = "Some disreputable character has scrawled "
        "graffiti over this wall.  You may be able to \"read\" it.\n";
    ETP->add_sign(
      extra,
      words,	
      "graffiti",
      ({"graffiti","scrawl"}),
      language
    );
    old = present("graffiti",ETP);
    old->add_property("determinate","some ");
    old->add_property("there","on the wall");
    if(old) old->add_property("graffiti",words);

    write( "As you finish scrawling on the wall, the piece of charcoal "
           "breaks.\n" );
    say( (string)this_player()->the_short() +" finishes scrawling "+
      "on the wall.\n" );
    this_object()->done_check();
    writing_on = 0;
} /* end_writing() */
