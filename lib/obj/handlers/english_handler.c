/**
 *
 * English Handler
 * @author Shaggy
 * 
 * This handler is a collection of methods that attempt to evaluate how much
 * like english a line of text is.  Some tests might be more expensive than
 * others but the general idea is to be much cheaper than a dictionary lookup.
 * 
 * The general principle it to return a value that is closer to 100 the better
 * the english with 50 being borderline (ie. likely to be gibberish).
 * The returned value is thus a bit like a percentage chance that it is
 * english.
*/

/* 
 * Frequencies of letters and 1/(standard deviations) in a 500K am board archive (titles removed) 
 * The format is pairs:  freq per line * 1000     1/(std dev) per line * 1000    
 * 1st-26th pairs:  letters a-z
 * 27th pair is spaces
 * 28th pair is other characters (excluding newline)
*/
float *freq=({ 0.070012, 15.782218, 0.011949, 43.328090, 0.021800, 31.780955,
                 0.041222, 22.534301, 0.081093, 11.110506, 0.016799, 34.952377,
                 0.019235, 35.094824, 0.031751, 21.939816, 0.055348, 15.959625,
                 0.001028, 116.456628, 0.007624, 57.669862, 0.029144,
                 22.558570, 0.028764, 31.148966, 0.047194, 18.490345, 0.054178,
                 15.319177, 0.014415, 36.228697, 0.001335, 116.456628,
                 0.047561, 18.817823, 0.050139, 17.975454, 0.058789, 13.617099,
                 0.020638, 32.978418, 0.005222, 63.346619, 0.011713, 42.180909,
                 0.001264, 116.456628, 0.014661, 40.185678, 0.000530,
                 116.456628, 0.171614, 40.0, 0.000666, 116.456628,
                 0.084311, 10.304674
});
/**
 * FreqTest is a routine that evaluates how like english a line of text is.  It
 * compares the frequency of the letters, spaces and non-letters to the typical
 * number in English (taken from the am board archive).  A typical sentence
 * should give a percentage change of being english in the range 50-100.
 * Short or unusual sentences can get borderline evaluations (<60) even if they
 * are english.
 * 
 * @param line line of text to evaluate
 * @return percentage chance of being english 50-100 ok < 50 might be gibberish 
 *
 * @example "/obj/handlers/english_handler"->FreqTest("Short english sentence")
 * Returns: 75
 * @example "/obj/handlers/english_handler"->FreqTest("Sqeal spezzial sentenz")
 * Returns: 1
 * @example "/obj/handlers/english_handler"->FreqTest("eeeeeeee dfs fsfdsfddf")
 * Returns: 37
 * @example "/obj/handlers/english_handler"->FreqTest("too short");  
 * Returns: 59
 * @example "/obj/handlers/english_handler"->FreqTest("Don't like l33t sp33k neither");  
 * Returns: 36
 * @example "/obj/handlers/english_handler"->FreqTest("a small excursion to the
 *                                                     zoo should be permissible");
 * Returns: 83 
*/
int FreqTest( string line ) {
  int len,i,ch;
  float badness,term;
  int *let;

  let=allocate(29);
  i = len = strlen(line);

  while (i--) { 
    ch = line[i]; 
    if ((ch>=65 && ch<65+26) || (ch>=97 && ch<97+26)) let[(ch&31)-1]++; 
    else if (ch==' ') let[26]++; 
    else if (ch>='0' && ch<='9') let[27]++; 
    else let[28]++; 
  }

  badness = 0.0;
  for (i=0;i<29;i++) {
    term = to_float((to_float(let[i]) - freq[i<<1] * len) * freq[(i<<1)+1]);
    badness += (term>0 ? term : -term);
  }
  badness /= (29.0*len);
  i = 100 - 50*badness*badness;
  if (i<1) i=1;

  return i;
}

/**
 * ProfaneTest is a routine that evaluates how likely the text is to
 * be profane/swearing/obscenity and so on.  These are the sorts of
 * things that would never be allowed as a player name.  Spaces, non
 * letters and repeating letters are worked around by this test.
 *
 * @param line line of text to evaluate
 * @return percentage chance of being offensive: chances over 100% can be returned
 *         indicating greater likelihood of offense and/or more than one suspect word.
 *         If you want it as lily white as possible you should insist on 0.
*/
int ProfaneTest( string strin ) {
  int i,profane=0;
  string str,s;

  if (sizeof(strin)<1) return 0;

  strin=lower_case(strin)+" ";

  str="";
  for (i=0;i<sizeof(strin);i++) {
    if (strin[i]>='a' && strin[i]<='z') str+=strin[i..i];
  }

  if (sizeof(str)<2) return 0;

  s=str[0..0];
  for (i=1;i<sizeof(str);i++) {
    if (s[<1]!=str[i]) s+=str[i..i];
  }

  /* moderate and also ambiguous */
  if (regexp(s,"bastard|tit|shaft|viag") 
     || regexp(str,"poo|ass|butt")) profane+=25;

  /* fairly dicey and less likely to be ambiguous */
  if (regexp(s,"arse|sex|sux|crap|testic|fart|orgasm|orgy|turd|porn|niple|nazi|jesus|christ|satan|sperm|smeg|toilet|genital")
     || regexp(str,"piss|root|buttw|assho")
     || regexp(strin,"butt |ass ")) profane+=50;

  /* almost certainly profane */
  if (regexp(s,"netsex|arse|shit|screw|urine|rectum|tampon|vibrator|rectal|scatol|semen|suck|lick|dyke|dike|wank|mutha|prost|fag|feck|gay|homo|fuq|horny|blowjob|cuni") 
     || regexp(str,"phall|hooter|poof")) profane+=100;

  /* profane --- and yes, Hortense thinks analytic is a rude word */
  if (regexp(s,"anal|rape|rapist|fuck|fuk|fuch|kuck|cunt|kunt|whor|slut|clit|masturb|masterb|penis|cock|dick|pusy|bitch|vagina")) profane+=200;

  //  tell_creator("shaggy","start shout: %s\n",strin);
  //tell_creator("shaggy","text: %s profane chance: %d\n",s,profane);
  
  return profane;
}
