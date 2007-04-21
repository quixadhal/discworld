(daydream
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You daydream.")
      (rest "$mcname$ daydream$s$.")
    )
    (arguments
      (arguments happily, longingly, sleepily, #)
      (self "You daydream $arg$.")
      (rest "$mcname$ daydream$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You daydream about $hcname$.")
      (target "$mcname$ daydream$s$ about you.")
      (rest   "$mcname$ daydream$s$ about $hcname$.")
    )
    (arguments
      (arguments happily, longingly, sleepily, #)
      (self   "You daydream of $hcname$ $arg$.")
      (target "$mcname$ daydream$s$ of you $arg$.")
      (rest   "$mcname$ daydream$s$ of $hcname$ $arg$.")
    )
  )
)

