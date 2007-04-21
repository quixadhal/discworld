(grovel
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You grovel.")
      (rest "$mcname$ grovel$s$.")
    )
    (arguments
      (arguments shamelessly,violently,unashamedly)
      (self "You grovel $arg$.")
      (rest "$mcname$ grovel$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You grovel at $mhcname$ feet.")
      (target "$mcname$ grovel$s$ at your feet.")
      (rest  "$mcname$ grovel$s$ at $mhcname$ feet.")
    )
    (arguments
      (arguments  shamelessly,violently,unashamedly)
      (self   "You grovel $arg$ at $mhcname$ feet.")
      (target "$mcname$ grovel$s$ $arg$ at your feet.")
      (rest  "$mcname$ grovel$s$ $arg$ at $mhcname$ feet.")
    )
  )
)

