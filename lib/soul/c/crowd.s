(crowd
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> [into] <string>")
  (single
    (arguments
      (arguments wild,#)
      (self "The crowd goes $arg$.")
      (rest "The crowd goes $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  a corner,#)
      (self   "You crowd $hcname$ into $arg$.")
      (target "$mcname$ crowds you into $arg$.")
      (rest  "$mcname$ crowds $hcname$ into $arg$.")
    )
  )
)

