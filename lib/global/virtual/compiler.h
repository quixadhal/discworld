#define TMP_FILE "/global/virtual/tmp_file.c"

#define O_FUN ([ "item"  : "add_item",\
                 "name"  : "set_name",\
                 "long"  : "set_long",\
                 "light" : "set_light",\
                 "alias" : "add_alias",\
                 "plural" : "add_plural",\
                 "adjective" : "add_adjective",\
                 "main_plural" : "set_main_plural",\
                 "main plural" : "set_main_plural",\
                 "weight" : "set_weight",\
                 "value" : "set_value",\
                 "property" : "add_property",\
                 "clone" : ({ "do_clone", file_name(this_object()) }), \
                 "money" : "adjust_money",\
                 "short" : "set_short" ])

#define R_FUN ([ "exit" : "add_exit",\
                 "modify exit" : "modify_exit",\
                 "reset" : "add_reset_item",\
                 "dark mess" :  "set_dark_mess",\
                 "co ord" : "set_co_ord", \
                 "zone" : "set_zone", \
                 "alias" : "add_alias", \
                 "sign" : "add_sign" ])

#define ARM_FUN ([ "setup" : "setup_armour",\
                   "condition" : "set_condition", \
                   "enchant" : "set_enchant", \
                   "ac" : "add_ac", \
                   "max condition" : "set_max_cond", \
                   "immune" : "add_immune", \
                   "type" : "set_type", \
                   "wear remove" : "set_wear_remove_func", \
                   "dex minus" : "set_dex_minus", \
                   "damage chance" : "set_damage_chance" ])

#define WEP_FUN ([ "setup" : "new_weapon",\
                   "condition" : "set_condition", \
                   "enchant" : "set_enchant", \
                   "attack" : "add_attack", \
                   "max condition" : "set_max_cond", \
                   "immune" : "add_immune", \
                   "weapon skill" : "set_weap_skill", \
                   "damage chance" : "set_damage_chance" ])

#define MON_FUN ([ "move after" : "set_move_after", \
                   "move zone" : "add_move_zone", \
                   "race" : "set_race", \
                   "class" : "set_class", \
                   "guild" : "set_guild", \
                   "level" : "set_level", \
                   "attack" : "add_attack", \
                   "ac" : "add_ac", \
                   "skill" : "add_skill_level", \
                   "equip" : "init_equip", \
                   "remove ac" : "remove_ac", \
                   "remove attack" : "remove_attack", \
                   "attack everyone" : "set_attack_everyone", \
                   "random stats" : "set_random_stats", \
                   "money" : "give_money", \
                   "chat" : "add_chat_string", \
                   "chat chance" : "set_chat_chance", \
                   "achat" : "add_achat_string", \
                   "achat chance" : "set_achat_chance", \
                   "talk string" : "add_talk_string", \
                   "join fights" : "set_join_fights", \
                   "alignment" : "set_al", \
                   "follow speed" : "set_follow_speed", \
                   "aggresive" : "set_aggresive", \
                   "str" : "set_str", \
                   "con" : "set_con", \
                   "dex" : "set_dex", \
                   "wis" : "set_wis", \
                   "bonus int" : "set_bonus_int", \
                   "bonus str" : "set_bonus_str", \
                   "bonus con" : "set_bonus_con", \
                   "bonus dex" : "set_bonus_dex", \
                   "bonus wis" : "set_bonus_wis", \
                   "int" : "set_int", \
                   "gender" : "set_gender", \
                   "command" : "add_known_command", \
                   "consentrate" : "set_concentrate", \
                   "protect" : "do_protect", \
                   "follow" : "do_follow", \
                   "throw out" : "set_throw_out" ])
