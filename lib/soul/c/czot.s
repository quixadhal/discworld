(czot
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You czot anxiously.")
      (rest "$mcname$ czots anxiously.")
    )
    (arguments
        (arguments with the staff of Czot, #)
        (self "You czot $arg$.")
        (rest "$mcname$ czots $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You czot $hcname$.")
      (target "$mcname$ czots you.")
      (rest  "$mcname$ czots $hcname$.")
    )
    (arguments
        (arguments with the staff of Czot, #)
        (self "You czot $hcname$ $arg$.")
        (target "$mcname$ czots you $arg$.")
        (rest "$mcname$ czots $hcname$ $arg$.")
    )
  )
)

