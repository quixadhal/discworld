#include <effect.h>

string query_classification(){return "crafts.description"; }

//Desc[0]: short
//Desc[1]: long
//Desc[2]: adjective
//Desc[3]: alias
//Desc[4]: determinate

//string* description = ({"bing", "bing", "bing", "bing", "bing"});
 //Whee. Global effect variables. Kids, don't try this at home.

string* beginning(object thing, string* desc){
   //description = desc;
   switch(sizeof(desc)){
      case 5:
         thing->add_property("determinate", desc[4]);
      case 4:
         thing->add_alias(desc[3]);
      case 3:
         thing->add_adjective(desc[2]);
      case 2:
         thing->set_long(desc[1]);
      case 1:
         thing->set_short(desc[0]);
   }
   return desc;
}/*beginning()*/

string* merge_effect(object player, string* old_desc, string* new_desc){
   return new_desc;
}/*merge_effect()*/

mixed* restart(object thing, string* description, int id){
   switch(sizeof(description)){
      case 5:
         thing->add_property("determinate", description[4]);
      case 4:
         thing->add_alias(description[3]);
      case 3:
         thing->add_adjective(description[2]);
      case 2:
         thing->set_long(description[1]);
      case 1:
         thing->set_short(description[0]);
   }
   return ({thing, description, id});
}

/*A SkankyCode Creation, by Rodion*/