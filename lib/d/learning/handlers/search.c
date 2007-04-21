#include <learning.h>

#define SAVE SAVEPATH +"search"

mapping rooms,       /* rooms[ keyword ] = array of rooms with this keyword
                      */
        objects,     /* objects[ keyword ] = array of objects with this keyword
                      */
        keywords,    /* keywords[ filename ] = array of keywords defined
                      * by file
                      */
        functions;   /* functions[ filename ] = array of functions defined 
                      * by object filename
                      */
string *directories; /* Array holding the directories that still need
                      * checking.  
                      * Used only while collecting information from 
                      * rooms in the domain.
                      */
nosave string word;

/* create() is the first function called in any object when it is 
 * cloned.
 */
void create() {
   /* seteuid is needed for the save_object and restore_object calls 
    */
   seteuid( (string)master()->get_bb_uid() );
   /* restore the saved values for the 
    */
   unguarded( (: restore_object, SAVE :) );
} /* create() */

/* Given words keywords chosen so far,
 * return ({ ({ keywords }), ({ rooms }), ({ objects }) }),
 * where keywords are all keywords that are in at least one of the rooms
 * or objects, objects are all objects having all words defined
 * and rooms are all rooms having all words defined.
 */
mixed get_keywords( string *words ) {   
   string *found_words, *found_rooms, *found_objects, key, *value;
   
   if( sizeof( words ) ) {
      /* all files known */
      found_objects = keys( keywords );    
      foreach( word in words ) {
         /* remove all files that haven't defined the keyword */
         if( objects[ word ] ) {
            found_objects = filter_array( found_objects, 
                (: member_array( $1, objects[ word ] ) != -1 :) ); 
         } else {
            found_objects = ({ });
         }
      }
      /* all files known */
      found_rooms = keys( keywords );    
      foreach( word in words ) {
         /* remove all files that haven't defined the keyword */
         if( rooms[ word ] ) {
            found_rooms = filter_array( found_rooms, 
                (: member_array( $1, rooms[ word ] ) != -1 :) );
         } else {
            found_rooms = ({ });
         }
      }
      found_words = ({ });
      foreach( word in found_rooms ) {
         found_words -= keywords[ word ]; /* remove duplicates */
         found_words += keywords[ word ]; /* add new ones */
      }
      foreach( word in found_objects ) {
         found_words -= keywords[ word ]; /* remove duplicates */
         found_words += keywords[ word ]; /* add new ones */
      }
      found_words -= words; /* remove those found already */
   } else {
      found_rooms = ({ });
      foreach( key, value in rooms ) {
         found_rooms -= value; /* remove duplicates */
         found_rooms += value; /* add new ones */
      }
      found_objects = ({ });
      foreach( key, value in objects ) {
         found_objects -= value; /* remove duplicates */
         found_objects += value; /* add new ones */
      }
      found_words = keys( rooms );
      found_words -= keys( objects );
      found_words += keys( objects );
   }
   return ({ found_words, found_rooms, found_objects });
} /* get_keyword() */

/* Get information for specific file.
 */
void update_info_for( string file ) {
   string *words, word;
   object obj;

   if( !catch( file->force_load() ) ) {
      obj = find_object( file );
      words = obj->query_property( "commented functions" );
      if( words ) {
         functions[ file ] = words;
      }
      words = obj->query_property( "keywords" );
      if( words ) {
         keywords[ file ] = words;
         if( function_exists( "add_exit", obj ) ) { /* it's a room */
            foreach ( word in words ) {
               if( rooms[ word ] ) {
                  rooms[ word ] -= ({ file }); /* remove previous definition */
                  rooms[ word ] += ({ file }); /* add new */
               } else {
                  rooms[ word ] = ({ file });
               }
            }
         } else {
            foreach ( word in words ) {
               if( objects[ word ] ) {
                  objects[ word ] -= ({ file }); /* remove previous definition */
                  objects[ word ] += ({ file }); /* add new */
               } else {
                  objects[ word ] = ({ file });
               }
            }
         }
      }
      if( !directories ) { /* This is a single room so save the new info */
         unguarded( (: save_object, SAVE :) );
      }
   }
} /* update_info_for() */

/* This function does the same as the update command used on the room
 * Basically, it moves all users in the room, to the void, destructs the room
 * loads it again, and moves prople back again.
 */
void update_search( string room ) {
   object *obs;
   int i;

   obs = all_inventory( find_object( room ) );
   i = sizeof( obs );
   while( i--  ) {
      if ( userp( obs[ i ] ) ) {
         obs[ i ]->move( "/room/void" );
      } else {
         obs = delete( obs, i, 1 );
      }
   }
   room->dest_me();
   room->force_load();
   obs->move( room );
} /* update_search() */

/* collects the information from the next directory in line, adding new
 * subdirectories when found.
 */
void collect_one() {
   string dir, file, *funcs;
   mixed dirs, file_info;
   
   dir = directories[ 0 ];
   dirs = get_dir( dir, -1 );
   if( dirs ) {
      foreach ( file_info in dirs ) {
         if( file_info[ 1 ] == -2 ) { /* it's a directory */
            if( file_info[ 0 ] != "old" ) { /* ignore the files in the old dir */
               directories = directories + ({ dir + file_info[ 0 ] +"/" });
            }
         } else { /* it's a file */
            if( ( sizeof( file_info[ 0 ] ) > 2 ) && 
                ( file_info[ 0 ][ <2 .. <1 ] == ".c" ) ) {
               update_info_for( dir + file_info[ 0 ][ 0 .. <3 ] );
            }
         }
      }
   }
   directories = directories[ 1 .. <1 ]; /* remove the dir that where just checked*/
   if( find_call_out( "collect_one" ) == -1 && sizeof( directories ) ) {
      call_out( "collect_one", 3 );
   } else if( !sizeof( directories ) ) { /* we are done so save */
      /* tell me it's done */
      tell_object( find_living( "olorin" ), "Collect finished.\n" ); 
      directories = 0;
      /* and save it */
      unguarded( (: save_object, SAVE :) );
      /* this call is to restore all the keyword and function exits 
       * to the first search and function rooms
       */
      update_search( LEARNING +"search" );
      update_search( LEARNING +"functions" );
   }
} /* collect_one */

void collect() {
   /* This function will start a run through all files in the subdirectories of
    * "/d/learning" and collect the needed information.
    */
   rooms=([ ]);
   objects=([ ]);
   functions=([ ]);
   keywords=([ ]);
   directories=({ "/d/learning/" });
   collect_one();
} /* collect() */

int query_collecting() {
   if( directories ) {
      return 1;
   } else { 
      return 0;
   }
} /* query_collecting() */

string find_room( string *words ) {
   object thing, *things;
   string word_mark;

   if( !sizeof( words ) )
     return LEARNING +"search";
   word_mark = implode( sort_array( words, 1 ), ", " );
   things = children( SEARCH_ROOM ) - ({ find_object( SEARCH_ROOM ) });
   foreach ( thing in things ) {
      if ( (string)thing->query_marker() == word_mark )
        return file_name( thing );
   }
   thing = clone_object( SEARCH_ROOM );
   thing->set_marker( word_mark, words );
   return file_name( thing );
} /* find_room() */

string find_function_room( string word ) {
   object thing, *things;

   if( !word )
     return LEARNING +"functions";
   things = children( FUNCTION_ROOM ) - ({ find_object( FUNCTION_ROOM ) });
   foreach ( thing in things ) {
      if ( (string)thing->query_marker() == word )
        return file_name( thing );
   }
   thing = clone_object( FUNCTION_ROOM );
   thing->set_marker( word );
   return file_name( thing );
} /* find_function_room() */

string *query_functions() {
   string *functions_found, key, *value;
   
   functions_found = ({ });
   foreach ( key, value in functions ) {
      functions_found -= value; /* remove duplicates */
      functions_found += value;
   }
   return sort_array( functions_found, 1 );
} /* query_functions() */

string *query_rooms_with_function( string word ) {
   string *rooms_found, key, *value;
   
   rooms_found = ({ });
   foreach ( key, value in functions ) {
      if ( member_array( word, value ) != -1 )
        rooms_found += ({ key });
   }
   return uniq_array(rooms_found);
} /* query_rooms_with_function() */

