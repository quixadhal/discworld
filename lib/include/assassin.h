#include <guilds.h>

#ifndef __ASSASSIN_H
#define __ASSASSIN_H

#define ASSASSIN              "/std/guilds/assassin"
#define ASSASSINS             ( GUILDS + "assassins/" )
#define ASSASSIN_INHERIT      ( ASSASSINS + "inherit/" )
#define ASSASSIN_HANDLERS     "/obj/handlers/guild_things/"

#define ASSASSIN_AM           ( ASSASSINS + "Ankh-Morpork/" )
#define ASSASSIN_GENUA        ( ASSASSINS + "Genua_dev/" )
#define ASSASSIN_OC           ( ASSASSINS + "Ohulan-Cutash/" )
#define ASSASSIN_KLK          ( ASSASSINS + "Khot-lip-khin/" )
#define ASSASSIN_SHEEPRIDGE   ( ASSASSINS + "Sheepridge/" )

#define ASSASSINCHARS         ( ASSASSINS + "chars/" )
#define ASSASSINITEMS         ( ASSASSINS + "items/" )

#define ASSASSIN_SAVE         ( GUILDSAVE + "assassins/" )
#define ASSASSIN_WARDROBES    ( ASSASSIN_SAVE + "wardrobes/" )
#define ASSASSIN_VOTES        ( ASSASSIN_SAVE + "votes/" )
#define ASSASSIN_AM_SAVE      ( ASSASSIN_SAVE + "Ankh-Morpork/" )
#define ASSASSIN_OC_SAVE      ( ASSASSIN_SAVE + "Ohulan-Cutash/" )

#define RUN_HANDLER           ( ASSASSIN_HANDLERS + "run" )
#define ROOM_TRACKER          ( ASSASSIN_HANDLERS + "assassins_gm_room_tracker" )

#define GUILDROOM             ( ASSASSIN_INHERIT + "guildroom" )
#define RUNROOM               ( ASSASSIN_INHERIT + "runroom" )
#define INSIDERUN             ( ASSASSIN_INHERIT + "insiderun" )
#define OUTSIDERUN            ( ASSASSIN_INHERIT + "outsiderun" )
#define ASSASSINS_SHOP        ( ASSASSIN_INHERIT + "assassins_shop" )
#define INHUMATION_OFFICE     ( ASSASSIN_INHERIT + "inhumation_office" )

#define DISAVOWED             "/std/guilds/disavowed"

#endif
