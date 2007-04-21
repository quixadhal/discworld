(froth
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You froth at the mouth.")
      (rest "$mcname$ froth$s$ at the mouth.")
    )
  )
  (targeted
    (no-arguments
      (self   "You froth at the mouth at $hcname$.")
      (target "$mcname$ froth$s$ at the mouth at you.")
      (rest   "$mcname$ froth$s$ at the mouth at $hcname$.")
    )
  )
)
