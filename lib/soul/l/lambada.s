(lambada
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You lambada by yourself.")
      (rest "$mcname$ lambada$s$ by $mobj$self.")
    )
    (arguments  
    (arguments  sensuously, happily, hard and heavy, fast and furious,
                in high heels, across the dance floor, in a smoky dance 
                club, slowly, seductively, closely, tightly, clumsily,
                carelessly, like a Latin lover, #)
      (self "You lambada $arg$.")
      (rest "$mcname$ lambada$s$ $arg$.")
    )
  )
  (targeted
  (no-arguments
    (self "You lambada with $hcname$.")
    (target "$mcname$ lambada$s$ with you.")
			 (rest "$mcname$ lambada$s$ with $hcname$.")
  )
  (arguments
    (arguments  sensuously, happily, hard and heavy, fast and furious,
                in high heels, across the dance floor, in a smoky dance 
                club, slowly, seductively, closely, tightly, clumsily,
                carelessly, like a Latin lover, #)
    (self "You lambada $arg$ with $hcname$." )
    (target "$mcname$ lambada$s$ $arg$ with you." )
    (rest "$mcname$ lambada$s$ $arg$ with $hcname$.")
   )
  )
)
