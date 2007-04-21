/**
 * Find Member
 * This function finds all the occurences of target in array
 * It returns an array of the positions
 * @author Terano
 */
int *find_member( mixed target, mixed *array ) {

int *locs = ({ });
int start = 0;

  while ( start != -1 ) {
    start = member_array( target, array, start );
    if ( start != -1 ) {
      locs += ({ start });
      start ++;
    }
  }

  return locs;
}   
  