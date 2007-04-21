/**
 * A nice little hearts game
 * @author Pinkfish
 * @started Thu Dec  7 08:41:08 PST 2000
 */
inherit DIR "multiplayer_base";
inherit DIR "card_base";
#include "card_base.h"

class playing_card* deck;

void setup() {
   set_short("hearts game");
   add_adjective("hearts");
   set_name("game");

   set_long("A bunch of playing cards that looks wonderful for playing "
            "a game of hearts with.\n");

   add_player_id_type("green", 0);
   add_player_id_type("red", 0);
   add_player_id_type("blue", 0);
   add_player_id_type("yellow", 0);
   add_player_id_type("mauve", 0);
   add_player_id_type("vermillion", 0);
   add_player_id_type("cyan", 0);
} /* setup() */
