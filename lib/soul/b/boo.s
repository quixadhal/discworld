/* Saffra Sun July 28 2001 */
(boo
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (single
    (no-arguments
      (self "Boo who?")
      (rest "$mcname$ boo$s$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You sneak up behind $hcname$ and shout 'Boo!'.")
      (target "$mcname$ sneak$s$ up behind you and shout$s$ 'Boo!'.")
      (rest   "$mcname$ sneak$s$ up behind $hcname$ and shout$s$ 'Boo!'.")
      
    )
    (arguments
      (arguments  box, coffee pot, hole, sock drawer, basket, 
                  secret hiding place, #)
      (self   "You jump out of a $arg$ and shout 'Boo!' at $hcname$.")
      (target "$mcname$ jump$s$ out of a $arg$ and shout$s$ 'Boo!' at you.")
      (rest   "$mcname$ jump$s$ out of a $arg$ and shout$s$ 'Boo!' at $hcname$.")
  
    )
  )
)
