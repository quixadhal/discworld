(bow
  (pattern "<indirect:any-living>")
  (pattern "to <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<string> to <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "to <indirect:any-living> <string>")
  (single
     (no-arguments
        (self  "You bow.")
        (rest  "$mcname$ bow$s$.")
     )
     (arguments
        (arguments solemnly,deeply,formally,hastily,slightly,
                   disgustedly,floridly,flamboyantly,
                   insolently,clumsily,dextrously,wildly,
                  colourfully,with a flourish,graciously,#)
        (self  "You bow $arg$.")
        (rest  "$mcname$ bow$s$ $arg$.")
      )
  )
  (targeted
     (no-arguments
        (self "You bow to $hcname$.")
        (rest "$mcname$ bow$s$ to $hcname$.")
        (target "$mcname$ bow$s$ to you.")
     )
     (arguments
        (arguments solemnly,deeply,formally,hastily,slightly,
                   disgustedly,floridly,flamboyantly,
                   insolently,clumsily,dextrously,wildly,
                   colourfully,with a flourish,graciously,#)
        (self "You bow $arg$ to $hcname$.")
        (rest "$mcname$ bow$s$ $arg$ to $hcname$.")
        (target "$mcname$ bow$s$ $arg$ to you.")
     )
  )
)

