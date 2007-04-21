(ssit
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You sit down.")
      (rest "$mcname$ sit$s$ down.")
    )
    (arguments
      (arguments quickly,slowly,clumsily,carefully,thoughtfully,
                 with one hand tied behind your back,in shock,gracefully,
                 gratefully,inconsiderately)
      (self "You sit down $arg$.")
      (rest "$mcname$ sit$s$ down $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  heavily, lightly, delicately, suddenly, #)
      (self   "You sit on $hcname$ $arg$.")
      (target "$mcname$ sit$s$ on you $arg$.")
      (rest  "$mcname$ sit$s$ on $hcname$ $arg$.")
    )
  )
)

