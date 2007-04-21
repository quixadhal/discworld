(whistle
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments innocently,appreciatively,loudly,musically,mischievously,
                 pleasantly,discordantly,tunelessly,covertly,#)
      (self "You whistle $arg$.")
      (rest "$mcname$ whistle$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments appreciatively,innocently,loudly,musically,mischievously,
                 pleasantly,discordantly,tunelessly,covertly,#)
      (self   "You whistle $arg$ at $hcname$.")
      (target "$mcname$ whistle$s$ $arg$ at you.")
      (rest   "$mcname$ whistle$s$ $arg$ at $hcname$.")
    )
  )
)

