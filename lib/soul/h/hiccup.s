(hiccup
  (pattern "[at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (single
    (no-arguments
      (self "You hiccup.")
      (rest "$mcname$ hiccup$s$.")
    )
    (arguments 
    (arguments loudly, quietly, amusingly, frequently, contagiously, 
               violently, meditatively, thoughtfully, disgustingly, 
               rhythmically, thoroughly, uncontrollably, #)
      (self "You hiccup $arg$.")
      (rest "$mcname$ hiccup$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You hiccup at $hcname$.")
      (target "$mcname$ hiccup$s$ at you.")
      (rest "$mcname$ hiccup$s$ at $hcname$.")
    )
    (arguments 
    (arguments loudly, quietly, amusingly, frequently, contagiously, 
               violently, meditatively, thoughtfully, disgustingly, 
               rhythmically, thoroughly, uncontrollably, #)
      (self "You hiccup $arg$ at $hcname$.")
      (target "$mcname$ hiccup$s$ $arg$ at you.")
      (rest "$mcname$ hiccup$s$ $arg$ at $mcname$.")
    )
  )
)

/* Made targettable Dec 27 2001 Saffra */