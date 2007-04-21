/**
 *
 *  Simple Blackboard For Simple Houses
 *
 *  Suggested by Lanfear.
 *  @author Drakkos
 *  @started 16/06/2000
 *
 */

inherit "/std/room/furniture/basic";


int query_allowed (string);

string read_stuff();
string query_words();

string _owner = "";
string _words = "";

void setup() {
    
  set_name ("blackboard");
  set_short ("blackboard");
  add_alias (({"board", "sign"}));
  set_long ("The residue of faded chalk-dust and long forgotten "
      "doodles lies heavily on the blackboard.  The grain of the "
      "board is worn and slightly mottled with age, hinting at "
      "a long and dignified history of classrooms and university "
      "lecterns.  Or perhaps just hinting at cheap wood and "
      "bad upkeep.\n");
  set_read_mess ((: read_stuff :));    
  set_allowed_room_verbs( (["standing" : "stands",
                          "hanging"  : "hangs"]) );
  set_value(0);
}

void init() {
  this_player()->add_command ("write", this_object(), "[on] <direct:object>");
  this_player()->add_command ("clean", this_object(), "<direct:object>");
}    

void set_words (string bing) {
    _words = bing;
}

string query_words () {
    return _words;
}
            
string read_stuff() {
    string bing = "";
    
    if (sizeof (query_words())) {
        bing += query_words();            
    }
    
    if (!sizeof (bing)) {
        bing = "A few random letters that escaped cleaning can be seen on "
            "the blackboard.";
    }
    
    return bing;
}

int do_clean() {
    if (!query_allowed (this_player()->query_name())) {
        this_player()->add_failed_mess (this_object(), "You cannot clean " 
            "the blackboard!\n", ({ }));
        return 0;
    }
    
    set_words ("");
    return 1;
}

int do_write() {
    string bean;
    if (!query_allowed (this_player()->query_name())) {
        this_player()->add_failed_mess (this_object(), "You cannot write on " 
            "the blackboard!\n", ({ }));
        return 0;
    }
                
    bean = query_words();
                    
    this_player()->do_edit(bean, "blackboard_text", this_object());
    
    this_player()->add_succeeded_mess (this_object(), "$N find$s a handy "
        "piece of chalk and start$s to write on the blackboard.\n", ({ }));
    return 1;

}

void blackboard_text(string text) {

    if (!text) {
      tell_room (environment (this_object()), this_player()->one_short()
          + " steps back from the blackboard without making any "
          "changes.\n", this_player());
      tell_object (this_player(), "Okay, nothing has been changed.\n");
      return;
    }

    set_words (text);
                
    tell_room(environment (this_object()), this_player()->one_short() 
      + " finishes writing on the blackboard.\n", this_player());
}


int query_allowed (string name) {

    if (function_exists ("query_owner", environment (this_object()))) {
      if (name == environment (this_object())->query_owner()) {
        return 1;
      }
    }
        
    if (function_exists ("query_allowed", environment (this_object()))) {
      if (member_array (name, environment(this_object())->query_allowed())
          != -1) {
        return 1;
      }
      else {
        return 0;
      }
      
    }    
    return 1;
}


mapping query_dynamic_auto_load() {
   return ([
     "::" : ::query_dynamic_auto_load(),
      "words" : query_words(),
   ]);

} 

void init_dynamic_arg( mapping map ) {
   ::init_dynamic_arg( map[ "::" ] );
   set_words (map["words"]);
   
   set_read_mess ((: read_stuff :));    
}


