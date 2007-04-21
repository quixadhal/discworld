(mime
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (single
    (arguments
      (arguments being stuck inside a fishtank, climbing some stairs,
       blowing up a balloon which bursts, 
       carrying a large sheet of glass, drinking a bottle of scumble, 
       being hung by the Patrician, pushing a large box,
       being hung over a scorpion pit, #)
      (self "You mime $arg$.")
      (rest "$mcname$ mime$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments being stuck inside a fishtank, climbing some stairs,
       blowing up a balloon which bursts, 
       carrying a large sheet of glass, drinking a bottle of scumble, 
       being hung by the Patrician, pushing a large box,
       being hung over a scorpion pit, #)
      (self "You mime $arg$ for $mhcname$ amusement.")
      (target "$mcname$ mime$s$ $arg$ for your amusement.")
      (rest  "$mcname$ mime$s$ $arg$ for $mhcname$ amusement.")
    )
  )
)

