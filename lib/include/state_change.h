/*
 * state_change.h - include file for /obj/handlers/state_change.c
 */

#ifndef __STATE_CHANGE_H
#define __STATE_CHANGE_H

class state_change {
    string result;       // Path of result
    float factor;        // Ratio of old amount to new amount
    function *func;      // Array of function pointers for configuring
                         // the result on-the-fly
    int playtester;      // Whether the state change is restricted to
                         // playtesters only.
}

#define STATE_CHANGE     ("/obj/handlers/state_change")

#endif
