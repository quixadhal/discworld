(bubble
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "I'm forever blowing bubbles!")
      (rest "$mcname$ blow$s$ a bubble.")
    )
    (arguments
    (arguments  wetly, damply, sarcastically, happily, like a dolphin,  
                like a fish, like a frog, like a horde of small children, 
                like a monkey, #) 
      (self   "You blow bubbles $arg$.")
      (target "$mcname$ blow$s$ bubbles $arg$.")
      (rest   "$mcname$ blow$s$ bubbles $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You blow bubbles at $hcname$.")
      (target "$mcname$ blow$s$ bubbles at you.")
      (rest  "$mcname$ blow$s$ bubbles at $hcname$.")
    )
    (arguments
    (arguments  wetly, damply, sarcastically, happily, like a dolphin,  
                like a fish, like a frog, like a horde of small children, 
                like a monkey, #) 
      (self   "You blow bubbles at $hcname$ $arg$.")
      (target "$mcname$ blow$s$ bubbles at you $arg$.")
      (rest   "$mcname$ blow$s$ bubbles at $hcname$ $arg$.")
   )
  )
)
