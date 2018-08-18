/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: query_number.c,v 1.6 2001/08/02 21:04:02 ceres Exp $
 */
string number_as_string(int n);

/**
 * Cotains some simul_efuns to convert integers into textual numbers.
 * @author Pinkfish
 */

/**
 * This method turns a number into a string.  It returns a string of the
 * form "one hundred and twenty four".  This function should be used and
 * not number_as_string below.
 * The limit argument is used to set the cut off point at which it will
 * print "many".
 *
 * @param i the number to return as a string
 * @param limit the largest number to process
 * @return the number as a nice string
 */
string query_num(int n, int limit) {
  string ret;
  int i;

  if (limit && n>limit) return "many"; /* this is a little pointless ... */
  if (n < 0) return "many"; /* fo wraps... */
  if (n > 99999) return "many"; /* sorry 'bout the hard limit */
  ret = 0;
  if ((i = n/1000)) {
    n = n%1000;
    if (!n)
      return number_as_string(i) + " thousand";
    ret = number_as_string(i) + " thousand";
  }
  if ((i = n/100)) {
    n = n%100;
    if (ret) {
      if (!n)
        return ret + " and " + number_as_string(i) + " hundred";
      ret += ", " + number_as_string(i) + " hundred";
    } else {
      if (!n)
        return number_as_string(i) + " hundred";
      ret = number_as_string(i) + " hundred";
    }
  }
  if (ret)
    return ret + " and " + number_as_string(n);
  return number_as_string(n);
} /* query_num() */

/**
 * This method turns a number less than 100 into a string.  This function
 * should not be used.  It will only work for numbers less than 100.
 *
 * @param n the number to turn into a string
 * @return the number as a nice string
 * @see query_num()
 */
string number_as_string(int n) {
  string ret;

  if (!n) return "zero";
  if (n<20 && n>9) return ({ "ten", "eleven", "twelve", "thirteen",
                             "fourteen", "fifteen", "sixteen", "seventeen",
                             "eighteen", "nineteen" })[n-10];
  ret = ({ "", "", "twenty", "thirty", "forty", "fifty", "sixty",
           "seventy", "eighty", "ninety"})[n/10];
  if ((n = n%10)&&(ret!="")) ret += "-";
  return ret + ({ "", "one", "two", "three", "four", "five", "six",
                  "seven", "eight", "nine"})[n];
} /* number_as_string() */


/**
 * This method returns the ordinal of a number as a string (ie, "first",
 * "thirtieth", etc.)
 * @param n the number for which you want the ordinal
 * @returns the ordinal of the number as a string
 * @see query_num(), ordinal()
 */
string word_ordinal(int num)  {
  string word = "";
  int    part;
  
  switch (part = num % 100)  {
  case 0:
    word = "th";
    break;
  case 1 .. 12:
    word = ({
      "first",   "second", "third", "fourth", "fifth",    "sixth",
        "seventh", "eighth", "ninth", "tenth",  "eleventh", "twelfth"
        })[part - 1];
    break;
    
  case 13 .. 19:
    word = query_num(num % 100, 99999) + "th";
    break;
    
  default:
    if (part % 10 == 0)  {
      word = ({
        "twentieth", "thirtieth", "fortieth", "fiftieth", "sixtieth",
          "seventieth", "eightieth", "ninetieth"
          })[part / 10 - 2];
    }
    else  {
      word = ({
        "twenty", "thirty", "forty", "fifty", "sixty", "seventy",
          "eighty", "ninety"
          })[part / 10 - 2] + "-" + word_ordinal(part % 10);
    }
    break;
  }
  
  num = (num / 100) * 100;
  if (num != 0)   {
    if (part > 0)  word = query_num(num, 99999) + " and " + word;
    else word = query_num(num, 99999) + word;
  }
  return word;
}

/**
 * Convert a number into a number of times. eg. 1 = once, 2 = twice, 3 = three
 * times etc. etc.
 *
 * @param the number in question
 * @return a string equivalent.
 */
string query_times(int num) {
  string retval;
  
  switch(num) {
  case 0:
    retval = "";
    break;
  case 1:
    retval = "once";
    break;
  case 2:
    retval = "twice";
    break;
  default:
    retval = query_num(num, 0) + " times";
  }

  return retval;
}

string query_japan_digit(int num, int kana) {
    if (num == 0) {
        return kana ? "〇" : "nuru";
    }
    switch( num % 10 ) {
        case 1:
            return kana ? "一" : "ichi";
        case 2:
            return kana ? "二" : "ni";
        case 3:
            return kana ? "三" : "san";
        case 4:
            return kana ? "四" : "yon";
        case 5:
            return kana ? "五" : "go";
        case 6:
            return kana ? "六" : "roku";
        case 7:
            return kana ? "七" : "nana";
        case 8:
            return kana ? "八" : "hachi";
        case 9:
            return kana ? "九" : "kyu";
        default:
            return "";
    }
}

string query_japan_tens(int num, int kana) {
    string val = "";

    if( num < 10 ) return query_japan_digit(num, kana);

    switch(( num / 10 ) % 10 ) {
        case 0:
            val = "";
            break;
        case 1:
            val = kana ? "十" : "ju";
            break;
        default:
            val = query_japan_digit(num/10, kana) + (kana ? "十" : "ju");
            break;
    }
    return val + query_japan_digit(num, kana);
}

string query_japan_hundreds(int num, int kana) {
    string val = "";

    if( num < 100 ) return query_japan_tens(num, kana);

    switch(( num / 100 ) % 10 ) {
        case 0:
            val = "";
            break;
        case 1:
            val = kana ? "百" : "hyaku";
            break;
        case 3:
            val = kana ? "三百" : "sanbyaku";
            break;
        case 6:
            val = kana ? "六百" : "roppyaku";
            break;
        case 8:
            val = kana ? "八百" : "happyaku";
            break;
        default:
            val = query_japan_digit(num/100, kana) + (kana ? "百" : "kyaku");
            break;
    }
    return val + query_japan_tens(num, kana);
}

string query_japan_thousands(int num, int kana) {
    string val = "";

    if( num < 1000 ) return query_japan_hundreds(num, kana);

    switch(( num / 1000 ) % 10 ) {
        case 0:
            val = "";
            break;
        case 1:
            val = kana ? "千" : "sen";
            break;
        case 3:
            val = kana ? "三千" : "sanzen";
            break;
        case 5:
            val = kana ? "五千" : "gozen";
            break;
        case 8:
            val = kana ? "八千" : "hassen";
            break;
        default:
            val = query_japan_digit(num/1000, kana) + (kana ? "千" : "sen");
            break;
    }
    return val + query_japan_hundreds(num, kana);
}

string query_japan_tenthousands(int num, int kana) {
    string val = "";

    if( num < 10000 ) return query_japan_thousands(num, kana);

    switch(( num / 10000 ) % 10 ) {
        case 1:
            val = kana ? "一万" : "ichiman";
            break;
        case 2:
            val = kana ? "二万" : "niman";
            break;
        case 3:
            val = kana ? "三万" : "sanman";
            break;
        case 4:
            val = kana ? "四万" : "yonman";
            break;
        case 5:
            val = kana ? "五万" : "goman";
            break;
        case 6:
            val = kana ? "六万" : "rokuman";
            break;
        case 7:
            val = kana ? "七万" : "nanaman";
            break;
        case 8:
            val = kana ? "八万" : "hachiman";
            break;
        case 9:
            val = kana ? "九万" : "kyuman";
            break;
        default:
            val = "";
            break;
    }
    return val + query_japan_thousands(num, kana);
}

string query_japan_hundredthousands(int num, int kana) {
    string val = "";

    if( num < 100000 ) return query_japan_tenthousands(num, kana);

    val = query_japan_tens( (num/10000), kana);

    return val + query_japan_tenthousands(num, kana);
}

// 10,000 -> 10
// 100K -> 100
// 1M -> 1000
// 10M -> 10K
string query_japan_tenmillions(int num, int kana) {
    string val = "";

    if( num < 1000000 ) return query_japan_hundredthousands(num, kana);

    val = query_japan_hundreds( (num/10000), kana);

    return val + query_japan_hundredthousands(num, kana);
}

string query_japan_num(int num, int kana) {
    return query_japan_hundredthousands(num, kana);
}

