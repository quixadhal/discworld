(wince
  (pattern "<string>")
  (pattern "[at] <indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You wince.")
      (rest "$mcname$ winces.")
    )
    (arguments
      (arguments  in shock, unhappily, angrily, at the thought of coding, 
                  slightly, #)
      (self "You wince $arg$.")
      (rest "$mcname$ wince$s$ $arg$.")
      (target "$mcname$ wince$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You wince at $hcname$.")
      (rest "$mcname$ wince$s$ at $hcname$.")
      (target "$mcname$ wince$s$ at you.")
    )
    (arguments
      (arguments  in shock, unhappily, angrily, at the thought of coding, 
                  slightly, #)
      (self "You wince $arg$ at $hcname$.")
      (rest "$mcname$ wince$s$ $arg$ at $hcname$.")
      (target "$mcname$ wince$s$ $arg$ at you.")
    )
  )
)
