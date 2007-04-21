/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: auto_mailer.c,v 1.5 2003/03/12 13:09:13 wyvyrn Exp $
 * $Log: auto_mailer.c,v $
 * Revision 1.5  2003/03/12 13:09:13  wyvyrn
 * Tabs to spaces
 *
 * Revision 1.4  2001/03/13 04:36:19  ceres
 * Fixed runtime if this_player() wasn't set.
 *
 * Revision 1.3  2001/03/01 10:07:42  taffyd
 * Added new flag for Ringo.. make to use test_player() in player_handler... and ran it through the new indenter just to try it out.
 *
 * Revision 1.2  2000/11/04 18:22:11  pinkfish
 * Remove the debug print.
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
*/
/**
 * Allows objects to send mail as if they were from the given player.
 */
#include <mail.h>
#include <player_handler.h>

/**
 * Allows people to send mail automaticly from the player in question.
 * Thus allows mail to be sent on someone being promoted to creator
 * and stuff.
 * @param to who the mail is to
 * @param from who the mail is from
 * @param cc the people to carbon copy
 * @param body the body of the message
 * @param send_inter should be send it ove the inter mud system?
 * @param only_to ignore the cc fields
 * @return the same value as do_mail_message does, 0 on failure
 */
int auto_mail(string to,
              string from,
              string sub,
              string cc,
              string body,
              int send_inter,
              string only_to,
              int flag)
{
  if (PLAYER_HANDLER->test_player(from) &&
      this_player() &&
      from != this_player()->query_name()) {
    write("AUTO_MAILER: Illegal access!\n");
    unguarded((: write_file, "/log/CHEAT",
           "Illegal access to AUTO_MAILER.\n" "Backtrace: " +
           back_trace() :));
    return 0;
  }
  return MAILER->do_mail_message(to, from, sub, cc, body,
                 send_inter, only_to, flag);
}                               /* auto_mail() */
