(luck
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You wish $hcname$ luck.")
      (target "$mcname$ wish$s$ you luck.")
      (rest  "$mcname$ wish$s$ $hcname$ luck.")
    )
    (arguments
      (arguments  good,bad,lots of,no,an iota of,a frogfull of)
      (self   "You wish $hcname$ $arg$ luck.")
      (target "$mcname$ wish$s$ you $arg$ luck.")
      (rest  "$mcname$ wish$s$ $hcname$ $arg$ luck.")
    )
  )
)

