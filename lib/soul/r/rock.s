(rock
  (pattern "<string>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments  heels, nose, feet, elbows)
      (self "You rock on your $arg$.")
      (rest "$mcname$ rock$s$ on $mposs$ $arg$.")
    )
    (arguments
      (arguments  around the clock, in consternation, uncertainly to and fro)
      (self "You rock $arg$.")
      (rest "$mcname$ rock$s$ $arg$.")
    )
    (arguments
      (arguments  around the clock)
      (self "You rock $arg$.")
      (rest "$mcname$ rock$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  gently in their arms)
      (self   "You rock $hcname$ gently in your arms.")
      (target "$mcname$ rock$s$ you gently in $mposs$ arms.")
      (rest   "$mcname$ rock$s$ $hcname$ gently in $mposs$ arms.")
    )
  )
)

