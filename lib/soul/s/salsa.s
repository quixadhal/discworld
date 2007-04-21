(salsa
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (targeted
    (no-arguments
      (self   "You dance a salsa with $hcname$.")
      (target "$mcname$ dances a salsa with you.")
      (rest   "$mcname$ dances a salsa with $hcname$.")
    )
    (arguments
      (arguments  quick, fast, stylish, sexy, wonderful, spontaneous)
      (self   "You dance a $arg$ salsa with $hcname$.")
      (target "$mcname$ dances a $arg$ salsa with you.")
      (rest   "$mcname$ dances a $arg$ salsa with $hcname$.")
    )
    (arguments
      (arguments  attractive, amazing, energetic, intriguing)
      (self   "You dance an $arg$ salsa with $hcname$.")
      (target "$mcname$ dances an $arg$ salsa with you.")
      (rest   "$mcname$ dances an $arg$ salsa with $hcname$.")
    )
    (arguments
      (arguments  sensually, stylishly, provocatively, teasingly, 
                  the night away)
      (self   "You salsa $arg$ with $hcname$.")
      (target "$mcname$ salsas $arg$ with you.")
      (rest   "$mcname$ salsa $arg$ with $hcname$.")
    )
    (arguments
      (arguments  with a rose between your teeth, with tequila, 
                  with more tequila, and a cactus, #)
      (self   "You dance a salsa with $hcname$ $arg$.")
      (target "$mcname$ dances a salsa with you $arg$.")
      (rest   "$mcname$ dances a salsa with $hcname$ $arg$.")
    )
  )
)

