(shrug
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You shrug.")
      (rest "$mcname$ shrug$s$.")
    )
    (arguments
      (arguments resignedly,tiredly,happily,confusedly,uncaringly,
                 uncertainly,like Ember,#)
      (self "You shrug $arg$.")
      (rest "$mcname$ shrug$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You shrug at $hcname$.")
      (target "$mcname$ shrug$s$ at you.")
      (rest "$mcname$ shrug$s$ at $hcname$.")
    )
    (arguments
      (arguments resignedly,tiredly,happily,confusedly,uncaringly,
                 uncertainly,back,like Ember,#)
      (self "You shrug $arg$ at $hcname$.")
      (target "$mcname$ shrug$s$ $arg$ at you.")
      (rest "$mcname$ shrug$s$ $arg$ at $hcname$.")
    )
  )
)

