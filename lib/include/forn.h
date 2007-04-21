/**
 *  Forn include file
 *  @author Drakkos
 *  @stared 28/11/2000
 */


#ifndef __FORN_H
#define __FORN_H

/* Domain define */
 
#define FORN                   "/d/forn/"

/* General defines */

#define CHARS                  FORN + "chars/" 
#define DOCS                   FORN + "docs/"
#define FORN_INCLUDE           FORN + "include/"
#define HANDLERS               FORN + "handlers/"
#define INHERITS               FORN + "inherits/"
#define HOSPITAL_DIR           FORN + "hospital/"
#define IDEAS                  FORN + "ideas/"
#define ITEMS                  FORN + "items/"
#define LOG                    FORN + "log/"
#define SECURE                 FORN + "secure/"
#define UTILS                  FORN + "utils/"
#define WEB                    FORN + "public_html/"
#define EFFECTS                FORN + "effects/"

/* Save file defines */

#define SAVE_DIR               "/save/"
#define SAVE                   SAVE_DIR + "forn/"

/* Area defines */

#define GENUA                  FORN + "genua/"

/* Miscellaneous defines */

#define POLICY                 DOCS + "policy/"
#define HELP                   DOCS + "help_files/"
#define SCRIPTS                DOCS + "scripts/"
#define WARGAME_MAPS           DOCS + "wargame_maps/"

/* Genua defines */

#define GENUA_CHARS            GENUA + "chars/"
#define GENUA_CITY             GENUA + "genua_city_dev/"
#define GENUA_EFFECTS          GENUA + "effects/"
#define GENUA_HANDLERS         GENUA + "handlers/"
#define GENUA_HOUSING          GENUA + "player_housing/"
#define GENUA_INHERITS         GENUA + "inherits/"
#define GENUA_ITEMS            GENUA + "items/"
#define GENUA_PLAYER_HOUSING   GENUA + "player_housing/"
#define GENUA_PLAYER_HOUSING   GENUA + "player_housing/"
#define GENUA_ROGUE_ENCAMPMENT GENUA + "rogue_encampment/"
#define GENUA_SHADOWS          GENUA + "shadows/"
#define GENUA_SLEEPING_CASTLE  GENUA + "sleeping_castle/"
#define GENUA_SWAMP            GENUA + "swamp/"
#define GENUA_VILLAGES         GENUA + "villages/"
#define GENUA_YELLOW_ROAD      GENUA + "yellow_brick_road/"
#define GENUA_BANKER           GENUA_INHERITS + "coffee_nostra_banker"
#define GENUA_BASE             GENUA_INHERITS + "genua_base"
#define GENUA_CITIZEN          GENUA_INHERITS + "protected_npc"
#define GENUA_CAPO             GENUA_INHERITS + "capo_inherit"
#define GENUA_CITYROOM         GENUA_INHERITS + "cityroom"
#define GENUA_CITY_GUARD       GENUA_INHERITS + "city_guard"
#define GENUA_COMMERCE         GENUA_INHERITS + "genua_commerce"
#define GENUA_CONSIGLIERE      GENUA_INHERITS + "consigliere"
#define GENUA_GENERAL_SHOP     GENUA_INHERITS + "genua_general_shop"
#define GENUA_HOTEL_INSIDE     GENUA_INHERITS + "genua_hotel_inside"
#define GENUA_HOTEL_OUTSIDE    GENUA_INHERITS + "genua_hotel_outside"
#define GENUA_INNER_RING       GENUA_INHERITS + "genua_inner_ring"
#define GENUA_INSIDE_ROOM      GENUA_INHERITS + "genua_inside_room"
#define GENUA_ITEM_SHOP        GENUA_INHERITS + "genua_item_shop"
#define GENUA_MAFIA_HOOD       GENUA_INHERITS + "coffee_nostra_hood"
#define GENUA_MAFIA_INHERIT    GENUA_INHERITS + "mafia_inherit"
#define GENUA_MARKET_INHERIT   GENUA_INHERITS + "genua_market_inherit"
#define GENUA_MONEYCHANGER     GENUA_INHERITS + "genua_bureau_de_change"
#define GENUA_MONSTER          GENUA_INHERITS + "genua_monster"
#define GENUA_OUTER_RING       GENUA_INHERITS + "genua_outer_ring"
#define GENUA_OUTSIDE_ROOM     GENUA_INHERITS + "genua_outside_room"
#define GENUA_PLAYER_HOUSE     GENUA_INHERITS + "genua_player_house"
#define GENUA_PLAYER_HOUSE_OUTSIDE GENUA_INHERITS + "genua_player_house_outside"
#define GENUA_PUB              GENUA_INHERITS + "genua_pub"
#define GENUA_PUB_OUTSIDE      GENUA_INHERITS + "genua_pub_outside"
#define GENUA_SOLDIER          GENUA_INHERITS + "genua_soldier"
#define GENUA_WATCH_INHERIT    GENUA_INHERITS + "watch_house"
#define GRAVEYARD_INHERIT      GENUA_INHERITS + "graveyard_inherit"
#define MURDER_SUSPECT         GENUA_INHERITS + "murder_suspect"

/* Handler directory defines */

#define CRIME_HANDLER_DIR      GENUA_HANDLERS + "ledger/"
#define PUNISHMENT_HANDLER_DIR GENUA_HANDLERS + "punishment/"
#define SERVICE_HANDLER_DIR    PUNISHMENT_HANDLER_DIR + "service/"

/* Genua Village Areas */

#define BOIS                   GENUA_VILLAGES + "bois/"

/* Bois Defines */

#define BOIS_BEARS_HOUSE       BOIS + "bears_house/"
#define BOIS_GRANDMA           BOIS + "grandma/"
#define BOIS_LAKE              BOIS + "lake/"
#define BOIS_MAIN_ROAD         BOIS + "main_road/"
#define BOIS_THREE_PIGS        BOIS + "three_pigs/"
#define BOIS_VILLAGE           BOIS + "village/"
#define BOIS_CHARS             GENUA_CHARS + "bois/"


/* Genua city areas */

#define BLACK_MARKET           GENUA_CITY + "black_market/"
#define CALYPSO_STREET         GENUA_CITY + "calypso_street/"
#define CARREFOUR_CRESCENT     GENUA_CITY + "carrefour_crescent/"
#define CITY_LIBRARY           GENUA_CITY + "city_library/"
#define COFFEE_NOSTRA_ROOM     GENUA_CITY + "coffee_nostra/"
#define DARLING_ROAD           GENUA_CITY + "darling_road/"
#define DAUPHIN_STREET         GENUA_CITY + "dauphin_street/"
#define DEAD_MANS_WALK         GENUA_CITY + "dead_mans_walk/"
#define DIAMOND_CRESCENT       GENUA_CITY + "diamond_crescent/"
#define DOLMEN_WALK            GENUA_CITY + "dolmen_walk/"
#define FELICITY_AVENUE        GENUA_CITY + "felicity_avenue/"
#define FLORIN_AVENUE          GENUA_CITY + "florin_avenue/"
#define GENUA_AVENUE           GENUA_CITY + "genua_avenue/"
#define GENUA_BARRACKS         GENUA_CITY + "barracks/"
#define GENUA_CASINO           GENUA_CITY + "genua_casino/"
#define GENUA_CITY_PARK        GENUA_CITY + "genua_city_park/"
#define GENUA_DOCKS            GENUA_CITY + "genua_docks/"
#define GENUA_GRAVEYARD        GENUA_CITY + "graveyard/"
#define GENUA_HAUNTED_HOUSE    GENUA_CITY + "haunted_house/"
#define GENUA_HOTEL            GENUA_CITY + "genua_hotel/"
#define GENUA_KICK_CONSORTIUM  GENUA_CITY + "kick_consortium/"
#define GENUA_MAIN_GATE        GENUA_CITY + "main_gate/"
#define GENUA_MARKET           GENUA_CITY + "genua_market/"
#define GENUA_PALACE           GENUA_CITY + "genua_palace/"
#define GENUA_RACETRACK        GENUA_CITY + "genua_racetrack/"
#define GENUA_SMUGGLERS_COVE   GENUA_CITY + "smugglers_cove/"
#define GENUA_WEDDING_CHAPEL   GENUA_CITY + "wedding_chapel/"
#define GOLDEN_KEY_CRESCENT    GENUA_CITY + "golden_key_crescent/"
#define GRIM_STREET            GENUA_CITY + "grim_street/"
#define HOUSE_OF_MUSIC         GENUA_CITY + "house_of_music/"
#define LACHAISE_ROAD          GENUA_CITY + "lachaise_road/"
#define LADY_BROOK_STREET      GENUA_CITY + "lady_brook_street/"
#define LIQUOR_ALLEY           GENUA_CITY + "liquor_alley/"
#define LOVERS_WALK            GENUA_CITY + "lovers_walk/"
#define MARKET_STREET          GENUA_CITY + "market_street/"
#define MINARET_STREET         GENUA_CITY + "minaret_street/"
#define NAWLINS_ROAD           GENUA_CITY + "nawlins_road/"
#define OKRA_AVENUE            GENUA_CITY + "okra_avenue/"
#define PARADISE_CRESCENT      GENUA_CITY + "paradise_crescent/"
#define PIRATE_AVENUE          GENUA_CITY + "pirate_avenue/"
#define QUEENS_DRAG            GENUA_CITY + "queens_drag/"
#define ROYAL_AVENUE           GENUA_CITY + "royal_avenue/"
#define RUE_EPHEBE             GENUA_CITY + "rue_ephebe/"
#define SAMEDI_CRESCENT        GENUA_CITY + "samedi_crescent/"
#define SAPPHIRE_STREET        GENUA_CITY + "sapphire_street/"
#define SAVANNAH_ROAD          GENUA_CITY + "savannah_road/"
#define SEWERS                 GENUA_CITY + "sewers/"
#define TACTICUS_ACADEMY       GENUA_CITY + "tacticus_academy/"
#define TACTICUS_ROAD          GENUA_CITY + "tacticus_road/"
#define TCHUTTIFRUITTI_STREET  GENUA_CITY + "tchuttifruitti_street/"
#define TRADE_STREET           GENUA_CITY + "trade_street/"
#define YAGA_CRESCENT          GENUA_CITY + "yaga_crescent/"
#define VIEUX_PROMENADE        GENUA_CITY + "vieux_promenade/"
#define VOODOO_LANE            GENUA_CITY + "voodoo_lane/"
#define WATCH_HOUSE            GENUA_CITY + "watch_house/"
#define ZOMBIE_WALK            GENUA_CITY + "zombie_walk/"

/* Liquor Alley Pubs */

#define ABSINTHE_HOUSE         LIQUOR_ALLEY + "absinthe_house/"
#define BAYOU_BOURBON          LIQUOR_ALLEY + "bayou_bourbon/"
#define DAIQUIRY_DELIGHTS      LIQUOR_ALLEY + "daiquiry_delights/"
#define LA_MAISON_LIQUOR       LIQUOR_ALLEY + "la_maison_liquor/"
#define LADY_BROOK_LIQUOR      LIQUOR_ALLEY + "lady_brook_liquor/"
#define MADAME_DELIGHT         LIQUOR_ALLEY + "madame_delight/"
#define SALUT                  LIQUOR_ALLEY + "salut/"

/* Specific domain object defines */

#define BROCHURE               IDEAS + "brochure_text/"     
#define CALYPSO_FLOWER_HANDLER (GENUA_HANDLERS + "calypso_blooms")
#define CASTLE_HANDLER         (GENUA_HANDLERS + "sleeping_castle_handler")
#define CHIVALRY_HANDLER       (GENUA_HANDLERS + "chivalry")
#define COMMON_ROOM            (FORN + "common")
#define GENUA_DOCKS_HANDLER    (GENUA_HANDLERS + "genua_docks_handler")
#define GENUA_EMBASSY          UTILS + "embassy/"
#define COFFEE_RUN             (GENUA_EMBASSY + "coffee_run/")
#define GENUA_HANDLER          (GENUA_HANDLERS + "general_genua_handler")
#define GENUA_LEDGER           (CRIME_HANDLER_DIR + "genua_ledger")
#define HAUNTING_HANDLER       (GENUA_HANDLERS + "haunting_handler")
#define COFFEE_NOSTRA          (GENUA_HANDLERS + "coffee_nostra")
#define INVENTORY              (GENUA_HANDLERS + "inventory_handler")
#define HOSPITAL               (HOSPITAL_DIR + "hospital")
#define INTERVIEW_ROOM         (UTILS + "interview")
#define ODD_JOBS               (GENUA_HANDLERS + "odd_jobs")
#define PACKAGE                (INHERITS + "package_inherit")
#define PEER_REVIEW            (HANDLERS + "peer_review")
#define PLAYTESTING_ROOM       (UTILS + "playtesting")
#define RUMOUR_MILL            (HANDLERS + "rumour_mill")
#define MURDER_MYSTERY         (GENUA_HANDLERS + "murder_mystery")
#define PRISON_CELL            (WATCH_HOUSE + "cell")
#define SCRIPT_INHERIT         (INHERITS + "scripting")
#define SPECIAL_INTEREST       (HANDLERS + "special_interest")
#define TRAINABLE_PET          (INHERITS + "trainable_pet")
#define WEB_HANDLER            (WEB + "misc/web_handler")
#define WHITEBOARD             (INHERITS + "whiteboard")

/* Some other stuff */

#define GENUA_DOLLAR 300
#define GENUA_CENT (GENUA_DOLLAR / 100)
#define GENUA_FORIN (GENUA_DOLLAR * 10)

#endif
