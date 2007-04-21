(zot
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You zot anxiously.")
      (rest "$mcname$ zot$s$ anxiously.")
    )
    (arguments
        (arguments with the staff of Zot, #)
        (self "You zot $arg$.")
        (rest "$mcname$ zot$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You zot $hcname$.")
      (target "$mcname$ zot$s$ you.")
      (rest  "$mcname$ zot$s$ $hcname$.")
    )
    (arguments
        (arguments with the staff of Zot, #)
        (self "You zot $hcname$ $arg$.")
        (target "$mcname$ zot$s$ you $arg$.")
        (rest "$mcname$ zot$s$ $hcname$ $arg$.")
    )
  )
)

