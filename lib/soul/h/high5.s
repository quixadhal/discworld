(high5
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You high five.")
      (rest "$mcname$ high five$s$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You high five $hcname$.")
      (target "$mcname$ high five$s$ you.")
      (rest  "$mcname$ high five$s$ $hcname$.")
    )
  (arguments
    (arguments happily,annoyingly,courteously,hysterically,absentmindedly)
      (self "You high five $hcname$.")
      (target "$mcname$ high five$s$ you $arg$.")
      (rest "$mcname$ high five$s$ $hcname$ $arg$.")	
	       )
    )
 )

/* Changed by Hobbes, 961201 */