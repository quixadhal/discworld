
/*  This is the plant object thingy.  This is the quite the easy part.   */

inherit "std/object";

string plant_name, plant_type, plant_desc;

set_plant(name){  plant_name = name;  }
set_plant_type(type){ plant_type = type; }
set_plant_desc(desc){ plant_desc = desc; }

setup(){
    set_name(plant_name);
    set_short(plant_name+" "+plant_type);
    set_long(plant_desc);
    add_property("plant");
    }
