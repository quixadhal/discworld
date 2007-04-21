(fold
  (pattern "<string>")
  (single
    (no-arguments
      (self "You fold your arms.")
      (rest "$mcname$ fold$s$ $mposs$ arms.")
    )
    (arguments
      (arguments arrogantly,decisively,carefully,slowly,menacingly,halfheartedly,quickly,snappily)
      (self "You fold your arms $arg$.")
      (rest "$mcname$ fold$s$ $mposs$ arms $arg$.")
    )
  )
)

