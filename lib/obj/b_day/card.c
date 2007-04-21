/*
 * A birthday card to give to ppl on their birthday (in real life) 
 */
#define DEMON "/obj/b_day/demon"

#include <player_handler.h>

inherit "/std/object";

int creator_status, is_open = 0;
int opened_before = 0;
string owner_name, owner_language;
string* colour_array = ({
  "%^RED%^", "%^BOLD%^%^RED%^",
  "%^MAGENTA%^", "%^BOLD%^%^MAGENTA%^",
  "%^ORANGE%^", "%^YELLOW%^", 
  "%^GREEN%^", "%^BOLD%^%^GREEN%^",
  "%^WHITE%^", "%^BOLD%^%^WHITE%^", 
  "%^CYAN%^", "%^BOLD%^%^CYAN%^"
  });   

void setup_card(string name, string language, int status);

mapping init_query_static_auto_load() {

  return ([ "::" : ::int_query_static_auto_load(),
          "owner_name":owner_name, 
          "owner_language":owner_language, 
          "is_open":is_open, 
          "opened_before":opened_before]);

}/* mapping init_query_static_auto_load() */


void init_static_arg( mapping map ) {

  if ( map[ "::" ] ) {
    ::init_static_arg( map[ "::" ] );
  }
  if ( !undefinedp( map[ "owner_name" ] ) ) {
    owner_name = map[ "owner_name" ];
  }
  if ( !undefinedp( map[ "owner_language" ] ) ) {
    owner_language = map[ "owner_language" ];
  }
  if ( !undefinedp( map[ "is_open" ] ) ) {
    is_open = map[ "is_open" ];
  }
  if ( !undefinedp( map[ "opened_before" ] ) ) {
    opened_before = map[ "opened_before" ];
  }

}/* void init_static_arg() */


mapping query_static_auto_load() 
{
  if ( explode( file_name( this_object() ), "#" )[ 0 ] ==
       "/obj/b_day/card" ) {             
    return init_query_static_auto_load();
  }
  return 0;

} /* query_static_auto_load() */


mixed *query_read_mess()
{
  if(!is_open) {
    return ({ ({ "The card is addressed to " + owner_name + 
                   ".  Maybe you should open it?  Then again, if "
                   "you're not " + owner_name + ", maybe you "
                   "shouldn't.\n",  
                   0, owner_language, 1 }) });
  }
  return ::query_read_mess();
}


void set_closed_mesg()
{
    set_long("The closed card has been carefully handmade from pink "
        "cardboard. There is a rather vague illustration of a "
        "non-identifiable piece of flora on the front.\n");
}


void set_open_mesg()
{    
    set_long("The open card has been carefully handmade from pink "
        "cardboard.\n");
}


void setup_card(string own_name, string own_language, int cre_status) 
{
  string *wizards, list;
  int i;

  owner_name = own_name;
  owner_language = own_language;
  creator_status = cre_status;
  set_short(owner_name + "'s birthday card");
  add_adjective( ({ owner_name, owner_name + "'s"}) );

  wizards = get_dir("/w/");

  if(creator_status) {
    int start = member_array(lower_case(owner_name), wizards);
    wizards = wizards[0..start-1] + wizards[start+1..];
    list = 
      "\n    Have a wonderful birthday, " + owner_name + "!!!  \n" 
      "    LOVE AND KISSES FROM...                \n\n";
    for(i=0;i<sizeof(wizards);i++) {
      if(PLAYER_HANDLER->test_user(wizards[i])) {
        list += colour_array[random(sizeof(colour_array))] +
          capitalize(wizards[i]); 
        if(i == sizeof(wizards)-1) list += "%^RESET%^.\n\n";
        else if(i == sizeof(wizards)-2) list += "%^RESET%^ and ";
        else list += ",%^RESET%^ ";
      }
    }
    list += "... and thanks for all your work and time, and let's hope "
      "next year will be even more magical and fun, fun, fun!\n";  
  } else {
    
    list = 
      "\n    Have a wonderful birthday, " + owner_name + "!!!  \n" 
        "    LOVE AND KISSES FROM...                \n\n";
    for(i=0;i<sizeof(wizards);i++) {
      if(PLAYER_HANDLER->test_user(wizards[i])) {
        list += capitalize(wizards[i]);
        if(i == sizeof(wizards)-1) list += ".\n";
        else if(i == sizeof(wizards)-2) list += " and ";
        else list += ", ";
      }
    }
  }

  add_read_mess(list, "various hands", owner_language, 1);
  
} /* setup_card() */


void setup()
{
  set_name("card");
  set_short("birthday card");
  add_adjective("birthday");
  set_main_plural("birthday cards");
  add_property("determinate", "");
  add_property("no recycling", 1);
  set_closed_mesg();

  if(owner_name && owner_language) {
    setup_card(owner_name, owner_language, creator_status);
  }
}


void init()
{
    this_player()->add_command("open", this_object());
    this_player()->add_command("close", this_object());
}


int do_open()
{
    object demon;

    if(is_open) {
        write("It is already open.\n");
        return 1;
    }

    if(lower_case(owner_name) != this_player()->query_name()) {
      add_succeeded_mess( ({ 
        "*** POP ***\n"
          "An Origami Demon peers out from the card, sees you're not " 
          + owner_name + ", mutters about that not being very "
          "low-temperature, and vanishes back into the card.\n",
        "*** POP ***\n"
          "An Origami Demon peers out from " + owner_name + "'s "
          "birthday card at " + this_player()->query_cap_name() +
          ", smirks and crawls back into the card.\n"
      }) );
      return 1;
    }

    is_open = 1;
    set_open_mesg();

    if(opened_before) return 1;
    opened_before = 1;

    demon = clone_object(DEMON);
    demon->move(environment(this_player()));
    demon->set_owner(owner_name);
    demon->add_language(owner_language);
    demon->set_language(owner_language);

    /*changed the write/say to add_succeeded_mess, Siel June 2001 */

    add_succeeded_mess( ({ 
      "*** POP ***\n"
        "An Origami Demon explodes from your card as you open it!\n",
      "*** POP ***\n"
        "An Origami Demon explodes from " + owner_name + "'s birthday "
        "card as " + owner_name +" opens it!\n"
    }) );

    call_out("start_demon", 2, demon);
    return 1;
}


void start_demon(object thing) {
  thing->do_the_act(0);
} /* start_demon() */


int do_close()
{
    if(!is_open) {
        write("It is not open for you to close!\n");
        return 1;
    }
    is_open = 0;
    set_closed_mesg();
    return 1;
}


mixed *stats() 
{
  return ::stats() + ({ ({ "owner name",owner_name }), 
                          ({ "owner language",owner_language }), }); 

}/* mixed *stats() */

