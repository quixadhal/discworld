(buzz
  (pattern "<indirect:any-living> [ear]")
  (single
    (no-arguments
      (self "You buzz gently.")
      (rest "$mcname$ buzz$s$ gently.")
    )
    (arguments
      (arguments #)
      (self "You buzz $arg$.")
      (rest "$mcname$ buzz$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You buzz in $mhcname$ ear.")
      (target "$mcname$ buzz$s$ in your ear.")
      (rest   "$mcname$ buzz$s$ in $mhcname$ ear.")
    )
  )
)

