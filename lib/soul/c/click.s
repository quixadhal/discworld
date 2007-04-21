(click
  (pattern "[at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "<string>")
  (single
    (no-arguments
      (self "You click your fingers.")
      (rest "$mcname$ clicks $mposs$ fingers.")
    )
    (arguments
      (arguments slowly, loudly, happily, quickly, like Bertie Beetle, #)
      (self "You click $arg$.")
      (rest "$mcname$ clicks $arg$.")
    )      
  )
  (targeted
    (no-arguments
      (self "You click your fingers at $hcname$.")
      (target "$mcname$ clicks $mposs$ fingers at you.")
      (rest "$mcname$ clicks $mposs$ fingers at $hcname$.")
    )
    (arguments
      (arguments slowly, loudly, happily, quickly, like Bertie Beetle, #)
      (self "You click $arg$ at $hcname$.")
      (target "$mcname$ clicks $arg$ at you.")
      (rest "$mcname$ clicks $arg$ at $hcname$.")
    )      
  )
)
