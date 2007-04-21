#ifndef __CWC_H
#define __CWC_H

/** 
 * The base #define for Cwc.  All defines must ultimately be built on
 * this define.
 */ 
#define CWC "/d/cwc/"

/**
 * The location of Cwc wide inheritables.
 */ 
#define INHERIT CWC "inherit/"

/**
 * The location of the base city inherit.  This file should be inherited
 * into all rooms in Cwc that link into the hospital, inside and outside.
 */ 
#define CITY_INHERIT INHERIT "city_inherit"

/**
 * The location of the Cwc hospital inheritable.
 */ 
#define HOSPITAL_INHERIT INHERIT "hospital"

/** 
 * The location of hospital data files.
 */ 
#define HOSPITAL_PATH CWC "hospital/"

/** 
 * The location for Cwc wide handlers.
 */
#define CWC_HANDLERS CWC "handlers/"

/**
 * The location of the Cwc teleport inheritable.  This inherit should be 
 * added to rooms to make them follow the restricted teleport rules 
 * for Cwc.
 */ 
#define CWC_TELEPORT_INHERIT "/d/cwc/inherit/teleport_inherit"

/**
 * The location for Bes Pelargic.  All #define's in /d/cwc/Bes_Pelargic/path.h
 * are built on this #define.
 */ 
#define BES_PELARGIC CWC "Bes_Pelargic/"

/** 
 * The location for the city of Hunghung.  All #defines in /d/cwc/Hunghung/path.h
 * are built on this #define.
 */ 
#define HUNGHUNG CWC "Hunghung/"

/** 
 * The location of the generic NPC characters directory.  Most NPCs in here
 * are generic NPCs which can be used with the hospital.  They must be then
 * configured to move and walk in the city properly.
 */ 
#define CWC_CHARS CWC "chars/"

/** 
 * The location of Cwc wide items.  The Cwc armoury accesses files in this
 * directory.
 */ 
#define CWC_ITEMS CWC "items/"

/**
 * The location of the Cwc save directory.
 */ 
#define CWC_SAVE "/save/cwc/"
#define SAVE "/save/cwc/"

/**
 * This #define points to the secure Cwc directory.  This directory is 
 * only really used for domain administrative purposes.
 */ 
#define SECURE CWC "secure/"

/** 
 * The location of the Cwc origami handler.  This handler determines 
 * how you can fold a piece of paper, and transforms a piece of paper
 * into a folded item.
 */ 
#define ORIGAMI_HANDLER CWC_HANDLERS + "origami_handler"

/**
 * The default start location for people in Cwc.  This is an absolute path
 * and should be revised at a later date.
 */ 
#define CWC_START_LOCATION "/d/cwc/Bes_Pelargic/Bad_Poo-Ning/roads/piers/pier303"

/* These are miscellaneous #define's and need to be moved around to their
 * proper places instead of being here.
 */ 

#define SHOO_LI BES_PELARGIC "Shoo_Li/"
#define CUSTOM_NPCS 0


#endif
