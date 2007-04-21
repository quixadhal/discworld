(sit
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "on <string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You sit down.")
      (rest "$mcname$ sit$s$ down.")
    )
    (arguments
      (arguments quickly,slowly,clumsily,carefully,thoughtfully,
                 with one hand tied behind your back,in shock,gracefully,
                 gratefully,inconsiderately,#)
      (self "You sit down $arg$.")
      (rest "$mcname$ sit$s$ down $arg$.")
      (position
        (default
           (cmd "sit")
        )
        (sitting "ignore")
        (silent "yes")
      )
    )
  )
  (targeted
    (no-arguments
      (self   "You sit on $mhcname$ lap.")
      (target "$mcname$ sit$s$ on your lap.")
      (rest  "$mcname$ sit$s$ on $mhcname$ lap.")
      (position
        (actor
          (default
            (cmd "sit")
          )
          (sitting "ignore")
          (silent "yes")
        )
      )
    )
    (arguments
      (arguments  lap,face,leg,chest,arm,cat,sofa,computer,keyboard,dinner,#)
      (self   "You sit on $mhcname$ $arg$.")
      (target "$mcname$ sit$s$ on your $arg$.")
      (rest  "$mcname$ sit$s$ on $mhcname$ $arg$.")
      (position
        (actor
          (default
            (cmd "sit")
          )
          (sitting "ignore")
          (silent "yes")
        )
      )
    )
  )
)

