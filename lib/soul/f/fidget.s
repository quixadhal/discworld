(fidget
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You fidget." )
      (rest "$mcname$ fidget$s$." )
    )
    (arguments
      (arguments violently, spasmodically, suddenly, frantically, 
                 pointedly, from too much caffeine, from too much crack, #)
      (self "You fidget $arg$.")
      (rest "$mcname$ fidget$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You fidget at $hcname$." )
      (target "$mcname$ fidget$s$ at you." )
      (rest   "$mcname$ fidget$s$ at $hcname$." )
    )   
    (arguments
      (arguments violently, spasmodically, suddenly, frantically, 
                 pointedly, from too much caffeine, from too much crack, #)
      (self   "You fidget $arg$ at $hcname$.")
      (target "$mcname$ fidgetv $arg$ at you.")
      (rest  "$mcname$ fidget$s$ $arg$ at $hcname$.")
    )
  )
)

/* Saffra 2-21-00 */

