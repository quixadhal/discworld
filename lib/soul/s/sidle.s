(sidle
  (pattern "at <indirect:any-living>")
  (pattern "[like] <indirect:any-living>")
  (pattern "<string>")
  (single
    (arguments
      (arguments like Pinkfish, like Zoroaster, like Biggles, like sloths do, 
                 like a gherkin, like everyone else is, nonchalantly, quickly, 
                 unbelievably well, without using your hands, upside-down,   
                 hyperactively, standing still, artfully, around, intensely,
                  like only Bastian can, like a Lord, #)
      (self "You idle $arg$.")
      (rest "$mcname$ idle$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You idle at $hcname$.")
      (target "$mcname$ idle$s$ at you.")
      (rest  "$mcname$ idle$s$ at $hcname$.")
    )
    (no-arguments
      (self   "You idle like $hcname$.")
      (target "$mcname$ idle$s$ like you.")
      (rest  "$mcname$ idle$s$ like $hcname$.")
    )
  )
)
