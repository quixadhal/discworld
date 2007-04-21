(raise  
  (pattern "<indirect:any-living>")
  (pattern "<string> [against] <indirect:any-living>")
  (pattern "<indirect:any-living> [against] <string>")
  (pattern "<string>")
  (single
    (no-arguments
      (self "You raise your eyebrows.")
      (rest "$mcname$ raise$s$ $mposs$ eyebrows.")
    )
    (arguments
    (arguments  eyebrows, hand, finger, leg, foot, hat, bet, ante, #)
      (self "You raise your $arg$.")
      (rest "$mcname$ raise$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You raise your eyebrows at $hcname$.")
      (target "$mcname$ raise$s$ $mposs$ eyebrows at you.")
      (rest   "$mcname$ raise$s$ $mposs$ eyebrows at $hcname$.")
    )
    (arguments
    (arguments  eyebrows, hand, finger, leg, foot, hat, bet, ante, #)
      (self   "You raise your $arg$ at $hcname$.")
      (target "$mcname$ raise$s$ $mposs$ $arg$ at you.")
      (rest   "$mcname$ raise$s$ $mposs$ $arg$ at $hcname$.")
    )
  )
)
