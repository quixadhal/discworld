#  -*- LPC -*- #
#
# $Locker:  $
# $Id: zipzip.s,v 1.2 2003/04/16 04:20:14 saffra Exp $
#
#
#

(zipzip
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You zip zip zippity zip around the room.")
      (rest "$mcname$ zip zip zippity zip$s$ around the room.")
    )
  )
  (targeted
    (no-arguments
      (self   "You zip zip zippity zip around $hcname$.")
      (target "$mcname$ zip zip zippity zip$s$ around you.")
      (rest  "$mcname$ zip zip zippity zip$s$ around $hcname$.")
    )
  )
)

