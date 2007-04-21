(gesture
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments wildly, pompously, mystically, oddly, strangely, 
                 carefully, deliberately, off into the distance, 
                 rapidly, without moving, slowly, ponderously, 
                 mindlessly, insanely, nastily, suggestively, lovingly, #)
      (self "You gesture $arg$.")
      (rest "$mcname$ gesture$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments wildly, pompously, mystically, oddly, strangely, 
                 carefully, deliberately, off into the distance, 
                 rapidly, without moving, slowly, ponderously, 
                 mindlessly, insanely, nastily, suggestively, lovingly, #)
      (self   "You gesture $arg$ at $hcname$.")
      (target "$mcname$ gesture$s$ $arg$ at you.")
      (rest  "$mcname$ gesture$s$ $arg$ at $hcname$.")
    )
  )
)

