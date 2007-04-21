(catch
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You catch yourself.")
      (rest "$mcname$ catch$s$ $mobj$self.")
    )
    (arguments
      (arguments  stupid, silly, absurd, caring, helpful, 
                  potentially hazardous, mundane)
      (self "You catch yourself before you do something $arg$.")
      (rest "$mcname$ catch$s$ $mobj$self before $mpronoun$ does something $arg$.")
    )
    (arguments
      (arguments  a disease, a passing ball, on, the drift, #)
      (self "You catch $arg$.")
      (rest "$mcname$ catch$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You catch $hcname$.")
      (target "$mcname$ catch$s$ you.")
      (rest   "$mcname$ catch$s$ $hcname$.")
    )
    (arguments
      (arguments  just in time, quickly, warmly, happily, lovingly, 
                  in a net, out, telling the truth, lying, 
                  eating icecream, eating brussel sprouts, 
                  unawares, under the stairs, twice, in your arms, #)
      (self   "You catch $hcname$ $arg$.")
      (target "$mcname$ catch$s$ you $arg$.")
      (rest   "$mcname$ catch$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  attention, disease, enthusiasm, despondence, eye, 
                  ear, teddy bears, drift, meaning, intent, kiss)
      (self   "You catch $hcname$'s $arg$.")
      (target "$mcname$ catch$s$ your $arg$.")
      (rest   "$mcname$ catch$s$ $hcname$'s $arg$.")
    )
    (arguments
      (arguments  on)
      (self   "You catch $arg$ about what $hcname$ is saying.")
      (target "$mcname$ catch$s$ $arg$ about what you are saying.")
      (rest   "$mcname$ catch$s$ $arg$ about what $hcname$ is saying.")
    )
  )
)

