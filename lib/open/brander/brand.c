/* brand.c: written by Quantum for everyplayer 05/07/91 */

/* see brander.c */

#define THISFILE "players/truilkan/obj/brand"

#define BRAND "monster_brand"

#define VERSION "2.0"
#define VERSION_DATE "05/25/91"

string owner;
string thisMonster;
int dropped;
object brander;
object roombrand;

#define LIFETIME 10  /* amount of time brand will last in minutes */

real_name(ob)
{
   if (!ob)
      ob = this_player();
   return capitalize(call_other(ob,"query_real_name"));
}

name(ob)
{
   if (!ob)
      ob = this_player();
   return capitalize(call_other(ob,"query_name"));
}

do_destruct(ob)
{
   write("The brand dissolves.\n");
   call_other(brander,"brand_is_dead");
   if (roombrand)
      destruct(roombrand);
   destruct(ob);
   return 1;
}

visitation(o,a,m)
{
   string other;
   object ob;

   ob = find_player(lower_case(o));
   if (!ob)
      return 1;
   if (a)
      other = a;
   else
      other = "someone";
   tell_object(ob,"The shade of " + m + " appears to tell you that "
      + other + " ignored your brand.\n");
   return 1;
}

/* we assume that the monster is dead if this object is being dropped
   for the first time. */

drop()
{
    object attacker;
    string aname;

    if (dropped)  /* already dropped once */
       return 0;

    dropped = 1;
    attacker = call_other(environment(this_object()),"query_attack");
    aname = real_name(attacker);
    if (owner != aname) {
       /* let the branding player know that someone else made the kill */
       visitation(owner,aname,name(environment(this_object())));
       /* give the offending player time to see what he's done */
       call_out("do_destruct",60,this_object());
       return 0;
    }
    else {
       call_other(brander,"brand_is_dead");
       destruct(roombrand);
       /* this call must immediately precede the return 1 to prevent a bug */
       destruct(this_object());
       return 1;
    }
}

set_roombrand(obj)
{
    roombrand = obj;
}

set_owner(str)
{
   owner = str;
   thisMonster = name(environment(this_object()));
   return 1;
}

set_brander(ob)
{
   brander = ob;
}


query_name()
{
   return owner + "'s brand";
}

query_value()
{
   return 0;
}

get()
{
   return 1;
}

short()
{
   return owner + "'s brand";
}

long(arg)
{
   write("A brand showing that " + owner + " had a claim staked on the\n");
   write("monster which contained the brand.\n");
}

extra_look()
{
   if (!dropped)
      return thisMonster + " has been claimed by " + owner;
   else
      return 0;
}

reset(arg)
{
   if (!arg) {
      dropped = 0;
      call_out("do_destruct",LIFETIME * 60,this_object());
      return 0;
   }
   return 1;
}

id(str)
{
    return str == "brand" || str == BRAND;
}
