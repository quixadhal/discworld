#  -*- LPC -*- #
#
# $Locker:  $
# $Id: tapdance.s,v 1.1 2002/05/09 03:58:27 saffra Exp $
#
#
#

(tapdance
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You tapdance around like a centipede on acid.")
      (rest "$mcname$ tapdances around like a centipede on acid.")
    )
    (arguments
    (arguments across the room, to the Rim and back, to CWC and back, over a
               dead body, over Dogbolter's dead body, with new shiny shoes,         
               manically, madly, stylishly, like a duck, like Fred  
               and Ginger, without falling in the sink,  # )
      (self "You tapdance around $arg$.")
      (rest "$mcname$ tapdances around $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You tapdance around $hcname$ like a centipede on acid.")
      (target "$mcname$ tapdances around you like a centipede on acid.")
      (rest  "$mcname$ tapdances around $hcname$ like a centipede on acid.")
    )
    (arguments
    (arguments across the room, to the Rim and back, to CWC and back, over a
               dead body, over Dogbolter's dead body, with new shiny shoes,         
               manically, madly, stylishly, like a duck, like Fred  
               and Ginger, without falling in the sink,  # )
      (self   "You tapdance around $hcname$ $arg$.")
      (target "$mcname$ tapdances around you $arg$.")
      (rest  "$mcname$ tapdances around $hcname$ $arg$.")
    )
    (arguments
    (arguments grave)
      (self   "You tapdance on $mhcname$ grave.")
      (target "$mcname$ tapdances on your grave.")
      (rest   "$mcname$ tapdances on $mhcname$ grave.")
    )
  )
)
