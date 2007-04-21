(wag
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<indirect:any-living> [at] <string>")
  (pattern "<string> [at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You wag your tail.")
      (rest "$mcname$ wag$s$ $mposs$ tail.")
    )
    (arguments
      (arguments eyebrows, nose, ear, toe, foot, arm, leg, coffee table,
                  shower hose, green shoes)
      (self "You wag your $arg$.")
      (rest "$mcname$ wag$s$ $mposs$ $arg$.")
    )
    (arguments
      (arguments happily,enticingly,suggestively,vigorously,arrogantly,
                 nonchalantly,quickly,tunelessly)
      (self "You wag your tail $arg$.")
      (rest "$mcname$ wag$s$ $mposs$ tail $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You wag your tail at $hcname$.")
      (target "$mcname$ wag$s$ $mposs$ tail at you.")
      (rest   "$mcname$ wag$s$ $mposs$ tail at $hcname$.")
    )
    (arguments
      (arguments eyebrows, nose, ear, toe, foot, arm, leg, coffee table,
                  shower hose, green shoes)
      (self "You wag your $arg$ at $hcname$.")
      (target "$mcname$ wag$s$ $mposs$ $arg$ at you.")
      (rest "$mcname$ wag$s$ $mposs$ $arg$ at $hcname$.")
    )
    (arguments
      (arguments happily,enticingly,suggestively,vigorously,arrogantly,
                 tunelessly,nonchalantly,quickly)
      (self   "You wag your tail $arg$ at $hcname$.")
      (target "$mcname$ wag$s$ $mposs$ tail $arg$ at you.")
      (rest   "$mcname$ wag$s$ $mposs$ tail $arg$ at $hcname$.")
    )
  )
)

