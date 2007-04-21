(flap
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (single
    (arguments
      (arguments ears, arms, wings, eyelashes, dangly bits, upper arms,
                 wings, laundry, flag, #)
      (self "You flap your $arg$.")
      (rest "$mcname$ flap$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments ears, arms, wings, eyelashes, dangly bits, upper arms,
                 wings, laundry, flag, #)
      (self   "You flap your $arg$ at $hcname$.")
      (target "$mcname$ flap$s$ $mposs$ $arg$ at you.")
      (rest   "$mcname$ flap$s$ $mposs$ $arg$ at $hcname$.")
    )
  )
)

