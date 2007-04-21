(squirm
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [to] <indirect:any-living>")
  (pattern "[to] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You squirm.")
      (rest "$mcname$ squirm$s$.")
    )
    (arguments
    (arguments uncomfortably, antsily, guiltily, damply, impatiently, 
               helplessly, suggestively, spinelessly, nervously, sexily,  
               politely, idly, lewdly, under pressure, in Quirm, with joy, 
               like a worm, under the rug, #)
      (self "You squirm $arg$.")
      (rest "$mcname$ squirm$s$ $arg$.")
    )
    (arguments
      (arguments seat)
      (self "You squirm in your seat.")
      (rest "$mcname$ squirm$s$ in $mposs$ seat.")
    )
  )
  (targeted
    (no-arguments
      (self   "You squirm at $hcname$.")
      (target "$mcname$ squirm$s$ at you.")
      (rest  "$mcname$ squirm$s$ at $hcname$.")
    )
    (arguments 
      (arguments uncomfortably, antsily, guiltily, damply, impatiently, 
                 helplessly, suggestively, spinelessly, nervously, sexily,  
                 politely, idly, lewdly, under pressure, in Quirm, with joy, 
                 like a worm, under the rug, #)
      (self   "You squirm $arg$ at $hcname$.")
      (target "$mcname$ squirm$s$ $arg$ at you.")
      (rest   "$mcname$ squirm$s$ $arg$ at $hcname$.")
    )
  )
)

