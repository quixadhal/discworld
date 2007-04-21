(melt
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<string>")
  (single
    (no-arguments
      (self "You melt.")
      (rest "$mcname$ melt$s$.")
    )
    (arguments
      (arguments quickly,slowly,into a nice puddle,carefully,
           in anticipation,in ecstasy,seductively,submissively,a little,
           messily, #)
      (self "You melt $arg$.")
      (rest "$mcname$ melt$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments arms,mouth,hands,room,house,candle,moth,coffee,tea,
                 cupcake,pocket,pants,bed,bath,microwave,computer,
                 brainwaves,heart,mind,soul)
      (self   "You melt in $mhcname$ $arg$.")
      (target "$mcname$ melt$s$ in your $arg$.")
      (rest   "$mcname$ melt$s$ in $mhcname$ $arg$.")
    )
    (arguments
      (arguments with flames,coldly,excitedly,happily,brightly,
                 using a coffee table,in anticipation,seductively,quickly,
                 carefully,slowly,submissively,warmly,messily,goopily)
      (self   "You melt $arg$ in $mhcname$ arms.")
      (target "$mcname$ melt$s$ $arg$ in your arms.")
      (rest   "$mcname$ melt$s$ $arg$ in $mhcname$ arms.")
    )
    (arguments
      (arguments cover yourself in chocolate sauce)
      (self   "You cover yourself in chocolate sauce and melt in $mhcname$ arms.")
      (target "$mcname$ cover$s$ $mobj$self in chocolate sauce and melts in your arms.")
      (rest   "$mcname$ cover$s$ $mobj$self in chocolate sauce and melts in $mhcname$ arms.")
    )
    (arguments
      (arguments scream,faint,explode,overheat)
      (self   "You $arg$ and melt in $mhcname$ arms.")
      (target "$mcname$ $arg$s and melt$s$ in your arms.")
      (rest   "$mcname$ $arg$s and melt$s$ in $mhcname$ arms.")
    )
  )
)

