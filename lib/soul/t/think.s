(think
  (pattern "<string>")
  (single
    (no-arguments
      (self "You think carefully.")
      (rest "$mcname$ thinks carefully.")
    )
    (arguments
      (arguments about what a good duck Ranma is, #)
      (self "You think $arg$.")
      (rest "$mcname$ thinks $arg$.")
    )
  )
)

