(rolf
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<string> at <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You do a giant painting on the wall.")
      (rest "$mcname$ do$s$ a giant painting on the wall.  Can you tell what it is, yet?")
    )
    (arguments
      (arguments  My  Way, the National Anthem, a tune, something, Two Little Boys,
       Jake The Peg, #)
      (self "You play $arg$ on your didgeridoo.")
      (rest "$mcname$ play$s$ $arg$ on $mposs$ didgeridoo.")
    )
  )
  (targeted
    (no-arguments
      (self   "You do a giant painting on a wall for $hcname$.")
      (target "$mcname$ do$s$ a giant painting on a wall for you.  Can you tell what it is, yet?")
      (rest   "$mcname$ do$s$ a giant painting on a wall for $hcname$.  Can you tell what it is, yet?")
    )
    (arguments
      (arguments  My Way, the National Anthem, a tune, something, Two Little Boys,
       Jake The Peg, #)
      (self "You play $arg$ on your didgeridoo for $hcname$.")
      (target "$mcname$ play$s$ $arg$ on $mposs$ didgeridoo for you.")
      (rest "$mcname$ play$s$ $arg$ on $mposs$ didgeridoo.")
      )
  )
)
