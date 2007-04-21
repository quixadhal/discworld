(side5
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You side five.")
      (rest "$mcname$ side five$s$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You side five $hcname$.")
      (target "$mcname$ side five$s$ you.")
      (rest  "$mcname$ side five$s$ $hcname$.")
    )
  (arguments
    (arguments happily,annoyingly,courteously,hysterically,absentmindedly)
      (self "You side five $hcname$.")
      (target "$mcname$ side five$s$you $arg$.")
      (rest "$mcname$ side five$s$ $hcname$ $arg$.")	
	
	
       )
    )
 )
/* Saffra for Dek Aug 12 2001 */

