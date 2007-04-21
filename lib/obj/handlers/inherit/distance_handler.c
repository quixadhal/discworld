/**
 * This file handles the distance checking stuff for figuring out how
 * far apart two rooms or co-ordinates are.
 * @author Rodion
 * @started Wed Jul 25 15:22:24 PDT 2001
 */
#define BAD_ROOM -1
#define TRAVEL_NOT_ALLOWED -2
#define BAD_DOMAIN -3

int* query_co_ord(string room);

private int difference(int bing, int womble){
   if (womble > bing) {
      return womble - bing;
   }
   return bing - womble;
}

//Imprecise checking, for when coord data can't be found. We only allow certain
//domains now... And nothing outside /d/.
private int filename_check(string one, string two){
   string dom_one;
   string dom_two;
   string alpha;

   log_file("/log/PORTAL", "Unable to get co-ords for %O %O, doing filename based check.\n", one, two);
   dom_one = explode(one, "/")[1];
   dom_two = explode(two, "/")[1];
   
   //Alphabatize them so we only have half as many cases... This means if 
   //you're going from AM to Forn it will be the same as going from Forn to 
   //AM (am:forn)... as it should be.
   alpha = dom_one<dom_two?dom_one+":"+dom_two:dom_two+":"+dom_one;
   switch(alpha){
      case "am:am":
         return 3000;
      
      case "am:cwc":
         return TRAVEL_NOT_ALLOWED;
      
      case "am:ram":
         return 3000000;
      
      case "am:klatch":
         return 1000000;
      
      case "am:sur":
         return 500000;
      
      case "am:forn":
         return TRAVEL_NOT_ALLOWED;
      
      case "am:guilds":
         return 1000;
   
      case "cwc:ram":
      case "cwc:klatch":
      case "cwc:forn":
      case "cwc:guilds":
      case "cwc:sur":
         return TRAVEL_NOT_ALLOWED;
      
      case "cwc:cwc":
         return 500000;

      case "forn:forn":
         return TRAVEL_NOT_ALLOWED;
      
      case "forn:ram":
         return TRAVEL_NOT_ALLOWED;
      
      case "forn:guilds":
         return TRAVEL_NOT_ALLOWED;
      
      case "forn:klatch":
         return TRAVEL_NOT_ALLOWED;
      
      case "forn:sur":
         return TRAVEL_NOT_ALLOWED;
      
      case "guilds:guilds":
         return 1000;
      
      case "guilds:klatch":
         return 500000;
      
      case "guilds:sur":
         return 500000;
      
      case "guilds:ram":
         return 1500000;
      
      case "klatch:klatch":
         return 1000;
      
      case "klatch:sur":
         return 3300000;
      
      case "klatch:ram":
         return 4000000;
      
      case "ram:ram":
         return 500000;
      
      case "ram:sur":
         return 900000;
      
      case "sur:sur":
         return 500000;
   }
   return BAD_DOMAIN;
}

/**
 * This method figures out the distance between two co-ordinates.
 * @param co_ord_one the first co-ordinate
 * @param co_ord_two the second co-ordinate
 * @return the distance
 */
int query_distance_from_coords(int* co_ord_one, int* co_ord_two){
   float x;
   float y;
   float z;
   float distanceplane;
   float distance;

   x = difference(co_ord_one[0], co_ord_two[0]);
   y = difference(co_ord_one[1], co_ord_two[1]);
   z = difference(co_ord_one[2], co_ord_two[2]);
   distanceplane = sqrt(pow(x, 2) + pow(y, 2));
   distance = sqrt(pow(distanceplane, 2) + pow(z, 2));
   return to_int(distance);
}

/**
 * This method figures out the distance between two rooms and returns the
 * result.  The result will be in co-ordinate units.
 * @param one the first room
 * @param two the second room
 * @return the distance between them
 */
int query_distance_from_rooms(mixed one, mixed two){
   mixed co_ord_one;
   mixed co_ord_two;

   if (objectp(one)) {
      one = file_name(one);
   }
   if (objectp(two)) {
      two = file_name(two);
   }

   if (one->query_co_ord()) {
      co_ord_one = one->query_co_ord();
   } else {
      co_ord_one = query_co_ord(one);
   }
   if (two->query_co_ord()) {
      co_ord_two = two->query_co_ord();
   } else {
      co_ord_two = query_co_ord(two);
   }
   if(co_ord_one == 0 || co_ord_two == 0) {
      return filename_check(one, two);
   }

   return query_distance_from_coords(co_ord_one, co_ord_two);
}
