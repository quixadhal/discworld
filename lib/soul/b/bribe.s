#  -*- LPC -*- #
#
# $Locker:  $
# $Id: bribe.s,v 1.2 2003/04/24 01:54:11 presto Exp $
#
#
#

(bribe
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<indirect:any-living> [with] <string>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You attempt to bribe yourself.  How odd.")
      (rest "$mcname$ attempt$s$ to bribe $mobj$self.  How odd.")
    )
    (arguments
      (arguments money, fame, fortune, riches, power, untold power, 
                 lots of good lovin', nutella, chocolate, favours, 
                 political favours, special favours, jewels, untold treasures, 
                 #)
      (self "You attempt to bribe yourself with $arg$.")
      (rest "$mcname$ attempt$s$ to bribe $mobj$self with $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You bribe $hcname$.")
      (target "$mcname$ bribe$s$ you.")
      (rest "$mcname$ bribe$s$ $hcname$.")
    )
    (arguments
      (arguments money, fame, fortune, riches, power, untold power, 
                 lots of good lovin', nutella, chocolate, favours, 
                 political favours, special favours, jewels, untold treasures, 
                 #)
      (self   "You bribe $hcname$ with $arg$.")
      (target "$mcname$ bribe$s$ you with $arg$.")
      (rest  "$mcname$ bribe$s$ $hcname$ with $arg$.")
    )
  )
)

/* Saffra Dec 27 2001 */
