/*
 * A birthday card to give to ppl on their birthday (in real life) 
 */
#define DEMON "/obj/b_day/demon"
inherit "/std/object";

int is_open, opened_before;
string owner_name;

int do_read()
{
  if(!is_open) {
    write("The card is addressed to you. Maybe you should open it?\n");
    return 1;
  }
  return ::do_read();
}

void set_closed_mesg()
{
    set_long("The closed card has been carefully handmade from pink " +
        "cardboard. There is a rather vague illustration of a " +
        "non-identifiable piece of flora on the front.\n");
}

void set_open_mesg()
{    
    set_long("The open card has been carefully handmade from pink " +
        "cardboard.\n");
}

void setup()
{
  string *wizards, list;
  int i;

  is_open = opened_before = 0;
  owner_name = (string)this_player()->query_cap_name();
  set_name("card");
  add_adjective("birthday");
  set_short(owner_name + "'s birthday card");
  set_main_plural("birthday cards");
  add_property("determinate", "");
  set_closed_mesg();
  reset_drop();

  wizards = get_dir("/w/");

  list = "Written inside the card, in various hands, is:\n\n" +
        "  Have a wonderful Birthday, " + owner_name + "!!!\n" +
        "LOVE AND KISSES FROM...\n\n    ";
  for(i=0;i<sizeof(wizards);i++)
    if(file_size("/w/" + wizards[i]) == -2) {
      list += capitalize(wizards[i]);
      if(i == sizeof(wizards)-1) list += ".\n";
      else if(i == sizeof(wizards)-2) list += " and ";
      else list += ", ";
    }
  set_read_mess(list);
}

void init()
{
    ::init();
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
    is_open = 1;
    set_open_mesg();

    if(opened_before) return 1;
    opened_before = 1;

    demon = clone_object(DEMON);
    demon->move(environment(this_player()));
    demon->set_owner(this_player());
    say("*** POP ***\nAn Origami Demon explodes from " + owner_name +
"'s birthday card as " + owner_name +" opens it!\n");
    
    write("*** POP ***\n" +
"An Origami Demon explodes from your card as you open it!\n");
    return 1;
}

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

mixed query_auto_load()
{
    return ({ is_open, opened_before });
}

void init_arg(mixed arg)
{
    if(!this_player()->query_is_birthday_today()) {
       write("You throw away your old, tattered birthday card...\n");
        call_out("dest_me", 0);
       return;
    }
    /* it is still your birthday! */
    is_open = arg[0];
    opened_before = arg[1];

    if(is_open) set_open_mesg();
    else set_closed_mesg();
}
