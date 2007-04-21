(twirl
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You twirl around.")
      (rest "$mcname$ twirls around.")
    )
    (arguments
    (arguments in circles, stylishly, like a dervish, like a Morris Dancer, 
               a baton, a lock of hair, #)
      (self "You twirl $arg$.")
      (rest "$mcname$ twirl$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You twirl $hcname$ round in circles.")
      (target "$mcname$ twirl$s$ you round in circles.")
      (rest   "$mcname$ twirl$s$ $hcname$ round in circles.")
    )
    (arguments
    (arguments  stylishly, like a dervish, like a Morris Dancer, 
               a baton, a lock of hair, #)
      (self   "You twirl $hcname$ round $arg$.")
      (target "$mcname$ twirl$s$ you round $arg$.")
      (rest   "$mcname$ twirl$s$ $hcname$ round $arg$.")
    )
  )
)
/* Modified by Saffra Oct 09 2001 */