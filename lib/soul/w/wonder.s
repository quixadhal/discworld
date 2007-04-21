(wonder
  (pattern "<string>")
  (single
    (no-arguments
      (self "You wonder about reality and how boring it can be.")
     (rest "$mcname$ wonders about reality and how boring it can be.")
    )
    (arguments
      (arguments #)
      (self "You wonder $arg$.")
      (rest "$mcname$ wonder$s$ $arg$.")
    )
  )
)

