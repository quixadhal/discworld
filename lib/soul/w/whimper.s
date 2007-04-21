(whimper
  (pattern "<string> at <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> at <indirect:any-living>")
  (pattern "at <indirect:any-living> <string>")
  (pattern "[at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You whimper.")
      (rest "$mcname$ whimper$s$.")
    )
    (arguments
      (arguments painfully, fearfully, carefully, diminutively, happily,
                 winningly, in triumph, with the wombles, #)
      (self "You whimper $arg$.")
      (rest "$mcname$ whimper$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You whimper at $hcname$.")
      (target "$mcname$ whimper$s$ at you.")
      (rest "$mcname$ whimper$s$ at $hcname$.")
    )
    (arguments
      (arguments painfully, fearfully, carefully, diminutively, happily,
                 winningly, in triumph, with the wombles, #)
      (self "You whimper $arg$ at $hcname$.")
      (target "$mcname$ whimper$s$ $arg$ at you.")
      (rest "$mcname$ whimper$s$ $arg$ at $hcname$.")
    )
  )
)

