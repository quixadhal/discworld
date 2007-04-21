(dare
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You dare $hcname$.")
      (target "$mcname$ dare$s$ you.")
      (rest  "$mcname$ dare$s$ $hcname$.")
    )
    (arguments
      (arguments to go creator,to read Godot's code,#)
      (self   "You dare $hcname$ $arg$.")
      (target "$mcname$ dare$s$ you $arg$.")
      (rest  "$mcname$ dare$s$ $hcname$ $arg$.")
    )
  )
)

