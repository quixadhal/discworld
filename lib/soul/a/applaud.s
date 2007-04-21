(applaud
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments wholeheartedly,loudly,softly,vaguely,briefly)
      (self "You applaud $arg$.")
      (rest "$mcname$ applaud$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  wholeheartedly,loudly,softly,vaguely,briefly)
      (self   "You applaud $hcname$ $arg$.")
      (target "$mcname$ applaud$s$ you $arg$.")
      (rest  "$mcname$ applaud$s$ $hcname$ $arg$.")
    )
  )
)

