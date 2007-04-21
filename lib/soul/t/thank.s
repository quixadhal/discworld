(thank
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You thank $hcname$.")
      (target "$mcname$ thanks you.")
      (rest   "$mcname$ thanks $hcname$.")
    )
    (arguments
      (arguments  profusely, very much, gratuitously, lots, slightly, 
                  reservedly, muchly, verily, verily muchly, again, 
                  for all the help, for all the fish, sarcastically, 
                  a million times, with fruits and sprinkles, lot and lots, for being so wonderful, 
                  for nothing, for being there, for the last time, heaps, #)
      (self   "You thank $hcname$ $arg$.")
      (target "$mcname$ thanks you $arg$.")
      (rest   "$mcname$ thanks $hcname$ $arg$.")
    )
    (arguments
      (arguments  time, for your time, for their time, for his time, 
                  for her time)
      (self   "You thank $hcname$ for $hposs$ time.")
      (target "$mcname$ thanks you for your time.")
      (rest   "$mcname$ thanks $hcname$ for $hposs$ time.")
    )
  )
)

