(glow
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
    (arguments in the dark, incandescently, fluorescently, brightly, dimly, 
               like a blacklight, flourescent orange, like the stars on a 
               moonless night, like a beckoning star, like an ingenue, like an 
               escaped spell, like a firefly, from within, like a glowworm, 
               like a candle in the darkness, like a cheap special effect, like 
               a glow-in-the-dark Sonkie, #)
      (self "You glow $arg$.")
      (rest "$mcname$ glow$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You glow at $hcname$.")
      (target "$mcname$ glow$s$ at you.")
      (rest   "$mcname$ glow$s$ at $hcname$.")
    )
    (arguments
    (arguments in the dark, incandescently, fluorescently, brightly, dimly, 
               like a blacklight, flourescent orange, like the stars on a 
               moonless night, like a beckoning star, like an ingenue, like an 
               escaped spell, like a firefly, from within, like a glowworm, 
               like a candle in the darkness, like a cheap special effect, like 
               a glow-in-the-dark Sonkie, #)
      (self   "You glow at $hcname$ $arg$.")
      (target  "$mcname$ glow$s$ at you $arg$.")
      (rest  "$mcname$ glow$s$ at $hcname$ $arg$.")
    )
  )
)

/* Saffra 2-01-02 For idea rep */
