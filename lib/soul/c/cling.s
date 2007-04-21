#  -*- LPC -*- #
#
# $Locker: presto $
# $Id: cling.s,v 1.1 2002/02/01 07:56:01 saffra Exp presto $
#
#
#

(cling
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single    
    (no-arguments
      (self  "You cling on for dear life.")
      (rest  "$mcname$ clings on for dear life.")
    )
    (arguments
      (arguments tightly, passionately, fearfully, possessively, paranoically,   
                 like a pair of socks, like static, by your fingertips, for 
                 dear life, desperately, like a wool sweater, like a groupie, 
                 to the north face of the Uxbridge Road, #)
      (self "You cling on $arg$.")
      (rest "$mcname$ clings on $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You cling onto $hcname$.")
      (target "$mcname$ clings onto you.")
      (rest   "$mcname$ clings onto $hcname$.")
    )
    (arguments
      (arguments tightly, passionately, fearfully, possessively, paranoically,   
                 like a pair of socks, like static, by your fingertips, for 
                 dear life, desperately, like a wool sweater, like a groupie, 
                 to the north face of the Uxbridge Road, #)
      (self   "You cling onto $hcname$ $arg$.")
      (target "$mcname$ clings onto you $arg$.")
      (rest  "$mcname$ cling onto $hcname$ $arg$.")
    )
  )
)
/* Saffra, for idea rep, 2-1-02 */

