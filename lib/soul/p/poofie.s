(poofie
  (pattern "<string>")
  (pattern "[at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You poofie.")
      (rest "$mcname$ poofie$s$.")
    )
    (arguments
      (arguments playfully, with a large puff, with reckless abandon, vaguely, suddenly, affectionately, randomly, #)
      (self "You poofie $arg$.")
      (rest "$mcname$ poofie$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You poofie at $hcname$.")
      (target "$mcname$ poofie$s$ at you.")
      (rest  "$mcname$ poofie$s$ at $hcname$.")
    )
    (arguments
      (arguments playfully, with a large puff, with reckless abandon, vaguely, suddenly, affectionately, randomly, #)
      (self   "You poofie $arg$ at $hcname$.")
      (target "$mcname$ poofie$s$ $arg$ at you.")
      (rest  "$mcname$ poofie$s$ $arg$ at $hcname$.")
    )
  )
)

