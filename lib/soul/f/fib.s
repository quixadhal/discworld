(fib
  (pattern "[to] <string>")
  (pattern "[to] <indirect:any-living>")
  (pattern "<string> [to] <indirect:any-living>")
  (pattern "[to] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You fib.")
      (rest "$mcname$ fib$s$.")
    )
    (arguments
      (arguments wildly, about a girl, about a guy, about two gnomes and a goblin,
      shamelessly, like a barsteward, like a Director, like a Trustee, like a newbie multiplayer,
       convincingly, in desparation, about code deadlines, like a clowney flyboat, #)
      (self "You fib $arg$.")
      (rest "$mcname$ fib$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You fib to $hcname$.")
      (target "$mcname$ fib$s$ to you.")
      (rest  "$mcname$ fib$s$ to $hcname$.")
    )
    
    (arguments
      (arguments wildly, about a girl, about a guy, about two gnomes and a goblin,
      shamelessly, like a barsteward, like a Director, like a Trustee, like a newbie multiplayer,
       convincingly, in desparation, about code deadlines, like a clowney flyboat, #)
      (self   "You fib $arg$ to $hcname$.")
      (target "$mcname$ fib$s$ $arg$ to you.")
      (rest   "$mcname$ fib$s$ $arg$ to $hcname$.")
    )  
  )
)

