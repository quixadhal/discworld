/** 
 * This file contains the classes and defines used in the 
 * armour call back system. 
 */ 

#ifndef __SYS__AC_CALLBACK
/** @ignore yes */ 
#define __SYS__AC_CALLBACK


/** 
 * This class stores armour class call back data.
 * @member ob the object that owns the callback.
 * @member id the callback id, used to remove it.
 * @member func the function called when it is triggered. (either 
 * evaluated if it is a function pointer, or call_other()'d if it 
 * is an array in the format ({ object, function }) 
 * @member priority an integer indicating the priority of the call back,
 * functions with a higher priority are evaluated first.
 * @member extra any extra data.
 */ 
class ac_callback_data { 
    object ob; 
    int id; 
    function func;
    int priority; 
    mixed extra; 
}
#endif /* __SYS__AC_CALLBACK */ 

