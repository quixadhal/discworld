/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: newbie_curse.c,v 1.1 1998/01/06 04:02:57 ceres Exp $
 * $Log: newbie_curse.c,v $
 * Revision 1.1  1998/01/06 04:02:57  ceres
 * Initial revision
 * 
*/
#include "path.h"
#define MY_NAME "NEWBIE SIMULATOR"

/*
 * The Newbie Simulator. Completely shuts off your conversational abilities,
 * replacing about half of your comments with "amusing" alternative from the
 * list below. 
 * 
 * Veronica 1/94, modified from the Upper Case curse. (with the spelling 
 *                                 cleaned up, too... )
 */

object my_player;

int init_curse(object pl) {
  string str;
  int i;

  if (sscanf((str = file_name(this_object())), "%s#%d", str, i) != 2) {
/* A class not a clone */
    seteuid((string)"/secure/master"->creator_file(str));
    return (int)clone_object(str)->init_curse(pl);
  }
  my_player = pl;
  pl->add_curse(MY_NAME, str);
  if (!shadow(pl, 1)) {
    pl->remove_curse(MY_NAME);
    return 0;
  }
  return 1;
} /* init_curse() */

/*
 * No special requirements for removeing this curse...
 */
int query_remove() { return 1; }

/*
 * Called when the player logs on
 */
int player_start(object pl) {
  object ob;

  if (!pl)
    pl = this_player();
  ob = clone_object(file_name(this_object()));
  ob->init_curse(pl);
} /* player_start() */

/*
 * This gets called with the name of the curse we are getting rid of.
 */
int destruct_curse(string str) {
  if (str == MY_NAME)
    destruct(this_object());
  else
    my_player->destruct_curse(str);
} /* destruct_curse() */

protected string newbie(string str) {
  string *says;
  string ret;
  int i;
  says = ({
	"Are you a creator?\n", 
	"How can I level in this mud?\n",
	"How do I kill things?\n", 
	"This is nothing like my nintendo... where are the baddies?\n", 
	"Where can I get a good weapon?\n",
	"Where do I get money?\n",
	"I'm going to kill you now. Prepare to die.\n",
	"All mudders stink.\n",
	"How can I do those funny nudge and poke things?\n",
	"Why can't I see invisible people?\n",
	"I still don't get this. Who's Terry Pratchett?\n",
	"I'm going back to Diku's, they're much better.\n",
	"Can you tell me where a good quest is?\n",
	"I hate this mud.\n",
	"I'm not really saying this; I've been possessed.\n",
	"Personally, I prefer Volkswagens.\n",
	"What do you mean?\n",
	"Isn't there something you can do?\n",
	"Can you show me what happens when you PK, please?\n",
	"Why can't I kill you? It keeps giving a funny message...\n",
	"I've got a great bug for getting xp...\n",
	"How do I join a guild?\n",
	"How do I go berzerk?\n",
	"I'll be fourteen next week.\n",
	"Where are you in RL?\n",
	"I'm bored. How do I win?\n",
	"Hit me.\n",
	"I'm a newbie! I'm a newbie!\n",
	"If I said you had a beautfiul body...\n",
	"Underground, Overground, wombling free.\n"
	});
  ret = str;
  if (random(100) > 30) {
	ret = (says[(random(sizeof(says)))]);
  }
  return ret;
} /* newbie() */

protected string newbie_emote(string str) {
  string *emotes_male;
  string *emotes_female;
  string ret;
  int i;
  emotes_male = ({
	"looks extremely puzzled.\n",
	"stares around myopically.\n",
	"is obviously hugely well muscled.\n",
	"is a black belt in real life.\n",
	"stares at you intimidatingly.\n",
	"is stunningly masculine.\n",
	"has a real life as well as this mud stuff.\n",
	"doesn't like mud geeks.\n",
	"is a creator on another mud.\n",
	"is a creator on this mud.\n",
	"hasn't looked back after the first twelve million xp.\n",
	"has been asked to be a creator, but turned it down.\n",
	"has loads of girlfriends.\n",
	"has been told he looks like Richard Gere in rl.\n",
	"was consulted for the joy of sex.\n",
	"wonders why anyone would ever drive anything other than a Porsche.\n"
	});

  emotes_female = ({
	"carefully brushes out her long, golden hair.\n",
	"looks a bit like Kim Basinger in RL.\n",
	"has really _great_ tits.\n",
	"stares at you seductively and bites her lower lip.\n",
	"has modelled for a couple of magazines.\n",
	"looks really puzzled.\n",
	"carefully picks her nose with a long, elegant fingernail.\n",
	"tries to straighten her hose without anyone noticing.\n",
	"carefully applies some new lip gloss.\n",
	"isn't really into this competition thing.\n",
	"is really only fourteen years old.\n"
	});

  if ((int)(my_player->query_gender()) == 2 ) {
	ret = (emotes_female[(random(sizeof(emotes_female)))]);
  }
  if ((int)(my_player->query_gender()) == 1 ) {
	ret = (emotes_male[(random(sizeof(emotes_male)))]);
  }
  return ret;
} /* newbie() */

int do_say(string str) {
  return (int)my_player->do_say(newbie(str));
} /* do_say() */

int do_tell(string str) {
  return (int)my_player->do_tell(newbie(str));
} /* do_tell() */

int do_loud_say(string str) {
  return (int)my_player->do_loud_say(newbie(str));
} /* do_loud_say() */

int do_echo(string str) {
  return (int)my_player->do_echo(newbie(str));
} /* do_echo() */

int do_emote_all(string str) {
  return (int)my_player->do_echo_all(newbie_emote(str));
} /* do_emote_all() */

int do_emote(string str) {
  return (int)my_player->do_emote(newbie_emote(str));
} /* do_emote() */

int do_whisper(string str) {
  return (int)my_player->do_whisper(newbie(str));
} /* do_whisper() */

int do_echo_to(string str) {
  return (int)my_player->do_echo_to(newbie(str));
} /* do_echo_to() */

int do_shout(string str) {
  return (int)my_player->do_shout(newbie(str));
} /* do_shout() */

int do_talk(string str) {
  return (int)my_player->do_talk(newbie(str));
} /* do_talk() */
