(loom
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self   "You loom.")
      (rest   "$mcname$ loom$s$.")
    )
    (arguments 
    (arguments intimidatingly, impressively, suddenly, from a great height, 
               inexpertly, like a weaver, like a tapestry, like a standing 
               stone, like impending doom, with intimations of doom, with 
               explications of doom, from the shadows, like the Great Wall of 
               Agatea, like a Trustee, #)
      (self   "You loom $arg$.")
      (rest   "$mcname$ loom$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You loom over $hcname$.")
      (target "$mcname$ loom$s$ over you.")
      (rest   "$mcname$ loom$s$ over $hcname$.")
    )
    (arguments 
    (arguments intimidatingly, impressively, suddenly, from a great height, 
               inexpertly, like a weaver, like a tapestry, like a standing 
               stone, like impending doom, with intimations of doom, with 
               explications of doom, from the shadows, like the Great Wall of 
               Agatea, like a Trustee, #)
      (self   "You loom over $hcname$ $arg$.")
      (target "$mcname$ loom$s$ over you $arg$.")
      (rest   "$mcname$ loom$s$ over $hcname$ $arg$.")
    )
  )
)

/* Rewrote Mar 30 2003 Saffra */
