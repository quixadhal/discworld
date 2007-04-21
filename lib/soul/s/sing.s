(sing
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [to] <indirect:any-living>")
  (pattern "[to] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments in appreciation of life,#)
      (self "You sing $arg$.")
      (rest "$mcname$ sing$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You sing to $hcname$.")
      (target "$mcname$ sing$s$ to you.")
      (rest  "$mcname$ sing$s$ to $hcname$.")
    )
   (arguments 
      (arguments in appreciation of life,#)
      (self   "You sing $arg$ to $hcname$.")
      (target "$mcname$ sing$s$ $arg$ to you.")
      (rest   "$mcname$ sing$s$ $arg$ to $hcname$.")
    )
   )
  )