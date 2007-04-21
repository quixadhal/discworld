inherit "/std/nationality/basic";

void setup() {
   set_name( "pumpkin" );
   set_information( "Pumpkin is a large orange ball shaped town.\n");
   add_region( "round",
               "Pumpkins which are round.",
               "/d/dist/start/entrance",
               "/std/nationality/accents/regional/round");

   add_region( "square",
               "Sto Lat is the second biggest city in the Morporkian kingdom, "
               "it sits on the river Ankh on the way to the Carrack "
               "Mountains.  It is a harder starting location than "
               "Ankh-Morpork but close enough that it is not that much of an "
               "issue.",
               "/d/dist/start/entrance",
               "/std/nationality/accents/regional/square");

   set_language("pumpkin");
   set_currency_area("Pumpkin");
   set_default_start_location("/d/am/buildings/drum/mendeddrum");
}
