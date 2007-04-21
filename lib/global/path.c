/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: path.c,v 1.6 2001/08/25 01:51:06 ceres Exp $
 * $Log: path.c,v $
 * Revision 1.6  2001/08/25 01:51:06  ceres
 * Fixed warnings in /global/lord
 *
 * Revision 1.5  1999/09/24 23:59:03  turrican
 * Fixed handling of .. in path components
 *
 * Revision 1.4  1999/09/23 02:02:08  pinkfish
 * fixed up some stuff.
 *
 * Revision 1.3  1999/08/30 01:37:34  pinkfish
 * Add in some stuff to control the pahts a bit nicer.
 *
 * Revision 1.2  1999/08/30 01:18:23  pinkfish
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
*/
private string home_dir;
private nosave string current_path;

/**
 * This method expands the specified path to get a nice new path
 * based on the home directory or expanding the ~ stuff.
 * @param str the path to expand
 * @return the expanded path
 */                        
string get_path(string str) {
   string *array;
   string *array1;
   string temp;
   int i;
   
   if (!str) {
      if(!home_dir) {
         home_dir = "/w/" + this_player()->query_name();
      }
      str = home_dir;
   }
   if (str == "~") {
      str = "w/"+this_player()->query_name();
   } else {
      if(str[0] == '~') {
         if(str[1] == '/') {
            sscanf(str, "~%s", temp);
            str = "/w/" + this_player()->query_name() + temp;
         } else {
            string name;
            if (sscanf(str,"~%s/%s",name,str)!=2) {
               name = extract(str,1);
               str = "w/"+name;
            } else {
               /* 
                * cheat at this point and just assume they are a
                * creator. sigh i kno i know 
                */
               str = "w/"+name+"/"+str;
            }
         }
      } else {
         if (str[0] != '/') {
            str = current_path+"/"+str+"/";
         }
      }
      if (str == "/") {
         return "/"; 
      } else {
         array = explode(str,"/") - ({ "" }) ;
         array1 = ({ });
      }
      for (i = 0; i < sizeof(array); i++) {
         reset_eval_cost();
         if (array[i] == "..") {
           if (sizeof(array1)) {
             array1 = array1[0..<2];
           }
         } else if (array[i] != ".") {
           array1 += ({ array[i] });
         }
      }
      if (sizeof(array1)) {
         str = implode(array1, "/");
      } else {
         str = "";
      }
   }
   return "/"+str;
} /* get_path() */

/**
 * This method expands the wildcard based file name and turns it
 * into a list of files
 * @param str the wildcard based file name
 * @return the array of file names
 */
string *get_files( string str ) {
   int loop;
   int count;
   string *filenames;
   string rmpath;
   string temp1;
   string temp2;
   string *names;
   string *files;
   
   names = explode(str, " ")- ({ "" });
   if(sizeof(names) == 0) {
      return ({ });
   }
   filenames = ({ });
   for(count = 0; count < sizeof(names); count++) {
      str = names[count];
      str = rmpath = get_path(str);
      if(rmpath == "/") {
         filenames += ({ "/" });
         continue;
      }
      if(sscanf(rmpath, "%s/%s", temp1, temp2) == 2) {
         string *path_elements;
         
         path_elements = explode(rmpath, "/") - ({ "" });
         rmpath = implode(path_elements[0..sizeof(path_elements) - 2], "/");
      }
      files = get_dir(str);
      
      if(files) {
         int loop2;
         for(loop2 = 0; loop2 < sizeof(files); loop2++) {
            filenames += ({ rmpath + "/" + files[loop2] });
         }
      }
   }
   for(loop = 0; loop < sizeof(filenames); loop++) {
      if(filenames[loop][0] != '/') {
         filenames[loop] = "/" + filenames[loop];
      }
   }
   return filenames;
} /* get_files() */

/* Changed to be *.* so as to handle virtual wombles as well */
/**
 * This method expands the wildcard based file name and turns it
 * into a list of c files.  This automaticaly adds a .c to any
 * files.
 * @param str the wildcard based file name
 * @return the array of file names
 */
string *get_cfiles(string str) {
   int loop;
   string temp, *names, temp2;
   
   names = explode(str, " ") - ({ "" });
   for(loop = 0; loop < sizeof(names); loop++) {
      if(sscanf(names[loop], "%s.%s", temp, temp2) != 2) {
         names[loop] += ".c";
      }
   }
   str = implode(names, " ");
   return get_files(str);
} /* get_cfiles() */

/**
 * This method returns the current path.
 * @return the current path
 */
string query_current_path() { 
   return current_path; 
} /* query_current_path() */

/**
 * This method returns the home directory of the creator.
 * @return the home directory
 */
string query_home_dir() {
   return home_dir;
} /* query_home_dir() */

/**
 * This method sets the current home directory of the creator.
 * @param dir the new home directory
 */
protected int set_home_dir(string dir) {
   home_dir = dir;
   return 1;
} /* set_home_dir() */

/**
 * This method sets the current path for the creator.
 * @param str the current path
 */
void set_current_path(string str) {
   current_path = str;
} /* set_current_path() */
