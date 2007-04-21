(noise
  (pattern "<string>")
  (single
    (no-arguments
      (self "You make a funny noise.")
      (rest "$mcname$ make$s$ a funny noise.")
    )
    (arguments
      (arguments bungle,flurgle,zongle-wop,wop,whirr,burble,frog,
                 moo,train,hairy,twisted,wap,pop,schwing,gong,koala,
                 rat,mouse,chicken,womble,guppy,cow,rabbit,kangaroo,gumby,#)
      (self "You make a $arg$ noise.")
      (rest "$mcname$ make$s$ a $arg$ noise.")
    )
  )
)

