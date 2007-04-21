(satisfy
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self   "You satisfy yourself.")
      (rest   "$mcname$ satisfies $mobj$self.")
    )
    (arguments
      (arguments carrot, parsnip, fish, womble, duck, chicken, cup of coffee,
                 cup of tea, bendy straw, wombat, hedgehog, creator, liaison,
                 Lord, High Lord, badger )
      (self  "You satisfy yourself with a $arg$.")
      (rest  "$mcname$ satisfies $mobj$self with a $arg$.")
    )
    (arguments
      (arguments  to pass the time, on a coffee table, like a womble,
                  like a duck, like an opera singer, like a goose,
                  because noone else will, like noone else can,
                  like a penguin, #)
      (self  "You satisfy yourself $arg$.")
      (rest  "$mcname$ satisfies $mobj$self $arg$.")
    )
    (arguments
      (arguments  lovingly, absentmindedly, impatiently, noisily, sneakily,
                  repeatedly )
      (self  "You $arg$ satisfy yourself.")
      (rest  "$mcname$ $arg$ satisfies $mobj$self.")
    )
  )
  (targeted
    (no-arguments
      (self   "You satisfy $hcname$.")
      (target "$mcname$ satisfies you.")
      (rest  "$mcname$ satisfies $hcname$.")
    )
    (arguments
      (arguments carrot, parsnip, fish, womble, duck, chicken, cup of coffee,
                 cup of tea, bendy straw, wombat, hedgehog, creator, liaison,
                 Lord, High Lord, badger)
      (self  "You satisfy $hcname$ with a $arg$.")
      (target "$mcname$ satisfies you with a $arg$.")
      (rest  "$mcname$ satisfies $hcname$ with a $arg$.")
    )
    (arguments
      (arguments  to pass the time, on a coffee table, like a womble,
                  like a duck, like an opera singer, like a goose,
                  because noone else will, like noone else can,
                  like a penguin, like a wombat, #)
      (self   "You satisfy $hcname$ $arg$.")
      (target "$mcname$ satisfies you $arg$.")
      (rest  "$mcname$ satisfies $hcname$ $arg$.")
    )
    (arguments
      (arguments lovingly, absentmindedly, impatiently, noisily, sneakily,
                 repeatedly )
      (self   "You $arg$ satisfy $hcname$.")
      (target "$mcname$ $arg$ satisfies you.")
      (rest  "$mcname$ $arg$ satisfies $hcname$.")
    )
  )
)
