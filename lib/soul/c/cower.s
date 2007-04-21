(cower
  (pattern "[from] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [from] <indirect:any-living>")
  (pattern "[from] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You cower in a corner.")
      (rest "$mcname$ cowers in a corner.")
    )
    (arguments
      (arguments fearfully,slyly,shyly,in shame,nastily,the head,the
                 sensitive parts,#)
      (self "You cower $arg$.")
      (rest "$mcname$ cowers $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You cower in a corner from $hcname$.")
      (target "$mcname$ cowers in a corner from you.")
      (rest  "$mcname$ cowers in a corner from $hcname$.")
    )
    (arguments
      (arguments fearfully,slyly,shyly,in shame,nastily,the head,the 
                 sensitive parts,#)
      (self   "You cower $arg$ from $hcname$.")
      (target "$mcname$ cowers $arg$ from you.")
      (rest  "$mcname$ cowers $arg$ from $hcname$.")
    )
     (arguments
       (arguments before)
          (self "You cower $arg$ $hcname$.")
	  (target "$mcname$ cowers $arg$ you.")
          (rest  "$mcname$ cowers $arg$ $hcname$".)
      )
   )
)
