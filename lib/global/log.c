/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: log.c,v 1.1 1998/01/06 04:54:05 ceres Exp $
 * $Log: log.c,v $
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
*/
private nosave mapping last_error;

void logging_commands() {
}

void set_last_error(mapping err) {
  last_error = err;
}

mapping get_last_error() {
  return (mapp(last_error)?last_error:0);
}
