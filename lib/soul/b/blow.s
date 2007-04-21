(blow
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments  a kiss, bubbles, a whistle, your top)
      (self "You blow $arg$.")
      (rest "$mcname$ blow$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You blow a kiss at $hcname$.")
      (target "$mcname$ blow$s$ a kiss at you.")
      (rest   "$mcname$ blow$s$ a kiss at $hcname$.")
    )
    (arguments
      (arguments  a kiss, bubbles, a whistle, your top)
      (self   "You blow $arg$ at $hcname$.")
      (target "$mcname$ blow$s$ $arg$ at you.")
      (rest   "$mcname$ blow$s$ $arg$ at $hcname$.")
    )
  )
)
