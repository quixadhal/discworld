/**
* Player-run shop employee badge.
* This object is the badge claimable by employees of player-run shops.
* It allows employees of a particular shop to chat to each other on a
* deidcated channel.  It also sets a player-title, and shows which employees
* are currently logged on.
* This object should not be inherited by anything, but should be created
* by the player_shop office.
* @author Ringo
* @started 1st August 1999
*/
#include <player_shop.h>

inherit "/obj/armour";

private int do_badge(string);
private int do_chat(string);
private int get_properties();
private string cap_name(string);

private string _office, 
_title,
_channel,
_owner;

private int _echo;

void setup()
{
    set_name("badge");
    set_short("employee badge");
    add_adjective("employee");
    add_adjective("tin");
    add_adjective("small");
    set_main_plural("employee badges");
    set_weight(1);
    set_value(0);
    set_type("badge");
    setup_armour(640);
    set_damage_chance(5);
    set_wear_remove_func(file_name(this_object()),
      "wear_remove_badge");
    add_property("no recycling", 1);
    add_extra_look(this_object());
    if (!query_property("office") || !query_property("owner"))
    {
        return;
    }
    _office = query_property("office");
    _owner = query_property("owner");
    _echo = query_property("echo");
    _title = _office->shop_very_short();
    _channel = _office->query_channel();
    set_long( "This is a small tin badge which marks " +
      cap_name(_owner) + " as an employee of "+
      _office->query_shop_name()+ ", "+ _office->query_place()+ ".\n");
    add_adjective(_channel);
}
/* setup() */


/** @ignore yes */
private string cap_name(string name) 
{
    if (test_player(name))
        return PLAYER_HANDLER->query_cap_name(name);
    else return capitalize(name);
}

/** @ignore yes */
void init()
{
    object tp;

    if (!_office || !_owner)
    {
        if (!get_properties()) return;
        set_long("This is a small tin badge which marks "+
          cap_name(_owner) + " as an employee of "+
          _office->query_shop_name()+ ", "+ _office->query_place()+ ".\n");
        add_adjective(_channel);
    }
    tp = this_player();
    if (!_office->query_employee(tp->query_name()) &&
      !_office->query_retired(tp->query_name()) &&
      !tp->query_creator())
    {
        return;
    }
    add_command("badge", ({ "help", "echo" }), (: do_badge($5) :));
    add_command("h"+ _channel, "", (: do_badge("hist") :));
    add_command(_channel+"?", "", (: do_badge("who") :));
    add_command(_channel, "<string'text'>", (: do_chat($4[0]) :));
    add_command(_channel+"@", "<string'text'>", (: do_chat("@"+$4[0]) :));
}
/* init() */


/** @ignore yes */
private int do_badge(string args)
{
    if (!_office || !_owner)
    {
        if (!get_properties()) return 1;
    }
    add_succeeded_mess("");
    if (args == "who")
    {
        tell_object(this_player(),
          "The following employees from "+ _office->query_shop_name()+
          " are logged on: \n" + _office->badge_list());
        return 1;
    }
    else if (args == "hist")
    {
        string result = "$P$"+ _channel+ " channel$P$\n";
        mixed *hist = HISTORY_HANDLER->query_chat_history(lower_case(_channel));

        tell_object(this_player(),
          "You ask the badge for the last few chats on the "+ _channel+ 
          " channel\n");
        if (!arrayp(hist) || !sizeof(hist))
        {
            tell_object(this_player(),
              "The badge mutters that nothing has been said on this channel, "
              "or it is not being remembered.\n");
            return 1;
        }
        result += implode(map(hist, (: "$I$5=$C$" + $1[0] + $1[1] + 
              "%^RESET%^" :)), "\n" ); 
        tell_object(this_player(), result);
    }
    else if (args == "echo")
    {
        if (_echo)
        {
            tell_object(this_player(), "Local echo now off.\n");
            _echo = 0;
        }
        else
        {
            tell_object(this_player(), "Local echo now on.\n");
            _echo = 1;
        }
        add_property("echo", _echo);
    }
    else
    {
        string result = sprintf("Commands for this badge:\n\n"
          "   %-13s - chat a message to the other employees\n"
          "   %-13s - emote a message to the other employees\n"
          "   %-13s - list employees logged on\n"
          "   h%-12s - show recent chat history\n"
          "   badge echo    - Turn local echo on/off\n\n"
          "You can also set your preferred chat colour using the \"colour "
          "shopchat <colour>\" command.\n", _channel, _channel + "@",
          _channel + "?", _channel, _channel);
        tell_object(this_player(), result);
    }
    return 1;
}
/* do_badge() */


/** @ignore yes */
private int do_chat(string wisp)
{
    string sender;
    object *things, tp;

    add_succeeded_mess("");
    if (!_office || !_owner)
    {
        if (!get_properties()) return 1;
    }
    tp = this_player();
    if (!tp->query_property(_channel+ "_on"))
    {
        return notify_fail("You are not wearing the " + _channel+
          " badge!\n");
    }
    if (tp->query_property("gagged"))
    {
        return notify_fail("You have been gagged!  You cannot chat until "
          "your gag is removed.  Perhaps you should talk to a creator about "
          "this.\n");
    }
    if (!tp->query_creator())
    {
        wisp = replace_string(wisp, "%^", "");
    }
    if (!wisp || ( wisp == "" ) || wisp == "@")
    {
        return notify_fail("Chat what?\n");
    }
    if (environment(this_object()) != tp)
    {
        return notify_fail("You're too far from the badge to whisper into "
          "it.\n");
    }
    tell_object(tp,  "You whisper into the badge.\n");
    sender = tp->query_cap_name();
    if (tp->query_name() == CREATOR && tp->query_invis())
    {
        sender = tp->query_short();
    }
    wisp = replace(wisp, sprintf("%c", 7), "!");
    things = children(BADGE);

    if (_echo)
    {
        things -= ({find_object(BADGE)});
    }
    else
    {
        things -= ({this_object(), find_object(BADGE)});
    }
    things->receive(_channel ,sender, wisp);
    if (wisp[0] == '@')
    {
        if (wisp[1] == ''')
      {
         HISTORY_HANDLER->add_chat_history(_channel, sender, wisp[1..1000]);
      }
      else
      {
         HISTORY_HANDLER->add_chat_history(_channel, sender+ " ", wisp[1..1000]);
      }
   }
   else
   {
      HISTORY_HANDLER->add_chat_history(_channel, sender +": ", wisp);
   }
   return 1;
}
/* do_chat() */


/** @ignore yes */
string extra_look(object thing)
{
   object tp;
   
   tp = this_player();
   if (!_office || !_owner)
   {
      if (!get_properties()) return "";
   }
   if (!_office->query_employee(tp->query_name()) &&
      !_office->query_retired(tp->query_name()) && !tp->query_creator())
   {
      return "";
   }
   return "You can get a list of commands by typing \"badge help\"\n";
}
/* extra_look() */


/** @ignore yes */
private int get_properties()
{
   if (!query_property("office") || !query_property("owner"))
   {
      tell_object(environment(), "This badge is broken.  "
         "Please claim another.\n");
      move("/room/rubbish");
      return 0;
   }
   _office = query_property("office");
   _owner = query_property("owner");
   _echo = query_property("echo");
   _title = _office->shop_very_short();
   _channel = _office->query_channel();
   return 1;
}
/* get_properties() */


/** @ignore yes */
string query_channel() { return copy(_channel); }


/** @ignore yes */
void receive(string channel, string sender, string text)
{
   object owner = environment();
   string *ignoring, prev_ob;
   
   if (!_office || !_owner)
   {
      if (!get_properties()) return;
   }
   prev_ob = base_name(previous_object());
   if (prev_ob != BADGE && prev_ob != _office->query_shop_front()
      /* Remove this condition once Tarnach's changed over */
      && prev_ob != "/d/am/square/emporium/TARNACH/consumables")
   {
      LOG_ERROR("shop_badge.c", "receive("+channel+","+sender+","+text+")");
      return;
   }
   if (!owner || (!living(owner)))
   {
      return;
   }
   if (!_office->query_employee(owner->query_name()) &&
      !_office->query_retired(owner->query_name()) &&
      !owner->query_creator())
   {
      return;
   }
   ignoring = owner->query_property("ignoring");
   if (ignoring && member_array(lower_case(sender), ignoring) != -1)
   {
      return;
   }
   if (channel != _channel || !owner->query_property(_channel+ "_on"))
   {
      return;
   }
   if (text[0] == '@')
   {
      if (text[1] == ''')
      {
         text = "["+ lower_case( _channel )+ "] "+ sender + text[1..1000];
      }
      else
      {
         text = "["+ lower_case( _channel )+ "] "+ sender +" "+ text[1..1000];
      }
   }
   else
   {
      text = "["+ lower_case( _channel )+ "] "+ sender +": "+ text;
   }
   
   tell_object(owner, owner->colour_event("shopchat", "%^CYAN%^") +
      "$I$5=$C$"+ text + "%^RESET%^\n");
   return;
}
/* receive() */


/** @ignore yes */
void set_props(string path, string owner)
{
   if (!path || path == "")
   {
      return;
   }
   if (previous_object() && previous_object() != find_object(path)) 
   {
      LOG_ERROR("shop_badge.c", "set_props("+path+","+owner+")");
      return;
   }
   set_long("This is a small tin badge which marks " +
      cap_name( owner ) + " as an employee of "+
      path->query_shop_name()+ ", "+ path->query_place()+ ".\n");
   add_property("office", path);
   add_property("owner", owner);
   _office = path;
   _owner = owner;
   _title = _office->shop_very_short();
   _channel = _office->query_channel();
   add_adjective( _channel );
}
/* set_props() */


/** @ignore yes */
void wear_remove_badge(object thing)
{   
   if (!_office || !_owner)
   {
      if (!get_properties()) return;
   }
   if (thing)
   {
      int level;
      string start;

      if (!living(thing))
      {
         return;
      }

if (thing->query_creator())
{
thing->add_property(_channel+ "_on", 1);
return;
}

      level = _office->query_employee(thing->query_name());
      
      /* Once Tarnach's transferred over replace following line with
       * if ( !level )
       */
      if (!level || level == -1)
      {
         if (_office->query_retired(thing->query_name()))
         {
            start = "Retired manager of ";
         }
         else return;
      }
      else if (level & MANAGER)
      {
         start = "Manager of ";
      }
      else if (level & SUPERVISOR)
      {
         start = "Supervisor of ";
      }
      else
      {
         start = "Employee of ";
      }
      thing->add_property(_channel+ "_on", 1);
      thing->set_title(_title, start + _office->query_shop_name());
      return;
   }
   thing = previous_object()->query_worn_by();
   if (thing)
   {
      thing->remove_property(_channel+ "_on");
      thing->remove_title(_title);
   }
}
/* wear_remove_badge() */
