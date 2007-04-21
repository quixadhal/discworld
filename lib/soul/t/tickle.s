(tickle
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You tickle $hcname$.")
      (target "$mcname$ tickles you.")
      (rest   "$mcname$ tickles $hcname$.")
      
    )
    (arguments
      (arguments  mercilessly, ruthlessly, fearlessly, quickly, sneakily,
                  weakly, carefully, gently, harshly, inconceivably slowly,
                  incessantly, playfully, passionately, delicately, cutely, 
                  quietly, incandescently, excitingly, with a frog,
                  on a coffee table, in a shower, professionally,
                  passionately, seductively, sensuously, #)
      (self   "You tickle $hcname$ $arg$.")
      (target "$mcname$ tickles you $arg$.")
      (rest   "$mcname$ tickles $hcname$ $arg$.")
     
    )
    (arguments
      (arguments  to death)
      (self   "You tickle $hcname$ $arg$.")
      (target "$mcname$ tickles you $arg$.")
      (rest   "$mcname$ tickles $hcname$ $arg$.")
      
    )
  )
)

