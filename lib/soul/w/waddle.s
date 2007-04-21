(waddle
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You waddle.")
      (rest "$mcname$ waddle$s$.")
    )
    (arguments
      (arguments around, slowly, heavily, roundly, rollingly, like a duck, 
                 like a penguin, like a wizard, #)
      (self "You waddle $arg$.")
      (rest "$mcname$ waddle$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You waddle at $hcname$.")
      (target "$mcname$ waddle$s$ at you.")
      (rest  "$mcname$ waddle$s$ at $hcname$.")
    )
    (arguments
      (arguments slowly, heavily, roundly, rollingly, like a duck, 
                 like a penguin, like a wizard, #)
      (self   "You waddle $arg$ at $hcname$.")
      (target "$mcname$ waddle$s$ $arg$ at you.")
      (rest  "$mcname$ waddle$s$ $arg$ at $hcname$.")
    )
    (arguments
      (arguments around, away from, up to, over, under)
      (self   "You waddle $arg$ $hcname$.")
      (target "$mcname$ waddle$s$ $arg$ you.")
      (rest  "$mcname$ waddle$s$ $arg$ $hcname$.")
    )
  )
)
