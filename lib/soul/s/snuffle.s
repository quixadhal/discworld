(snuffle
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You snuffle.")
      (rest "$mcname$ snuffle$s$.")
    )
    (arguments
      (arguments  sadly, tiredly, #)
      (self "You snuffle $arg$.")
      (rest "$mcname$ snuffle$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You snuffle $hcname$.")
      (target "$mcname$ snuffle$s$ you.")
      (rest   "$mcname$ snuffle$s$ $hcname$.")
    )
    (arguments
      (arguments  fondly, happily, sadly, hungrily, lovingly, angrily, 
                  like a dog, like a puppy, wetly, warmly, tiredly, 
                  hung over and dead tired, belatedly, 
                  with a twinkle in your eye, #)
        (self   "You snuffle $hcname$ $arg$.")
        (target "$mcname$ snuffle$s$ you $arg$.")
        (rest   "$mcname$ snuffle$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  arm, leg, chest, foot, head, cheek, left knee, neck, nose, 
                  armpit, backpack, womble)
        (self   "You snuffle $mhcname$ $arg$.")
        (target "$mcname$ snuffle$s$ your $arg$.")
        (rest   "$mcname$ snuffle$s$ $mhcname$ $arg$.")
    )
  )
)

