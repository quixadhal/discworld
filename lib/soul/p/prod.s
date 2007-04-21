(prod
  (pattern "[at] <indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (targeted
      (no-arguments
          (self "You prod $hcname$.")
          (target "$mcname$ prod$s$ you.")
          (rest "$mcname$ prod$s$ $hcname$.")
      )
      (arguments
          (arguments with a stick, cautiously, happily, mischievously,
                     cheekily, idiotically, wryly, sceptically,
                     derisively, annoyingly, absentmindedly,
                     whilst you work, distractedly, pointlessly,
                     silently, loudly, rudely, in the buttock, with laughter,
                     vaguely, obviously, #)
          (self   "You prod $hcname$ $arg$.")
          (target "$mcname$ prod$s$ you $arg$.")
          (rest  "$mcname$ prod$s$ $hcname$ $arg$.")
    )
  )
)

