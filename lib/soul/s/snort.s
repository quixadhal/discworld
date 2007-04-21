(snort
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
  (no-arguments
      (self "You snort.")
      (rest "$mcname$ snort$s$.")
    )
    (arguments
      (arguments in contempt,in disgust,happily,backwards,sideways,cheekily,#)
      (self "You snort $arg$.")
      (rest "$mcname$ snort$s$ $arg$.")
    )
  )
  (targeted
  (no-arguments
      (self   "You snort at $hcname$.")
      (target "$mcname$ snort$s$ at you.")
      (rest  "$mcname$ snort$s$ at $hcname$.")      
    )
    (arguments
      (arguments in contempt,in disgust,happily,backwards,sideways,cheekily,#)
      (self   "You snort $arg$ at $hcname$.")
      (target "$mcname$ snort$s$ $arg$ at you.")
      (rest  "$mcname$ snort$s$ $arg$ at $hcname$.")
    )
  )
)
/* HOBBES */
