/***
 * Five minute hack of a command to display where people
 * are logged on from.
 *    --Tape
 ***/
 
#define COUNTRIES_NO_SHOW_PROP "don't show on countries list"

// the common countries mapping is used as a sort of cache
mapping common_countries = ([
   "N/A" : ({ 0, "An unresolved address" }),
   "com" : ({ 0, "Commercial" }),
   "edu" : ({ 0, "Educational" }),
   "net" : ({ 0, "Network" }),
   "uk" : ({ 0, "United Kingdom" }),
   "au" : ({ 0, "Australia" })
   ]);

mapping all_countries = ([
   "ad" : ({ 0, "Andorra", }),
   "ae" : ({ 0, "United Arab Emirates" }),
   "af" : ({ 0, "Afghanistan" }),
   "ag" : ({ 0, "Antigua and Barbuda" }),
   "ai" : ({ 0, "Anguila" }),
   "al" : ({ 0, "Albania" }),
   "am" : ({ 0, "Armenia" }),
   "an" : ({ 0, "Netherlands Antilles" }),
   "ao" : ({ 0, "Angola" }),
   "aq" : ({ 0, "Antarctica" }),
   "ar" : ({ 0, "Argentina" }),
   "as" : ({ 0, "American Samoa" }),
   "at" : ({ 0, "Austria" }),
   "au" : ({ 0, "Australia" }),
   "aw" : ({ 0, "Aruba" }),
   "az" : ({ 0, "Azerbaijan" }),
   "ba" : ({ 0, "Bosnia and Herzegovina" }),
   "bb" : ({ 0, "Barbados" }),
   "bd" : ({ 0, "Bangladesh" }),
   "be" : ({ 0, "Belgium" }),
   "bf" : ({ 0, "Burkina Faso" }),
   "bg" : ({ 0, "Bulgaria" }),
   "bh" : ({ 0, "Bahrain" }),
   "bi" : ({ 0, "Burundi" }),
   "bj" : ({ 0, "Benin" }),
   "bm" : ({ 0, "Bermuda" }),
   "bn" : ({ 0, "Brunei Darussalam" }),
   "bo" : ({ 0, "Bolivia" }),
   "br" : ({ 0, "Brazil" }),
   "bs" : ({ 0, "Bahamas" }),
   "bt" : ({ 0, "Bhutan" }),
   "bv" : ({ 0, "Bouvet Island" }),
   "bw" : ({ 0, "Botswana" }),
   "by" : ({ 0, "Belarus" }),
   "bz" : ({ 0, "Belize" }),
   "ca" : ({ 0, "Canada" }),
   "cc" : ({ 0, "Cocos (Keeling) Islands" }),
   "cf" : ({ 0, "Central African Republic" }),
   "cg" : ({ 0, "Congo" }),
   "ch" : ({ 0, "Switzerland" }),
   "ci" : ({ 0, "Cote D'Ivoire (Ivory Coast)" }),
   "ck" : ({ 0, "Cook Island" }),
   "cl" : ({ 0, "Chile" }),
   "cm" : ({ 0, "Cameroon" }),
   "cn" : ({ 0, "China" }),
   "co" : ({ 0, "Colombia" }),
   "cr" : ({ 0, "Costa Rica" }),
   "cs" : ({ 0, "Czechoslovakia (former)" }),
   "cu" : ({ 0, "Cuba" }),
   "cv" : ({ 0, "Cape Verde" }),
   "cx" : ({ 0, "Christmas Island" }),
   "cy" : ({ 0, "Cyprus" }),
   "cz" : ({ 0, "Czech Republic" }),
   "de" : ({ 0, "Germany" }),
   "dk" : ({ 0, "Denmark" }),
   "dm" : ({ 0, "Dominica" }),
   "do" : ({ 0, "Dominican Republic" }),
   "dz" : ({ 0, "Algeria" }),
   "ec" : ({ 0, "Ecuador" }),
   "ee" : ({ 0, "Estonia" }),
   "eg" : ({ 0, "Egypt" }),
   "eh" : ({ 0, "Western Sahara" }),
   "er" : ({ 0, "Eritrea" }),
   "es" : ({ 0, "Spain" }),
   "et" : ({ 0, "Ethiopia" }),
   "fi" : ({ 0, "Finland" }),
   "fj" : ({ 0, "Fiji" }),
   "fk" : ({ 0, "Falkland Islands (Malvinas)" }),
   "fm" : ({ 0, "Micronesia" }),
   "fo" : ({ 0, "Faroe Islands" }),
   "fr" : ({ 0, "France" }),
   "fx" : ({ 0, "France, Metropolitan" }),
   "ga" : ({ 0, "Gabon" }),
   "gd" : ({ 0, "Grenada" }),
   "ge" : ({ 0, "Georgia" }),
   "gf" : ({ 0, "French Guiana" }),
   "gh" : ({ 0, "Ghana" }),
   "gi" : ({ 0, "Gibraltar" }),
   "gl" : ({ 0, "Greenland" }),
   "gm" : ({ 0, "Gambia" }),
   "gn" : ({ 0, "Guinea" }),
   "gp" : ({ 0, "Guadeloupe" }),
   "gq" : ({ 0, "Equatorial Guinea" }),
   "gr" : ({ 0, "Greece" }),
   "gs" : ({ 0, "S. Georgia and S. Sandwich Isls." }),
   "gt" : ({ 0, "Guatemala" }),
   "gu" : ({ 0, "Guam" }),
   "gw" : ({ 0, "Guinea-Bissau" }),
   "gy" : ({ 0, "Guyana" }),
   "hk" : ({ 0, "Hong Kong" }),
   "hm" : ({ 0, "Heard and McDonald Islands" }),
   "hn" : ({ 0, "Honduras" }),
   "hr" : ({ 0, "Croatia (Hrvatska)" }),
   "ht" : ({ 0, "Haiti" }),
   "hu" : ({ 0, "Hungary" }),
   "id" : ({ 0, "Indonesia" }),
   "ie" : ({ 0, "Ireland" }),
   "il" : ({ 0, "Israel" }),
   "in" : ({ 0, "India" }),
   "io" : ({ 0, "British Indian Ocean Territory" }),
   "iq" : ({ 0, "Iraq" }),
   "ir" : ({ 0, "Iran" }),
   "is" : ({ 0, "Iceland" }),
   "it" : ({ 0, "Italy" }),
   "jm" : ({ 0, "Jamaica" }),
   "jo" : ({ 0, "Jordan" }),
   "jp" : ({ 0, "Japan" }),
   "ke" : ({ 0, "Kenya" }),
   "kg" : ({ 0, "Kyrgyzstan" }),
   "kh" : ({ 0, "Cambodia" }),
   "ki" : ({ 0, "Kiribati" }),
   "km" : ({ 0, "Comoros" }),
   "kn" : ({ 0, "Saint Kitts and Nevis" }),
   "kp" : ({ 0, "Korea (North)" }),
   "kr" : ({ 0, "Korea (South)" }),
   "kw" : ({ 0, "Kuwait" }),
   "ky" : ({ 0, "Cayman Islands" }),
   "kz" : ({ 0, "Kazakhstan" }),   
   "la" : ({ 0, "Laos" }),
   "lb" : ({ 0, "Lebanon" }),
   "lc" : ({ 0, "Saint Lucia" }),
   "li" : ({ 0, "Liechtenstein" }),
   "lk" : ({ 0, "Sri Lanka" }),
   "lr" : ({ 0, "Liberia" }),
   "ls" : ({ 0, "Lesotho" }),
   "lt" : ({ 0, "Lithuania" }),
   "lu" : ({ 0, "Luxembourg" }),
   "lv" : ({ 0, "Latvia" }),
   "ly" : ({ 0, "Libya" }),
   "ma" : ({ 0, "Morocco" }),
   "mc" : ({ 0, "Monaco" }),
   "md" : ({ 0, "Moldovia" }),
   "mg" : ({ 0, "Madagascar" }),
   "mh" : ({ 0, "Marshall Islands" }),
   "mk" : ({ 0, "Macedonia" }),
   "ml" : ({ 0, "Mali" }),
   "mm" : ({ 0, "Myanmar" }),
   "mn" : ({ 0, "Mongolia" }),
   "mo" : ({ 0, "Macau" }),
   "mp" : ({ 0, "Northern Mariana Islands" }),
   "mq" : ({ 0, "Martinique" }),
   "mr" : ({ 0, "Mauritania" }),
   "ms" : ({ 0, "Montserrat" }),
   "mt" : ({ 0, "Malta" }),
   "mu" : ({ 0, "Mauritius" }),
   "mv" : ({ 0, "Maldives" }),
   "mw" : ({ 0, "Malawi" }),
   "mx" : ({ 0, "Mexico" }),
   "my" : ({ 0, "Malaysia" }),
   "mz" : ({ 0, "Mozambique" }),
   "na" : ({ 0, "Namibia" }),
   "nc" : ({ 0, "New Caledonia" }),
   "ne" : ({ 0, "Niger" }),
   "nf" : ({ 0, "Norfolk Islands" }),
   "ng" : ({ 0, "Nigeria" }),
   "ni" : ({ 0, "Nicaragua" }),
   "nl" : ({ 0, "Netherlands" }),
   "no" : ({ 0, "Norway" }),
   "np" : ({ 0, "Nepal" }),
   "nr" : ({ 0, "Nauru" }),
   "nt" : ({ 0, "Neutral Zone" }),
   "nu" : ({ 0, "Niue" }),
   "nz" : ({ 0, "New Zealand (Aotearoa)" }),
   "om" : ({ 0, "Oman" }),
   "pa" : ({ 0, "Panama" }),
   "pe" : ({ 0, "Peru" }),
   "pf" : ({ 0, "French Polynesia" }),
   "pg" : ({ 0, "Papua New Guinea" }),
   "ph" : ({ 0, "Philippines" }),
   "pk" : ({ 0, "Pakistan" }),
   "pl" : ({ 0, "Poland" }),
   "pm" : ({ 0, "St. Pierre and Miquelon" }),
   "pn" : ({ 0, "Pitcairn" }),
   "pr" : ({ 0, "Puerto Rico" }),
   "pt" : ({ 0, "Portugal" }),
   "pw" : ({ 0, "Palau" }),
   "py" : ({ 0, "Paraguay" }),
   "qa" : ({ 0, "Qatar" }),
   "re" : ({ 0, "Reunion" }),
   "ro" : ({ 0, "Romania" }),
   "ru" : ({ 0, "Russian Federation" }),
   "rw" : ({ 0, "Rwanda" }),
   "sa" : ({ 0, "Saudi Arabia" }),
   "sb" : ({ 0, "Solomon Islands" }),
   "sc" : ({ 0, "Seychelles" }),
   "sd" : ({ 0, "Sudan" }),
   "se" : ({ 0, "Sweden" }),
   "sg" : ({ 0, "Singapore" }),
   "sh" : ({ 0, "St. Helena" }),
   "si" : ({ 0, "Slovenia" }),
   "sj" : ({ 0, "Svalbard and Jan Mayen Islands" }),
   "sk" : ({ 0, "Slovak Republic" }),
   "sl" : ({ 0, "Sierra Leone" }),
   "sm" : ({ 0, "San Marino" }),
   "sn" : ({ 0, "Senegal" }),
   "so" : ({ 0, "Somalia" }),
   "sr" : ({ 0, "Suriname" }),
   "st" : ({ 0, "Sao Tome and Principe" }),
   "su" : ({ 0, "USSR (former)" }),
   "sv" : ({ 0, "El Salvador" }),
   "sy" : ({ 0, "Syria" }),
   "sz" : ({ 0, "Swaziland" }),
   "tc" : ({ 0, "Turks and Caicos Islands" }),
   "td" : ({ 0, "Chad" }),
   "tf" : ({ 0, "French Southern Territories" }),
   "tg" : ({ 0, "Togo" }),
   "th" : ({ 0, "Thailand" }),
   "tj" : ({ 0, "Tajikistan" }),
   "tk" : ({ 0, "Tokelau" }),
   "tm" : ({ 0, "Turkmenistan" }),
   "tn" : ({ 0, "Tunisia" }),
   "to" : ({ 0, "Tonga" }),
   "tp" : ({ 0, "East Timor" }),
   "tr" : ({ 0, "Turkey" }),
   "tt" : ({ 0, "Trinidad and Tobago" }),
   "tv" : ({ 0, "Tuvalu" }),
   "tw" : ({ 0, "Taiwan" }),
   "tz" : ({ 0, "Tanzania" }),
   "ua" : ({ 0, "Ukraine" }),
   "ug" : ({ 0, "Uganda" }),
   "uk" : ({ 0, "United Kingdom" }),
   "um" : ({ 0, "US Minor Outlying Islands" }),
   "us" : ({ 0, "United States" }),
   "uy" : ({ 0, "Uruguay" }),
   "uz" : ({ 0, "Uzbekistan" }),
   "va" : ({ 0, "Vatican City State (Holy See)" }),
   "vc" : ({ 0, "Saint Vincent and the Grenadines" }),
   "ve" : ({ 0, "Venezuela" }),
   "vg" : ({ 0, "Virgin Islands (British)" }),
   "vi" : ({ 0, "Virgin Islands (U.S.)" }),
   "vn" : ({ 0, "Viet Nam" }),
   "vu" : ({ 0, "Vanuatu" }),
   "wf" : ({ 0, "Wallis and Futuna Islands" }),
   "ws" : ({ 0, "Samoa" }),
   "ye" : ({ 0, "Yemen" }),
   "yt" : ({ 0, "Mayotte" }),
   "yu" : ({ 0, "Yugoslavia" }),
   "za" : ({ 0, "South Africa" }),
   "zm" : ({ 0, "Zambia" }),
   "zr" : ({ 0, "Zaire" }),
   "zw" : ({ 0, "Zimbabwe" }),
   "com" : ({ 0, "Commercial" }),
   "edu" : ({ 0, "Educational" }),
   "gov" : ({ 0, "US Government" }),
   "int" : ({ 0, "International" }),
   "mil" : ({ 0, "US Military" }),
   "net" : ({ 0, "Network" }),
   "org" : ({ 0, "Non-Profit Organization" }),
   "arpa" : ({ 0, "Old Style Arpanet" }),
   "nato" : ({ 0, "NATO Field" })
   ]);

int cmd();
int handle_no_show( int hide );
string resolve_domain_suffix( object player );

int cmd() {

   int left, right, size;
   float percentage;
   object person, *users;
   string suffix, message, country;
   mixed *unsorted, *sorted, *data;
   mapping countries;
   
   // get a copy of the common countries
   countries = copy( common_countries );
   
   // filter only those users who want to be displayed and are visible
   users = filter( users(), (: $1->query_visible( this_player() ) &&
      !$1->query_property( COUNTRIES_NO_SHOW_PROP ) :) );
   
   if( !size = sizeof( users ) ) {
      return notify_fail( "No visible players.\n" );
   }
   
   // go through them
   foreach( person in users ) {
   
      // figure out what their domain suffix is
      suffix = resolve_domain_suffix( person );
      
      // if it's not on the list of common countries
      if( undefinedp( countries[ suffix ] ) ) {
         // if it's not on the list of all countries
         if( undefinedp( all_countries[ suffix ] ) ) {
            countries[ "N/A" ][ 0 ] ++;
            continue;
         } else {
            // add it to the common countries for next time
            common_countries += ([ suffix : all_countries[ suffix ] ]);
            countries += ([ suffix : copy( all_countries[ suffix ] ) ]);
         }
      }

      countries[ suffix ][ 0 ] ++;
   }
   
   unsorted = ({ });

   size -= countries[ "N/A" ][ 0 ];
   map_delete( countries, "N/A" );
      
   // go through the countries and sort them in descending order
   foreach( country, data in countries ) {
      // if this country had zero entries (no one logging on from there)
      if( !data[ 0 ] ) {
         // remove it from the common countries list for next time
         map_delete( common_countries, country );
         // go to the next country
         continue;
      } else {
         data[ 1 ] += " (%^BOLD%^" + upper_case( country ) + "%^RESET%^)";
         unsorted += ({ data });
      }
   }

   sorted = sort_array( unsorted, -1 );
   
   message = "\nA total of %^BOLD%^" + query_num( size ) +
      "%^RESET%^ visible " + ( size != 1 ? "users" : "user" ) +
      " with resolvable addresses logged on:\n";

   left = this_player()->query_cols() / 3 * 2;
   left -= 3;
   right = this_player()->query_cols() - left;

   // make it look pretty
   foreach( data in sorted ) {
      percentage = data[ 0 ] * 100.0 / size;
      message += sprintf( "   %-=*'.'s%*-=s\n",
      left, data[ 1 ],
      right, "  " + to_int( percentage + 0.5 ) + "%" );
   }

   // stuff it through the user's more prompt
   this_player()->more_string( message );
   
   return 1;
   
} /* cmd() */


string resolve_domain_suffix( object player ) {
   
   string addy, *host;
   
   addy = query_ip_name( player );
   
   if( !addy ) {
      return 0;
   }
   
   host = explode( lower_case( addy ), "." );
   
   // tell_creator( this_player(), "%O\n", host );   

   return host[ sizeof( host ) - 1 ];
   
} /* resolve_domain_suffix() */


int handle_no_show( int hide ) {
   switch( hide ) {
      case 1:
         if( this_player()->query_property( COUNTRIES_NO_SHOW_PROP ) ) {
            tell_object( this_player(), "You are already hidden on the "
               "countries list.\n" );
            return 1;
         }
         this_player()->add_property( COUNTRIES_NO_SHOW_PROP, 1 );
         tell_object( this_player(), "You will no longer be included in "
            "the countries list.\n" );
         return 1;
      case 0:
         if( !this_player()->query_property( COUNTRIES_NO_SHOW_PROP ) ) {
            tell_object( this_player(), "You are already visible on "
               "the countries list.\n" );
            return 1;
         }
         this_player()->remove_property( COUNTRIES_NO_SHOW_PROP );
         tell_object( this_player(), "You will now be shown on the "
            "countries list.\n" );
         return 1;
      default: 
         return notify_fail( "This shouldn't happen.\n" );
   }
} /* handle_no_show() */


mixed *query_patterns() {
   return ({ "", (: cmd() :),
             "hide", (: handle_no_show( 1 ) :),
             "show", (: handle_no_show( 0 ) :)
          });
} /* query_patterns() */

