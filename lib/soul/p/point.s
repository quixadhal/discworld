(point
  (pattern "[at] <string>")
  (pattern "[at] <indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You point.")
      (rest "$mcname$ points.")
    )
    (arguments
      (arguments north, south, east, west, up, down, around, nowhere in 
                 particular, in two directions at once, #)
      (self "You point $arg$.")
      (rest "$mcname$ point$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You point at $hcname$.")
      (target "$mcname$ point$s$ at you.")
      (rest  "$mcname$ point$s$ at $hcname$.")
    )
    (arguments
      (arguments baton, pencil, wand, broom, sword, finger, thumb, quill,     
                 pointy hat, argument, breadstick, Irish setter, #)
      (self   "You point your $arg$ at $hcname$.")
      (target "$mcname$ point$s$ $mposs$ $arg$ at you.")
      (rest   "$mcname$ point$s$ $mposs$ $arg$ at $hcname$.")
    )
    (arguments
      (arguments accusingly, straight, right, pointingly)
      (self   "You point $arg$ at $hcname$.")
      (target "$mcname$ point$s$ $arg$ at you.")
      (rest   "$mcname$ point$s$ $arg$ at $hcname$.")
    )  
  )
)

