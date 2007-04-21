/**
 * This is the clothing include file.  It has all the defines and
 * classes need to make your clothing more fun.
 * @author Pinkfish
 */
#ifndef __CLOTHING_H
/** @ignore yes */
#define __CLOTHING_H
 
/**
 * This points to the clothing handler object.
 */
#define CLOTHING_HANDLER "/obj/handlers/clothing_handler"
 
class clothing_type {
   string *_not_remove;
   string *_hiding;
   int _depth;
}
 
#endif
