/* roombrand.c: written by Truilkan for everyplayer 05/25/91 */

/* see brander.c */

/*
  This object brands the room containing the branded monster.  It is
  intended to be used in conjuction with an object that brands the
  monster (such as truilkan/obj/brand.c).  This object allows
  the existence of the brand to be known without requiring
  the player to look at the branded monster.  This object doesn't
  try to follow monsters that wimp out since monster is close to death
  anyway and to do otherwise would require expense of using a heart_beat().
  This object also doesn't follow wandering monsters since wandering
  monsters are typically not high level monsters and thus aren't
  candidates for marking anyway.  This object does follow monsters
  that move at resets.  Note that the brander object (the object
  that applies the brands) is responsible for destructing this object.
*/

#define THISFILE "players/truilkan/obj/marker"

#define BRAND "room_brand"

#define VERSION "2.0"
#define VERSION_DATE "05/25/91"

string owner;
string monsterName;
object monsterObj;
object myEnv;
int messages;

init()
{
    if (!monsterObj || (myEnv != environment(monsterObj)))
        return;
    if (!messages)
        return;
    if (this_player()->query_real_name() == owner) {
        write("As you enter, you notice your mark upon " +
            capitalize(monsterName) + ".\n\n");
        return;
    }
    write("As you look around, you notice that someone has claimed "
        + capitalize(monsterName) + ".\n");
}


set_messages(arg)
{
    messages = arg;
}

set_owner(str)
{
   owner = lower_case(str);
   return 1;
}

set_monsterName(str)
{
    monsterName = str;
}

set_monsterObj(obj)
{
    monsterObj = obj;
}

query_value()
{
   return 0;
}

set_myEnv(ob)
{
    if (ob)
        myEnv = ob;
    else
        myEnv = environment(this_object());
}

get()
{
   return 0;
}

id(str)
{
    return str == BRAND;
}

reset(arg)
{
    if (!arg)
        set_messages(0);
    if (monsterObj && (myEnv != environment(monsterObj))) {
        object monsterEnv;

        monsterEnv = environment(monsterObj);
        move_object(this_object(),monsterEnv);
        set_myEnv(monsterEnv);
    }
}
