#  -*- LPC -*- #
#
# $Locker: presto $
# $Id: clear.s,v 1.1 2001/12/27 11:34:28 saffra Exp presto $
#
#
#

(clear
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You clear your throat.")
      (rest "$mcname$ clears $mposs$ throat.")
    )
    (arguments
      (arguments loudly, quietly, politely, thoroughly, impatiently, 
                 unobtrusively, rudely, phlegmatically, #)
      (self "You clear your throat $arg$.")
      (rest "$mcname$ clears $mposs$ throat $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments loudly, quietly, politely, thoroughly, impatiently, 
                 unobtrusively, rudely, phlegmatically, #)
      (self   "You clear your throat $arg$ at $hcname$.")
      (target "$mcname$ clears $mposs$ throat $arg$ at you.")
      (rest  "$mcname$ clears $mposs$ throat $arg$ at $hcname$.")
    )
  )
)

/* Saffra Dec 27 2001 */