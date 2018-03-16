/^\.\\\".*/ { printf("%s\n", $0); next }
/^\..*/ {
# Process the dot command cutely.
# When we get a new .command finish off the old one.
  if (in_header) printf("%s\n", end_header)
  in_header=0
  if ($1 == ".nf") { next }
  if ($1 == ".SH") {
# Section header.
    title=substr($0, length($1)+2, 1)
    for (i=length($1)+3;i<length($0)+1;i++) {
      test=substr($0, i, 1)
      if (test == "A") {
        test="a"
      } else if (test == "B") {
        test="b"
      } else if (test == "C") {
        test="c"
      } else if (test == "D") {
        test="d"
      } else if (test == "E") {
        test="e"
      } else if (test == "F") {
        test="f"
      } else if (test == "G") {
        test="g"
      } else if (test == "H") {
        test="h"
      } else if (test == "I") {
        test="i"
      } else if (test == "J") {
        test="j"
      } else if (test == "K") {
        test="k"
      } else if (test == "L") {
        test="l"
      } else if (test == "M") {
        test="m"
      } else if (test == "N") {
        test="n"
      } else if (test == "O") {
        test="o"
      } else if (test == "P") {
        test="p"
      } else if (test == "Q") {
        test="q"
      } else if (test == "R") {
        test="r"
      } else if (test == "S") {
        test="s"
      } else if (test == "T") {
        test="t"
      } else if (test == "U") {
        test="u"
      } else if (test == "V") {
        test="v"
      } else if (test == "W") {
        test="w"
      } else if (test == "X") {
        test="x"
      } else if (test == "Y") {
        test="y"
      } else if (test == "Z") {
        test="z"
      }
      title=sprintf("%s%s", title, test)
    }
    if (title == "Description") {
      printf("%s\n%s\n", title, ".SP 5 5")
      end_header=".EP"
    } else {
      printf("%s\n%s\n", title, ".SI 5")
      end_header=".EI"
    }
    in_header=1
    next;
  }
  if ($1 == ".TH") {
    title=substr($2, 1, 1)
    for (i=2;i<length($2)+1;i++) {
      test=substr($2, i, 1)
      if (test == "A") {
        test="a"
      } else if (test == "B") {
        test="b"
      } else if (test == "C") {
        test="c"
      } else if (test == "D") {
        test="d"
      } else if (test == "E") {
        test="e"
      } else if (test == "F") {
        test="f"
      } else if (test == "G") {
        test="g"
      } else if (test == "H") {
        test="h"
      } else if (test == "I") {
        test="i"
      } else if (test == "J") {
        test="j"
      } else if (test == "K") {
        test="k"
      } else if (test == "L") {
        test="l"
      } else if (test == "M") {
        test="m"
      } else if (test == "N") {
        test="n"
      } else if (test == "O") {
        test="o"
      } else if (test == "P") {
        test="p"
      } else if (test == "Q") {
        test="q"
      } else if (test == "R") {
        test="r"
      } else if (test == "S") {
        test="s"
      } else if (test == "T") {
        test="t"
      } else if (test == "U") {
        test="u"
      } else if (test == "V") {
        test="v"
      } else if (test == "W") {
        test="w"
      } else if (test == "X") {
        test="x"
      } else if (test == "Y") {
        test="y"
      } else if (test == "Z") {
        test="z"
      }
      title=sprintf("%s%s", title, test)
    }
    printf("%s\n%s\n%s\n%s\n", ".DT", title, "Discworld driver help", title);
    next
  }
# Hmmm.  Unknown . command.  cringe....
  printf("%s\n", $0)
  next
}
{
  printf("%s\n", $0)
}
END { if (in_header) print end_header; }
