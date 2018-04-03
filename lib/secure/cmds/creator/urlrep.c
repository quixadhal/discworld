/*  -*- LPC -*-  */

/*
 * A command to display data about the URL's that
 * URLbot has services for I3.
 */

inherit "/cmds/base";
#include <creator.h>
#include <network.h>

mapping url_data;

mixed cmd(string arg) {
    string *bits;
    int count;

    if(!arg) {
        return notify_fail("urlrep --list\n");
    } else {
        bits = explode(arg, " ") - ({0,""});
        switch(lower_case(bits[0])) {
            case "list":
            case "-list":
            case "--list":
            case "-l":
                tell_object(this_player(), sprintf("\n%-10.10s %-15s %-25s %-5s %s\n",
                            "Date", "Channel", "Speaker", "Count", "URL Info"));
                tell_object(this_player(), sprintf("%-10.10s %-15s %-25s %-5s %s\n",
                            "----------", "---------------",
                            "-------------------------", "-----",
                            "------------------------------"));
                url_data = SERVICES_D->getUrlData();
                count = 0;
                foreach( string url in sort_array( keys(url_data),
                           (: url_data[$1]["time"] <= url_data[$2]["time"] ? -1 : 1 :)) ) {
                    string result = url_data[url]["result"];
                    int pos;

                    if((pos = strsrch(result, " is ")) >= 0) {
                        // We probably found title information
                        result = result[(pos + 4)..];
                    } else if((pos = strsrch(result, " :: ")) >= 0) {
                        // This should catch any tinyurl forms that my script generates.
                        int pos2;

                        if((pos2 = strsrch(result[0..pos], "%^GREEN%^http")) >= 0) {
                            result = result[pos2..(pos)] + "%^RESET%^";
                        }
                    //} else if((pos = strsrch(result, "http://tinyurl.com/")) >= 0) {
                    //    // We got a tinyurl anyways
                    //    result = "%^GREEN%^" + result[pos..(pos+27)] + "%^RESET%^";
                    }
                    tell_object(this_player(), sprintf("%-10.10s %-15s %-25s %5d %s\n",
                                SERVICES_D->timestamp(url_data[url]["time"]),
                                SERVICES_D->GetLocalChannel(url_data[url]["channel"]),
                                implode( ({ url_data[url]["user"], url_data[url]["mud"] }), "@"),
                                url_data[url]["counter"],
                                result
                                ));
                    /*
                    tell_object(this_player(), sprintf("%-10.10s %-15s %-25s %5s %-30.30s\n",
                                "",
                                "",
                                "",
                                "",
                                url
                                ));
                     */
                    count++;
                }
                tell_object(this_player(), sprintf("\nURLbot processed %s unique url%s.\n",
                            count == 0 ? "no" : 
                            count == 1 ? "one" : query_num(count),
                            count == 1 ? "" : "s"
                            ));
                break;
            default:
                return notify_fail("urlrep --list\n");
                break;
        }
    }
    return 1;
}
