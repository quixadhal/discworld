(flollop
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You flollop.")
      (rest "$mcname$ flollop$s$.")
    )
    (arguments
    (arguments  around, like Zem, in a mattress-like manner, in a swamp, 
                on Sqornshellous Zeta, globberingly, #)
      (self "You flollop $arg$.")
      (rest "$mcname$ flollop$s$ $arg$.")
    )
  )
  (targeted
  (no-arguments
    (self "You flollop at $hcname$.")
    (target "$mcname$ flollop$s$ at you.")
    (rest "$mcname$ flollop$s$ at $hcname$.")
  )
  (arguments
   (arguments like Zem, in a mattress-like manner, in a swamp,
                on Sqornshellous Zeta, globberingly, #)
    (self "You flollop at $hcname$ $arg$.")
    (target "$mcname$ flollop$s$ at you $arg$." )
    (rest "$mcname$ flollop$s$ at $hcname$ $arg$." )
    )
  )
)
/* Saffra 2001 */