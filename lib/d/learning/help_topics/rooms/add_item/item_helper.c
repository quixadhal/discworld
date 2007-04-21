/****** This room is made my Mithal, learn and enjoy  ***/

#include "path.h"

inherit "/std/room";

string pattern;
string verb;

void setup() {
   set_short("add_item room #15, Testing and creating a form ");
   set_light(100);

   set_long("add_item room #15, Testing and creating a form "
            +"This item is ment to help in the formulation of items."
	    +"it provides a way to use add_item without coding.  I "
	    +"will try to explain it.  *sigh* You can change the pattern and "
	    +"name of the verb construct seperately. And you can add items to "
	    +"this room.  The 'add an object named XXXX' command creates an "
	    +"object with the name XXXX useing the current verb and pattern.  "
	    +"Once you have added the item, you can look at it, and use the "
	    +"defined verb on it.  The verb will spit out all of the "
	    +"arguments to the function, like room 12.  In this way you can "
	    +"test verb patterns - since many of them don't work - without "
	    +"any coding.  It should help you get an idea how it all works.  "
	    +"\nA reference sheet.\nA stupid object.\n");

   add_exit("west",PATH+"item_conflicts","path");
   add_exit("north",MAIN,"path" );

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

   pattern = "%D";
   verb    = "use";
   
   add_item("stupid object"
      ,({
            "long",       "This object is used to create items in the room!\n"
            +" set object pattern to <pattern> \n"
	    +" name object verb to <verb> \n"
            +" query object  ( returns the current pattern and verb ) \n"
            +" add an object named <item>\n"
            ,"set"      ,({this_object(),"do_set","%D 'pattern' 'to' %s" })
	    ,"name"     ,({this_object(),"do_name","%D 'verb' 'to' %s" })
            ,"query"    ,"@@do_query:"+file_name(this_object())+"@@\n"
            ,"add"      ,({this_object(),"do_add","'an' %D 'named' %s" })
            
        }));
   
   /************ END OF SETUP ***********************/
}

/************** COMMANDS ****************************************/

int do_set(mixed a1,mixed a2,mixed a3,mixed a4, mixed a5, mixed a6)
{
   pattern = (string) a5[1];
   return 1;
}

int do_name(mixed a1,mixed a2,mixed a3,mixed a4, mixed a5, mixed a6)
{
   verb = (string) a5[1];
   return 1;
}

int do_add(mixed a1,mixed a2,mixed a3,mixed a4, mixed a5, mixed a6)
{
   object *obs;

  add_item( a5[1]
      ,({
            "long",
	       "This object's name is: "+a5[1]
	        +"\nAnd it can be used with the pattern:"+pattern
		+":\nAnd verb:"+verb+":\n\n"
            ,verb      ,({this_object(),"do_use",pattern })     
        }));
   
   obs = all_inventory(this_object());
   obs->move("/room/void");
    obs->move(this_object());
   this_player()->move(this_object());
   return 1;
}

string do_query()
{ return "\nPattern:"+pattern+":Verb:"+verb+":\n"; }

int  do_use(mixed a1, mixed a2, mixed a3, mixed a4, mixed a5, mixed a6)
{

  printf("\nThe Verb:(A1):  %O\nThe Objects(A2):\n%O\nDirect Object(A3):  %O\nIndirect Object(A4):  %O\nReturn Array(A5):\n%O\nPattern(A6):  %O\n\n",a1,a2,a3,a4,a5,a6);
  
  tell_object(this_player(),"Hope that worked...\n");
  
  return 1;
}
    
