inherit "/std/nationality/accents/accent_base.c";

string query_accent_name() {
   return "Morporkian";
}


string garble_say( string mess, object person,
                   object sayer, int say_type, string default_language ) {
   mess = ::national_garble( mess, person, sayer, say_type, default_language,
                             query_accent_name() );
   
   return replace( mess, ({ "au", "$AU_PROTECT",
                               "oo", "$OO_PROTECT",
                               "ou", "$OU_PROTECT",
                               "ua", "ue",
                                "ui", "ue",
                                "uo", "ua",
                                "$OO_PROTECT", "oo",
                               "$OU_PROTECT", "ou",
                               "$AU_PROTECT", "au"
                               }) );
}

