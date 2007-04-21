/**
 * The main include file for the autodocumentation system.
 * @author Pinkfish
 */
#ifndef __AUTODOC_H
/** @ignore */
#define __AUTODOC_H

/** The type index into the function definition save type. */
#define AUTO_TYPE 0
/** The args index into the function definition save type. */
#define AUTO_ARGS 1
/** The docs index into the function definition save type. */
#define AUTO_DOCS 2

/** The index for the docs in the class array. */
#define AUTO_CLASS_DOCS 2
/** The index of the members sectio nof the main class array. */
#define AUTO_CLASS_MEMBERS 1
/** The index for the types in the class array. */
#define AUTO_CLASS_TYPE 1
/** The index for the name of the member in the class array. */
#define AUTO_CLASS_NAME 0

/**
 * In the args section of the function definition stuff, this is the
 * type index.
 */
#define AUTO_ARGS_TYPE 0
/**
 * In the args section of the function definition stuff, this is the
 * args index.
 */
#define AUTO_ARGS_NAME 1

/** The name in the index array. */
#define AUTO_INDEX_NAME      0
/** The file name of the index in the index array. */
#define AUTO_INDEX_FILE_NAME 1
/** The args in the index array. */
#define AUTO_INDEX_ARGS      2
/** The summary string in the index array. */
#define AUTO_INDEX_SUMMARY   3

/** The path of the parser to the autodoc stuff. */
#define AUTODOC_FILE "/obj/handlers/autodoc/autodoc_file"
/** The path of the file to make the resulting nroff files. */
#define AUTODOC_NROFF "/obj/handlers/autodoc/autodoc_nroff"
/** The path of the file to make the html files. */
#define AUTODOC_HTML "/obj/handlers/autodoc/autodoc_html"
/** The main autodoc handler object. */
#define AUTODOC_HANDLER "/obj/handlers/autodoc/autodoc_handler"

/** Where to put the nroff documents. */
#define NROFF_DOC_DIR "/doc/autodoc/"
/** The path of the file to put the single function nroff files. */
#define NROFF_DOC_SINGLE "/doc/creator/autodoc/"
/** Where to put the html documents. */
#define HTML_DOC_DIR "/www/external/autodoc/"

/** The path of the master object. */
#define MASTER_OB "/secure/master.c"
/** Where to save the parsed files. */
#define SAVE_DIR "/save/autodoc/"

/** The delay bewteen each time we save the index file.   1 day. */
#define SAVE_INDEX_DELAY (60 * 60 * 24)

#endif
