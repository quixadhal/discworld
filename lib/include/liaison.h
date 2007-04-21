/**
 *  Liaison domain include file
 *  @author Wyvyrn
 *  @stared 2002-09-05
 */


#ifndef __LIAISON_H
#define __LIAISON_H

/* Domain define */
 
#define LIAISON                "/d/liaison/"

/* Yeth, Mathter! */

#define LIAISON_MASTER         LIAISON + "master"

/* General defines */

#define HANDLERS               LIAISON + "handlers/"
#define HELPERS                LIAISON + "helpers/"
#define IDEAS                  LIAISON + "ideas/"
#define ITEMS                  LIAISON + "items/"
#define NEWBIE                 LIAISON + "NEWBIE/"
#define SECURE                 LIAISON + "secure/"
#define TOYS                   LIAISON + "TOYS/"
#define UTILS                  LIAISON + "utils/"
#define WEB                    LIAISON + "public_html/"


/* Save file defines */

#define SAVE_DIR               "/save/"
#define SAVE                   SAVE_DIR + "liaison/"
#define LIAISON_DOMAIN_INFO    SECURE + "domain_info.txt"


/* Docs defines */

#define DOC_DIR                "/doc/creator/liaison/"
#define POLICY                 DOC_DIR + "policy/"


/* Newbie defines */

#define NEWBIE_CREATOR         NEWBIE + "CREATOR/"

#define NEWBIE_CHARS           NEWBIE + "chars/"
#define NEWBIE_DOC             NEWBIE + "doc/"
#define NEWBIE_ITEMS           ITEMS + "NEWBIE/"
#define NEWBIE_SAVE            NEWBIE + "save/"

/* Toys defines */

#define TOYS_IDEAS             TOYS + "ideas/"

#endif
