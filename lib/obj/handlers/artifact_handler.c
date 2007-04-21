/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: artifact_handler.c,v 1.1 1998/01/06 05:03:33 ceres Exp $
 * $Log: artifact_handler.c,v $
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/**
 * Keeps track of all the artifacts and the players known artifacts
 * arrays.
 */
#define SAVE_FILE "/save/artifacts"
#define SAVE_DIR "/save/artifacts/"

mapping artifacts;

/*
 * artifacts mapping has elements of the form:
 *    name : ({ file, max level, min level, bonus to make, value info })
 * e.g. "wand of eternal light" :
 *        ({ "/obj/wands/eternal_light", 85, 65, 250, 10000 })
 */

void create() {
   seteuid( (string)"/secure/master"->
         creator_file( file_name( this_object() ) ) );
   artifacts = ([ ]);
   if ( file_size( SAVE_FILE +".o" ) > 0 )
      unguarded( (: restore_object, SAVE_FILE :) );
} /* create() */

/**
 * This method returns the artifacts mapping.
 * The artifacts mapping has elements of the form:<br><pre><code>
 *    name : ({ file, max level, min level, bonus to make, value info })
 * e.g. "wand of eternal light" :
 *        ({ "/obj/wands/eternal_light", 85, 65, 250, 10000 })</code></pre>
 * @return the artificats mapping
 */
mapping query_artifacts() { return artifacts; }

/** Saves the current state to disk. */
void save_me() { unguarded( (: save_object, SAVE_FILE :) ); }

/**
 * Sets the entire artificats mapping.  This function should not be
 * used.  This looks rather too dangerous, commented out.
 * @param map the mapping to set it to
 *
void set_artifacts( mapping map ) {
  artifacts = map;
  save_me();
} * set_artifacts() */

/**
 * Returns the information about the named artificats.
 * @param name the artificant name to get information on
 * @return the informatioj on the artificant, 0 if the artifact does not exist
 */
int query_artifact( string name ) {
  if ( !artifacts[ name ] )
    return 0;
  return 1;
} /* query_artifact() */

/**
 * Add a new artifact into the artificat mapping. 
 * @see query_artificats()
 * @param name the name of the artifact
 * @param file the file name of the artifact
 * @param max the maximum level
 * @param min the minimum level
 * @param bonus the bonus required to make
 * @param value the value of the artifact
 */
void add_artifact( string name, string file, int max, int min, int bonus,
                   int value ) {
  artifacts[ name ] = ({ file, max, min, bonus, value });
  save_me();
} /* add_artifact() */

/**
 * Remove the artifact of the given name.
 * @param name the name of the artifiact to remove
 */
void remove_artifact( string name ) {
  if ( !artifacts[ name ] )
    return;
  artifacts = m_delete( artifacts, name );
  save_me();
} /* remove_artifact() */

/**
 * Find the file associated with the artifact.
 * @param name the name of the artifact
 * @return the file associated with the artifact
 */
string query_artifact_file( string name ) {
  if ( !artifacts[ name ] )
    return 0;
  return artifacts[ name ][ 0 ];
} /* query_artifact_file() */

/**
 * Find the maximum level associated with the artifact.
 * @param name the name of the artifact
 * @return the maximum level associated with the artifact
 */
int query_artifact_max( string name ) {
  if ( !artifacts[ name ] )    return 0;
  return artifacts[ name ][ 1 ];
} /* query_artifact_max() */

/**
 * Find the minimum level associated with the artifact.
 * @param name the name of the artifact
 * @return the minimum level associated with the artifact
 */
int query_artifact_min( string name ) {
  if ( !artifacts[ name ] )
    return 0;
  return artifacts[ name ][ 2 ];
} /* query_artifact_min() */

/**
 * Find the bonus to make associated with the artifact.
 * @param name the name of the artifact
 * @return the bonus to make associated with the artifact
 */
int query_artifact_bonus( string name ) {
  if ( !artifacts[ name ] )
    return 0;
  return artifacts[ name ][ 3 ];
} /* query_artifact_bonus() */

/**
 * Find the value associated with the artifact.
 * @param name the name of the artifact
 * @return the value associated with the artifact
 */
int query_artifact_value_info( string name ) {
  if ( !artifacts[ name ] )
    return 0;
  return artifacts[ name ][ 4 ];
} /* query_artifact_value_info() */

/**
 * Find the level of the artifact.  The level is determined as the
 * difference between the minimum and maximum artifact level squared
 * divided by the bonus passed in minus the bonus to create  plus the difference
 * between the maximum and minimum levels plus the maximum level.<br>
 * ie:<pre><code>
 * level = (((max-min) * (max-min)) / (bonus - make_bonus + max - min)) + min
 * </pre></code>
 * @param name the name of the artifact
 * @param bonus the bonus modifier
 * @return the level of the artifact
 */
int query_artifact_level( string name, int bonus ) {
  int number;

  if ( !artifacts[ name ] )
    return 0;
  number = artifacts[ name ][ 1 ] - artifacts[ name ][ 2 ];
  number *= number;
  number /= bonus - artifacts[ name ][ 3 ] + artifacts[ name ][ 1 ] -
      artifacts[ name ][ 2 ];
  number += artifacts[ name ][ 2 ];
  return number;
} /* query_artifact_level() */

/**
 * Determine the value of the artificant.  The value is determined by
 * the following equation:<br><code><pre>
 * value = (value * max) / query_artifiact_level(name, bonus)
 * </code></pre>
 * @param name the name of the artifact
 * @param bonus the bonus modifier
 * @return the value of the artifact
 */
int query_artifact_value( string name, int bonus ) {
  int number;

  if ( !artifacts[ name ] )
    return 0;
  number = artifacts[ name ][ 4 ] * artifacts[ name ][ 1 ];
  number /= query_artifact_level( name, bonus );
  return number;
} /* query_artifact_value() */

/**
 * Let a player attempt to find out what type of artifact the
 * thing is.
 * @param thing the item to diagnose
 * @param person the person doing the diagnosis
 */
void diagnose_artifact( object thing, object person, object) {
   int charges;
   string type;

   if ( !thing || !person )
      return;
   type = (string)thing->query_artifact_type();
   if ( !stringp( type ) ) {
      tell_object( person, "There doesn't seem to be anything special about "+
            (string)thing->the_short() +".\n" );
      return;
   }
   charges = (int)thing->query_charges();
   tell_object( person, (string)thing->the_short() +" is a level "+
         (int)thing->query_level() +" magic "+ type +".  It has "+
         ( charges == 1 ? "one charge" : query_num( charges ) +" charges" ) +
         " remaining.\n" );
} /* diagnose_artifact() */

/** 
 * The name of the wand.
 * @return the wand name
 */
string query_wand_name() { return "wand of artifact probing"; }

/**
 * Zap the wand!
 * @param thing the thing to zap with the wand
 * @param person the person doing the zapping
 * @param wand the wand being zapped
 */
void zap( object thing, object person, object wand ) {
  call_out( "diagnose_artifact", 0, thing, person, wand );
} /* zap() */

/**
 * A person will not forget how to recognise an artifact (unless they
 *     refresh, in which case all of them are forgotten, i.e. the file
 *     is wiped) so to let a person learn to recognise an artifact, add
 *     a new line to their file consisting of its name and to see if
 *     they know an artifact, check in their file for its name. This
 *     method doesn't require a library handler.
 * @param person the person check to see if they know the artifact
 * @param artifact the artifact being checked
 * @return 1 if they know it, 0 if they do not
 */
int query_known( string person, string artifact ) {
  int i;
  string *list;
  if ( !artifacts[ artifact ] )
    return 0;
  if ( unguarded((: file_length, SAVE_DIR + person :)) < 1 )
    return 0;
  list = explode( unguarded((: read_file, SAVE_DIR + person :)), "\n" );
  for ( i = 0; i < sizeof( list ); i++ ) {
    if ( list[ i ] == artifact )
      return 1;
  }
  return 0;
} /* query_known() */

/**
 * Place an artifact into a players known artifact list.
 * @param person the name of the person knowing the artifact
 * @param artifact the name of the artifact being know
 */
void add_known( string person, string artifact ) {
  if ( query_known( person, artifact ) )
    return;
  unguarded((: write_file, SAVE_DIR + person, artifact +"\n" :));
} /* add_known() */
