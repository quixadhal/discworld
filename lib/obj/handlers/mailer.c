/*  -*- LPC -*-  */
/*
 * $Id: mailer.c,v 1.27 2003/03/19 20:44:14 ceres Exp taffyd $
 */

/**
 * The mailer handler object.  This is used to send mail and do oyther
 * stuff related to mail.
 *  The new mailer object ! *shiver*
 *  Thanks to Wodan and Pinkfish for ideas and help.
 *  By Turrican@Discworld, May 1995.
 * @author Turrican
 * @started May 1995
 *
 * @change 1-10-96, Turrican
 *  Changed the way Ccs and including yourself get done
 * @change 17-09-97, Olorin
 *  Swapped 'r' and 'R' to make don't sent to CC's the default use
 */

#include <mime.h>
#include <mail.h>
#include <localtime.h>
#include <player_handler.h>

#define FOLDER_H "/obj/handlers/folder_handler.c"
#define MAIL_PATH "/save/mail/"
#define COLS (int)owner->query_cols()
#define ROWS (int)owner->query_rows()
#undef CONVERTER

nosave class mail_header *folder;
nosave int *deleted, *newish, no_menu, last_read, full_header;
nosave string current, to, subject, cc, mailrc;
nosave mixed *do_this_last;
nosave object owner;
nosave string *ignore = ({
  "email-version",
  "x-lines",
  "expires",
  "transport-options",
  "x-mailer",
  "errors-to",
  "lines",
  "priority",
  "newsgroups",
  "default-options",
  "auto-forward-count",
  "ua-message-id",
  "approved",
  "followup-to",
  "message-version",
  "message-id",
  "message-service",
  "message-type",
  "sender",
  "end-of-header",
  "content-type",
  "path",
  "report-version",
  "submitted-by",
  "message-protocol",
  "posting-number",
  "x-postmark",
  "apparently-to",
  "organization",
  "posted-date",
  "return-receipt-to",
  "keywords",
  "x-sun-charset",
  "summary",
  "in-reply-to",
  "reply-to",
  "distribution",
  "sent-by",
  "company",
  "references",
  "mts-message-id",
  "x-sequence",
  "via",
  "content-charset",
  "status",
  "confirmed-by",
  "ua-content-id",
  "content-length",
  "mime-version",
  "auto-forwarded-from",
  "content-transfer-encoding",
  "received",
  "end-of-protocol",
  "x-uidl",
  "precedence",
  "return-path"
});
string *folder_names;

/* PROTOTYPES */

varargs protected void rm_message(string input);
protected void unrm_message(string input);
private void print_message(int number);
varargs private void reply_message(int input, int flag);
private void save_me();
private int load_me();
void dest_me();
private void read_messages(string fname);
protected void read_loop(string input);
private void prompt();
protected void change_folder(string input);
varargs private void delete_it(string bonk, int last, string newish_folder);
varargs protected void move_message(string input, int flag);
private void save_message(string input);
protected void get_recipient(string input);
varargs protected void get_subject(string input, int number, int flag);
varargs protected void main_menu(string fname, int flag, int *range);
protected void forward_message(string input);
void finish_write_message(string input);
string format_date(int x);
private int *expand_range(string str);
private int valid_name(string str);
void read_mail(string str, string sub);
private void forward_email(int number);
private void write_message();
private void check_external_mail();

/* BEGIN */

private void create() {
    mapping aliases;

    seteuid("Mailer");
    no_menu = full_header = 0;
    last_read = -1;
    newish = ({ });
    folder = ({ });
    folder_names = ({ "inbox" });
    deleted = ({ });
    current = "";
    if (clonep()) {
        owner = this_player();
        if (owner) {
            aliases = owner->query_aliases();
            if (aliases) {
                mailrc = aliases[".mailrc"];
            }
        }
    } else {
        check_external_mail();
    }
} /* create() */

#define HEADER_NAME 1
#define HEADER_VAL  2

private string folder_filename(string name) {
    return MAIL_PATH+name[0..0] + "/"  + name;
}

private string strip_header(string message) {
    mixed *ra;
    int i;
    string header;

    if ((i = strsrch(message, "\n\n")) == -1) {
        return message;
    }
    header = message[0..i];
    message = message[i+1..];
    ra = reg_assoc(header,
      ({ "^[!-9;-~]+:", "((\n?[ \t])+[^\n]*(\n|$))+" }),
      ({ HEADER_NAME, HEADER_VAL }));
    for (i = 1; i + 2 < sizeof(ra[0]); i += 2) {
        if (ra[1][i] == HEADER_NAME && ra[1][i+2] == HEADER_VAL) {
            if (member_array(replace_string(lower_case(ra[0][i]), ":", ""),
                ignore) != -1) {
                ra[0][i] = ra[0][i+2] = "";
            }
        }
    }
    return implode(ra[0], "") + message;
}

private void check_external_mail() {
    string *dir, fname, mess, t, ccs;
    class mail_message msg = new(class mail_message);
    class mime_header hdr;

    dir = unguarded((: get_dir, EXTERNAL_MAIL_PATH :));
    if (!dir) {
        call_out((: check_external_mail :), 60);
        return;
    }
    foreach (fname in dir) {
        mess = unguarded((: read_file, EXTERNAL_MAIL_PATH + fname :));
        mess = replace(mess, ({ "\r\n", "\n", "\t", "        " }));
        hdr = MIME->parse_headers(mess);
        if (!hdr) {
            unguarded((: rm, EXTERNAL_MAIL_PATH + fname :));
            continue;
        }
        msg->from = hdr->header_m["from"];
        msg->subject = hdr->header_m["subject"];
        t = hdr->header_m["to"];
        if (!t) {
            continue;
        }
        msg->to = explode(t, ",");
        ccs = hdr->header_m["cc"];
        if (!ccs) {
            ccs = "";
        }
        msg->cc = explode(ccs, ",");
        msg->body = mess;
        FOLDER_H->add_it(msg, 1);
        unguarded((: rm, EXTERNAL_MAIL_PATH + fname :));
    }
    call_out((: check_external_mail :), 60);
} /* check_external_mail() */

/**
 * This method sets the call back fuinction to use when
 * the mailer has finished. ({ ob, func })
 * @param bing the call back function
 */
void set_do_this_last(mixed *bing) { do_this_last = bing; }
/**
 * This method returns the call back fuinction to use when
 * the mailer has finished. ({ ob, func })
 * @return the call back function
 */
mixed *query_do_this_last() { return do_this_last; }

/* For backwards compatibility. */
/**
 * This method allows a message to be mailed.  It checks the previous
 * object to make sure it is one of the allowed set to
 * do mailing.
 * @param t who it is to
 * @param from who it is from
 * @param sub the subject of the message
 * @param ccs the carbon copy recipients
 * @param body the body of the message
 * @param only_to only mail to the to address
 * @param flag prevent this_player() from getting the messages if flag != 0
 * @example
 * MAIL_HANDLER->do_mail_message("pinkfish", "gumboot, killer tomato":,
 *                               "About the tomatoes", "",
 *                   "The grass ius greener yesterday,.\nYours\nGumboot.");
 */
varargs int do_mail_message(string t, string from, string sub, string ccs,
  string body, int, string only_to, int flag) {
    string *cc_e, *goto;
    class mail_message msg;

    if(file_name(previous_object())[0..12] != "/secure/login" &&
       file_name(previous_object())[0..13] != "/secure/nlogin" &&
       file_name(previous_object())[0..13] != "/obj/handlers/" &&
       file_name(previous_object())[0..4] != "/www/" &&
       file_name(previous_object())[0..11] != "/net/daemon/" &&
       file_name(previous_object())[0..11] != "/global/lord" &&
       file_name(previous_object())[0..14] != "/global/creator" &&
       file_name(previous_object())[0..18] != "/global/auto_mailer" &&
       file_name(previous_object())[0..12] != "/cmds/creator" &&
       file_name(previous_object())[0..9] != "/cmds/lord" &&
       file_name(previous_object()) != "/d/am/buildings/post/parcel" &&
       file_name(previous_object()) != "/d/am/buildings/apex/admin_office" &&
       file_name(previous_object()) != "/d/am/bookkeepers/weichert_office" &&
       file_name(previous_object()) != "/d/ram/ohulan/market/post_office" &&
       file_name(previous_object()) != "/d/ram/interview" &&
       file_name(previous_object()) != "/d/forn/utils/interview" &&
       file_name(previous_object()) != "/d/am/buildings/flintwick/lawyer_office" &&
       file_name(previous_object()) != "/d/am/buildings/council/court" &&
       file_name(previous_object()) != "/d/klatch/djel/city/palace/council_court" &&
       file_name(previous_object()) != "/d/guilds/error_tracker" &&
       file_name(previous_object())[0..25] != "/d/ram/ohulan/market/post2") { 
        printf("MAILER: illegal access (%O).\n", file_name(previous_object()));
        return 0;
    }
    if ((file_name(previous_object())[0..11] == "/global/lord" ||
        file_name(previous_object())[0..14] == "/global/creator") &&
        (lower_case(from) != (string)this_player()->query_name())) {
        printf("MAILER: illegal acces.\n");
        return 0;
    }
    if (!ccs) {
        ccs = "";
    }
    cc_e = explode(ccs, ",")-({""});
    goto = explode(t, ",")-({""});
    if (only_to) {
        goto = explode(only_to, ",")-({""});
    }
    msg = new(class mail_message);
    msg->to = goto;
    msg->cc = cc_e;
    msg->body = "From " + from + " " + ctime(time()) +
    "\nDate: " + format_date(time()) +
    "\nFrom: " + from +
    "\nTo: " + t +
    "\nSubject: " + sub +
    "\n" + (sizeof(cc_e)?"Cc: "+ ccs+ "\n":"")+
    "\n" + body;
    msg->from = lower_case(from);
    msg->subject = sub;
    FOLDER_H->add_it(msg, flag);
    return 1;
} /* do_mail_message() */

/**
 * This method returns the mail information which is placed into the
 * the finger command.
 * @param pname the name of the player
 * @return the function mail string
 */
string finger_mail(string pname) {
    return FOLDER_H->finger_mail(lower_case(pname));
} /* finger_mail() */

/**
 * This method returns a string saying if the player has new mail or not.
 * This is what is used when the player first logs on.
 * @param pname the name of the player
 * @return the new mail string
 * @example
 * str = MAIL_HANDLER->new_mail(this_player()->query_name());
 */
string new_mail(string pname) {
    return FOLDER_H->check_mail(lower_case(pname));
} /* new_mail() */

/**
 * This method prints the prompt which is used in the main mail loop.
 */
private void prompt() {
    printf("\nCommand (h for main menu): ");
} /* prompt() */

/**
 * This method is the main entry point to the mailer.  It is
 * what is called to start up the system when a mailer is used.
 * @example
 * mailer = clone_object(MAIL_HANDLER);
 * mailer->read_mail();
 */
void read_mail(string str, string sub) {
    if (this_player()->query_property("guest")) {
        write("Sorry, mailer access is not allowed for guests.\n");
        if (do_this_last && objectp(do_this_last[0])) {
            call_other(do_this_last[0], do_this_last[1], do_this_last[2]);
        } else {
            dest_me();
        }
        return;
    }

    if (!load_me()) {
        if (do_this_last && objectp(do_this_last[0])) {
            call_other(do_this_last[0], do_this_last[1], do_this_last[2]);
        } else {
            dest_me();
        }
        return;
    }
    if (str) {
        no_menu = 1;
        if (!sub) {
            get_recipient(str);
        } else {
            to = str;
            get_subject(sub);
        }
        return;
    }
    MAIL_TRACK->add_mailer(this_object(), owner->query_name()); 
    main_menu("inbox");
    return;
} /* read_mail() */

/** @ignore yes */
varargs private void main_menu(string fname, int flag, int *range) {
    int i, size, offs, cols, fromcols, statcols;

    cols = COLS;
    printf("%|=*s", cols, "" + mud_name() + " mailer system version 2.0\n\n");
    if (!strlen(fname)) {
        fname = "inbox";
    }
    current = fname;
    if (!flag) {
        read_messages(current);
        last_read = -1;
        deleted = ({ });
    }
    if (!range) {
        range = ({ });
    }
    newish = ({ });
    size = sizeof(folder);
    for (i = 0; i < size; i++) {
        if (folder[i]->status == "N") {
            newish += ({ i });
        }
    }
    if (sizeof(newish) && newish[0] != 0 && last_read == -1) {
        last_read = newish[0]-1;
    }
    if (size && !sizeof(newish) && last_read == -1) {
        last_read = size-1;
    }
    printf(owner->fix_string(sprintf("%s%|=*s%s","%^CYAN%^",cols,
          "Folder "+current+" with "+
          (size?""+size:"no")+
          " message"+(size == 1?"":"s")+".\n\n",
          "%^RESET%^")));
    if (!sizeof(range)) {
        i = size - ROWS + 11;
        if (i < 0) {
            i = 0;
        }
        range = expand_range(sprintf("%d-%d", i, size));
    }
    size = sizeof(range);
    if (size) {
        offs = range[0]-1;
    } else {
        offs = 0;
    }
    statcols = cols / 3;
    fromcols = 2 * cols / 3 - 8;
    for (i = 0; i<size; i++) {
        printf(owner->
          fix_string(sprintf("%s%-6s %-*.*s %-*.*s%s\n",
              (last_read == i+offs?
                "%^REVERSE%^":""), 
              (last_read == i+offs?">":" ")+
              folder[i+offs]->status+
              " " + (i+offs+1), statcols, statcols, "From: " +
              folder[i+offs]->from, fromcols, fromcols, 
              "Subject: "+
              replace_string(terminal_colour(folder[i+offs]->subject,
                  ([ ])), "%", "%%"),
              "%^RESET%^")));
    }
    printf("%|=*s", cols, "\n  You can use any of the following commands by "
      "entering the first character;\nd)elete or u)ndelete mail,  m)ail a "
      "message,  r)eply or f)orward mail,  q)uit,\n"
      "> = move messages, c)hange folder, i)ndex of folders, ? = help\n"
      "To read next unread message, press <return>.\n");
    printf("Command: ");
    input_to("read_loop");
} /* main_menu() */

/** @ignore yes */
protected void read_loop(string input) {
    int i, num;
    string s1, s2, comm, *tmp;

    /*
     * ok this should (theoreticaly) do some clever things and get the
     * message number being reffered to out of the junk.  But then again,
     * maybe not.
     */
    num = 0;
    i = 0;
    if (sscanf(input, "%d%s", num, input) != 2) {
        comm = "";
        if (sscanf(input, "%s %d %s", s1, num, s2) == 3) {
            i = 1;
            input = s1 + " " + s2;
        }
    }
    if (sscanf(input, "%s %s", comm, input) != 2) {
        comm = input;
        input = "";
    }
    if (i)
        input = ""+num+" "+input;
    switch(comm) {
    case "q" :  if (current == "inbox") {
            if (sizeof(folder) && 
              (sizeof(folder) != sizeof(newish)) && 
              (sizeof(deleted) != sizeof(folder))) {
                printf( "\nMove read message(s) to \"received\" folder? "
                  "(y/[n]): ");
                input_to("get_yesno");
                break; 
            }
        }
        delete_it(current, 1);
        break;
    case "d" :
        if (input != "") {
            rm_message(input);
            break;
        }
        printf( "\nDelete which messages (number or range): " );
        input_to("rm_message");
        break; 
    case "u" :
        if (input != "") {
            unrm_message(input);
            break;
        }
        printf( "\nUndelete which messages (number or range): " );
        input_to("unrm_message");
        break;
    case "c" :
        if (input != "") {
            change_folder(input);
            break;
        }
        printf( "\nChange to which folder (= for inbox): " );
        input_to("change_folder");
        break;
    case "m" :
        if (input != "") {
            get_recipient(input);
            break;
        } 
        printf( "\nRecipient: " );
        input_to("get_recipient");
        break;
    case "R" :
        reply_message(num);
        break;
    case "r" :
        reply_message(num, 1);
        break;
    case "$" :
        main_menu(current);
        break;
    case "f" :
        if (input != "") {
            forward_message(input);
            break;
        }
        printf("\nForward a message.  Ranges are not supported.\n"
          "Enter the number of the message and the name(s) of the "
          "recipient(s), separated by a comma: ");
        input_to("forward_message");
        break;
    case "F" :
        forward_email(num);
        break;
    case "i" :
        printf("\nHere's the index of your folders:\n");
        printf("\n%-#*s\n", COLS, implode(folder_names, "\n")); 
        prompt();
        input_to("read_loop");
        break;
    case "L" :
        if (full_header) {
            printf("\nOkay, now showing abbreviated header.\n");
            full_header = 0;
        } else {
            printf("\nOkay, now showing full header.\n");
            full_header = 1;
        }
        prompt();
        input_to("read_loop");
        break;
    case "l" :
        if (input != "") {
            if (!MAIL_TRACK->query_list(input)) {
                printf( "\nSorry, list "+input+" does not exist.\n" );
                prompt();
                input_to("read_loop");
                break;
            }
            printf( "\nMembers of list "+ input +":\n" );
            tmp = MAIL_TRACK->query_members(input);
        } else {
            printf( "\nCurrently available mailing lists:\n" );
            tmp = MAIL_TRACK->query_mailing_lists();
        }
        printf("\n%-#*s\n", COLS, implode(tmp,"\n"));
        prompt();
        input_to("read_loop");
        break;
    case ">" :
        if (!PLAYER_HANDLER->test_user(owner->query_name())) {
            printf( "You can't do that as a guest.\n" );
            break;
        }
        if (input != "") {
            move_message(input);
            break;
        }
        printf("\nMove message to folder.\nEnter the number "
          "(or range) of the message and the name of the folder, "
          "separated by a comma: ");
        input_to("move_message");
        break;
    case "h" :  
        if (input != "") {
            main_menu(current, 1, expand_range(input));
            break;
        }
        main_menu(current, 1);
        break;
    case "s" :
        save_message(input);
        break;
    case "?" :
        owner->set_finish_func("finish_print");
        comm = read_file("/doc/helpdir/mailer");
        owner->more_string(comm);
        break;
    case "" :
        if (num) {
            print_message(num-1);
            last_read = num-1;
            break;
        }
        if (newish == ({ })) {
            printf("\nNo more new messages in folder.\n");
            prompt();
            input_to("read_loop");
            break;
        }
        last_read = newish[0];
        print_message(newish[0]);
        break;
    default :
        printf("\nUnknown command.  Use \"?\" for help.\n");
        prompt();
        input_to("read_loop");
        break; 
    }
} /* read_loop() */

/** @ignore yes */
protected void get_yesno(string input) {
    int bing, i;

    if (!input || (input == "")) {
        printf("No.\n");
        delete_it(current, 1);
        return;
    }
    if (lower_case(input) == "n") {
        delete_it(current, 1);
        return;
    }
    if (lower_case(input) == "y") {
        bing = sizeof(folder);
        for (i = 0; i < bing; i++) {
            if ((member_array(i, newish) == -1) && 
              (member_array(folder[i]->number, deleted) == -1)) {
                move_message(""+(i+1)+" received", 1); 
            }
        }
    }
    delete_it(current, 1);
    return;
} /* get_yesno() */

private void forward_email(int number) {
    int i;
    string email, body;

    if (!number) {
        if (!(i = sizeof(folder))) {
            printf("No messages in folder.\n");
            prompt();
            input_to("read_loop");
            return;
        }
        if (newish != ({ })) {
            number = newish[0];
            if (number == 0) {
                printf("All your messages are still unread: aborting.\n");
                prompt();
                input_to("read_loop");
                return;
            }
        }
        else if (last_read > -1)
            number = last_read+1;
        else 
            number = i;
    }
    if (sizeof(folder) < number) {
        printf("Oh dear.  No message with that number.\n");
        prompt();
        input_to("read_loop");
        return;
    }
    number--;
    if (!(email = PLAYER_HANDLER->test_email(owner->query_name()))) {
        printf("Sorry, your E-mail address is not set.  Use chfn or email "
          "to set it.\n");
        prompt();
        input_to("read_loop");
        return;
    }
    if (email[0] == ':') {
        email = email[1..];
    }
    body = FOLDER_H->load_message(owner->query_name(), current,
      folder[number]->number);
    if (body) {
        SMTP->eventSendMail(email, owner->query_name(), body);
        printf("Message #%d forwarded to your E-mail address.\n", number + 1);
    }
    prompt();
    input_to("read_loop");
} /* forward_mail() */

private string rewrite_local(string rcpt) {
    string tmpr;

    if ((tmpr = FOLDER_H->check_local(rcpt))) {
        return tmpr;
    }
    return rcpt;
} /* rewrite_local() */

varargs private void reply_message(int number, int flag) {
    int i;
    string body;
    class mime_header hdr;

    if (!number) {
        if (!(i = sizeof(folder))) {
            printf("No messages in folder.\n");
            prompt();
            input_to("read_loop");
            return;
        }
        if (newish != ({ })) {
            number = newish[0];
            if (number == 0) {
                printf("All your messages are still unread: aborting.\n");
                prompt();
                input_to("read_loop");
                return;
            }
        }
        else if (last_read > -1)
            number = last_read+1;
        else 
            number = i;
    }
    if (sizeof(folder) < number) {
        printf("Oh dear.  No message with that number.\n");
        prompt();
        input_to("read_loop");
        return;
    }
    number--;
    to = folder[number]->from;
    body = FOLDER_H->load_message(owner->query_name(), current,
      folder[number]->number);
    if (!body) {
        prompt();
        input_to("read_loop");
        return;
    }
    hdr = MIME->parse_headers(body);
    if (!hdr) {
        prompt();
        input_to("read_loop");
        return;
    }
    if (hdr->header_m["reply-to"]) {
        to = hdr->header_m["reply-to"];
    }
    if (!flag) {
        string tmp;

        cc = hdr->header_m["cc"];
        tmp = hdr->header_m["to"];
        if (tmp) {
            if (cc) {
                cc += "," + tmp;
            } else {
                cc = tmp;
            }
        }
        if (cc == "") {
            cc = 0;
        }
        if (cc) {
            cc = implode(map(MIME->get_email_addrs(cc)[0],
                (: rewrite_local($1) :))-({ owner->query_name(), to }),
              ",");
        }
    }
    printf("Include original message? (y/[n]/q) ");
    input_to("finish_reply_message", 0, number);
} /* reply_message() */

/** @ignore yes */
protected void finish_reply_message(string input, int number) {
    int flag, i;
    string s1;

    if (!input || input == "" || lower_case(input)[0] != 'y') {
        if (lower_case(input)[0] == 'q') {
            cc = 0;
            to = "";
            printf("Aborting.\n");
            prompt();
            input_to("read_loop");
            return;
        }
        printf( "No.\n" );
    } else flag = 1;
    subject = folder[number]->subject;
    if (sscanf(subject, "Re:#%d %s", i, s1) != 2)
        subject = "Re:#1 " + subject;
    else 
        subject = sprintf("Re:#%d %s", (i+1), s1);
    printf("Press return for a subject of \"%s\"\nSubject: ", subject);
    if (flag)
        input_to("get_subject", 0, ++number, 1);
    else
        input_to("get_subject");
} /* finish_reply_message() */

/** @ignore yes */
protected void change_folder(string input) {
    if (!input || input == "") {
        prompt();
        input_to("read_loop");
        return;
    }
    if (input == "=") {
        delete_it(current, 0, "inbox");
        return;
    }
    if (member_array(input, folder_names) == -1) {
        printf("No folder named %s.\n", input);
        prompt();
        input_to("read_loop");
        return;
    }
    delete_it(current, 0, input);
} /* change_folder() */

/** @ignore yes */
protected void get_recipient(string input) {
    string *str, *full, nn;
    int i;
    mixed *addrs;

    if (!input || input == "") {
        printf("No recipient given: aborting.\n");
        prompt();
        input_to("read_loop");
        return;
    }
    addrs = MIME->get_email_addrs(input);
    str = addrs[0];
    full = addrs[1];
    for (i=0;i<sizeof(str);i++){
        nn = (string)owner->expand_nickname(str[i]);
        if (str[i] != nn) {
            str[i] = full[i] = nn;
        }
        if (!valid_name(str[i])) {
            printf("%s is not a valid recipient.\n", str[i]);
            str = delete(str, i, 1);
            full = delete(full, i--, 1); 
        }
    }
    if (sizeof(full))
        to = implode(full, ","); 
    else {
        if (no_menu) {
            delete_it("inbox", 1);
            return;
        }
        prompt();
        input_to("read_loop");
        return;
    }
    subject = "";
    printf("Subject: ");
    input_to("get_subject");
} /* get_recipient() */

/** @ignore yes */
varargs protected void get_subject(string input, int number, int flag) {
    if ((!input || input == "") && (!subject || subject == "")) {
        printf("No subject given: aborting.\n");
        if (no_menu) {
            delete_it("inbox", 1);
            return;
        }
        prompt();
        input_to("read_loop");
        return;
    }
    if (input && input != "")
        subject = input;
    printf("Cc: ");
    input_to("get_cc", 0 , number, flag);
} /* get_subject() */

private int valid_name(string str) {
    string mud;

    str = lower_case(str);
    if (!sscanf(str, "%s@%s", str, mud) || 
      lower_case(mud) == lower_case(mud_name())) {
        /* local...  */
        return (int)PLAYER_HANDLER->test_user(str) ||
        (int)MAIL_TRACK->query_list(str);
    }
    return 1;
} /* valid_name() */  

/** @ignore yes */
varargs protected void get_cc(string input, int number, int flag) {
    string *str, body, *full, nn;
    int i;
    mixed *addrs;

    if (!input || input == "**" || input == "") {
        if (number) {
            body = FOLDER_H->load_message(owner->query_name(), current, 
              folder[number-1]->number);
            if (!body) {
                prompt();
                input_to("read_loop");
                return;
            }
            if (!flag) {
                finish_write_message("> " + replace_string(body, "\n", "\n> ") + "\n");
                return;
            }
            owner->do_edit("> " + replace_string(body, "\n", "\n> ") + "\n", 
              "finish_write_message");
            return;
        }
        write_message();
        return;
    }
    addrs = MIME->get_email_addrs(input);
    str = addrs[0];
    full = addrs[1];
    for (i = 0; i < sizeof(str); i++) {
        nn = (string)owner->expand_nickname(str[i]);
        if (str[i] != nn) {
            str[i] = full[i] = nn;
        }
        if (!valid_name(str[i])) {
            printf("%s is not a valid recipient.\n", str[i]);
            str = delete(str, i, 1);
            full = delete(full, i--, 1);
        }
    }
    if (cc)
        cc += "," + implode(full, ",");
    else
        cc = implode(full, ",");
    printf("Cc: ");
    input_to("get_cc", 0, number, flag);
}  /* get_cc() */

private void write_message() {
    owner->do_edit(0, "finish_write_message");
} /* write_message() */

/** @ignore yes */
void finish_write_message(string input) {
    if (!input || input == "") {
        if (no_menu) {
            delete_it("inbox", 1);
            return;
        }
        prompt();
        input_to("read_loop");
        return;
    }
    printf("Cc: ");
    input_to("get_cc_after", 0, input);
} /* finish_write_message() */

/** @ignore yes */
protected void get_cc_after(string input, string body) {
    mixed *goto, cc_e, *addrs;
    class mail_message msg;
    string *str, *full, nn;
    int i;

    if (!input || input == "**" || input == "") {
        body += owner->append_signature();
        FOLDER_H->mark_read(owner->query_name(), current, newish);
        goto = explode(to, ",") - ({ "" });
        if (cc && cc != "") {
            cc_e = explode(cc, ",") - ({ "" });
        } else {
            cc_e = ({ });
        }
        msg = new(class mail_message);
        msg->to = goto;
        msg->cc = cc_e;
        msg->body = "From " + owner->query_name() + " " + ctime(time()) +
        "\nDate: " + format_date(time()) +
        "\nFrom: " + owner->query_name() +
        "\nTo: " + to +
        "\nSubject: " + subject +
        "\n" + (sizeof(cc_e)?"Cc: " + cc + "\n":"") +
        "\n" + body;
        msg->from = owner->query_name();
        msg->subject = subject;
        FOLDER_H->add_it(msg, 0);
        to = "";
        cc = 0;
        subject = "";
        printf("Message sent.\n");
        if (no_menu) {
            delete_it("inbox", 1);
            return;
        }
        prompt();
        input_to("read_loop");
        return;
    }
    addrs = MIME->get_email_addrs(input);
    str = addrs[0];
    full = addrs[1];
    for (i = 0; i < sizeof(str); i++) {
        nn = (string)owner->expand_nickname(str[i]);
        if (str[i] != nn) {
            str[i] = full[i] = nn;
        }
        if (!valid_name(str[i])) {
            printf("%s is not a valid recipient.\n", str[i]);
            str = delete(str, i, 1);
            full = delete(full, i--, 1);
        }
    }
    if (cc)
        cc += "," + implode(full, ",");
    else
        cc = implode(full, ",");
    printf("Cc: ");
    input_to("get_cc_after", 0, body);
} /* get_cc_after() */

varargs private void delete_it(string fname, int last, string newish_folder) {
    if (!fname)
        fname = current;
    if (deleted != ({ })) {
        printf("Delete message(s)? ([y]/n) ");
        input_to("finish_delete_it", 0, fname, last, newish_folder);
        return;
    }
    if (last) {
        FOLDER_H->mark_read(owner->query_name(), current, newish);
        save_me();
        if (do_this_last && objectp(do_this_last[0]))
            call_other(do_this_last[0], do_this_last[1], do_this_last[2]);
        else
            dest_me();
        return;
    }
    FOLDER_H->mark_read(owner->query_name(), current, newish);
    if (!newish_folder || newish_folder == "")
        main_menu(fname);
    else
        main_menu(newish_folder);
} /* delete_it() */

/** @ignore yes */
protected void finish_delete_it(string input, string fname, int last, 
  string newish_folder) {
    if (lower_case(input) == "n" || (input && input != "" && 
        lower_case(input) != "y")) {
        printf("Keeping message(s).\n");
        deleted = ({ });
        if (last) {
            FOLDER_H->mark_read(owner->query_name(), current, newish);
            save_me();
            if (do_this_last && objectp(do_this_last[0]))
                call_other(do_this_last[0], do_this_last[1], do_this_last[2]);
            else
                dest_me();
            return;
        }
        FOLDER_H->mark_read(owner->query_name(), current, newish);
        if (!newish_folder || newish_folder == "")
            main_menu(fname);
        else
            main_menu(newish_folder);
        return;
    }
    printf("Deleting message(s).\n");
    FOLDER_H->mark_read(owner->query_name(), current, newish);
    FOLDER_H->delete_it(owner->query_name(), fname, deleted);
    deleted = ({ });
    save_me();
    if (last) {
        if (do_this_last && objectp(do_this_last[0]))
            call_other(do_this_last[0], do_this_last[1], do_this_last[2]);
        else
            dest_me();
        return;
    }
    if (!newish_folder || newish_folder == "")
        main_menu(fname);
    else
        main_menu(newish_folder);
    return;
} /* finish_delete_it() */

/** @ignore yes */
varargs protected void rm_message(string input, int flag) {
    int i, tmp;
    int *range;

    if (!input || input == "") {
        prompt();
        input_to("read_loop");
        return;
    }
    range = expand_range(input);
    tmp = sizeof(range);
    for (i = 0; i < tmp; i++) {
        if (member_array(folder[range[i]-1]->number, 
            deleted) == -1) {
            newish -= ({ range[i]-1 });
            deleted += ({ folder[range[i]-1]->number });
            folder[range[i]-1]->status = "D";
            printf("Marked message %d as deleted.\n", range[i]);
        }
    }
    if (!flag) {
        prompt();
        input_to("read_loop");
    }
} /* rm_message() */

/** @ignore yes */
protected void unrm_message(string input) {
    int i, tmp;
    int *range;

    if (!input || input == "") {
        prompt();
        input_to("read_loop");
        return;
    }
    range = expand_range(input);
    tmp = sizeof(range);
    for (i = 0; i < tmp; i++) {
        if (member_array(folder[range[i]-1]->number, 
            deleted) > -1) {
            deleted -= ({ folder[range[i]-1]->number });
            folder[range[i]-1]->status = "U";
            printf("Undeleted message %d.\n", range[i]);
        }
    }
    prompt();
    input_to("read_loop");
} /* unrm_message() */

/** @ignore yes */
protected void forward_message(string input) {
    int number, i;
    string *str, *full, nn;
    mixed *addrs;

    if (!input || input == "") {
        prompt();
        input_to("read_loop");
        return;
    }
    if (sscanf(input, "%d %s", number, to) != 2) {
        printf("Wrong syntax (ranges are not supported with forward).\n");
        prompt();
        input_to("read_loop");
        return;
    } 
    if (number > sizeof(folder) || !number) {
        printf("Oh dear. No message with that number.\n");
        prompt();
        input_to("read_loop");
        return;
    }
    addrs = MIME->get_email_addrs(to);
    str = addrs[0];
    full = addrs[1];
    for (i = 0; i < sizeof(str); i++) {
        nn = (string)owner->expand_nickname(str[i]);
        if (str[i] != nn) {
            str[i] = full[i] = nn;
        }
        if (!valid_name(str[i])) {
            printf("%s is not a valid recipient.\n", str[i]);
            str = delete(str, i, 1);
            full = delete(full, i--, 1);
        }
    }
    if (sizeof(full)) {
        to = implode(full, ",");
    } else {
        prompt();
        input_to("read_loop");
        return;
    }
    cc = 0;
    printf("Edit outgoing message? (y/[n]) ");
    input_to("edit_it", 0, number);
} /* forward_message() */

/** @ignore yes */
protected void edit_it(string input, int number) {
    subject = folder[number-1]->subject + " (fwd)";
    if (!input || input == "")
        printf( "No.\n" );
    printf("Press return for a subject of \"%s\"\nSubject: ", subject);
    if (!strlen(input) || (lower_case(input) == "n"))
        input_to("get_subject", 0, number);
    else if (lower_case(input) == "y")
        input_to("get_subject", 0, number, 1);
    return;
} /* edit_it() */

/** @ignore yes */
varargs protected void move_message(string input, int flag) {
    int *range, i, bing;
    string to_folder, number;

    if (!input || input == "") {
        prompt();
        input_to("read_loop");
        return;
    }
    if (!sscanf(input, "%s %s", number, to_folder)) {
        printf("Wrong syntax.\n");
        prompt();
        input_to("read_loop");
        return;
    }
    if (to_folder == current) {
        printf("Destination folder is the same as source folder: not moved.\n");
        prompt();
        input_to("read_loop");
        return;
    }
    if (to_folder == "=")
        to_folder = "inbox";
    range = expand_range(number);
    bing = sizeof(range);
    FOLDER_H->mark_read(owner->query_name(), current, newish);
    for (i = 0; i < bing; i++) { 
        int status;

        if (member_array(to_folder, folder_names) == -1) { 
            if (FOLDER_H->can_create_folder(owner->query_name(), to_folder)) {
                folder_names += ({ to_folder });
            } else {
                printf("Cannot create a folder called %s, please choose "
                    "another another name.\n", to_folder);
                break; 
            }
        }
        deleted += ({ folder[range[i]-1]->number });
        status = FOLDER_H->move_it(owner->query_name(), current, to_folder,
          folder[range[i]-1]->number);
        folder[range[i]-1]->status = "D";
        switch(status) {
        case 2:
          printf("Folder full!\n");
        case 1:
          printf("Marked message %d as deleted.\n", range[i]);
          break;
        case 0:
          if(!flag)
            printf("Saved message %d to folder %s.\n", range[i], to_folder);
        }
    }
    if (!flag) {
        prompt();
        input_to("read_loop");
    }
} /* move_message() */

private void save_message(string input) {
    int *range, i, bing, ret;
    string to_file, number, body, err;

    if (!wizardp(owner)) {
        printf("Sorry, only creators can save mail to files.\n");
        prompt();
        input_to("read_loop");
        return;
    }
    if (!input || input == "") {
        if (last_read != -1)
            input = sprintf("%d ~/mbox",(last_read+1));
        else {
            printf("No messages in folder or all messages still unread.\n");
            prompt();
            input_to("read_loop");
            return;
        } 
    }
    if (!sscanf(input, "%s %s", number, to_file)) {
        printf("Wrong syntax.\n");
        prompt();
        input_to("read_loop");
        return;
    }
    seteuid(geteuid(owner));
    to_file = (string)owner->get_path(to_file);
    if (!master()->valid_write(to_file, owner, "write_file")) {
        printf("You cannot write to that file.\n");
        prompt();
        input_to("read_loop");
        return;
    }
    if ((i=file_size(to_file)) > 0)
        printf("Appending to existing file.\n");
    else if (i == -2) {
        printf("That is a directory.\n");
        prompt();
        input_to("read_loop");
        return;
    }
    range = expand_range(number);
    bing = sizeof(range);
    i = 0;
    for (i = 0; i < bing; i++) {
        body = FOLDER_H->load_message(owner->query_name(), current, 
          folder[range[i]-1]->number);
        if (!body) {
            continue;
        }
        err = catch(ret = write_file(to_file, body + "\n\n"));
        if (err) {
            printf("Writing to file failed: %s\n", err);
        } else if (!ret) {
            printf("Writing to file failed.\n");
        } else {
            printf("Saved message %d to %s.\n", range[i], to_file);
        }
    }
    seteuid("Mailer");
    prompt();
    input_to("read_loop");
} /* save_message() */

private void print_message(int number) {
    string body;

    if (number > sizeof(folder)-1 || number < 0) {
        printf("No message with that number.\n");
        prompt();
        input_to("read_loop");
        return;
    }
    if (folder[number]->status == "N") {
        newish -= ({ number });
        folder[number]->status = " ";
    }
    body = FOLDER_H->load_message(owner->query_name(), current, 
      folder[number]->number);
    if (!body) {
        rm_message(""+(number+1));
        return;
    }
    owner->set_finish_func("finish_print");
    printf("\n\nMessage %d\n", (number + 1));
    owner->more_string((full_header?body:strip_header(body)),
      "Message "+(number+1));
} /* print_message() */

/** @ignore yes */
void finish_print() {
    printf("\n");
    prompt();
    input_to("read_loop");
} /* finish_print() */

private void save_me() {
    if (!PLAYER_HANDLER->test_user(owner->query_name())) {
        return;
    }
    if (current != "inbox") {
        if (FOLDER_H->check_empty(owner->query_name(), current)) {
            folder_names -= ({ current });
        }
    }
    unguarded((: save_object, folder_filename(owner->query_name()) :));
} /* save_me() */

private int load_me() {
#ifdef CONVERTER
    if ("/obj/handlers/converter"->query_busy(owner->query_name())) {
        printf( "You cannot use the mailer now, your mail is being converted.\n" );
        return 0;
    }
#endif
    unguarded((: restore_object, folder_filename(owner->query_name()) :));
    if (!folder_names) {
        folder_names = ({ "inbox" });
    }
    return 1;
} /* load_me() */

/**
 * This method returns the list of folders associated with the player.
 * @param pname the player name
 * @return an array containing the folder names
 */
string *query_folders(string pname) {
    unguarded((: restore_object, folder_filename(pname) :));
    if (!folder_names)
        return ({});
    return folder_names;
} /* query_folders() */

private void read_messages(string fname) {
    folder = FOLDER_H->get_messages(owner->query_name(), fname);
} /* read_messages() */

/** @ignore yes */
void dest_me() {
    MAIL_TRACK->delete_mailer(this_object());
    destruct(this_object());
} /* dest_me() */

/**
 * This formats the date as needed by the mailer object.
 * @param x the date to format
 * @return the new date
 */
string format_date(int x) {
    string str;
    string mon;
    mixed *tm;

    if (x<0 || !intp(x))
        return "Bad time";

    tm = localtime(x);
    str = DAYS[tm[LT_WDAY]];
    mon = MONTHS[tm[LT_MON]];
    str = sprintf("%s, %d %s %d %02d:%02d %s", str, tm[LT_MDAY], mon,
      tm[LT_YEAR], tm[LT_HOUR], tm[LT_MIN], tm[LT_ZONE]);
    return str;
} /* format_date() */

private int *expand_range(string str) {
    int *ms, i, start, end;

    if (!str)
        str = "";
    if (!sizeof(folder))
        return ({ });
    str = replace_string(str, " ", "");
    ms = ({ });
    if (sscanf(str, "%*sall%*s")==2) {
        int size = sizeof(folder) + 1;
        for (i = 1; i < size; i++) {
            ms += ({ i });
        }
        return ms;
    }
    while (sscanf(str, "%d%s", start, str) == 2) {
        if (start < 1)
            start = 1;
        if (start > sizeof(folder))
            start = sizeof(folder);
        if (str && str != "" && (str[0] == '-' || ((strlen(str) > 1) &&
              str[0..1] == ".."))) {
            sscanf(str, "%*(-|(..))%d%s", end, str);
            if (end >= start) {
                if (end > sizeof(folder))
                    end = sizeof(folder);
                for (i = start; i <= end; i++)
                    if (member_array(i, ms) == -1)
                        ms += ({ i });
            }
        } else
        if (member_array(start, ms) == -1)
            ms += ({ start });
        sscanf(str, ",%s", str);
    }
    return ms;
} /* expand_range() */
