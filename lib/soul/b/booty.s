#  -*- LPC -*- #
#
# $Locker:  $
# $Id: booty.s,v 1.2 2003/04/24 01:54:11 presto Exp $
#
#
#

(booty
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<string> <indirect:any-living>" )
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You shake your booty.")
      (rest "$mcname$ shake$s$ $mposs$ booty.")
    )
    (arguments
    (arguments  all over the place, like a martini, like a mound of jello, 
                like a dancing queen, #)
      (self "You shake your booty $arg$.")
      (rest "$mcname$ shake$s$ $mposs$ booty $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You shake your booty at $hcname$.")
      (target "$mcname$ shake$s$ $mposs$ booty at you.")
      (rest   "$mcname$ shake$s$ $mposs$ booty at $hcname$.")
    )
    (arguments
    (arguments  all over the place, like a martini, like a mound of jello, 
                like a dancing queen, #)
      (self   "You shake your booty at $hcname$ $arg$.")
      (target "$mcname$ shake$s$ $mposs$ booty at you $arg$.")
      (rest   "$mcname$ shake$s$ $mposs$ booty at $hcname$ $arg$.")
    )
  )
)
/* Saffra Sun July 28 2001 */
