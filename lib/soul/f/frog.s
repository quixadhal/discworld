(frog
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments mindlessly,silently,happily,carefully,#)
      (self "You frog around $arg$.")
      (rest "$mcname$ frog$s$ around $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You turn $hcname$ into a frog$position$.")
      (target "$mcname$ turn$s$ you into a frog$position$.")
      (rest  "$mcname$ turn$s$ $hcname$ into a frog$position$.")
      
      (position
        (default "crouch")
        (silent "yes")
      )
    )
    (arguments
      (arguments  slowly,carefully,excruciatingly,happily,gleefully,magically)
      (self   "You $arg$ turn $hcname$ into a frog$position$.")
      (target "$mcname$ $arg$ turn$s$ you into a frog$position$.")
      (rest  "$mcname$ $arg$ turn$s$ $hcname$ into a frog$position$.")
      (position
        (default "crouch")
        (silent "yes")
      )
    )
  )
)

