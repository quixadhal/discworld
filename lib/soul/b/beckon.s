#  -*- LPC -*- #
#
# $Locker:  $
# $Id: beckon.s,v 1.2 2003/04/24 01:54:11 presto Exp $
#
#
#

(beckon
   (pattern "<string>")
   (pattern "[at] <indirect:any-living>")
   (pattern "[at] <indirect:any-living> <string>")
   (pattern "for <indirect:any-living>")
  (single
    (no-arguments
      (self "You beckon away.")
      (rest "$mcname$ beckon$s$ away.")
    )
    (arguments
      (arguments temptingly, imperially, mysteriously, seductively, regally,
                 impatiently, commandingly, pleadingly, forcefully, beggingly,
                 with one finger, with your little finger, with purple tentacles, 
                 #)
      (self "You beckon $arg$.")
      (rest "$mcname$ beckon$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You beckon $hcname$.")
      (target "$mcname$ beckon$s$ you.")
      (rest  "$mcname$ beckon$s$ $hcname$.")
    )
    (arguments
      (arguments over, temptingly, imperially, mysteriously, seductively, 
                 regally, impatiently, commandingly, pleadingly, forcefully, 
                 beggingly, with one finger, with your little finger, 
                 with purple tentacles, #)
      (self   "You beckon $hcname$ $arg$.")
      (target "$mcname$ beckon$s$ you $arg$.")
      (rest  "$mcname$ beckon$s$ $hcname$ $arg$.")
    )
  )
)

/* Saffra Dec 27 2001 */
