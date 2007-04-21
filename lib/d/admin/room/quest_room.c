/*  quest_room.c

  This room is used to add, list, and remove quests.

  This file was constructed by Godot (Marshall Buhl).  If you
  make any changes to it, please leave comments in the code
  detailing the changes and let him know about them.  This file
  is not the original, so it is important that he be informed.
  If not, he may unknowingly throw away your changes by
  uploading an new version.

    Change log:

  12/05/92 - Created by Godot.

*/
#include "quest_handler.h"
#include "path.h"

inherit "/std/room/basic_room";

/*
    Global variables.
*/
int level;
string  old_quest;
string  story;
string  title;

/*
    Function prototypes.
*/
int do_create( string quest );
int do_delete( string quest );
int do_help( string what );
int do_level( string new_level );
int do_list();
int do_name( string new_name );
int do_story( string new_story );
int do_title( string new_title );
void  remove_quest( string answer );
void  init();
void  setup();

/*
    Functions.
*/
int
do_create( string quest )
{
/*
  Create the quest.
*/
    if ( !quest )
    {
  write( "Syntax is: create <quest>, where <quest> will be the name of "
       + "the new quest.\n" );
  return 1;
    }
    if ( !level )
    {
  write( "You must set the quest level first.\n" );
  return 1;
    }
    if ( !story )
    {
  write( "You must set the quest story first.\n" );
  return 1;
    }
    if ( !title )
    {
  write( "You must set the quest title first.\n" );
  return 1;
    }
    if ( QUEST_HANDLER->add_quest( quest , level , title , story ) )
    {
  tell_room( this_object() , "Quest \"" + quest + "\" added.\n" );

  level = 0;
  story = 0;
  title = 0;
    }
    else
  tell_room( this_object() , "Quest failed to add.\n" );

    return 1;
}

int
do_delete( string quest )
{
    string  *quests;

    if(!"/secure/master"->query_lord( geteuid( this_player() ) ))
      return 1;
    
      quests = (mixed*) QUEST_HANDLER->query_quest_names();

    if ( quest )
    {
        if ( member_array( quest, quests ) == -1 )
  {
      write( "Quest not found.\n" );
      return 1;
  }
  old_quest = quest;

  write( "\nYou should not remove someone else's quest!!!\n\n"
       + "Are your sure you want to remove this quest? (y/n) > " );
  input_to( "remove_quest" );
  return 1;
    }
    write( "Syntax: remove <quest>, where <quest> is the name of an "
   + "existing quest.\n" );
    return 1;
}

int
do_help( string what )
{
    if ( what != "room" )  return 0;

    write( "Quest room help:\n\n"
   + "  create <string> - Use current settings to create a new quest\n"
   + "                    whose name is <string>.\n"
   + "  delete <string> - Delete the quest whose name is <string>.\n"
   + "                    This option available only to Lords.\n"
   + "  help room       - Generate this help screen.\n"
   + "  level <int>     - Set the level of difficulty to <int>.\n"
   + "  list            - List the current quests.\n"
   + "  story <string>  - Set the quest story to <string>.\n"
   + "  title <string>  - Set the string that is added to the player's\n"
   + "                    title to <string>.\n"
   );
    return 1;
}

int
do_level( string new_level )
{
    if ( new_level )
    {
  sscanf( new_level , "%d" , level );

  tell_room( this_object() , "Quest level set to " + level + ".\n" );
    }
    else
  write( "Syntax: level <new_level>, where new_level is an integer "
       + "indicating how difficult the quest is.\n" );
    return 1;
}

int do_list() {
  int i;
  string list;
  mixed *names;

  if(!"/secure/master"->query_lord( geteuid( this_player() ) ))
    return 1;

  list = "These are the Discworld quests:\n\n";
  names = (mixed *)QUEST_HANDLER->query_quest_names();
  for ( i = 0; i < sizeof( names ); i++ ) {
    list += "Quest "+ ( i + 1 ) +"  \""+ names[ i ] +"\", level "+
        (int)QUEST_HANDLER->query_quest_level( names[ i ] ) +"\n    title: "+
        (string)QUEST_HANDLER->query_quest_title( names[ i ] ) +"\n    story: "+
        (string)QUEST_HANDLER->query_quest_story( names[ i ] ) +"\n";
    if ( i < sizeof( names ) - 1 )
      list += "\n";
   }
  reset_eval_cost();
  this_player()->more_string( list, "Quest List", 1 );
  return 1;
} /* do_list() */

int
do_story( string new_story )
{
    if ( new_story )
    {
  story = new_story;
  tell_room( this_object() , "Quest story set to:\n" + story + "\n" );
    }
    else
  write( "Syntax: story <new_story>, where new_story is a string that "
       + "describes the quest.\n" );
    return 1;
}

int
do_title( string new_title )
{
    if ( new_title )
    {
  title = new_title;
  tell_room( this_object() , "Quest title set to \"" + title + "\".\n" );
    }
    else
  write( "Syntax: title <new_title>, where new_title is the title that "
       + "is appended to the players name for \"who\".\n" );
    return 1;
}

void
init()
{
#ifdef 0
/*
  Make it so only bigwigs can use this object.
*/
    string  euid;
    object  tp;

    tp   = this_player();
    euid = geteuid( tp );

    if ( ( "/secure/master"->god( euid )             )
      || ( "/secure/master"->high_programmer( euid ) )
      || ( "/secure/master"->query_lord( euid )      )
      || ( tp->query_creator()                       )
      || ( tp->query_app_creator()                   ) )
    {
  add_action( "do_create" , "create" );
  add_action( "do_help"   , "help"   );
  add_action( "do_level"  , "level"  );
  add_action( "do_story"  , "story"  );
  add_action( "do_title"  , "title"  );

  if ( ( "/secure/master"->god( euid )             )
    || ( "/secure/master"->high_programmer( euid ) )
    || ( "/secure/master"->query_lord( euid )      ) )
  {
      add_action( "do_delete" , "delete" );
      add_action( "do_list"   , "list"   );
  }
/*
      Reinitialize settings.
*/
  level = 0;
  story = 0;
  title = 0;

  do_help( "room" );

  tell_room( this_object() , "\nQuest settings have been reset to "
         + "nulls by init().\n\n" );
    }
    ::init();
#endif
}

void remove_quest( string answer ) {
/*
  This function is called by input_to() in do_remove().
*/
    answer = lower_case( answer );

    if ( answer == "y" )
    {
  if ( QUEST_HANDLER->delete_quest( old_quest ) )
      tell_room( this_object() , "Quest \"" + old_quest
             + "\" deleted.\n" );
  else
      tell_room( this_object() , "Quest \"" + old_quest + "\" was not "
             + "deleted.\n" );
  return;
    }
    tell_room( this_object() , "Quest \"" + old_quest
           + "\" was not deleted.\n" );
    return;
}

void
setup()
{
    set_short( "Quest Room" );
    //    set_long( "This room is used to create, list, and remove quests.\n"
    //    );
    set_long("Don't use this room, get a lord to use the quests command instead. This room will break the quests!\n");
    set_light( 100 );
    add_exit( "west" , ROOM+"development" , "corridor" );
}

/* EOF */
