(slack
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
      (no-arguments
           (self "You slack off.")
           (rest "$mcname$ slack$s$ off.")
      )
      (arguments
        (arguments like a High Lord, like a student, ferociously,
                 slackingly, half-heartedly, loosely, madly,
                 competitively, tiredly, professionally, #)
           (self   "You slack $arg$.")
      					(target "$mcname$ slack$s$ $arg$.")
           (rest   "$mcname$ slack$s$ $arg$.")
      )
  )
  (targeted
      (no-arguments
           (self   "You slack at $hcname$.")
           (target "$mcname$ slack$s$ at you.")
	          (rest   "$mcname$ slack$s$ at $hcname$.")
      ) 
      (arguments
        (arguments more than, less than, like)
           (self   "You slack $arg$ $hcname$.")
      					(target "$mcname$ slack$s$ $arg$ you.")
           (rest  "$mcname$ slack$s$ $arg$ $hcname$.")
      )
      (arguments
        (arguments like a High Lord, like a student, ferociously,
                   slackingly, half-heartedly, loosely, madly,
                   competitively, tiredly, professionally, #)
           (self   "You slack at $hcname$ $arg$.")
           (target "$mcname$ slack$s$ at you $arg$.")
           (rest   "$mcname$ slack$s$ at $hcname$ $arg$.")
      )
  )
)