/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: cmr_handler.c,v 1.4 1998/10/04 23:31:43 ceres Exp $
 * $Log: cmr_handler.c,v $
 * Revision 1.4  1998/10/04 23:31:43  ceres
 * improved autodoc
 *
 * Revision 1.3  1998/10/04 23:14:03  ceres
 * Updated the documentation
 *
 * Revision 1.2  1998/10/04 22:43:41  ceres
 * Added documentation and fixed error with using obsolete functions.
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
#include <cmr.h>
#include <colour.h>
#include <library.h>

#define SAVE_FILE "/save/cmr_handler"

inherit "/std/object";

/**
 * This handler deals with identifying materials and colours. Your skill and
 * experience are used to determine if you can successfully identify a
 * particular material or colour.
 *
 * @author Deutha
 */
string *colour_names;
mapping colour_details;
string *material_names;
mapping material_details;

void load_cmr_handler();
void save_cmr_handler();
void create();
string add_colour( string word, int fine, int crude, int qp_thresh,
                int sp_thresh );
int *query_colour_details( string word );
int delete_colour( string word );
string *query_colour_names();
string identify_colour( string word, object player );
string add_material( string word, int colour, int type, int skill_reqd,
                  int skill_thresh );
int *query_material_details( string word );
int delete_material( string word );
string *query_material_names();
string identify_material( string word, object player, int article );
string query_material_adjective( string word );

/**
 * @ignore yes
 */
void load_cmr_handler() {
  if ( file_size( SAVE_FILE +".o" ) > 0 )
    unguarded((: restore_object, SAVE_FILE :));
} /* load_cmr_handler() */

/**
 * @ignore yes
 */
void save_cmr_handler() {
  unguarded((: save_object, SAVE_FILE :));
} /* save_cmr_handler() */

/**
 * @ignore yes
 */
void create() {
  colour_names = ({ });
  colour_details = ([ ]);
  material_names = ({ });
  material_details = ([ ]);
  ::create();
  load_cmr_handler();
} /* create() */

/**
 * This function is used to add a known colour to the handler.
 *
 * @param word The name of the colour
 * @param fine Not sure
 * @param crude Not sure
 * @param qp_thresh Quest points required to help identify this colour
 * @param sp_thresh Social points required to help identify this colour
 */
string add_colour( string word, int fine, int crude, int qp_thresh,
                int sp_thresh ) {
  int temp;
  temp = member_array( word, colour_names );
  if ( temp != -1 ) return "colour already exists";
  colour_names += ({ word });
  colour_details += ([ word : ({ fine, crude, qp_thresh, sp_thresh }) ]);
  save_cmr_handler();
  return "the colour "+ word +", a shade of "+ ( ( fine == crude )?
         "pure ":MODIFIERS[fine] ) + COLOURS[crude] +", with thresholds of "+
         qp_thresh +" xp and "+ sp_thresh +" sp";
} /* add_colour() */

/**
 * Returns the array of information stored about a particular colour. The
 * array is setup using add_colour.
 *
 * @param word The colour in question
 * @see add_colour
 */
int *query_colour_details( string word ) {
  int temp;
  temp = member_array( word, colour_names );
  if ( temp == -1 ) return ({ -1 });
  return colour_details[word];
} /* query_colour_details() */

/**
 * Remove a colour from the handler.
 *
 * @param word The colour to remove.
 */
int delete_colour( string word ) {
  int temp;
  temp = member_array( word, colour_names );
  if ( temp == -1 ) return 0;
  colour_names -= ({ word });
  colour_details = m_delete( colour_details, word );
  save_cmr_handler();
  return 1;
} /* delete_colour() */

/**
 * List all the known colours in the handler.
 *
 * @return An array of strings of colour names.
 */
string *query_colour_names() { return colour_names + ({ }); }

/**
 * Used to determine if a player can identify a particular colour. The
 * function returns the colours name or a string giving a rough description
 * if they're unable to identify it.
 *
 * @param word The colour they're trying to identify.
 * @param player The player trying to identify the colour.
 * @return Description.
 */
string identify_colour( string word, object player ) {
  int temp, *args;

  temp = member_array( word, colour_names );

  if ( temp == -1 )
    return "unknown colour";

  args = allocate( 4 );
  args = colour_details[word];

  if ( !player )
    return word;
  
  switch ( ( ( LIBRARY->query_quest_points(player->query_name()) ) > args[2] )+
           ( ( player->query_max_gp() ) > args[3] ) ) {
  case 0:
    return COLOURS[args[1]];
  case 1:
    return ( ( ( args[0] == args[1] )?"pure ":MODIFIERS[args[0]] ) +
             COLOURS[args[1]] );
  default:
    return word;
  }
} /* identify_colour() */

/**
 * Add a new material to the handler.
 * The colour of the material is an index into the COLOUR array defined in
 * colour.h. The type of material is an index into the MATERIALS array defined
 * in cmr.h. skill_reqd is an index into the SKILLS array defined in cmr.h but
 * can also take the special value of 99 to indicate that the material can only
 * be identified if someone has told the person what the material is or 0 to
 * indicate that everyone knows what the material is.
 * If the skill_reqd is a skill the skill_thresh is the bonus needed in that
 * skill to identify the material.
 *
 * @param word The name of the material
 * @param colour The index into the colour array for the colour of the
                 material. (see colour.h)
 * @param type Type of material
 * @param skill_reqd Index into the array of skills (see cmr_handler.h)
 * @param skill_thresh The bonus required in the appropriate skill.
 */
string add_material( string word, int colour, int type, int skill_reqd,
                  int skill_thresh ) {
  int temp;
  string text;
  temp = member_array( word, material_names );
  if ( temp != -1 ) return "material already exists";
  material_names += ({ word });
  material_details += ([ word : ({ colour, type, skill_reqd,
                         skill_thresh }) ]);
  save_cmr_handler();
  if ( !skill_reqd )
    text = " that is always recognised";
  else
    if ( skill_reqd == LEARNT )
      text = " that is recognised through knowledge";
    else
      text = " that has a threshold of "+ skill_thresh +" in "+
             SKILLS[skill_reqd];
  return word +", a"+ ( ( colour == COL_ORANGE )?"n ":" " ) +
         COLOURS[colour] +" "+ MATERIALS[type] + text;
} /* add_material() */

/**
 * Return the details of a material set with add_material
 *
 * @param word The material in question
 * @return An integer array of material details.
 */
int *query_material_details( string word ) {
  int temp;
  temp = member_array( word, material_names );
  if ( temp == -1 ) return ({ -1 });
  return material_details[word];
} /* query_material_details() */

/**
 * Remove a material from the handler.
 *
 * @param word The name of the material to remove.
 */
int delete_material( string word ) {
  int temp;
  temp = member_array( word, material_names );
  if ( temp == -1 ) return 0;
  material_names -= ({ word });
  material_details = m_delete( material_details, word );
  save_cmr_handler();
  return 1;
} /* delete_material() */

/**
 * List all the known materials by name.
 */
string *query_material_names() { return material_names + ({ }); }

/**
 * Used to determine if a player can identify a particular material.
 * The function returns the materials name or a string giving a rough
 * description if they're unable to identify it.
 *
 * Some materials can be identified by being high enough at a particular skill
 * (defined in cmr.h) while others are only learned by being taught and others
 * are known to everyone.
 *
 * @param word The name of the material
 * @param player The player trying to identify it.
 * @param article Definite or indefinite article.
 * @return Description.
 *
 * @see cmr_library
 * @see cmr
 */
string identify_material( string word, object player, int article ) {
  int temp, *args;
  string prefix;
  temp = member_array( word, material_names );
  if ( temp == -1 ) return "unknown material";
  args = allocate( 4 );
  args = material_details[word];
  prefix = ( ( article )?"a"+ ( ( args[0] == COL_ORANGE )?"n ":" " ):"" );
   if ( !args[ 2 ] || !player )
      return word;
  if ( args[2] == LEARNT )
    if ( member_array( word,
        (string *)CMR_LIBRARY->query_known_materials( player->query_name() ) )
        == -1 )
      return prefix + COLOURS[ args[ 0 ] ] +" "+
          MATERIALS[ args[ 1 ] ];
    else
      return word;
  if ( ( player->query_skill_bonus( SKILLS[args[2]] ) ) > args[3] )
    return word;
  else return prefix + COLOURS[args[0]] +" "+ MATERIALS[args[1]];
} /* identify_material() */

/**
 * Not sure about this. Seems to return a colour and adjective for a material.
 * eg. slate returns 'grey mineral'.
 * @param The name of the material.
 * @return A description.
 */
string query_material_adjective( string word ) {
  int temp, *args;
  temp = member_array( word, material_names );
  if ( temp == -1 ) return "an unknown material";
  args = allocate( 4 );
  args = material_details[word];
  return COLOURS[args[0]] +" "+ MATERIAL_ADJECTIVES[args[1]];
} /* query_material_adjective() */

/**
 * Returns the ansi colour associated with the material eg. yellow for gold,
 * red for a red mineral pebble etc.
 * @param word the material's name.
 * @return ansi colour sequence for that colour.
 */
string query_material_ansi_colour( string word ) {
  int temp, *args;
  temp = member_array( word, material_names );
  if ( temp == -1 ) return "";
  args = allocate( 4 );
  args = material_details[word];
  return ANSI_COLOURS[args[0]];
} /* query_material_ansi_colour() */
