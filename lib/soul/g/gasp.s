(gasp
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> [in] <string>")
  (single
    (arguments
      (arguments in astonishment, in excitement, in terror, in fear, in fright, 
                 in pain, for air, excitedly, #) 
      (self "You gasp $arg$.")
      (rest "$mcname$ gasp$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments in astonishment, in excitement, in terror, in fear, in fright, 
                 in pain, for air, excitedly, #) 
      (self   "You gasp $arg$ at $hcname$.")
      (target "$mcname$ gasp$s$ $arg$ at you.")
      (rest   "$mcname$ gasp$s$ $arg$ at $hcname$.")
    )
  )
)
