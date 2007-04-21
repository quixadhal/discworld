/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: valid_bind.c,v 1.4 2002/01/22 22:39:23 wodan Exp $
 * $Log: valid_bind.c,v $
 * Revision 1.4  2002/01/22 22:39:23  wodan
 * small optimisation
 *
 * Revision 1.3  2000/02/18 17:15:54  wodan
 * security fix
 *
 * Revision 1.2  1998/10/02 04:02:59  pinkfish
 * Updated to work with the new virtual objects.
 *
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
 */

private nosave object _simul_efun;

int valid_bind(object binder, object old_owner, object new_owner) {
   string *womble;
   string fname;

   if(!objectp(_simul_efun)) {
     _simul_efun = find_object("/secure/simul_efun");
   }
     
   if(binder == _simul_efun) {
      return 1;
   }

   if(file_name(new_owner)[0..7] == "/secure/") {
      return 0;
   }

   if(interactive(new_owner)) {
      return 0;
   }
   
   womble = explode(file_name(old_owner), "/");
   if (sizeof(womble) < 1) {
      return 0;
   }
   fname = womble[<1];
   if (fname[0] != '.') {
      return 0;
   }
   return old_owner == binder;
} /* valid_bind() */
