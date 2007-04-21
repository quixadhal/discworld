(protest
  (pattern "<string>")
  (single
    (no-arguments
      (self "You protest.")
      (rest "$mcname$ protest$s$.")
    )
    (arguments
      (arguments vehemently,loudly,strongly,weakly,noisily,
                 pointlessly,annoyingly,#)
      (self "You protest $arg$.")
      (rest "$mcname$ protest$s$ $arg$.")
    )
  )
)

