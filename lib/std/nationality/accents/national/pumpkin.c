string garble_say( string mess, object person,
                   object sayer, int say_type ) {
   return replace( mess, ({ 
                            "eh", "a",
                            "ah", "eh",
                            "ou", "oo"
                         }) );
}

string query_accent_name() {
   return "Pumpkinian";
}
