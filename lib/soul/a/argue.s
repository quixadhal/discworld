(argue
  (pattern "<string>")
  (pattern "[with] <indirect:any-living>")
  (pattern "<string> [with] <indirect:any-living>")
  (pattern "[with] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You argue the point.")
      (rest "$mcname$ argue$s$ the point.")
    )
    (arguments
      (arguments  happily, using big words, teasingly, flirtatiously,
                  nonchalantly, cunningly, illogically, mercilessly,
                  that the world's about to end, #)
      (self "You argue $arg$.")
      (rest "$mcname$ argue$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You argue with $hcname$.")
      (target "$mcname$ argue$s$ with you.")
      (rest   "$mcname$ argue$s$ with $hcname$.")
    )
    (arguments
      (arguments  the point, happily, using big words, teasingly, flirtatiously,
                  nonchalantly, cunningly, illogically, mercilessly,
                  that the world's about to end)
      (self   "You argue $arg$ with $hcname$.")
      (target "$mcname$ argue$s$ $arg$ with you.")
      (rest   "$mcname$ argue$s$ $arg$ with $hcname$.")
    )
    (arguments
      (arguments  the unfairness of it all, what happened,
                  whose fault it is,  trivial things,
                  whose turn it is to do the washing up, 
                  what colour the psychedelic frogs were, #)
      (self   "You argue with $hcname$ about $arg$.")
      (target "$mcname$ argue$s$ with you about $arg$.")
      (rest   "$mcname$ argue$s$ with $hcname$ about $arg$.")
    )
    (arguments
      (arguments  frog, womble)
      (self   "You argue that $hcname$ isn't a $arg$.")
      (target "$mcname$ argue$s$ that you aren't a $arg$.")
      (rest   "$mcname$ argue$s$ that $hcname$ isn't a $arg$.")
    )
  )
)

