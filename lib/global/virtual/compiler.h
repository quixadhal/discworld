#define TMP_FILE "/global/virtual/tmp_file.c"

#define O_FUN ([ "item"  : "add_item",\
                 "name"  : "set_name",\
                 "long"  : "set_long",\
                 "light" : "set_light",\
                 "alias" : "add_alias",\
                 "plural" : "add_plural",\
                 "adjective" : "add_adjective",\
                 "plural adjective" : "add_plural_adjective",\
                 "main_plural" : "set_main_plural",\
                 "main plural" : "set_main_plural",\
                 "weight" : "set_weight",\
                 "length" : "set_length",\
                 "width" : "set_width",\
                 "value" : "set_value",\
                 "value info" : "set_value_info", \
                 "material" : "set_material", \
                 "property" : "add_property",\
                 "clone" : ({ "do_clone", file_name(this_object()) }), \
                 "money" : "adjust_money",\
                 "effect" : "add_effect",\
                 "read mess" : "add_read_mess", \
                 "short" : "set_short" ])

#define R_FUN ([ "exit" : "add_exit",\
                 "modify exit" : "modify_exit",\
                 "reset" : "add_reset_item",\
                 "dark mess" :  "set_dark_mess",\
                 "co ord" : "set_co_ord", \
                 "size" : "set_room_size", \
                 "zone" : "set_zone", \
                 "cloned object" : "add_cloned_object", \
                 "alias" : "add_alias", \
                 "sign" : "add_sign" ])

#define ARM_FUN ([ "setup" : "setup_armour",\
                   "no limbs" : "set_no_limbs", \
                   "condition" : "set_cond", \
                   "enchant" : "set_enchant", \
                   "ac" : "add_ac", \
                   "max condition" : "set_max_cond", \
                   "immune" : "add_immune_to", \
                   "type" : "set_type", \
                   "wear remove" : "set_wear_remove_func", \
                   "wear effect" : "add_wear_effect", \
                   "dex minus" : "set_dex_minus", \
                   "damage chance" : "set_damage_chance" ])

#define CLO_FUN ([ "setup" : "setup_clothing",\
                   "condition" : "set_cond", \
                   "enchant" : "set_enchant", \
                   "max condition" : "set_max_cond", \
                   "immune" : "add_immune_to", \
                   "type" : "set_type", \
                   "wear remove" : "set_wear_remove_func", \
                   "wear effect" : "add_wear_effect", \
                   "dex minus" : "set_dex_minus", \
                   "pocket mess" : "set_pocket_mess", \
                   "pocket" : "add_pocket", \
                   "stuck" : "set_stuck", \
                   "key" : "set_key", \
                   "damage chance" : "set_damage_chance", \
                   "pair" : "set_is_pair" ])

#define CLOTRANS_FUN ([ \
                   "race" : "set_worn_by_race", \
                   "mount modifier" : "set_mount_difficulty_modifier", \
                   "direction modifier" : "set_direction_difficulty_modifier", \
                 ])

#define SCA_FUN ([ "setup" : "setup_scabbard",\
                   "condition" : "set_cond", \
                   "enchant" : "set_enchant", \
                   "max condition" : "set_max_cond", \
                   "immune" : "add_immune_to", \
                   "type" : "set_type", \
                   "wear remove" : "set_wear_remove_func", \
                   "wear effect" : "add_wear_effect", \
                   "dex minus" : "set_dex_minus", \
                   "types" : "set_types", \
                   "damage chance" : "set_damage_chance" ])

#define CONT_FUN ([ \
                    "amount" : "set_amount", \
                    "continuous" : "set_continuous", \
                    "weight unit" : "set_weight_unit", \
                    "medium alias" : "set_medium_alias", \
                    "pile name" : "set_pile_name", \
                    "amount types" : "set_amount_types", \
                    "pile types" : "set_pile_types", \
                  ])

#define FOOD_FUN ([ \
                    "bites gone" : "set_bites_gone", \
                    "weight per bite" : "set_weight_per_bite", \
                    "eat mess" : "set_eat_mess", \
                    "eat effects" : "add_eat_effect", \
                    "piece description" : "set_piece_description", \
                  ])

#define WEP_FUN ([ "setup" : "new_weapon",\
                   "condition" : "set_condition", \
                   "enchant" : "set_enchant", \
                   "attack" : "add_attack", \
                   "max condition" : "set_max_cond", \
                   "immune" : "add_immune", \
                   "no limbs" : "set_no_limbs", \
                   "attack message" : "add_attack_message", \
                   "damage chance" : "set_damage_chance" ])

#define MON_FUN ([ "move after" : "set_move_after", \
                   "move zone" : "add_move_zone", \
                   "race" : "set_race", \
                   "class" : "set_class", \
                   "guild" : "set_guild", \
                   "level" : "set_level", \
                   "attack" : "add_attack", \
                   "attack message" : "add_attack_message", \
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
                   "aggressive" : "set_aggressive", \
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
                   "concentrate" : "set_concentrate", \
                   "protect" : "do_protect", \
                   "follow" : "do_follow", \
                   "throw out" : "set_throw_out", \
                   "basic setup" : "basic_setup" ])
