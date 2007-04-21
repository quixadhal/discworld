(leap
  (pattern "<string>")
  (single
    (no-arguments
      (self "You leap into the air.")
      (rest "$mcname$ leap$s$ into the air.")
    )
    (arguments
      (arguments longingly,excitedly,quickly,slowly,awkwardly,politely,quietly,noisily)
      (self "You leap $arg$ into the air.")
      (rest "$mcname$ leap$s$ $arg$ into the air.")
    )
  )
)

