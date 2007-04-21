(leer
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You leer at yourself.")
      (rest "$mcname$ leer$s$ at $mobj$self... hmmm, interesting....")
    )
    (arguments
    (arguments lasciviously, lewdly, wickedly, smarmily, seductively,
               like a dirty old man, like a dirty old woman, like a
               sailor on a lonely night, like a cross-eyed Elvis
               impersonator, like Drakkos, #)
      (self "You leer $arg$.")
      (rest "$mcname$ leer$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You leer at $hcname$.")
      (target "$mcname$ leer$s$ at you.")
      (rest  "$mcname$ leer$s$ at $hcname$.")
    )
    (arguments
    (arguments lasciviously, lewdly, wickedly, smarmily, seductively,
               like a dirty old man, like a dirty old woman, like a
               sailor on a lonely night, like a cross-eyed Elvis
               impersonator, like Drakkos, #)
       (self   "You leer at $hcname$ $arg$.")
       (target "$mcname$ leer$s$ at you $arg$.")
       (rest   "$mcname$ leer$s$ at $hcname$ $arg$.")
    )
    (arguments
    (arguments code, bits, computer, toes, lips, fika, earlobes, monkey, 
               great big blunt bonus, eyebrow, food, mouse, toys, underwear,             
               fork, frog, womble, sarcasm, innuendo, pants, oranges, 
               cucumbers, strawberries, leer)
       (self   "You leer at $mhcname$ $arg$.")
       (target "$mcname$ leer$s$ at your $arg$.")
       (rest   "$mcname$ leer$s$ at $mhcname$ $arg$.")
    )
  )
)
