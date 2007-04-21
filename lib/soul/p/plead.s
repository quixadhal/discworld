(plead
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (single
    (arguments
      (arguments  mercy, frogs, lots of money, a Decstation 5000, 
                  this stupid soul to go away, an X windows terminal, 
                  creators, bil, pinkfish's sanity)
      (self "You plead for $arg$.")
      (rest "$mcname$ plead$s$ for $arg$.")
    )
    (arguments
      (arguments  miserably)
      (self "You plead miserably - you're a miserable pleader.")
      (rest "$mcname$ plead$s$ miserably - $mpronoun$'s a miserable pleader.")
    )
    (arguments
      (arguments  insanity, a momentary lapse of reason, the fifth, 
                  irrationally, helpfully, strategically, #)
      (self "You plead $arg$.")
      (rest "$mcname$ plead$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
       (self   "You plead at $hcname$.")
       (target "$mcname$ plead$s$ at you.")
       (rest   "$mcname$ plead$s$ at $hcname$.")
    )
    (arguments
      (arguments  insanity, a momentary lapse of reason, the fifth, 
                  irrantionally, helpfully, strategically, #)
      (self   "You plead $arg$ at $hcname$.")
      (target "$mcname$ plead$s$ $arg$ at you.")
      (rest   "$mcname$ plead$s$ $arg$ at $hcname$.")
    )
    (arguments
      (arguments  mercy, oranges, apples, beetroots, a lollipop)
      (self   "You plead with $hcname$ for $arg$.")
      (target "$mcname$ plead$s$ with you for $arg$.")
      (rest   "$mcname$ plead$s$ with $hcname$ for $arg$.")
    )
  )
)

