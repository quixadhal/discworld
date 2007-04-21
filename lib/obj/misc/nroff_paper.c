inherit "/obj/misc/paper";
#include "nroff.h"

/* This will nroff to a file in the same dir. */

private nosave mixed *stuff = ({ });

string file_name = 0;
string language = 0;

void create() {
  seteuid(getuid());
  ::create();
}

void set_file_name(string fname) {
  file_name = fname;
}

string query_file_name() { return file_name; }

void set_language(string lang) { language = lang; }

string query_language() { return language; }

mixed *query_read_mess() {
  string str;

  if(sizeof(stuff) == 0) {
    if (file_name) {
      str = unguarded( (: NROFF_HAND->cat_file(file_name+"_nroff", 1) :) );
      if (!str) {
        unguarded((: NROFF_HAND->create_nroff(file_name, file_name+"_nroff") :));
        str = unguarded( (: NROFF_HAND->cat_file(file_name+"_nroff", 1) :) );
      }
      
      if(!str)
        return ::query_read_mess() +
          ({ ({ "Unable to nroff file.\n", 0, "common", 0 }) });
      
      if( language ) {
         return stuff = ({ ({ str, 0, language, 0 }) });
      }
      
      return stuff = ({ ({ str, 0, "common", 0 }) });
    }
  }
  
  return stuff + ::query_read_mess();
}


mapping query_dynamic_auto_load() {
   mapping map;

   map = ::query_dynamic_auto_load();
   map["read mess"] = ::query_read_mess();

   return map;
   
} /* query_dynamic_auto_load() */
