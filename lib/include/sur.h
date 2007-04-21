/* sur.h - Common defines for the Sur domain
 *
 * Rhinehold, 08-Aug-01
 *
 */

#if !defined(__SUR__)
#define __SUR__

#define SUR                    "/d/sur/"

// Cities and towns

#define STO_LAT                SUR "StoLat/"
#define NOWHERE                SUR "Nowhere/"
#define DINKY                  SUR "Dinky/"
#define SHEEPRIDGE             SUR "Sheepridge/"
#define HOLYWOOD               SUR "Holywood/"
#define PEKAN_FORD             SUR "PekanFord/"
#define HILLSHIRE              SUR "Hillshire/"
#define CONSORT                SUR "Consort/"
#define SCROGDEN               SUR "Scrogden/"
#define RHAM_NITZ              SUR "Rham_Nitz/"
#define CHIRM                  SUR "Chirm_dev/"
#define JOWSERS_COVE           SUR "JowsersCove_dev/"
#define PSEUDOPOLIS            SUR "Pseudopolis_dev/"
#define QUIRM                  SUR "Quirm_dev/"
#define SCROTE                 SUR "Scrote_dev/"
#define STO_HELIT              SUR "Sto_Helit/"
#define STO_KERRIG             SUR "StoKerrig_dev/"
#define WEEZENT                SUR "Weezent/"

// Large areas

#define STO_PLAINS             SUR "Sto_Plains/"
#define SKUND_FOREST           SUR "Skund/"
#define CARRACK_MTNS           SUR "Carrack_Mtns/"
#define MORPORK_MTNS           SUR "Morpork_Mtns/"
#define ANKH_RIVER             SUR "Ankh_River/"

// Small areas

#define BEL_SHAMHAROTH         SUR "Bel_Shamharoth/"
#define DRYAD                  SUR "Dryad/"
#define HAUNTED_HOUSE          SUR "HauntedHouse/"

// Misc

#define SUR_ITEMS              SUR "items/"
#define SUR_CHARS              SUR "chars/"
//#define SUR_HOSPITAL           (SUR+"hospital")
#define SUR_HOSPITAL           SUR "hospital/sur_hospital"
#define SUR_ROADS              SUR "roads/"
#define SUR_CURRENCY_DEFAULT   "Ankh-Morpork"
#define SAVE                   "/save/"
#define SUR_SAVE               SAVE "sur/"

// Inherits

#define SUR_INHERIT            SUR "inherit/"
#define SUR_HUMAN              SUR_INHERIT "sur_human"
#define SUR_ANIMAL             SUR_INHERIT "sur_animal"
#define SUR_PEDDLER            SUR_INHERIT "sur_peddler"
#define SUR_INSIDE             SUR_INHERIT "inside_room"
#define SUR_OUTSIDE            SUR_INHERIT "outside_room"
#define SUR_ITEM_SHOP_INSIDE   SUR_INHERIT "item_shop_inside"
#define SUR_ITEM_SHOP_OUTSIDE  SUR_INHERIT "item_shop_outside"
#define SUR_GEN_SHOP_INSIDE    SUR_INHERIT "general_shop_inside"
#define SUR_GEN_SHOP_OUTSIDE   SUR_INHERIT "general_shop_outside"

// Standard room sizes

#define SHORT_ROAD             10000
#define MEDIUM_ROAD            25000
#define LONG_ROAD              50000

#endif
