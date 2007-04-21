(stamp
  (pattern "<string>")
  (single
    (no-arguments
      (self "You stamp your foot.")
      (rest "$mcname$ stamp$s$ $mposs$ foot.")
    )
    (arguments
      (arguments angrily,impatiently,warningly,frighteningly,in frustration)
      (self "You stamp your foot $arg$.")
      (rest "$mcname$ stamp$s$ $mposs$ foot $arg$.")
    )
  )
)

