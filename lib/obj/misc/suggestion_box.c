/* This is a [hopefully] generic suggestion box. 
 * it logs its suggstions to a file
 *
 * Belle March 2002
 */


inherit "/std/object";
int do_fill();
string _savedir, _savefile;

/**
 * @@ignore_yes
 */
void setup() {
  set_short( "suggestion box" );
  set_long( "This is a suggestion box.  It is sitting on a pole which puts"
	    " the box at a convienient height for writing.  It has a pile "
	    "of small papers on it on which you can write suggestions.  "
	    "There is a slot in the top for stuffing the suggestion in "
	    "when you're done.\n" );
  set_name( "box" );
  set_main_plural( "boxes" );
  set_read_mess( "\nPlease fill in a suggestion form", "common" );
  add_property( "there", "sitting in the middle of the floor" );
  set_weight( 1000 ); 
  reset_get();
}

/**
 * @@ignore_yes
 */
void init() {
  add_command( "fill", "in a suggestion form", (: do_fill() :) );
}


/**
 * This function gives the suggestion box a file to write its suggestions to
 * @param string file The path of the directory to write suggestions to 
 * it had better end in / or you will be sorry.
 */
void set_save_file ( string file ) {
  _savefile = file;
  return;
}


/**
 * This function gives the suggestion box a directory to write its 
 * suggestions to. Either a save file *or* a save directory is required.
 * or your suggestions will not save.  If both are set then the save file
 * will be used.
 * @param string dir The path of the directory to write suggestions to 
 * it had better not end in / or you will be sorry.
 * @see set_save_file()
 */
void set_save_dir ( string dir ) {
  _savedir = dir;
  return;
}

/**
 * This function can be used to query what save file is in use for the
 * suggestion box.  Either a save file *or* a save directory is required.
 * or your suggestions will not save.  If both are set then the save file
 * will be used.
 * @return the full path to the file where the suggestions are kept
 * @see query_save_dir() 
 */

string query_save_file () {
  return _savefile;
}

/**
 * This function can be used to query what save directory is in use for the
 * suggestion box.  Either a save file *or* a save directory is required.
 * or your suggestions will not save.  If both are set then the save file
 * will be used.  If a save dir is used, the file name will be in the form of
 * save_dir/player_name-time.txt
 * @return the full path to the file where the suggestions are kept
 * @see query_save_file() 
 */

string query_save_dir () {
  return _savedir;
}

/**
 * @@ignore_yes
 */
int do_fill() { 
  tell_object( this_player(), "Please fill in your suggestion below.\n" );
  this_player()->do_edit( 0, "end_suggestion", this_object() );
  
  add_succeeded_mess( ({"", "$N fill$s in a suggestion form and stuff$s "
			  "it in the box.\n" }) );

  return 1;
}

/**
 * @@ignore_yes
 */
void end_suggestion( string text ) {
  string player, file;

  if( _savefile ) {
    // save to a file
    file = _savefile;
  } else if( _savedir ) {
    player = previous_object()->query_name();
    file = sprintf( query_save_dir() + "/%s-%d.txt", player, time() );
  } 

  debug_printf( "File: %s", file );

  if( !(unguarded( (: write_file, file, text :) ) ) ) {
    tell_object( previous_object(), "Sorry, your suggestion was not recorded."
		 "  Please use the 'bug object' command to bug report the "
		 "suggestion box.\n" );
    return;
  }

  tell_object( previous_object(), "You fill in a suggestion form and stuff "
	       "it in the box.\n" );
}
  
