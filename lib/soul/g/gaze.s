(gaze
  (pattern "[at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "<string> at <indirect:any-living>")
  (single
    (no-arguments
      (self "You gaze into the wild blue yonder.")
      (rest "$mcname$ gaze$s$ into the wild blue yonder.")
    )
  )
  (targeted
    (arguments
      (arguments lovingly, evilly, forlornly, intently, frogilly,#)
      (self "You gaze $arg$ at $hcname$.")
      (target "$mcname$ gaze$s$ $arg$ at you.")
      (rest "$mcname$ gaze$s$ $arg$ at $hcname$.")
    )
  )
)
