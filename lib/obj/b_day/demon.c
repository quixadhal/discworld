/* The singing birthday demon */
#define SONG "/obj/b_day/happy"

inherit "/obj/monster";

int line;
object owner;
string owner_name;

void setup()
{
    set_name("demon");
    add_adjective("tiny");
    add_adjective("pink");
    add_adjective("paper");
    add_adjective("origami");
    set_short("origami demon");
    set_main_plural("origami demons");
    add_plural("demons");
    set_long(
"This is a tiny, pink, paper demon. Looking closely you see that\n" +
"it has been made by someone with a black belt in Origami!\n");
    set_level(1);
    line = 1;
}

void set_owner(object person)
{
    owner = person;
    owner_name = (string)owner->query_cap_name();
}

void heart_beat()
{
    string line_to_sing, start, end;
    
    ::heart_beat();

/*  if(line == 1) shout("Someone tells you: It is " +
        owner_name + "'s Birthday today!\n", owner); */

    if(environment(owner) != environment()) {
        tell_room(environment(), "The origami demon rustles after "+
            owner_name + ".\n");
        move(environment(owner));
        tell_room(environment(), "The origami demon rustles in.\n" +
            "The origami demon says: Hey "+owner_name+"! I hadn't finished!\n");
        return;
    }   
    line_to_sing = read_file(SONG, line, 1);
    if(!strlen(line_to_sing)) {
        set_heart_beat(0);
        dest_me();
        return;
    }
    if(sscanf(line_to_sing, "%s$N%s", start, end) == 2)
        line_to_sing = start + owner_name + end;
    tell_room(environment(), line_to_sing + "\n");
    line++;
}
