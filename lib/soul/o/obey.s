(obey
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (single
    (no-arguments
      (self   "You hear and obey.")
      (rest   "$mcname$ hear$s$ and obey$s$.")
    )
    (arguments
      (arguments  submissively, reluctantly, like Igor, #)
      (self  "You obey $arg$.")
      (rest  "$mcname$ obey$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You hear and obey $hcname$.")
      (target "$mcname$ hear$s$ and obey$s$ you.")
      (rest  "$mcname$ hear$s$ and obey$s$ $hcname$.")
    )
    (arguments
      (arguments  submissively, reluctantly, like Igor, #)
      (self   "You obey $hcname$ $arg$.")
      (target "$mcname$ obey$s$ you $arg$.")
      (rest  "$mcname$ obey$s$ $hcname$ $arg$.")
    )
  )
)

