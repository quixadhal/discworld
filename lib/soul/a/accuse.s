(accuse
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<indirect:any-living> [of] <string>")
  (single
    (no-arguments
      (self "The butler did it!")
      (rest "$mcname$ accuse$s$ the butler.")
    )
    (arguments
      (arguments #)
      (self "You accuse $arg$.")
      (rest "$mcname$ accuse$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You accuse $hcname$.")
      (target "$mcname$ accuse$s$ you.")
      (rest  "$mcname$ accuse$s$ $hcname$.")
    )
    (arguments
      (arguments  #)
      (self   "You accuse $hcname$ of $arg$.")
      (target "$mcname$ accuse$s$ you of $arg$.")
      (rest  "$mcname$ accuse$s$ $hcname$ of $arg$.")
    )
  )
)

