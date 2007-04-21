(sburn
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You set fire to yourself.")
      (rest "$mcname$ sets fire to $mobj$self.")
    )
  )
  (targeted
    (arguments
      (arguments  hair, leg, arm, nose, teeth, frog, carpet, net lag,
                  fish, invis, bed, ego, eyes, house, furniture, tongue,
                  keyboard, brain, soul, code, ideas, hopes, dreams, cat,
                  personality, desires, beliefs, laundry, coffee table,
                  butter, handcuffs, ear, trousers, broomstick, #)
      (self   "You set fire to $mhcname$ $arg$.")
      (target "$mcname$ sets fire to your $arg$.")
      (rest  "$mcname$ sets fire to $mhcname$ $arg$.")
    )
  )
)

