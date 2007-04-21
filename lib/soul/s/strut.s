(strut
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You strut your stuff.")
      (rest "$mcname$ strut$s$ proudly around the room.")
    )
    (arguments
    (arguments proudly, egotistically, sarcastically, mockingly, 
               like a rooster, like an assassin, like a seamstress, 
               like a stray cat, #)
      (self "You strut $arg$.")
      (rest "$mcname$ strut$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You strut your stuff at $hcname$.")
      (target "$mcname$ strut$s$ $mposs$ stuff at you.")
      (rest "$mcname$ strut$s$ $mposs$ stuff at $hcname$.")
    )
    (arguments
    (arguments proudly, egotistically, sarcastically, mockingly, 
               like a rooster, like an assassin, like a seamstress, 
               like a stray cat, #)
      (self "You strut $arg$ at $hcname$.")
      (target "$mcname$ strut$s$ $arg$ at you.")
      (rest "$mcname$ strut$s$ $arg$ at $hcname$.")    
    )
  )
)
