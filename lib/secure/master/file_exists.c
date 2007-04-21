/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: file_exists.c,v 1.1 1998/01/06 05:12:03 ceres Exp $
 * $Log: file_exists.c,v $
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
*/
int file_exists( string path ) {
  if ( file_size( path ) < 0 )
    return 0;
  return 1;
} /* file_exists() */
