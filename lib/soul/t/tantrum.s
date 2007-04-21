(tantrum
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<string> at <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You throw a tantrum.")
      (rest "$mcname$ throws a tantrum.")
    )
    (arguments
      (arguments like a two year old, enthusiastically, in anger,
                 down the back of the couch, about dying, about lag,
                 like a whiny player, slowly, quickly, briefly,
                 quietly, loudly, emotionaly, expressionlessly,
                 like someone who needs to be sent to their room, #)
      (self "You throw a tantrum $arg$.")
      (rest "$mcname$ throws a tantrum $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You throw a tantrum at $hcname$.")
      (target "$mcname$ throws a tantrum at you.")
      (rest "$mcname$ throws a tantrum at $hcname$.")
    )
    (arguments
      (arguments like a two year old, enthusiastically, in anger,
                 down the back of the couch, about dying, about lag,
                 like a whiny player, slowly, quickly, briefly,
                 quietly, loudly, emotionaly, expressionlessly,
                 like someone who needs to be sent to their room, #)
      (self "You throw a tantrum at $hcname$ $arg$.")
      (target "$mcname$ throws a tantrum at you $arg$.")
      (rest "$mcname$ throws a tantrum at $hcname$ $arg$.")
    )
  )
)

