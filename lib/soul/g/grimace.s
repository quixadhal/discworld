(grimace
  (pattern "<indirect:any-living>")
  (pattern "[in] <string>")
  (pattern "<string> [in] <string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
        (self "You grimace.")
        (rest "$mcname$ grimace$s$.")
        )
      (arguments 
        (arguments disgust, anger, thought, power, despair)
      (self "You grimace in $arg$.")
      (rest "$mcname$ grimace$s$ in $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You grimace at $hcname$.")
      (target "$mcname$ grimace$s$ at you.")
      (rest  "$mcname$ grimace$s$ at $hcname$.")
       )
      (arguments 
        (arguments disgust, anger, thought, power, despair, #)
      (self   "You grimace in $arg$ at $hcname$.")
      (target "$mcname$ grimace$s$ in $arg$ at you.")
      (rest   "$mcname$ grimace$s$ in $arg$ at $hcname$.")
    )
  )
)
/*Hobbes 970317*/
