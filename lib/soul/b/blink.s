(blink
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You blink.")
      (rest "$mcname$ blink$s$.")
    )
    (arguments
       (arguments  owlishly,slowly,carefully,in amazement,happily,
          what-the-heck-is-going-on-here,I-can't-believe-what-I'm-seeing-here,#)
      (self "You blink $arg$.")
      (rest "$mcname$ blink$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You blink at $hcname$.")
      (target "$mcname$ blink$s$ at you.")
      (rest  "$mcname$ blink$s$ at $hcname$.")
    )
    (arguments
      (arguments  owlishly,slowly,carefully,in amazement,happily,
          what-the-heck-is-going-on-here,I-can't-believe-what-I'm-seeing-here,#)
      (self   "You blink $arg$ at $hcname$.")
      (target "$mcname$ blink$s$ $arg$ at you.")
      (rest  "$mcname$ blink$s$ $arg$ at $hcname$.")
    )
  )
)

