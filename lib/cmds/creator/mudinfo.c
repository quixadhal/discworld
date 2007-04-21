/*  -*- LPC -*-  */
/*
 * $Id: mudinfo.c,v 1.3 2002/03/24 20:22:44 wodan Exp $
 */
inherit "/cmds/base";
/* Display various information about the mud's status.

   File taken from the VikingMUD lib.  Written by Kniggit 930525

   Hamlet added some stuff, May 1997.
   Hamlet got annoyed and reworked the code, Jul 1997.
*/
 
#ifndef __COMPILER__
#define __COMPILER__     "probably gcc"
#define __OPTIMIZATION__ "bugged driver?"
#endif

#define WIDTH 15

#define PAD(x,y)      sprintf("%-*s: %s\n", WIDTH, x,"" + y)

varargs string pretty_time(int tim, int len);

private nosave int num_obs;

mixed cmd(string str) {
   mapping r;
   float cpu;
   string name, cc, tmp;
   int *ltype; //link type
   
   name = capitalize(mud_name()) + "   (" + query_host_name() + " " + 
          __PORT__ + ")";
   cc = __COMPILER__ + "   (" + __OPTIMIZATION__ + ")";
   r = rusage();
   cpu = ((r["utime"] + r["stime"]) / uptime() / 10.0);
   num_obs = 0;
   objects((: num_obs++ :));
   ltype = implode(users(), (:$1[compressedp($2)]++, $1:), ({0,0}));

   tmp = PAD("Mud name", name) +
         PAD("Driver", __VERSION__) +
         PAD("Architecture", __ARCH__) +
         PAD("Compiler", cc) +
         PAD("Driver uptime", pretty_time(uptime(), 4)) +
         PAD("Avg Cpu usage", sprintf("%4.2f%%",cpu)) +
         PAD("Load Average", query_load_average()) +
         PAD("No of users", sizeof(users())) +
         PAD("Using MCCP", ltype[1]) +
         PAD("Plain telnet", ltype[0]) +
         PAD("Named Livings", sizeof(named_livings())) + 
         PAD("Objects", num_obs) +
         PAD("Call Outs", sizeof(call_out_info()) + " pending.") +
         PAD("Memory usage", memory_info());

   tell_object(this_player(), tmp);
   return 1;
}


/*
   pretty_time() simul efun
   Converts seconds into "2 days, 3 seconds" for example
*/

varargs string pretty_time(int tim, int len) {
   int num;
   string *bits, tmp;

   if(!len)
     len = 99999;

   bits = ({ });
   if(tim >= 60*60*24 && sizeof(bits) < len)
      bits += ({ (num=tim/(60*60*24))+" day"+(num==1?"":"s") });
   if(tim >= 60*60 && tim%(60*60*24) && sizeof(bits) < len)
      bits += ({ (num=(tim/(60*60))%24)+" hour"+(num==1?"":"s") });
   if(tim >= 60 && tim%(60*60) && sizeof(bits) <len)
      bits += ({ (num=(tim/60)%60)+" minute"+(num==1?"":"s") });
   if(tim%60 && sizeof(bits) <len)
      bits += ({ (num=tim%60)+" second"+(num==1?"":"s") });

   for(len =0; len<sizeof(bits); len++)
      if(sscanf(bits[len],"%d %s", num, tmp) == 2 && num == 0)
         bits[len] = 0;

   return implode(bits, ", ");
}
