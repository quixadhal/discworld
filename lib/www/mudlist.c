#include <lib.h>
#include <privs.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmp(string a, string b) {
    if(lower_case(a) < lower_case(b))
        return -1;
    else if(lower_case(a) > lower_case(b))
        return 1;
    else
        return 0;
}

varargs string gateway(int strip_html) {
    string output = "";
    mapping mudlist = ([]);
    string mud = "";
    mixed *info = ({});
    string *list = ({});
    string *bg = ({ "#FFFFFF", "#DFFFDF" });
    int bgcolor = 0;
    string *keylist = ({});
    int i = 0;

    foreach( mud, info in INTERMUD_D->GetMudList() ) {
        if(info[0] == -1) {
            mudlist[mud] = info;
        }
    }

    //keylist = sort_array(keys(mudlist), 1);
    //keylist = sort_array(keys(mudlist), (: return lower_case($1) < lower_case($2) ? -1 : ( lower_case($1) > lower_case($2) ? 1 : 0 ); :) );
    keylist = sort_array(keys(mudlist), "cmp", this_object());

    for(i = 0; i < sizeof(keylist); i++) {
        mud = keylist[i];
        info = mudlist[mud];
        bgcolor = !bgcolor;
        list += ({ sprintf(
                    "<td bgcolor=\"%s\">%s</td>" + 
                    "<td bgcolor=\"%s\">%s</td>" +
                    "<td bgcolor=\"%s\">%s</td>" +
                    "<td bgcolor=\"%s\">%s</td>" +
                    "<td bgcolor=\"%s\">%s</td>" +
                    "<td align=\"right\" bgcolor=\"%s\">%s</td>",
                    bg[bgcolor], "<a href=\"http://www.awasteoftime.net/~bloodlines/i3log.php?mf=" + strip_colors(mud) + "\"> " + replace_string(strip_colors(mud), " ", "&nbsp;") + " </a>",
                    bg[bgcolor], replace_string(info[8], " ", "&nbsp;"),
                    bg[bgcolor], replace_string(info[7], " ", "&nbsp;"),
                    bg[bgcolor], replace_string(info[5], " ", "&nbsp;"),
                    bg[bgcolor], "<a href=\"http://" + info[1] + "/\"> " + info[1] + " </a>",
                    bg[bgcolor], "<a href=\"telnet://" + info[1] + ":" + info[2] + "/\"> " + info[2] + " </a>") });
    }
    output  = "<div align=\"center\"><table width=\"90%\" border=\"1\" cellspacing=\"0\" padding=\"0\">\n";
    output += "<tr class=\"header\"><td align=\"center\" colspan=\"6\">" + mud_name() + " recognizes " + consolidate(sizeof(mudlist), "a mud")+ ":</td></tr>\n";
    output += "<tr class=\"entry\">" + implode(list, "</tr>\n<tr class=\"entry\">") + "</tr>\n";
    output += "</table></div>\n";
    return output;
}

