inherit "/std/nationality/accents/accent_base.c";

string query_accent_name() {
   return "Pumpkian";
}


string garble_say( string mess, object person,
                   object sayer, int say_type, string default_language ) {
   mess = ::national_garble( mess, person, sayer, say_type, default_language,
                             query_accent_name() );
   
   return replace( mess, ({ "a", "aa",
                               "oo", "o",
                               "ou", "ooou"
                               "ua", "ue",
                                "ui", "ue",
                                "uo", "ua",
                               }) );
}

