// $Id: fry.c,v 1.2 2000/05/13 00:36:36 taffyd Exp $
// Lightning zap - based on meteor code
// Wrytten by Rue and Drakkos
// 15/04/2000

#include <playerinfo.h>
#define LIGHTNING "%^WHITE%^BOLD%^lightning%^RESET%^"
#define THUNDER "%^RED%^BOLD%^thunder%^RESET%^"

inherit "/cmds/base";

mapping banned = ([
    "macchirton"    :   "it's tradition.",
    "que"           :   "you're an arse.",
    ]);
    
mixed cmd(string words) {
  string player, reason;
  object ob;

    add_failed_mess("At Pinkfish's request this command has been disabled.\n" );
    return 0;

  if(member_array (this_player()->query_name(), keys(banned)) != -1)
  {
    return notify_fail("You cannot use 'fry', because " + 
        banned[this_player()->query_name()] + "\n");
  }
   
  if(!words || sscanf(words, "%s %s", player, reason ) != 2) {
    return notify_fail("Usage: fry <player> <reason>\n");
  }
  
  if (!"/secure/master"->query_senior(this_player()->query_name()))
  {
    return notify_fail ("Only Senior Creators and above may smite players "
        "with Righteous Vengeance!\n");
  }

  ob = find_player(this_player()->expand_nickname(player));
  
  if(!ob) {
    return notify_fail(player+" not found!\n");
  }
  
  write("Warning: This is not to be used for pranks.  If you "
    "do not have a valid reason to use this, do not use it!  "
    "Are you quite sure you wish to proceed? (y/n): ");
 

  input_to("are_sure", ob, reason);
  
  return 1;
} /* cmd() */

void are_sure(string str, object ob, string reason) {
  string gender = "boy";
  if(this_player()->query_gender() == 2) {
    gender = "girl";
  }
  str = lower_case(str);
  if(str[0] != 'y') {
    write("There's a good "+gender+"!  Now you go play nice with little "
        + ob->one_short() + "!\n");
    return;
  }
  
  write("Would you like the whole Disc to be aware of the reason "
    "for your ire? (y/n): ");
    
  input_to("positive", ob, reason);  
}

void positive (string str, object ob, string reason) {
    
  str = lower_case(str);
  if(str[0] != 'y') {
    write ("Okay, we'll keep it between you and me.\n");
    call_out("fry2", 5+random (3), ob);
  }
  else
  {
    write ("Bwah-ha-ha-ha!\n");
    call_out("fry2", 5+random (3), ob, reason);
  }
  
  PLAYERINFO_HANDLER->add_entry(this_player(),
                                ob->query_name(),
                                "meteor",
                                reason);
  log_file("SMITE", "%s: %s smote %s: %s\n", ctime(time()), 
    this_player()->query_short(), ob->query_short(), reason);

  shout("The sky grows dark.  Thick black clouds roll in.\n");
  shout("A loud peal of " + THUNDER + " rumbles, shaking the ground, "
    "and bolts of " + LIGHTNING + " flash through the sky!\n");
  write("Smiting "+capitalize (ob->query_name()) +" with Righteous Vengeance!\n");
} /* are_sure() */

varargs void fry2(object ob, string reason) {
    
     
    string *shout_mess = ({
        "A bolt of ferocious " + LIGHTNING + " rips the sky in two!  ",
        "A peal of deafening " + THUNDER + " rumbles o'er the land!  ",
        "The clouds gather, spitting forth bolts of vicious " + LIGHTNING + 
            " from their folds!  ",
        "The clouds gather into great dark masses, spitting " + LIGHTNING +
            " at the cowering Disc below!  "
        });

    string *player_mess = ({
        "A bolt of shimmering " + LIGHTNING + " spears the ground in front "
            "of you!\n",
        "A bolt of " + LIGHTNING + " pierces the air above you, melting "
            "your eyebrows.\n",
        "A bolt of " + LIGHTNING + " flares before your face, causing "
            "little blobs of light to dance over your retinas.\n"
            });

        
    shout (shout_mess[random(sizeof(shout_mess))] + "It appears one "
        "of the " + mud_name() + "'s creators is unhappy...\n");        
                    
    tell_object(ob, player_mess[random(sizeof(player_mess))]);
    call_out("fry3", 8 + random (10), ob, reason);
} /* nuke2() */

varargs void fry3(object ob, string reason) 
{
    
    string name;
    
    string *TMs = ({
        "You feel more able to spear players with bolts of lightning!\n",
        "You feel more confident about your aim when trying to hit small "
            "scurrying figures from Cori Celesti!\n",
        "You manage to grasp one of the principles of creator justice "
            "more firmly!\n",
        });
        
    string *shout_mess = ({
        "Another spear of " + LIGHTNING + " lances through the clouds, "
            "followed closely by a deafening peal of " + THUNDER + "!\n",
        "A deafening roll of " + THUNDER + " followed by a bolt of brilliant "
        + LIGHTNING + " rolls over the " + mud_list() + ".\n",
        "The clouds spit out another violent bolt of " + LIGHTNING + " as "
        + THUNDER + " shatters the relative peace of the " + mud_list() + ".\n"
        });
  
    string *player_mess = ({
        "A bolt of " + LIGHTNING + " strikes you and sends you into the "
            "%^RED%^oblivion%^RESET%^ of the real world!\n",
        "A bolt of " + LIGHTNING + " pierces the skies and strikes you!  "
        "%^RED%^Zzzzzot!%^RESET%^\n",
        "A bolt of " + LIGHTNING + " spears down from the sky and %^RED%^"
            "smites%^RESET%^ you!\n",
        });
  
  /*
        No point actually putting this through the tasker since it's only
        supposed to be a joke anyway.  :-P
  */
          
  if (random (100) > 80)
  {
        tell_object (this_player(), "The " + LIGHTNING + " blasts " + ob->short() +
            " into oblivion!\n");
        tell_object (this_player(), "%^YELLOW%^%^BOLD%^" + TMs[random(sizeof(TMs))]
            + "%^RESET%^");
        this_player()->add_skill_level ("fighting.combat.range.thrown", 1);
  }
                  
                  
  shout(shout_mess[random(sizeof(shout_mess))]);
  
  if (this_player()->query_invis())
  {
        name = "Someone";
  }
  else
  {
        name = this_player()->short();
  }
    
    
  if (reason)
  {
      shout(name + " shouts from the clouds:  This is " + reason + 
      ", " + ob->short() +"!\n");
  }
  else
  {
      shout(name + " cackles in the distance.\n");
  }
   
    
  tell_object(ob, player_mess[random(sizeof(player_mess))]);
  ob->save();
  ob->quit();
  shout(ob->query_cap_name()+" was smote by " + name + "'s "
    "lightning.\n"+ capitalize(ob->query_pronoun())+" is no more.\n");
  shout ("The clouds roll back across the sky.");

} /* fry3() */

