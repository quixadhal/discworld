/**
 * Appraise
 * For getting "obvious" physical characteristics of an object
 *
 * @author Dasquian
 * @started 7/8/02
 **/

#include <volumes.h>

int cmd(object *things);

/**
 * Converts a raw length into a useable string.
 */
string query_length_mess(int number) {
    int half_feet;
    
    switch(number) {
        case 0:
            return "not very";
        case 1:
            return "an inch";
        case 2:
            return "a couple of inches";
        case 3:
            return "three inches";
        case 4:
            return "four inches";
        case 5..7:
            return "about six inches";
        case 8..10:
            return "about nine inches";
        case 11..14:
            return "about a foot";
        case 15..21:
            return "about a foot and a half";
        case 22..27:
            return "about two feet";
        case 28..33:
            return "about two and a half feet";
        default:
            half_feet = (number + 3)/6;
            if( half_feet%2 ) {
                return "about " + query_num(half_feet/2) + " and a half feet";
            }
            else {
                return "about " + query_num(half_feet/2) + " feet";
            }
    }

} /* query_length_mess() */

string query_dimensions_mess(object thing) {
    return "is " + query_length_mess(thing->query_length()) + " long and " + 
        query_length_mess(thing->query_width()) + " wide.";
} /* query_dimensions_mess() */

string query_appearance_mess(object thing) {
    string material_mess;
    
    if( !sizeof(thing->query_materials()) ) {
        material_mess = "some unidentifiable material";
    }
    else {
        material_mess = query_multiple_short(thing->query_materials());
    }

    if(thing->query_colour()) {
        return "is " + thing->query_colour() + " and is made of " + 
            material_mess;
    }
    else {
        return "is made of " + material_mess;
    }
    
} /* query_appearance_mess() */


string query_type_mess(object thing) {

    if( thing->query_plant() ) {
        return "appears to be some sort of plant";
    }

    if( thing->query_food_object() ) {
        if( thing->query_liquid() ) {
            return "looks drinkable";
        }
        else {
            return "looks edible";
        }
    }

    if( thing->query_furniture() ) {
        return "could be placed as furniture";
    }

    if( thing->query_property("shop type") == "jewellers" ) {
        return "appears to be a piece of jewellery";
    }

    if( thing->query_weapon() ) {
        return "could be used as a weapon";
    }
    
    if( thing->query_armour() && thing->query_wearable()) {
        return "could be worn as armour";
    }
    
    if( thing->query_wearable() ) {
        return "looks like you could wear it";
    }   

    if (thing->query_no_limbs() > 0) {
        return "looks like you could hold it";
    }
    
    return 0;
    
} /* query_type_mess() */

/**
 * Give some descriptions about how much stuff the container can hold.
 */
string query_container_mess(object thing) {
   int volume;
   int size;
   string str;

   volume = thing->query_max_volume();
   if (thing->query_container()) {
      size = thing->query_max_size();
   }

   if (size || volume) {
      str = "";
      if (size) {
         str += "can hold about " + (size / 9) + " pounds";
      }
      if (volume) {
         if (size) {
            str += " and ";
         }
         str += "can hold about ";
         if (volume > VOLUME_GALLON) {
            str += ((volume + VOLUME_GALLON / 2) / VOLUME_GALLON) + " gallons";
         } else if (volume >= VOLUME_QUART) {
            str += ((volume + VOLUME_QUART / 2) / VOLUME_QUART) + " quarts";
         } else if (volume >= VOLUME_PINT) {
            str += ((volume + VOLUME_PINT / 2) / VOLUME_PINT) + " pints";
         } else if (volume >= VOLUME_GILL) {
            str += ((volume + VOLUME_GILL / 2) / VOLUME_GILL) + " gills";
         } else if (volume >= VOLUME_SHOT) {
            str += ((volume + VOLUME_SHOT / 2) / VOLUME_SHOT) + " shots";
         } else {
            str += "nothing";
         }
      }
      return str;
   }
   return 0;
}


int cmd(object *things) {
    object thing;
    string mess;
    string tmp;

    if(!sizeof(things)) {
        add_failed_mess("For some reason, you "
            "have nothing to appraise.  Please bugrep this.\n", ({ }) );
        return 0;
    }

    if(sizeof(things) > 1) {
        add_failed_mess("You cannot appraise "
            "more than one object at once.\n", ({ }) );
        return 0;
    }

    if (thing == this_player()) {
        if (this_player()->query_name() == "penguin") {
           write("Is that a tuxuedo you are wearing?  No, probably not.\n");
        } else {
           write("You appraise yourself.  " + (random(10) + 1) + " out of 10.\n");
        }
        return 0;
    }

    thing = things[0];

    if(environment(thing) != this_player()) {
        add_failed_mess("You cannot appraise "
            "objects you are not carrying.\n", ({ }) );
        return 0;
    }

    if(living(thing)) {
        write("You appraise " + thing->the_short() + " and mmmm, very "
            "sexy.\n");
        return 1;
    }
    
    if(thing->query_worn_by()) {
        add_failed_mess("You cannot appraise "
            "$I while you are wearing it.\n", ({ thing }) );
        return 0;        
    }

    if( !thing->query_clothing() ) {
        mess = thing->the_short() + " " + query_dimensions_mess(thing);
        mess += "  ";
    }
    else {
        mess = "";
    }
    
    tmp = query_type_mess(thing);
    if(tmp) {
        mess += "It " + query_appearance_mess(thing);
        mess += " and " + tmp + ".";
    }
    else {
        mess += "It " + query_appearance_mess(thing) + ".";
    }

    tmp = query_container_mess(thing);
    if (tmp) {
       mess += "  It " + tmp + ".";
    }
    
    
    if(thing->cond_string() && thing->cond_string() != "") {
        mess += "  " + thing->cond_string();
    }
    else {
        mess += "\n";
    }
    
    write("$P$Appraise$P$" + mess );

    add_succeeded_mess("");
    return 1;

} /* cmd() */


mixed *query_patterns() {
    return ({ "<indirect:object:me-here'item'>", (: cmd($1) :) });
} /* query_patterns() */
