(boingy
  (pattern "<string>")
  (pattern "[around] <indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You boingy.")
      (rest "$mcname$ boingy$s$.")
    )
    (arguments
      (arguments insanely, happily, lovingly, with a squeak, fluffily, 
                  sarcasticly, with bells on, with pride, #)
      (self    "You boingy $arg$.")
      (rest   "$mcname$ boingy$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You boingy around $hcname$.")
      (target "$mcname$ boingy$s$ around you.")
      (rest   "$mcname$ boingy$s$ around $hcname$.")
    )
    (arguments
      (arguments  insanely, happily, lovingly, with a squeak, fluffily, 
                  sarcasticly, with bells on, with pride, #)
      (self    "You boingy $arg$ at $hcname$.")
      (target "$mcname$ boingy$s$ $arg$ at you.")
      (rest   "$mcname$ boingy$s$ $arg$ at $hcname$.")
    )
  )
)
