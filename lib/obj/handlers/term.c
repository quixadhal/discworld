/*  -*- LPC -*-  */
/*
 * $Id: term.c,v 1.8 2003/02/21 00:53:43 pinkfish Exp $
 */
/**
 * This object handles all the terminal related stuff.  Figuring out to 
 * display each colour and verifying that the colours are correct.
 * The terminal handler.  This has all sorts of utterly useless junk on it
 * Share and enjoy.
 * Thrown up by Pinkfish.

    Modified by Godot to add the xterm terminal. (10/16/92)
    Added vt100 Chrisy 15 may 93

    Definitions:

        <colour>    foreground colour
        B_<colour>  background colour
        BOLD        bold or bright letters
        ENDTERM     unknown
        FLASH       flashing or blinking letters
        INITTERM    unknown
        RESET       return to the normal font -- not a terminal reset.
        STATUS      unknown
        WINDOW      unknown
        REVERSE     reverse video mode
        UNDERLINE   underline mode
 */

#define ANSI(p) sprintf("%c["+(p)+"m", 27)
#define ESC(p) sprintf("%c"+(p), 27)
#define CHAR(p) sprintf("%c", (p))

#define TERMINAL_DUMB "dumb"

private nosave mapping aliases, terms;

void create() {
  aliases = ([ "xterm-debian" : "xterm",
               "zmud" : "vt220",
               "amiga" : "vt220",
               "linux" : "vt220",
               "xterm-color" : "xterm",
               "vt102" : "vt100",
               "dec-vt100" : "vt100",
               "vt300" : "vt220",
               "vt100a" : "vt100",
               "vt100-ansi" : "vt100",
               "dec-vt220" : "vt220",
               "vt400" : "vt220",
               "vs100" : "xterm",
               "sun-cmd" : "vt220-nc",
               "unknown" : TERMINAL_DUMB,
               "ibm" : TERMINAL_DUMB,
               "iris-ansi-net" : "vt220",
               "network" : TERMINAL_DUMB,
               "vt320" : "vt220",
               "dtterm" : "vt220",
               "screen" : "vt220",
               "dec-vt320" : "vt220",
               "dec-vt100i" : "vt100",
               "ansii" : "ansi",
               "vt100i" : "vt100",
               "vt200-80" : "vt220",
               "ibm-3279-2-e" : TERMINAL_DUMB,
               "ibm-3279-2" : TERMINAL_DUMB,
               "vt200" : "vt220",
               "dec-vt200" : "vt220",
               "hft" : "vt220",
               "mac" : "vt220-nc",
               "vt340" : "vt220",
               "vt340-am" : "vt220",
               "ibm-3278-2" : TERMINAL_DUMB,
               "ibm-3278-2-e" : TERMINAL_DUMB,
               "kterm" : "xterm",
               "sun" : "vt220-nc",
               "sun1" : "vt220-nc",
               "xterms" : "xterm",
               "vs100s" : "xterm",
               "vt52" : TERMINAL_DUMB,
               "vt420" : "vt220",
               "dec-vt420" : "vt220",
               "aixterm-m" : "aixterm",
               "ibm-3151" : "ibm3151",
               "ibm-3278-5" : TERMINAL_DUMB,
               "du" : TERMINAL_DUMB,
               "dialup" : TERMINAL_DUMB,
               "dec-vt52" : TERMINAL_DUMB,
               "vt100-pro" : "vt100",
            ]);
  terms = ([ TERMINAL_DUMB : ([ "RESET" : "",
                         "BOLD" : "",
                         "FLASH" : "",
                         "BLACK" : "",
                         "RED" : "",
                         "BLUE" : "",
                         "CYAN" : "",
                         "MAGENTA" : "",
                         "ORANGE" : "",
                         "YELLOW" : "",
                         "GREEN" : "",
                         "WHITE" : "",
                         "BLACK" : "",
                         "B_RED" : "",
                         "B_ORANGE" : "",
                         "B_YELLOW" : "",
                         "B_BLACK" : "",
                         "B_CYAN" : "",
                         "B_WHITE" : "",
                         "B_GREEN" : "",
                         "B_MAGENTA" : "",
                         "STATUS": "",
                         "WINDOW": "",
                         "INITTERM": "",
                         "ENDTERM": "",
                         "REVERSE": "",
                         "UNDERLINE": "",
                         "%" : "%^",
                         ]),
             "ansi" : ([ "RESET" : ANSI("39;49")+ANSI("0;10"),
                         "BOLD" : ANSI(1),
                         "FLASH" : ANSI(5),
                         "BLACK" : ANSI(30),
                         "RED" : ANSI(31),
                         "GREEN" : ANSI(32),
                         "ORANGE" : ANSI(33),
                         "YELLOW" : ANSI("1;33"),
                         "BLUE" : ANSI(34),
                         "CYAN" : ANSI(36),
                         "MAGENTA" : ANSI(35),
                         "BLACK" : ANSI(30),
                         "WHITE" : ANSI(37),
                         "B_RED" : ANSI(41),
                         "B_GREEN" : ANSI(42),
                         "B_ORANGE" : ANSI(43),
                         "B_YELLOW" : ANSI("1;43"),
                         "B_BLUE" : ANSI(44),
                         "B_CYAN" : ANSI(46),
                         "B_BLACK" : ANSI(40),
                         "B_WHITE" : ANSI(47), /* 47? */
                         "B_MAGENTA" : ANSI(45),
                         "STATUS": "",
                         "WINDOW": "",
                         "INITTERM": "",
                         "ENDTERM": "",
                         "REVERSE" : ANSI(7),
                         "UNDERLINE" : ANSI(4),
                         "%" : "%^",
                         ]),
             "ansi-no-flash" : ([ "RESET" : ANSI("39;49")+ANSI("0;10"),
                         "BOLD" : ANSI(1),
                         "FLASH" : "",
                         "BLACK" : ANSI(30),
                         "RED" : ANSI(31),
                         "GREEN" : ANSI(32),
                         "ORANGE" : ANSI(33),
                         "YELLOW" : ANSI("1;33"),
                         "BLUE" : ANSI(34),
                         "CYAN" : ANSI(36),
                         "MAGENTA" : ANSI(35),
                         "BLACK" : ANSI(30),
                         "WHITE" : ANSI(37),
                         "B_RED" : ANSI(41),
                         "B_GREEN" : ANSI(42),
                         "B_ORANGE" : ANSI(43),
                         "B_YELLOW" : ANSI("1;43"),
                         "B_BLUE" : ANSI(44),
                         "B_CYAN" : ANSI(46),
                         "B_BLACK" : ANSI(40),
                         "B_WHITE" : ANSI(47), /* 47? */
                         "B_MAGENTA" : ANSI(45),
                         "STATUS": "",
                         "WINDOW": "",
                         "INITTERM": "",
                         "ENDTERM": "",
                         "REVERSE" : ANSI(7),
                         "UNDERLINE" : ANSI(4),
                         "%" : "%^",
                         ]),
             "freedom" : ([ "RESET" : ESC("G0"),
                         "BOLD" : ESC("G@"),
                         "FLASH" : ESC("G2"),
                         "BLACK" : "",
                         "RED" : "",
                         "GREEN" : "",
                         "ORANGE" : "",
                         "YELLOW" : "",
                         "BLUE" : "",
                         "CYAN" : "",
                         "MAGENTA" : "",
                         "BLACK" : "",
                         "WHITE" : "",
                         "B_RED" : ESC("GD"),
                         "B_GREEN" : ESC("GD"),
                         "B_ORANGE" : ESC("G4"),
                         "B_YELLOW" : ESC("G4"),
                         "B_BLUE" : ESC("G4"),
                         "B_CYAN" : ESC("GD"),
                         "B_BLACK" : ESC("GD"),
                         "B_WHITE" : ESC("G4"), /* 47? */
                         "B_MAGENTA" : ESC("G4"),
                         "STATUS": "",
                         "WINDOW": "",
                         "INITTERM": "",
                         "ENDTERM": "",
                         "REVERSE" : ESC("G4"),
                         "UNDERLINE" : ESC("G8"),
                         "%" : "%^",
                         ]),
             "ansi-status": ([
                         "RESET" : ANSI("39;49")+ANSI("0;10"),
                         "BOLD" : ANSI(1),
                         "FLASH" : ANSI(5),
                         "BLACK" : ANSI(30),
                         "RED" : ANSI(31),
                         "GREEN" : ANSI(32),
                         "ORANGE" : ANSI(33),
                         "YELLOW" : ANSI("1;33"),
                         "BLUE" : ANSI(34),
                         "CYAN" : ANSI(36),
                         "MAGENTA" : ANSI(35),
                         "BLACK" : ANSI(30),
                         "WHITE" : ANSI(37),
                         "B_RED" : ANSI(41),
                         "B_GREEN" : ANSI(42),
                         "B_ORANGE" : ANSI(43),
                         "B_YELLOW" : ANSI("1;43"),
                         "B_BLUE" : ANSI(44),
                         "B_CYAN" : ANSI(46),
                         "B_BLACK" : ANSI(40),
                         "B_WHITE" : ANSI(47), /* 47? */
                         "B_MAGENTA" : ANSI(45),
                         "STATUS" : ESC("[23;24r") + ESC(8),
                         "WINDOW" : ESC(7)+ESC("[0;22r")+ESC("[22H\n"),
                         "INITTERM" : ESC("[H")+ESC("[J")+
                                      ESC("[23;24r")+ESC("23H\n"),
                         "ENDTERM" : ESC("[0r")+ESC("[H")+ESC("[J"),
                         "REVERSE" : ANSI(7),
                         "UNDERLINE" : ANSI(4),
                         "%" : "%^",
                         ]),
             "xterm": ([ "RESET" : ANSI("39;49")+ESC("[m"),
                         "BOLD" : ANSI(1),
                         "FLASH" : ANSI(5),
                         "BLACK" : ANSI(30),
                         "RED" : ANSI(31),
                         "GREEN" : ANSI(32),
                         "ORANGE" : ANSI(33), /* Can't do brown */
                         "YELLOW" : ANSI("1;33"),
                         "BLUE" : ANSI(34),
                         "CYAN" : ANSI(36),
                         "MAGENTA" : ANSI(35),
                         "BLACK" : ANSI(30),
                         "WHITE" : ANSI(37),
                         "B_RED" : ANSI(41),
                         "B_GREEN" : ANSI(42),
                         "B_ORANGE" : ANSI(43), /* Can't do brown */
                         "B_YELLOW" : ANSI("1;43"),
                         "B_BLUE" : ANSI(44),
                         "B_CYAN" : ANSI(46),
                         "B_BLACK" : ANSI(40),
                         "B_WHITE" : ANSI(47), /* 47? */
                         "B_MAGENTA" : ANSI(45),
                         "WINDOW" : "",
                         "INITTERM" : "",
                         "ENDTERM" : "",
                         "REVERSE" : ANSI(7),
                         "UNDERLINE" : ANSI(4),
                         "%" : "%^",
                         ]),
             "vt100" : (["RESET" : ANSI("39;49")+ESC("[m")+CHAR(15),
                         "BOLD" : ANSI(1),
                         "FLASH" : ANSI(5),
                         "BLACK" : ANSI(30),
                         "RED" : ANSI(31),
                         "GREEN" : ANSI(32),
                         "ORANGE" : ANSI(33),
                         "YELLOW" : ANSI("1;33"),
                         "BLUE" : ANSI(34),
                         "CYAN" : ANSI(36),
                         "MAGENTA" : ANSI(35),
                         "WHITE" : ANSI(37),
                         "B_RED" : ANSI(41),
                         "B_GREEN" : ANSI(42),
                         "B_ORANGE" : ANSI(43),
                         "B_YELLOW" : ANSI("1;43"),
                         "B_BLUE" : ANSI(44),
                         "B_CYAN" : ANSI(46),
                         "B_BLACK" : ANSI(40),
                         "B_WHITE" : ANSI(47), /* 47? */
                         "B_MAGENTA" : ANSI(45),
                         "STATUS": "",
                         "WINDOW": "",
                         "INITTERM": "",
                         "ENDTERM": "",
                         "REVERSE" : ANSI(7),
                         "UNDERLINE" : ANSI(4),
                         "%" : "%^",
                         ]),
             "hpterm" : (["RESET" : ESC("&v0S")+ESC("&d@"),
                         "BOLD" : ESC("&dB"),
                         "FLASH" : "",
                         "BLACK" : "",
                         "RED" : ESC("&dB"),
                         "GREEN" : ESC("&dD"),
                         "ORANGE" : "",
                         "YELLOW" : ESC("&dB"),
                         "BLUE" : "",
                         "CYAN" : "",
                         "MAGENTA" : "",
                         "WHITE" : "",
                         "B_RED" : "",
                         "B_GREEN" : "",
                         "B_ORANGE" : "",
                         "B_YELLOW" : "",
                         "B_BLUE" : "",
                         "B_CYAN" : "",
                         "B_BLACK" : "",
                         "B_WHITE" : "",
                         "B_MAGENTA" : "",
                         "STATUS": "",
                         "WINDOW": "",
                         "INITTERM": "",
                         "ENDTERM": "",
                         "REVERSE" : ESC("&dB"),
                         "UNDERLINE" : ESC("&dD"),
                         "%" : "%^",
                         ]),
           "aixterm" : ([ "RESET" : ANSI("0;10")+ESC("(B"),
                         "BOLD" : ANSI(1),
                         "FLASH" : "",
                         "BLACK" : "",
                         "RED" : ANSI(1),
                         "BLUE" : "",
                         "CYAN" : "",
                         "MAGENTA" : "",
                         "ORANGE" : "",
                         "YELLOW" : ESC('p'),
                         "GREEN" : ESC('W'),
                         "WHITE" : "",
                         "BLACK" : "",
                         "B_RED" : "",
                         "B_ORANGE" : "",
                         "B_YELLOW" : "",
                         "B_BLACK" : "",
                         "B_CYAN" : "",
                         "B_WHITE" : "",
                         "B_GREEN" : "",
                         "B_MAGENTA" : "",
                         "STATUS": "",
                         "WINDOW": "",
                         "INITTERM": "",
                         "ENDTERM": "",
                         "REVERSE": ESC('p'),
                         "UNDERLINE": ESC('W'),
                         "%" : "%^",
                         ]),
           "ibm3151" : ([ "RESET" : ESC("4@"),
                         "BOLD" : ESC("4H"),
                         "FLASH" : ESC("4D"),
                         "BLACK" : "",
                         "RED" : ESC("4H"),
                         "BLUE" : "",
                         "CYAN" : "",
                         "MAGENTA" : "",
                         "ORANGE" : "",
                         "YELLOW" : ESC("4A"),
                         "GREEN" : ESC("4B"),
                         "WHITE" : "",
                         "BLACK" : "",
                         "B_RED" : "",
                         "B_ORANGE" : "",
                         "B_YELLOW" : "",
                         "B_BLACK" : "",
                         "B_CYAN" : "",
                         "B_WHITE" : "",
                         "B_GREEN" : "",
                         "B_MAGENTA" : "",
                         "STATUS": "",
                         "WINDOW": "",
                         "INITTERM": "",
                         "ENDTERM": "",
                         "REVERSE": ESC("4A"),
                         "UNDERLINE": ESC("4B"),
                         "%" : "%^",
                         ]),
             "vt220": ([ "RESET" : ANSI("39;49")+ESC("[m"),
                         "BOLD" : ANSI(1),
                         "FLASH" : ANSI(5),
                         "BLACK" : ANSI(30),
                         "RED" : ANSI(31),
                         "GREEN" : ANSI(32),
                         "ORANGE" : ANSI(33), /* Can't do brown */
                         "YELLOW" : ANSI("1;33"),
                         "BLUE" : ANSI(34),
                         "CYAN" : ANSI(36),
                         "MAGENTA" : ANSI(35),
                         "BLACK" : ANSI(30),
                         "WHITE" : ANSI(37),
                         "B_RED" : ANSI(41),
                         "B_GREEN" : ANSI(42),
                         "B_ORANGE" : ANSI(43), /* Can't do brown */
                         "B_YELLOW" : ANSI("1;43"),
                         "B_BLUE" : ANSI(44),
                         "B_CYAN" : ANSI(46),
                         "B_BLACK" : ANSI(40),
                         "B_WHITE" : ANSI(47),
                         "B_MAGENTA" : ANSI(45),
                         "WINDOW" : "",
                         "INITTERM" : "",
                         "ENDTERM" : "",
                         "REVERSE" : ANSI(7),
                         "UNDERLINE" : ANSI(4),
                         "%" : "%^",
                         ]),
             "vt100-nc" : ([ "RESET" : ESC("[m")+CHAR(15),
                         "BOLD" : ANSI(1),
                         "FLASH" : ANSI(5),
                         "BLACK" : ANSI(30),
                         "RED" : ANSI("37;7"),
                         "GREEN" : ANSI(1),
                         "ORANGE" : ANSI(37),
                         "YELLOW" : ANSI("1;4"),
                         "BLUE" : ANSI(37),
                         "CYAN" : ANSI(4),
                         "MAGENTA" : ANSI(37),
                         "WHITE" : ANSI(37),
                         "B_RED" : ANSI(41),
                         "B_GREEN" : ANSI(42),
                         "B_ORANGE" : ANSI(43),
                         "B_YELLOW" : ANSI("1;43"),
                         "B_BLUE" : ANSI(44),
                         "B_CYAN" : ANSI(46),
                         "B_BLACK" : ANSI(40),
                         "B_WHITE" : ANSI(47),
                         "B_MAGENTA" : ANSI(45),
                         "STATUS": "",
                         "WINDOW": "",
                         "INITTERM": "",
                         "ENDTERM": "",
                         "REVERSE" : ANSI(7),
                         "UNDERLINE" : ANSI(4),
                         "%" : "%^",
                         ]),           
             "vt220-nc" : ([ "RESET" : ESC("[m"),
                         "BOLD" : ANSI(1),
                         "FLASH" : ANSI(5),
                         "BLACK" : ANSI(30),
                         "RED" : ANSI("37;7"),
                         "GREEN" : ANSI(1),
                         "ORANGE" : ANSI(37),
                         "YELLOW" : ANSI("1;4"),
                         "BLUE" : ANSI(37),
                         "CYAN" : ANSI(4),
                         "MAGENTA" : ANSI("7;4"),
                         "WHITE" : ANSI(37),
                         "B_RED" : ANSI(41),
                         "B_GREEN" : ANSI(42),
                         "B_ORANGE" : ANSI(43),
                         "B_YELLOW" : ANSI("1;43"),
                         "B_BLUE" : ANSI(44),
                         "B_CYAN" : ANSI(46),
                         "B_BLACK" : ANSI(40),
                         "B_WHITE" : ANSI(47),
                         "B_MAGENTA" : ANSI(45),
                         "STATUS": "",
                         "WINDOW": "",
                         "INITTERM": "",
                         "ENDTERM": "",
                         "REVERSE" : ANSI(7),
                         "UNDERLINE" : ANSI(4),
                         "%" : "%^",
                         ]),           
          ]);
}

/**
 * This returns the mapping of colours based on the terminal type
 * entered in here.
 * @param str the terminal type
 * @return mapping of colours to escape codes
 */
mixed set_network_term_type(string str) {
  string type;

  str = lower_case(str);
  if (!terms[str] && !(type = aliases[str])) {
#ifdef DEBUG
    log_file("TERMTYPES", "No entry for \"%s\" (%s).\n", str,
             this_player()->query_name());
#endif    
    return 0;
  }
  if (type) {
     return terms[type];
  }
  return terms[str];
} /* set_network_term_type() */

/**
 * This returns the mapping of colours based on the terminal type
 * entered in here.
 * @param str the terminal type
 * @return mapping of colours to escape codes
 */
mapping set_term_type(string str) {
  if (!terms[str] && !(str = aliases[str])) {
    efun::tell_object(this_player(), "No entry for "+str+", using dumb.\n");
    str = TERMINAL_DUMB;
  }
  return terms[str];
} /* set_term_type() */

/**
 * This method returns all the currently available types of terminals.
 * @return the array of terminal types
 */
string *query_term_types() {
  return m_indices(terms);
} /* query_term_types() */

/**
 * This method returns the array of usable colours.
 * @return the array of usable colours
 */
string *query_colour_codes() {
   return keys(terms[TERMINAL_DUMB]);
} /* query_colour_codes() */
