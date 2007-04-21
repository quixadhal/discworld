/**
 * The include file for the newspaper archive shop.
 * @author Pinkfish
 * @started Wed May 23 16:31:59 PDT 2001
 */
#ifndef __NEWSPAPER_HANDLER
/** @ignore yes */
#define __NEWSPAPER_HANDLER

/**
 * This is the event called when there are no papers to list.
 */
#define NEWSPAPER_ARCHIVE_EVENT_NO_PAPERS "no_papers"

/**
 * This is the event called when the listing is attempted to be done.
 * The extra arguments are the default output string and the
 * array of papers.
 */
#define NEWSPAPER_ARCHIVE_EVENT_LIST "list"

/**
 * This is the event called when the paper asked to be listed is 
 * non-existant.  The extra argyment to this is the paper which 
 * was entered and unable to be matched.
 */
#define NEWSPAPER_ARCHIVE_EVENT_INVALID_PAPER "invalid_paper"

/**
 * This is the event called when the edition being bought is invalid.  The
 * extra arguments are the paper name, the edition being checked.
 */
#define NEWSPAPER_ARCHIVE_EVENT_INVALID_EDITION "invalid_edition"

/**
 * This event is called when the paper is too expensive to be bought.
 * The extra arguments are the paper name, the edition being bought
 * and how much it cost.
 */
#define NEWSPAPER_ARCHIVE_EVENT_TOO_POOR "too_poor"

/**
 * This event is called to move the paper into the person's inventory.
 * The extra arguements are the paper being bought, the edition number,
 * the cost and the newspaper object.
 */
#define NEWSPAPER_ARCHIVE_EVENT_MOVE_PAPER "move_paper"

/**
 * This event is called when the paper is finally bought.  The extra
 * arguments are the paper name, the edition being bought, the cost
 * and the paper object itself.
 */
#define NEWSPAPER_ARCHIVE_EVENT_BOUGHT "bought"

#endif
