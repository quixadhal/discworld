(blame
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You blame everyone else.")
      (rest "$mcname$ blame$s$ the world in general, for everything, ever.")
    )
    (arguments
      (arguments  the High Lords, the creators, the players, everyone, it on the boogie, #)
      (self "You blame $arg$.")
      (rest "$mcname$ blame$s$ $arg$.")
    )
    (arguments
      (arguments  everything)
      (self "You blame $arg$, and feel a bit better from your ranting.")
      (rest "$mcname$ blame$s$ $arg$ because $mpronoun$ is in the mood for a good rant, and do$s$ not feel particularly imaginative so $mpronoun$ decide$s$ to take it out on the whole world, just because!")
    )
    (arguments
      (arguments  take, accept, delegate, dispense with, pass on)
      (self "You $arg$ the blame.")
      (rest "$mcname$ $arg$ the blame.")
    )
  )
  (targeted
    (no-arguments
      (self   "You blame $hcname$.")
      (target "$mcname$ blame$s$ you.")
      (rest   "$mcname$ blame$s$ $hcname$.")
    )
    (arguments
      (arguments  take, accept)
      (self   "You $arg$ the blame for $hcname$.")
      (target "$mcname$ $arg$s the blame for you.")
      (rest   "$mcname$ $arg$s the blame for $hcname$.")
    )
    (arguments
      (arguments  delegate, stick)
      (self   "You $arg$ the blame to $hcname$.")
      (target "$mcname$ $arg$$s$ the blame to you.")
      (rest   "$mcname$ $arg$$s$ the blame to $hcname$.")
    )
    (arguments
      (arguments  for the Great Skill Cut, for crashing the driver, 
                  for the spam on the talker, until Thursday, for breaking it, 
                  for the runtime, for the Fire of Ankh-Morpork, #)
      (self   "You blame $hcname$ $arg$.")
      (target "$mcname$ blame$s$ you $arg$.")
      (rest   "$mcname$ blame$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  Assassins, Fools, Monks, Priests, Thieves, Warriors, 
                  Witches, Wizards)
      (self   "You blame $hcname$ for downgraping the $arg$ Guild.")
      (target "$mcname$ blame$s$ you for downgraping the $arg$ Guild.")
      (rest   "$mcname$ blame$s$ $hcname$ for downgraping the $arg$ Guild.")
    )
  )
)
