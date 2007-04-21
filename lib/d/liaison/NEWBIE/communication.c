/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: communication.c,v 1.8 2003/02/19 06:39:05 ceres Exp $
 *
*/
/* Added no teleport property- 26-06-98- Shalla */

#include "path.h"
inherit PATH + "inside";

object sign;

void setup() {
  set_zone( "Newbie" );
  set_short( "Communication room" );
  set_light( 75 );
  add_property("no teleport", 1);

  set_long("This small room seems to be filled with frogs which are bouncing "
           "around, wombling on and chattering away like crazy.  Around the "
           "walls are a number of posters that describe various aspects of "
           "communication on Discworld.  There are posters on 'general', "
           "'speaking', 'expression', 'emote', 'newbie' and "
           "'mail'.\n");
  
  add_item("frog", "There are small green frogs everywhere.  They seem very "
           "excited and are moshing, sniggering, hugging,... you name it, "
           "they are doing it.");
  add_item("poster", "There are several of these around the room.  To look "
           "at a particular poster type 'look' and the name of the "
           "poster.");
  add_item("wall", "It is hard to make out much of the walls behind the "
           "posters, but what you can see is painted a subtle sunflower "
           "yellow.");
  add_item("floor", "The floor is composed of stone slabs and covered "
           "with frogs.");

  sign=add_sign("A poster entitled 'What Do You Mean?'\n",
           "If you are new to online communication you may be surprised how "
           "easy it is to misunderstand and to be misunderstood when you "
           "only have typed words to go on.  So please take care in how you "
           "word things and also be prepared to seek clarification before "
           "getting upset with someone, they may not have meant it how you "
           "read it!", 0, "general", "general");
  sign->set_short("sign marked 'general'");
  sign->add_property("determinate","the ");
  
  sign=add_sign("A poster entitled 'It's Good To Talk'\n",
           "There are a number of ways to communicate verbally.\n"
           "To speak to everyone in the same room as yourself use the "
           "command 'say', 'lsay' (loud say).  To speak to an individual "
           "you may use 'whisper' if they are in the same room, or "
           "'tell' if they are elsewhere on the disc.  You may also "
           "'shout' which will be heard by everyone who is nearby, however "
           "shouting uses up social points which will regenerate slowly.\n",
           0, "speaking", "general");
  sign->set_short("sign marked 'speaking'");
  sign->add_property("determinate","the ");

  sign=add_sign("A poster entitled 'Womble On'\n",
           "Discworld also has a soul which allows you some emotional "
           "expression, thus you can 'laugh' or 'cry'.  There are "
           "approximately 550 soul commands so they are not listed here.  "
           "Use 'help soul' and 'look soul' to find out more about "
           "them.\n", 0, "expression", "general");
  sign->set_short("sign marked 'expression'");
  sign->add_property("determinate","the ");

  sign=add_sign("A poster entitled 'Laugh In An Old Fashioned Way'\n",
           "Emote allows free expression.  If Jim used 'emote jumps around "
           "in a slightly artistic manner' everyone in the room would see "
           "him do just that.  Initially you will not be able to emote, you "
           "must learn that skill.", 0, "emote", "general");
  sign->set_short("sign marked 'emote'");
  sign->add_property("determinate","the ");

  sign=add_sign("A poster entitled 'What Do You Mean?'\n",
           "For the first day of your time on Discworld you will have access "
           "to the newbie chat channel which is listened to by most creators "
           "and many of the players.  You may use this channel to ask for "
           "help and advice.  To use the channel type 'newbie <message>'"
           ", and to see the last few messages type 'hnewbie'.\n",
           0, "newbie", "general");
  sign->set_short("sign marked 'newbie'");
  sign->add_property("determinate","the ");

  sign=add_sign("A poster entitled 'But You're Not There'\n",
           "Discworld provides a mail system which allows you to send "
           "messages to players who are not currently logged in.  You must "
           "go to the post office to read or send mail.\n"
           "There are also a number of bulletin boards around the place on "
           "which people can place public messages.  The frog board is in "
           "the Mended Drum and the newspaper board is in the offices of "
           "the Ankh-Morpork Daily Times.  Your guild will also have a "
           "board located somewhere in the guildhouse.\n", 0, "mail", "general");
  sign->set_short("sign marked 'mail'");
  sign->add_property("determinate","the ");

  
  /* Make it so you cannot godmother out of here */
  add_property("no godmother", 1);

  room_chat(({120, 240, ({"A small frog bumps into you, apologises "
                            "profusely, and hops off.",
                            "A frog leaps up and down excitedly.",
                            "A frog wombles around you happily.",
                            "A frog bingles happily.",
                            "A frog parps wryly at you."})}));
                        
  add_exit( "foyer", PATH + "foyer", "door" );
  add_alias( "west", "foyer" );
} /* setup() */

