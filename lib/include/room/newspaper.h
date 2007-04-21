/**
 * The include file support stuff for newspaper offices and the 
 * newspaper controller.
 * @author Pinkfish
 * @started Fri Apr 27 17:19:23 PDT 2001
 */
#ifndef __NEWSPAPER_H
/** @ignore yes */
#define __NEWSPAPER_H

/**
 * The location of the press creditials file.
 */
#define NEWSPAPER_PRESS_CREDITIALS "/obj/misc/press_credentials"
/**
 * The location of the newspaper handler.
 */
#define NEWSPAPER_HANDLER "/obj/handlers/newspaper"
/**
 * The location of the newspaper object itself.
 */
#define NEWSPAPER_OB "/obj/misc/newspaper"
/**
 * The location of the housing subscription box.
 */
#define NEWSPAPER_SUBSCRIPTION_OB "/obj/furnitures/misc/newspaper_box"

/**
 * This is the start bit of the file name used for the file name of the
 * newspaper logo images.
 */
#define NEWSPAPER_WWW_LOGO_BASE "/external/newspaper/logos/logo"

/**
 * This means the article is plain text type.
 */
#define NEWSPAPER_ARTICLE_TYPE_PLAIN 1
/**
 * This means the article is html.
 */
#define NEWSPAPER_ARTICLE_TYPE_HTML 2
/**
 * This means the article is nroff.
 */
#define NEWSPAPER_ARTICLE_TYPE_NROFF 3
/**
 * This means the article is a section header.
 */
#define NEWSPAPER_ARTICLE_TYPE_SECTION 4
/**
 * This means the article is the advert insert point.
 */
#define NEWSPAPER_ARTICLE_TYPE_ADVERTS 5
/**
 * This is the mask to get the nroff type stuff.
 */
#define NEWSPAPER_ARTICLE_TYPE_MASK 7

/**
 * This means the article is anonymous.
 */
#define NEWSPAPER_ARTICLE_ANONYMOUS_FLAG 1024
/**
 * This means the article has no xp given for it.
 */
#define NEWSPAPER_ARTICLE_NO_XP_FLAG     2048

/**
 * The revenue for each hit on the web page. (10 pence).
 */
#define NEWSPAPER_WEB_HIT_REVENUE 40

/**
 * This is the amount of xp a reporter gets for making an article.
 */
#define NEWSPAPER_XP_PER_ARTICLE 60000

/**
 * This is the amount of xp an editor gets as a base for publishing an
 * issue.  They also get 10% of the total reporter xp.
 */
#define NEWSPAPER_XP_EDITOR 120000

/**
 * Information about an article to be published in a newspaper.
 * @element file_name the file which has the text in it
 * @element title the title of the article
 * @element author the author of the article
 * @element date_written the date the article was written at
 * @element type the type of text that is stored, html, nroff or plain
 * @element shown_name the name to show everyone
 * @element colour the colour of the article
 */
class article {
   string file_name;
   string title;
   string author;
   int date_written;
   int type;
   string shown_name;
   string colour;
}

/**
 * Information about an advert to be published in a newspaper.
 * @element text the text of the add
 * @element title the title of the advert
 * @element author the author of the advert
 * @element category the category the advert is in
 * @element date_written the date the advert was written at
 */
class advert {
   string text;
   string author;
   string category;
   int date_written;
   int value;
}

#endif
