/*
    This is a basic NPC!
    Written by Drakkos.
    
    26/09/2000
*/

inherit "/obj/monster";

void setup() {

  set_name("blob");    
  set_short("grey blob");
  set_long("This is a grey blob.  It is grey.  It is also quite "
      "blobby.\n");
  basic_setup("human", "warrior", 10);
  set_gender("male");
  add_adjective(({"oozing", "grey"}));
  set_main_plural("grey blobses");
  add_alias("porridge");

  add_respond_to_with(({ "@say",({"blob", "grey"}),
      }), "say Yes, I am a grey blob.");
  add_respond_to_with(({ "@say",({"ooze", "blue", "cardboard"}),
     ({"porridge", "bing", "womble"}),
      }), "' Yes, I'm oozing quite nicely, like grey "
          "blobs do.  Like porridge!");     
  add_respond_to_with(({ "@thank", ({ "you", "blob" }) }),
      "' Aw, shucks. T'weren't nuthin'.");
  add_respond_to_with(({ ({"@gnaw", "@bite", "@chew"}), ({ "you", 
    "blob" }) }),
      ({"' What did you do that for?!",  "scream", "cry", "weep"}));


  load_chat(20,({ 2, ": oozes around.",
      1, "' I'm very grey.",
      2 , "' I'm a blob.",
      2 , "@bing",
  }) );

  load_a_chat(20,({ 2, ": oozes all over you.",
      1, "' Lemme alone!.",
      2 , ": sobs bitter, slimy tears.",
  }) );
        
}

    