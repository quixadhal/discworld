(wrinkle
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You wrinkle your forehead.")
      (rest "$mcname$ wrinkles $mposs$ forehead.")
    )
    (arguments
      (arguments  nose, eyes, face, brow, leg, cat, prunes, time, 
                  cheese grater, #)
      (self "You wrinkle your $arg$.")
      (rest "$mcname$ wrinkle$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You wrinkle your forehead at $hcname$.")
      (target "$mcname$ wrinkle$s$ $mposs$ forehead at you.")
      (rest   "$mcname$ wrinkle$s$ $mposs$ forehead at $hcname$.")
    )
    (arguments
      (arguments  nose, eyes, face, brow, leg, cat, prunes, time, 
                  cheese grater, #)
      (self   "You wrinkle your $arg$ at $hcname$.")
      (target "$mcname$ wrinkle$s$ $mposs$ $arg$ at you.")
      (rest   "$mcname$ wrinkle$s$ $mposs$ $arg$ at $hcname$.")
    )
  )
)

