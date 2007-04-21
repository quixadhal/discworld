/**
 * Defines and support stuff for the board handler.  This is one of the very
 * early objects/files.  One which has not changed overly much either.
 * @author Pinkfish
 */
#ifndef __SYS__BOARD
/** @ignore yes */
#define __SYS__BOARD

/**
 * The master object.
 */
#define MASTER_OB "/secure/master"
/**
 * The name of the property on the player that has the news rc.  The news RC
 * is the last time each board was read.
 */
#define NEWS_RC "news rc"
/**
 * This is the property on the player that stores the board order.  The
 * board order is the order in which the boards are read from the web and
 * by using the 'new' word inside the mud itself.
 */
#define BOARD_ORDER "board order"
/**
 * This is the main save file for the board handler.  It has the current
 * boards, subjects, permissions etc saved to it.
 */
#define BOARD_FILE "/save/boards.o"
/**
 * This is the directory into which the individual board messages are
 * saved.
 */
#define BOARD_DIR "/save/boards/"

/**
 * This is the directory into which the individual board messages are
 * saved temporarily.
 */
#define BOARD_RAM_DIR "/save/ramdisk/boards/"

/**
 * The path to the board handler object itself.
 */
#define BOARD_HAND "/obj/handlers/board_handler"
/**
 * The path to the web board reader.
 */
#define BOARD_WEB "/www/boards"

/**
 * The index into the subject array that is the subject.
 * @see /obj/handlers/board_handler->get_subjects()
 */
#define B_SUBJECT 0
/**
 * The index into the subject array that is the name of the person who
 * posted the note.
 */
#define B_NAME 1
/**
 * The index into the subject array that is the note number associated with
 * the note.
 */
#define B_NUM 2
/**
 * The index into the subject array that contains the time the note was
 * posted.
 */
#define B_TIME 3
/**
 * The index into the subject array that determines who the followups and
 * replies to the note should go to.  If this value is 0 then the
 * followups go to the board and the replies go to the person who
 * posted it.  If it is a string then the followups and replies go to the
 * person in the string.
 */
#define B_REPLY_TYPE 4
/**
 * The index into the subject array that points back to the message we
 * followed up to.
 */
#define B_REPLY_NUM 5

class reply_type {
   int type;
   mixed data;
}

/**
 * This is the type to use if the reply should be mailed to the
 * poster.
 */
#define B_REPLY_POSTER 1
/**
 * This is the type to use if the reply should be mailed to the specified
 * person in the data field.
 */
#define B_REPLY_NAMED 2

/**
 * This is the parameter passed in the access method if the check is for
 * reading.
 */
#define B_ACCESS_READ 1
/**
 * This is the parameter passed in the access method if the check is for
 * writing.
 */
#define B_ACCESS_WRITE 2
/**
 * This is the parameter passed in the access method if the check is for
 * deleting.
 */
#define B_ACCESS_DELETE 3
/**
 * This is the parameter passed in the access method if the check is for
 * approving a post.
 */
#define B_ACCESS_APPROVE 4
/**
 * This is the parameter passed in the access method when an inform
 * is required.
 */
#define B_ACCESS_INFORM 5

/**
 * The default privilages for a board.
 */
#define B_PRIV_NORMAL                 0
/**
 * Make the board read only.
 */
#define B_PRIV_READ_ONLY              1
/**
 * Restrict access to a specified group of people.
 */
#define B_PRIV_ACCESS_RESTRICTED      2
/**
 * Restrict access to a specified group of files.
 */
#define B_PRIV_ACCESS_RESTRICTED_FILE 3
/**
 * Don't inform people when messages are posted.
 */
#define B_PRIV_NO_INFORM              256
/**
 * The type mask.
 */
#define B_PRIV_TYPE_MASK              255
/**
 * Do all the access checks and everything through another function check in
 * another file.
 */
#define B_PRIV_ACCESS_RESTRICTED_METHOD 4

/**
 * This amount of social points used to post to the boards.
 */
#define BOARD_SOCIAL_POINT_COST 50

#endif /* __SYS__BOARD */
