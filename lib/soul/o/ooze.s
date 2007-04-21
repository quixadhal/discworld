(ooze
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<indirect:any-living> [at] <string>")
  (pattern "<string> [at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You ooze.")
      (rest "$mcname$ ooze$s$.")
    )
    (arguments
      (arguments all over, coffee, capuccino, soda, tea, sweat, 
                 testosterone, estrogen, blood, saliva, guts, slime,
                 bodily fluids, charm, alcohol, code, #)
      (self "You ooze $arg$.")
      (rest "$mcname$ ooze$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You ooze all over $hcname$.")
      (target "$mcname$ ooze$s$ all over you.")
      (rest   "$mcname$ ooze$s$ all over $hcname$.")
    )
    (arguments
      (arguments coffee, capuccino, soda, tea, sweat, testosterone, 
                 estrogen, blood, saliva, guts, slime, bodily fluids, 
                 charm, alcohol, code, #)
      (self   "You ooze $arg$ all over $hcname$.")
      (target "$mcname$ ooze$s$ $arg$ all over you.")
      (rest   "$mcname$ ooze$s$ $arg$ all over $hcname$.")
    )
  )
)
/* Saffra wrote this in early 2000 sometime. */