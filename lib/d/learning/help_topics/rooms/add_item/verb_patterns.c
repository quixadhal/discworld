#include "path.h"

inherit "/std/room";

void setup() {
   set_short("add_item room #9, verb patterns ");
   set_light(100);

   set_long("add_item room #9, verb patterns\n"
            +"Outdated! see man add_command for patterns\n "
            +"As you guessed I am still holding out on you.  There is, "
            +"unfortunately, tons more!  Most things take the form: VERB "
            +"DIRECT_OBJECT or $V $D.  Add item has a limited number of "
            +"patterns that it can recognize, the first limitation is that "
            +"the verb must come first.  ie VERB PATTERN.  The default "
            +"pattern we have been using to date is %D.  We can change the "
            +"pattern!\nReference sheet.\nLarge pool.\nNote.\n");
   
   add_item("reference sheet",
            "Pattern: Is the add_command pattern( parse_command): \n"
            +"Example string = \" 'get' / 'take' %i \" \n"
            +"Syntax:\n"
            +"  'word'          obligatory text     \n"
            +"  [word]          optional text       \n"
            +"  /               Alternative marker  \n"
            +"  %o              Single item, object \n"
            +"  %l              Living objects      \n"
            +"  %s              Any text            \n"
            +"  %w              Any word            \n"
            +"  %p              One of a list (prepositions)  \n"
            +"  %i              Any items                     \n"
            +"  %d              Number 0- or tx(0-99)         \n"
            +"Things from add_command                         \n"
            +"  %D              Direct Object                 \n"
            +"  %I              Indirect object               \n\n"
            +"Currently %s and %w behave a bit erradically... \n"
            +"If you put 'text' before and after them it might help.\n"
            +"Hope this helps!\n" );
   
   add_item("note", ({ "long", "It can be read.",
      "read", "Here is the code for the pool:\n"
      +"  add_item(\"large pool\", ({  \n"
      +"       \"long\",\n"
      +"                   \"It looks like you can jump into it!\"\n"
      +"	,\"jump\",\n"
      +"		   ({ this_object(), \"do_jump\", \n"
      +"			\" 'into' / 'in' %D \"  })  \n"
      +"	 )} );\n\n"
      +"Ok. Not much different, just one more argument.  Look at the "
      +"reference sheet.  Looks complicated hunh?  The most useful ones "
      +"are %D, %I, %d, [text], 'text', and /.  I will demonstrate how to "
      +"use %d and %I in a few rooms.  I have also changed the succeed "
      +"mess, otherwise it would say: \"You jump into / in a large pool.\"  "
      +"Not good huh? *smile* Oh well on to the next room!\n" }));
   
   
   add_item( "large pool", ({
      "long", "It looks like you can jump into it!",
      "jump", ({ this_object(), "do_jump", " [into|in] <direct:object>" })
   }) );
   
   add_exit( "east", PATH +"func_arguments", "path");
   add_exit( "west", PATH +"verb_success", "path");
   add_exit( "start", MAIN, "path" );
   add_exit( "exit", LEARNING +"search", "path" );
} /* setup() */

int do_jump() {
   this_player()->add_succeeded_mess(previous_object(),
                                     "$N $V into a pool!\n",  ({}));
   return 1;
} /* do_jump() */
