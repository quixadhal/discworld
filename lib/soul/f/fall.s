(fall
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You fall over.")
      (rest "$mcname$ fall$s$ over.")
    )
    (arguments
      (arguments backwards, painfully, awkwardly, accidentally, purposefully,
                 happily, sadly, quickly, rapidly, down, up, #)
      (self "You fall $arg$.")
      (rest "$mcname$ fall$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You fall for $hcname$.")
      (target "$mcname$ fall$s$ for you.")
      (rest   "$mcname$ fall$s$ for $hcname$.")
    )
    (arguments
      (arguments  heavily, slowly, happily, quickly, twice, down, #)
      (self   "You fall $arg$ on $hcname$.")
      (target "$mcname$ fall$s$ $arg$ on you.")
      (rest   "$mcname$ fall$s$ $arg$ on $hcname$.")
    )
  )
)
