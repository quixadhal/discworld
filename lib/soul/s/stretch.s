(stretch
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [to] <indirect:any-living>")
  (pattern "[to] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You stretch.")
      (rest "$mcname$ stretches.")
    )
    (arguments
      (arguments lazily, slowly, seductively, carefully, excitedly, sleepily,
                 sadly, happily, your legs, your arms, your eyebrows, #)
      (self "You stretch $arg$.")
      (rest "$mcname$ stretch$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You stretch at $hcname$.")
      (target "$mcname$ stretch$s$ at $hcname$.")
      (rest "$mcname$ stretch$s$ at $hcname$.")
    )
    (arguments
      (arguments lazily, slowly, seductively, carefully, excitedly, sleepily,
                 sadly, happily, your legs, your arms, your eyebrows, #)
      (self "You stretch $arg$ at $hcname$.")
      (target "$mcname$ stretch$s$ $arg$ at you.")
      (rest "$mcname$ stretch$s$ $arg$ at $hcname$.")
    )
  )
)
