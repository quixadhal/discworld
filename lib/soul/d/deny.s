(deny
  (pattern " <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string>  <indirect:any-living>")
  (pattern " <indirect:any-living> <string>")
  (single
    (no-arguments
      (self  "You deny it all.")
      (rest  "$mcname$ deny$s$ it all.")
    )
    (arguments
      (arguments   being a witch, #)
      (self  "You deny $arg$.")
      (rest  "$mcname$ deny$s$ $arg$.")
    )
    (arguments
      (arguments   nothing)
      (self  "You deny $arg$!")
      (rest  "$mcname$ deny$s$ $arg$!")
    )
    (arguments
      (arguments   don't, doesn't)
      (self  "You do$s$n't deny it.")
      (rest  "$mcname$ do$s$n't deny it.")
    )
  )
  (targeted
    (no-arguments
      (self   "You deny $hcname$.")
      (target "$mcname$ deny$s$ you.")
      (rest   "$mcname$ deny$s$ $hcname$.")
    )
    (arguments
      (arguments   everything)
      (self   "You deny $arg$ and pin the blame on $hcname$.")
      (target "$mcname$ deny$s$ $arg$ and pin$s$ the blame on you.")
      (rest   "$mcname$ deny$s$ $arg$ and pin$s$ the blame on $hcname$.")
    )
    (arguments
      (arguments   it all, it ever happened, being a witch, 
                   spreading malicious rumours, telling humourous stories, 
                   any aspirations to overthrow the Disc, 
                   all knowledge of how that happened)
      (self   "You deny $arg$ to $hcname$.")
      (target "$mcname$ deny$s$ $arg$ to you.")
      (rest   "$mcname$ deny$s$ $arg$ to $hcname$.")
    )
    (arguments
      (arguments   accusations, baseless accusations, groundless accusations,
                   assertions, technically true assertions, libelous claims, 
                   existence, #)
      (self   "You deny $mhcname$ $arg$.")
      (target "$mcname$ deny$s$ your $arg$.")
      (rest   "$mcname$ deny$s$ $mhcname$ $arg$.")
    )
    (arguments
      (arguments   a second biscuit)
      (self   "You deny $hcname$ $arg$.")
      (target "$mcname$ deny$s$ you $arg$.")
      (rest   "$mcname$ deny$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments   knowing)
      (self   "You deny $arg$ $hcname$.")
      (target "$mcname$ deny$s$ $arg$ you.")
      (rest   "$mcname$ deny$s$ $arg$ $hcname$.")
    )
    (arguments
      (arguments   a fair trial, silence, freedom of speech, womble overground, 
                   womble underground, womble free, remain silent, a lawyer, 
                   rights)
      (self   "You deny $hcname$ the right to $arg$.")
      (target "$mcname$ deny$s$ you the right to $arg$.")
      (rest   "$mcname$ deny$s$ $hcname$ the right to $arg$.")
    )
    (arguments
      (arguments   don't, doesn't)
      (self   "You $V$0=doesn't,don't$V$ deny it to $hcname$.")
      (target "$mcname$ do$s$n't deny it to you.")
      (rest   "$mcname$ do$s$n't deny it to $hcname$.")
    )
  )
)

