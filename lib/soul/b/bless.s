(bless
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You bless $hcname$.")
      (target "$mcname$ bless$s$ you.")
      (rest  "$mcname$ bless$s$ $hcname$.")
    )
    (arguments
      (arguments  benignly,hypocritically,in the name of Pishe,in the name of Pinkfish,with your frog,#)
      (self   "You bless $hcname$ $arg$.")
      (target "$mcname$ bless$s$ you $arg$.")
      (rest  "$mcname$ bless$s$ $hcname$ $arg$.")
    )
  )
)

/* Implemented by Hobbes and suggested by Gruper 970419 */
