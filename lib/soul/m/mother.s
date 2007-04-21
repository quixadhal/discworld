(mother
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>") 
  (targeted
    (no-arguments
      (self   "You mother $hcname$.")
      (target "$mcname$ mother$s$ you.")
      (rest   "$mcname$ mother$s$ $hcname$.")
    )
    (arguments
      (arguments  fussily, happily, lovingly, caringly, 
                  carefully, worriedly, dotingly, 
                  like your mother would, like your father would, #)
      (self   "You mother $hcname$ $arg$.")
      (target "$mcname$ mother$s$ you $arg$.")
      (rest   "$mcname$ mother$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  with a wooden spoon)
      (self   "You mother $hcname$ $arg$.")
      (target "$mcname$ mother$s$ you $arg$.  Whap!  Whap!  Whap!")
      (rest   "$mcname$ mother$s$ $hcname$ $arg$.")
    )
  )
)

