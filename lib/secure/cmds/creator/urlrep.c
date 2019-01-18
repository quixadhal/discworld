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
    string extra;
    int *bins;
    int extended = 0;

    if(!arg) {
        return notify_fail("urlrep --list | --remove <url>\n");
    } else {
        bits = explode(arg, " ") - ({0,""});
        extra = implode(bits[1..], " ");
        switch(lower_case(bits[0])) {
            case "count":
            case "-count":
            case "--count":
            case "-c":
                url_data = SERVICES_D->getUrlData();
                count = 0;
                bins = ({ });
                foreach( string checksum in sort_array( keys(url_data),
                           (: url_data[$1]["time"] <= url_data[$2]["time"] ? -1 : 1 :)) ) {
                    count = url_data[checksum]["counter"];
                    bins[count]++;
                }
                foreach( int i in bins ) {
                    tell_object(this_player(), sprintf("\n%d urls were found %d times.\n",
                                i, bins[i]));
                }
                break;
            case "extended":
            case "-extended":
            case "--extended":
            case "-x":
                extended = 1;
            case "list":
            case "-list":
            case "--list":
            case "-l":
                if(!undefinedp(extra) && extra == "url") {
                    tell_object(this_player(), sprintf("\n%-10.10s %-35s %-5s %-8s %s\n",
                                "Date", "Info", "Count", "Checksum", "URL"));
                    tell_object(this_player(), sprintf("%-10.10s %-35.35s %-5s %-8s %s\n",
                                "----------", "-----------------------------------",
                                "-----", "--------", "--------------------------"));
                } else {
                    tell_object(this_player(), sprintf("\n%-10.10s %-15s %-25s %-5s %-8s %s\n",
                                "Date", "Channel", "Speaker", "Count", "Checksum", "Title"));
                    tell_object(this_player(), sprintf("%-10.10s %-15s %-25s %-5s %-8s %s\n",
                                "----------", "---------------",
                                "-------------------------", "-----", "--------",
                                "--------------------------"));
                }
                url_data = SERVICES_D->getUrlData();
                count = 0;
                foreach( string checksum in sort_array( keys(url_data),
                           (: url_data[$1]["time"] <= url_data[$2]["time"] ? -1 : 1 :)) ) {
                    string url = url_data[checksum]["url"];
                    string result = url_data[checksum]["result"];
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
                    result = strip_colours(result);
                    if(!undefinedp(extra) && extra == "url") {
                        // sprintf is buggy as hell for formatting...
                        string tmp_output;
                        string tmp_time = SERVICES_D->timestamp(url_data[checksum]["time"]);
                        string tmp_result = result + "------------------------------";
                        string tmp_count = "     " + url_data[checksum]["counter"];
                        tmp_time = tmp_time[0..9];
                        tmp_result = tmp_result[0..34];
                        tmp_count = tmp_count[<5..<1];
                        tmp_output = tmp_time + " " + tmp_result + " " + tmp_count + " " + checksum + " " +url + "\n";
                        tell_object(this_player(), tmp_result + "\n");
                        /*
                        tell_object(this_player(), sprintf("%-:10s %-:25s %:5d %s\n",
                                    SERVICES_D->timestamp(url_data[url]["time"]),
                                    result,
                                    url_data[url]["counter"],
                                    url
                                    ));
                                    */
                    } else {
                        tell_object(this_player(), sprintf("%-10.10s %-15s %-25s %5d %8s %s\n",
                                    SERVICES_D->timestamp(url_data[checksum]["time"]),
                                    //SERVICES_D->GetLocalChannel(url_data[checksum]["channel"]),
                                    url_data[checksum]["channel"],
                                    implode( ({ url_data[checksum]["user"], url_data[checksum]["mud"] }), "@"),
                                    url_data[checksum]["counter"], checksum,
                                    result
                                    ));
                    }
                    if(extended) {
                        tell_object(this_player(), "URL: " + url + "\n");
                    }
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
            case "delete":
            case "-delete":
            case "--delete":
            case "remove":
            case "-remove":
            case "--remove":
                url_data = SERVICES_D->getUrlData();

                if(undefinedp(url_data[extra])) {
                    tell_object(this_player(), sprintf("\nURL %s not found by URLbot.\n", extra));
                } else {
                    tell_object(this_player(), sprintf("\nURL %s deleted from URLbot.\n", extra));
                    SERVICES_D->deleteUrl(extra);
                }
                break;
            case "wipe":
            case "-wipe":
            case "--wipe":
                tell_object(this_player(), sprintf("\nALL URL data deleted!\n"));
                SERVICES_D->wipeUrls();

            default:
                return notify_fail("urlrep --list | --delete <url>\n");
                break;
        }
    }
    return 1;
}
