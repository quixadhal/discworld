(babble
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You babble.")
      (rest "$mcname$ babble$s$.")
    )
    (arguments
      (arguments  incoherently, uncontrollably, ceaselessly, beeble bubble,
        drunkenly, like a brook, about frogs, indiscriminately, on and on,
        maddeningly, insanely, like a fish, merrily, pointlessly,
        randomly, #)
      (self "You babble $arg$.")
      (rest "$mcname$ babble$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You babble at $hcname$.")
      (target "$mcname$ babble$s$ at you.")
      (rest   "$mcname$ babble$s$ at $hcname$.")
    )
    (arguments
      (arguments  incoherently, uncontrollably, ceaselessly, beeble bubble,
        drunkenly, like a brook, about frogs, indiscriminately, on and on,
        maddeningly, insanely, like a fish, merrily, pointlessly,
        randomly, #)
      (self   "You babble $arg$ at $hcname$.")
      (target "$mcname$ babble$s$ $arg$ at you.")
      (rest   "$mcname$ babble$s$ $arg$ at $hcname$.")
    )
  )
)
