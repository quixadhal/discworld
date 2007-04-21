(exp
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You raise yourself to a random power.")
      (rest "$mcname$ raise$s$ $mobj$self to a random power.")
    )
    (arguments
      (arguments #)
      (self "You raise yourself to the power of $arg$.")
      (rest "$mcname$ raise$s$ $mobj$self to the power of $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You raise $hcname$ to a random power.")
      (target "$mcname$ raise$s$ you to a random power.")
      (rest  "$mcname$ raise$s$ $hcname$ to a random power.")
    )
    (arguments
      (arguments  #)
      (self   "You raise $hcname$ to the power of $arg$.")
      (target "$mcname$ raise$s$ you to the power of $arg$.")
      (rest  "$mcname$ raise$s$ $hcname$ to the power of $arg$.")
    )
  )
)

