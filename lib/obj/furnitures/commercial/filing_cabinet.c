inherit "/std/room/furniture/document_handler";

void setup() {
   set_short("filing cabinet");
   set_name("cabinet");
   add_alias("filing");
   set_long("A big grey wooden box with draws that slide in and out.  It "
            "looks like it could hold bunches of documents, nicely order "
            "and available for quick retrieval.");

   set_allowed_positions(({"sitting", "lying", "kneeling", "meditating"}));
   set_allowed_room_verbs((["sitting" : "sits" ]));
   set_weight(2000);
   set_value(240000);

   set_commercial_size(5);
   set_commercial_type("utility");
}
