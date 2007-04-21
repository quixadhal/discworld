inherit "/std/object";

#define CMD 0
#define PAT 1


void add_cmds( object player );
int do_add( string command, string pattern );
int do_remove( int i );
int do_match( object *indirect_obs, string dir_match, 
    string *indirect_match, mixed args, string pattern );
string read_msg();

string *cmds = ({});

void setup() {
  set_name( "matcher" );
  set_short( "Tannah's pattern matcher" );
  add_property( "determinate", "" );
  add_alias( "pattern matcher" );
  set_long(
    "This is a very handy wossname, useful for testing patterns you "
    "might like to use in an add_command.\n"
  );
  add_adjective( ({ "handy", "patterned" }) );
  set_weight( 1 );
  set_value( 0 );
  add_property( "no recycling", 1 );
}

void init() {
  add_cmds( this_player() );
}

void add_cmds( object player ) {
  set_read_mess( (: read_msg :) );
  player->add_command( "add", this_object(), "command <word'command'> "
      "with pattern <string:quoted'pattern'>",
      (: do_add( $4[0], $4[1] ) :) );
  player->add_command( "remove", this_object(), "command <number>", 
      (: do_remove( $4[0] ) :) );

  if( sizeof( cmds ) ) {
    for( int i = 0; i < sizeof( cmds ); i++ )
      player->add_command( cmds[i][CMD], this_object(), cmds[i][PAT], 
          (: do_match :) );
  }
}

int do_add( string command, string pattern ) {
  cmds += ({ ({ command, pattern }) });
  this_player()->remove_object( this_object(), 1 );
  add_cmds( this_player() );
  printf( "You add the command \"%s\" with the pattern \"%s\" to the "
      "pattern matcher.\n", command, pattern );
  return 1;
}

int do_remove( int i ) {
  if( i >= sizeof( cmds ) || i < 0 ) {
    printf( "Invalid command number.\n" );
    return 0;
  }
  printf( "You remove the command \"%s\" with the pattern \"%s\" from "
      "the pattern matcher.\n", cmds[i][CMD], cmds[i][PAT] );
  cmds = cmds[0..i-1] + cmds[i+1..];
  this_player()->remove_object( this_object(), 1 );
  add_cmds( this_player() );
  return 1;
}  

int do_match( object *indirect_obs, string dir_match, 
    string *indirect_match, mixed args, string pattern ) {
  printf( "Indirect objects: %O\n", indirect_obs );
  printf( "Direct match: %s\n", dir_match );
  printf( "Indirect match: %O\n", indirect_match );
  printf( "Args: %O\n", args );
  printf( "Pattern: \"%s\"\n", pattern );
  return 1;
}

string read_msg() {
  int i;
  string msg;

  msg = "The pattern matcher is currently set to test the following "
        "commands and patterns:\n";
  if( !sizeof( cmds ) ) msg += "  None.\n";
  else for( i = 0; i < sizeof( cmds ); i++ ) {
    msg += sprintf( "[%d] \"%s\", \"%s\"\n", i, cmds[i][CMD], 
           cmds[i][PAT] );
  }
  msg += "See 'syntax add' and 'syntax remove' to modify the list.";
  return msg;
  msg += "For further [nearly accurate] information on add_command, "
    "see the files in /w/tannah/learning/add_cmd.\n";
}

void list_cmds() {
  printf( "Commands and patterns added:\n%O\n", cmds );
}
