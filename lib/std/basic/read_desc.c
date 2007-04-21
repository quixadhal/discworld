/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: read_desc.c,v 1.13 2003/01/31 04:57:51 pinkfish Exp $
 * 
 */
/**
 * This method keeps track of all the messages on the object for
 * reading etc.  Keeps track of the language, the text and the
 * size.
 * @author Pinkfish
 */
#include <language.h>
#include <nroff.h>

inherit "/std/basic/desc";

/* this one also contians info on read_messages... */

private nosave mixed *_read_mess = ({ });
private nosave int _max_size = 100;
private nosave int _cur_size;

varargs mixed add_read_mess(mixed str, string type, string lang, int size);
mixed query_property(string name);

/**
 * This method sets the maximum amount of writing to be able
 * to be placed onto the object.
 * @param siz the maximum size
 * @see query_max_size()
 * @see add_read_mess()
 */
void set_max_size(int siz) { _max_size = siz; }
/**
 * This method returns the current maximum size of the object.
 * @return the current maximum size
 * @see set_max_size()
 * @see add_read_mess()
 */
int query_max_size() { return _max_size; }

/**
 * This method sets the current amount of writing size on
 * the object.
 * @see set_max_size()
 * @see add_read_mess()
 * @see query_cur_size()
 * @param siz the new current size
 */
void set_cur_size(int siz) { _cur_size = siz; }
/**
 * This method returns the current amount of wreiting size on the
 * object.
 * @see set_max_size()
 * @see add_read_mess()
 * @see set_cur_size()
 * @return the current amount of writing on the object
 */
int query_cur_size() { return _cur_size; }

/**
 * calling this is very rude unless you are createing the object as it
 * erases all of the writing off it.
 * @ignore yes
 */
varargs void set_read_mess(mixed str, string lang, int size) { 
   if ( pointerp( str ) ) {
      _read_mess = str;
      return;
   }
  if (!lang)
    return;
  
  if (!size)
    size = 1;
  if ( str )
    _read_mess = ({ ({ str, 0, lang, size }) });
  else
    _read_mess = ({ });
} /* set_read_mess() */

/**
 * This method returns the current read messae array on the object.
 * The returned array consists of an array of arrays.  The internal
 * array has 4 elements, the first is the text of the mnessage
 * the second is the type of the message,m the third is the languae
 * and the last element is the size of the wiritn .
 *<p>
 * The defines in the file /include/language.h should be used
 * for the indexes into this array.
 * @return the read message array
 * @see add_read_mess()
 * @see set_max_size()
 * @see /include/language.h
 */
mixed *query_read_mess() { return copy( _read_mess ); }

/***
 * This method adds a new read message onto the object. 
 * <p>
 * The str bit is the actual string to add, it is the message which
 * will get printed. The type is the type of the writing, like "spidery
 * writing" or "charcoal based letters". The language is the
 * language in which it is written and the size is the size of the
 * letters. The size is used to see if the writing can fit on the page.
 * <p>
 * If no type is
 * given, then no type bit is printed and if no size is specified a
 * size of 1 is the default. 
 * <p>
 * This also adjusts the size of stuff on the object.  If too much
 * is attemnpted to be written ont
 * the object, the remainer will be cut off.
 * @param str the text of the message
 * @param type the type of the message
 * @param lang the language the message is in
 * @param size the size of the message
 * @see query_read_mess()
 * @see set_max_size()
 * @see /obj/handlers/language->query_language_size()
 * @see /obj/handlers/language->squidge_text()
 */
varargs mixed add_read_mess(mixed str, string type, string lang, int size) { 
  int de_size;

/* fail! */
  if (_cur_size >= _max_size) {
     return "";
  }
  if (!size) {
    size = 1;
  }
  de_size = size*((int)LANGUAGE_HAND->query_language_size(lang, str));
  if (_cur_size+de_size > _max_size) {
/* Try and squidge the text on somehow... */
    str = (string)LANGUAGE_HAND->squidge_text(lang, str,
                                              (_max_size-_cur_size)/size);
    if (!strlen(str))
      return ;
    _cur_size += size*((int)LANGUAGE_HAND->query_language_size(lang, str));
  }
  _read_mess += ({ ({ str, type, lang, size }) });
  return str;
} /* add_read_mess() */

/**
 * This method removes a piece of writing off the object.
 * <p>
 * If you know the actual message or the language or the type you can remove
 * that message.
 *<p>
 * Using the type and or language is a very dodgy way of doing this.  Using
 * a combination is much better.
 * <p>
 * Any of the parameters set to a non-zero value will be used for the search.
 * @param str the text string
 * @see /obj/handlers/language->query_language_size()
 * @see add_read_mess()
 * @see query_read_mess()
 * @param type the type of the text
 * @param lang the language it is written in
 * @return 0 if the text was not found, 1 if it was
 */
int remove_read_mess(string str, string type, string lang) {
  int i;

  for (i=0;i<sizeof(_read_mess);i++) {
    if (str && _read_mess[i][READ_STR] != str)
      continue;
    if (type && _read_mess[i][READ_TYPE] != type)
      continue;
    if (lang && _read_mess[i][READ_LANG] != lang)
      continue;
    _cur_size -= _read_mess[i][READ_SIZE]
                *((int)LANGUAGE_HAND->query_language_size(lang,
                                                    _read_mess[i][READ_STR]));
    _read_mess = _read_mess[0..i - 1] + _read_mess[i + 1..];
    return 1;
  }
  /* Nup.  We traversed it all...  and there was nothing! */
  return 0;
} /* remove_read_mess() */

/**
 * This method returns the read message for the object.  This is what should
 * be displayed to the player if they attempt to read this object.
 * @return the read message, 0 if there is none
 */
string query_readable_message(object player, int ignore_labels) {
  string message;
  string file;
  string type;
  string lang;
  //string garbler;
  mixed mess;
  mixed *bit;
  mixed *read_mess;
  int pos;
  
  if(!player)
    player = this_player();
  
  read_mess = query_read_mess();
  if ( !sizeof( read_mess ) ) {
    return 0;
  }
  message = "";
  foreach ( bit in read_mess ) {
    mess = bit[ READ_MESS ];
    type = bit[ READ_TYPE ];
    lang = bit[ READ_LANG ];
    if ( functionp( mess ) ) {
      mess = evaluate( mess );
    }
    if ( stringp( mess ) ) {
      if ( sscanf( mess, "$$%s$$", file ) ) {
        mess = unguarded( (: read_file, file +".txt" :) );
        if ( !mess ) {
          mess = unguarded( (: NROFF_HAND->cat_file($(file)+"_nroff", 1) :) );
          if(!mess) {
            unguarded((: NROFF_HAND->create_nroff($(file),
                                                  $(file)+"_nroff") :));
            mess = unguarded((: NROFF_HAND->cat_file($(file)+"_nroff", 0) :));
            
            if(!mess)
              mess = "Warning: text file cannot be read!";
          }
        }
      }
    }
    mess = LANGUAGES->garble_text(lang, mess, this_object(), player);
#ifdef DISABLED   
    if (stringp(mess)) {
      pos = strsrch(mess, "Written in ");
      while (pos != -1) {
        if ((pos == 0 && !type) || (pos > 0 && mess[pos-1] == '\n')) {
          mess = mess[0..pos-1] + "---> " + mess[pos..pos + 4] + " " +
            mess[pos + 5..];
        } else {
          mess = mess[0..pos + 4] + " " +
            mess[pos + 5..];
        }
        pos = strsrch(mess[pos + 5..], "Written in ");
      }
    }
#endif    
    if ( !LANGUAGES->query_language_magic( lang ) ) {
      if ( !type && player && (lang == player->query_default_language())) {
        mess += "\n";
      } else {
        if (stringp(lang)) {
          mess = "Written"+ ( type ? " in "+ type : "" ) +
            ( lang != "general" ? " in "+ capitalize(lang) : "" ) +
            ": "+ mess +"\n";
        } else {
          mess = "Written in some weird broken language: " + mess + "\n";
        }
      }
    }
    message += mess;
  }
  return message;
} /* query_readable_mesage() */

/**
 * This method returns the short description to use with the reading.
 * It will go: You read <read short>:
 * <p>
 * It will automaticly add a $name$ onto the end of the string for you.
 * This will be replaced with the correct a/the short for the message
 * which is printed.
 * @param player the player reading the message
 */
string query_read_short(object player, int ignore_labels) {
   string id;
   string *read_mess;

   read_mess = query_read_mess();
   if ( !arrayp(read_mess) || !sizeof( read_mess ) ) {
      return 0;
   }

   id = query_property("read id");
   if (id) {
      id += " $name$";
   } else {
      id = "$name$";
   }
   return id;
} /* query_read_short() */
