(sneer
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self   "You sneer.")
      (rest   "$mcname$ sneer$s$.")
    )
    (arguments
      (arguments  with malice, maliciously, with attitude, superciliously, 
                  exaggeratedly, like an adolescent, #)
      (self   "You sneer $arg$.")
      (rest   "$mcname$ sneer$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You sneer at $hcname$.")
      (target "$mcname$ sneer$s$ at you.")
      (rest   "$mcname$ sneer$s$ at $hcname$.")
    )
    (arguments
      (arguments  with malice, maliciously, with attitude, superciliously, 
                  exaggeratedly, like an adolescent, #)
      (self   "You sneer $arg$ at $hcname$.")
      (target "$mcname$ sneer$s$ $arg$ at you.")
      (rest   "$mcname$ sneer$s$ $arg$ at $hcname$.")
    )
  )
)

