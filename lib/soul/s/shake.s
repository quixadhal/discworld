(shake
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "[my] <string>")
  (pattern "<string> [with|at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You shake your head.")
      (rest "$mcname$ shake$s$ $mposs$ head.")
    )
    (arguments
      (arguments sorrowfully, carefully, slowly, once, quickly, curtly, pinkly,        
                 vigorously, completely, fishily, sadly, tentatively, in  
                 agreement, in disbelief)
      (self "You shake your head $arg$.")
      (rest "$mcname$ shake$s$ $mposs$ head $arg$.")
    )
    (arguments
    (arguments booty, entire body, martini, head, hips, bottom, fingers)
      (self "You shake your $arg$.")
      (rest "$mcname$ shake$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You shake $hcname$.")
      (target "$mcname$ shake$s$ you.")
      (rest  "$mcname$ shake$s$ $hcname$.")
    )
    (arguments
      (arguments  roughly, shakily, hard, gently, #)
      (self   "You shake $hcname$ $arg$.")
      (target "$mcname$ shake$s$ you $arg$.")
      (rest   "$mcname$ shake$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  hands with, fins with, fingers with, legs with, pinkies with, 
                  tentacles with, your fist at, claws with, your head at, 
                  your booty at)
      (self   "You shake $arg$ $hcname$.")
      (target "$mcname$ shake$s$ $arg$ you.")
      (rest   "$mcname$ shake$s$ $arg$ $hcname$.")
    )
    (arguments
      (arguments head)
      (self   "You shake your head at $hcname$.")
      (target "$mcname$ shake$s$ $mposs$ head at you.")
      (rest   "$mcname$ shake$s$ $mposs$ head at $hcname$.")
    )
  )
)
