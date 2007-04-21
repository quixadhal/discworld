(croak
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern " <string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string> ")
  (single
    (no-arguments
      (self "Ribbit.")
      (rest "$mcname$ croaks (ribbit).")
    )
    (arguments
      (arguments like a frog, like a toad, like a tree frog, like a bullfrog, 
                 like a horny toad, like a dissected frog, like a spotted toad,
                 like a bad joke, # )
      (self "You croak $arg$.")
      (rest "$mcname$ croaks $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You croak (ribbit) at $hcname$.")
      (target "$mcname$ croaks (ribbit) at you.")
      (rest "$mcname$ croaks (ribbit) at $hcname$.")
    )
    (arguments
      (arguments like a frog, like a toad, like a tree frog, like a bullfrog, 
                 like a horny toad, like a dissected frog, like a spotted toad,
                 like a bad joke, # )
      (self "You croak $arg$ at $hcname$.")
      (target "$mcname$ croaks $arg$ at you.")
      (rest "$mcname$ croaks $arg$ at $hcname$.")
    )
  )
)
/* Modified by Saffra 7-22-01 */
