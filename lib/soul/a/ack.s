(ack
  (pattern "[at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You ack.")
      (rest "$mcname$ ack$s$.")
    )
    (arguments
      (arguments seductively,happily,carefully)
      (self "You ack $arg$.")
      (rest "$mcname$ ack$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You ack at $hcname$.")
      (target "$mcname$ ack$s$ at you.")
      (rest  "$mcname$ ack$s$ at $hcname$.")
    )
    (arguments
      (arguments  seductively,happily,carefully)
      (self   "You ack $arg$ at $hcname$.")
      (target "$mcname$ ack$s$ $arg$ at you.")
      (rest  "$mcname$ ack$s$ $arg$ at $hcname$.")
    )
  )
)

