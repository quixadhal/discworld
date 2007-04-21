(smooch
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You make a smoochy face.")
      (rest "$mcname$ make$s$ a smoochy face.")
    )
    (arguments
      (arguments wet, nice, passionate, soft, careful, fishy, friendly,
                 froggy, luscious, delicious, soppy, sappy, romantic, 
                 passionate, dispassionate, lovey-dovey, icky, yucky,
                 messy, mooshy, lush, precious, perfect, prissy, icy, #)
      (self   "You make a $arg$ smoochy face.")
      (rest   "$mcname$ make$s$ a $arg$ smoochy face.")
    )
  )
  (targeted
    (no-arguments
      (self   "You give $hcname$ a big wet smooch on the lips.")
      (target "$mcname$ give$s$ you a big wet smooch on the lips.")
      (rest   "$mcname$ give$s$ $hcname$ a big wet smooch on the lips.")
    )
    (arguments
      (arguments wet, nice, passionate, soft, careful, fishy, friendly,
                 froggy, luscious, delicious, soppy, sappy, romantic, 
                 passionate, dispassionate, lovey-dovey, icky, yucky,
                 messy, mooshy, lush, precious, perfect, prissy, icy, #)
      (self   "You give $hcname$ a $arg$ smooch on the lips.")
      (target "$mcname$ give$s$ you a $arg$ smooch on the lips.")
      (rest   "$mcname$ give$s$ $hcname$ a $arg$ smooch on the lips.")
    )
  )
)

