/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ken_mangle.c,v 1.1 1998/01/06 04:02:57 ceres Exp $
 * $Log: ken_mangle.c,v $
 * Revision 1.1  1998/01/06 04:02:57  ceres
 * Initial revision
 * 
*/
string *replace_matrix;
mapping trans;

void create() {  
  replace_matrix = ({ 
  "Aren't you ", "Aintcha ",
  "aren't you ", "aintcha ",
  "You are", "Yer ",
  "you are ", "yer ",
  "Are not ", "Ain't",
  "are not ", "ain't ",
  "Is not ", "Ain't ",
  " is not ", " ain't ",
  "What is ", "Woss",
  "what is ", "woss",
  "What are you ", "Wotcher ",
  "what are you ", "wotcher ",
  "What you are", "Wotcher",
  "what you are", "wotcher",
  "What you're", "Wotcher",
  "what you're", "wotcher",
  "What are your", "Wotcher",
  "what are your", "wotcher",
  "What do you ", "Wotcher ",
  "what do you ", "wotcher ",
  "What do your", "Wotcher",
  "what do your", "wotcher",
  "a h", "an '",
  "A h", "An '",
  " h", " '",
  " H", " '",
  "Th", "F",
  "Wh", "W",
  "wh", "w",
  "aw", "or",
  "ing ", "in' ",
  " th", " f",
  "th", "ff",
  ".", " .",
  "?", " ?",
  "!", " !",
  });

  trans = ([ ]);
  trans["stairs"] = "apples and pears";
  trans["Downstairs"] = "Down the apples and pears";
  trans["downstairs"] =	"down the apples and pears";
  trans["Upstairs"] = "Up the apples and pears";
  trans["upstairs"] = "up the apples and pears";
  trans["mouth"] = "North and South";
  trans["Mouth"] = "North and South";
  trans["Face"] = "Boat Race";
  trans["face"] = "Boat Race";
  trans["Trouble"] = "Barney Rubble";
  trans["trouble"] = "Barney Rubble";
  trans["wife"] = ({ "trouble and strife", "ole lady" });
  trans["Road"] = "Frog and Toad";
  trans["road"] = "frog and toad";
  trans["pub"] = "rub-a-dub ";
  trans["bar"] = "boozer";
  trans["husband"] = "ole man";
  trans["Party"] = "Knees-up";
  trans["party"] = "knees-up";
  trans["Parties"] = "Knees-ups";
  trans["parties"] = "knees-ups";
  trans["My"] = "Me";
  trans["my"] = "me";
  trans["Your"] = "Yer";
  trans["your"] = "yer";
  trans["You"] = "Yer";
  trans["you"] = "yer";
  trans["Those"] = "Them";
  trans["those"] = "them";
  trans["those"] = "them";
  trans["(those"] = "(them";
  trans["The"] = ({ "The bleedin'", "The" });
  trans["Fe"] = ({ "The bleedin'", "The" });
  trans["the"] = ({ "the bleedin'", "the" });
  trans["fe"] = ({ "the bleedin'", "the" });
  trans["Thus"] = "So";
  trans["thus"] = "so";
  trans["(thus"] = "(so";
  trans["thus"] = "so";
  trans["old"] = "ole";
  trans["to"] = "ter";
  trans["Aren't"] = "Ain't";
  trans["aren't"] = "ain't";
  trans["Isn't"] = "Ain't";
  trans["isn't"] = "ain't";
  trans["Hallo"] = "Wotcher";
  trans["Hello"] = "Wotcher";
  trans["Hullo"] = "Wotcher";
  trans["Hi"] = "Wotcher";
  trans["hallo"] = "wotcher";
  trans["hello"] = "wotcher";
  trans["hullo"] = "wotcher";
  trans["hi"] = "wotcher";
  trans["What"] = "Wot";
  trans["what"] = "wot";
  trans["Were"] = "Was";
  trans["were "] = "was";
  trans["Bother"] = "Ars";
  trans["bother"] = "ars";
  trans["Mother"] = "Muvver";
  trans["Other"] = "Uvver";
  trans["other"] = "uvver";
  trans["Father"] = "Favver";
  trans["father"] = "favver";
  trans["Rather"] = "Ravver";
  trans["rather"] = "ravver";
  trans["Weather"] = "Wevver";
  trans["weather"] = "wevver";
  trans["Leather"] = "Levver";
  trans["leather"] = "levver";
  trans["Wither"] = "Wivver";
  trans["wither"] = "wivver";
  trans["Either"] = "Eever";
  trans["either"] = "eever";
  trans["With"] = "Wiv";
  trans["with"] = "wiv";
  trans["Anything"] = "Anyfink";
  trans["anything"] = "anyfink";
  trans["Something"] = "Sumfink";
  trans["something"] = "sumfink";
  trans["Nothing"] = "Nuffink";
  trans["nothing"] = "nuffink";
  trans["guitars"] = "spoons";
  trans["guitar"] = "spoons";
  trans["drums"] = "spoons";
  trans["drum"] = "spoons";
  trans["trumpets"] = "spoons";
  trans["trumpet"] = "spoons";
  trans["violins"] = "spoons";
  trans["violin"] = "spoons";
  trans["clarinets"] = "spoons";
  trans["clarinet"] = "spoons";
  trans["trombones"] = "spoons";
  trans["trombone"] = "spoons";
  trans["oboes"] = "spoons";
  trans["oboe"] = "spoons";
  trans["flutes"] = "spoons";
  trans["flute"] = "spoons";
  trans["tubas"] = "spoons";
  trans["tuba"] = "spoons";
  trans["Data"] = "Info";
  trans["data"] = "info";
  trans["Directory"] = "Lockup";
  trans["directory"] = "lockup";
  trans["Directories"] = "Lockups";
  trans["directories"] = "lockups";
  trans["Stopped"] = "'Ad the mockers put on";
  trans["stopped"] = "'ad the mockers put on";
  trans["Stopping"] = "Putting the mockers on";
  trans["stopping"] = "putting the mockers on";
  trans["stops"] = "puts the mockers on";
  trans["stop"] = "put the mockers on";
  trans["STOP"] = "Put The Mockers On";
  trans["Londoner"] = "Cockney";
  trans["londoner"] = "Cockney";
  trans["friend"] = ({ "mucker", "mate", "china" });
  trans["sin'"] = "sing";
  trans["rin'"] = "ring";
} /* trans() */

string do_transmute(string str) {
  string tmp, *bits;
  mixed erp;
  int i;

  tmp = replace(" "+str+" ", replace_matrix);
  bits = explode(tmp, " ");
  for (i=0;i<sizeof(bits);i++) {
    if (erp = trans[bits[i]])
      if (stringp(erp))
        bits[i] = erp;
      else
        bits[i] = erp[random(sizeof(erp))];
  }
  tmp = implode(bits, " ");
  while (tmp[strlen(tmp)-1] == ' ')
    tmp = tmp[0..strlen(tmp)-2];
  switch (tmp[strlen(tmp)-1]) {
    case '?' :
      tmp = tmp[0..strlen(tmp)-3]+ ({ ", mate? ", ", guv? ",
                                      ", squire? " })[random(3)];
      break;
    case '!' :
      tmp = tmp[0..strlen(tmp)-3]+ ({ ", cor blimey!", "! Struth!",
		    ", guv!", " ! Honest guv!", ", mate! ", "!", })[random(6)];
      break;
    case '.' :
      tmp = tmp[0..strlen(tmp)-3];
    default :
      tmp += ({
		    ". I'll get out me spoons. ",
		    ". Yer can't 'ave a knees-up wivout a joanna. ",
		    ". Cor blimey guv, would I lie to you. ",
		    ". I'll make us all a nice cup of tea. ",
               })[random(4)];
  }
  return replace(tmp, ({ " !", "!", " .", ".", " ?", "?" }));
} /* do_transmute() */
