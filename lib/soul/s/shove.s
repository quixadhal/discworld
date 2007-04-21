(shove
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You shove $hcname$.")
      (target "$mcname$ shove$s$ you.")
      (rest  "$mcname$ shove$s$ $hcname$.")
    )
    (arguments
      (arguments  off the couch, into the pool, into a bowl of jelly,
                  off a cliff, out of the way, onto the bed, under the bed,
                  into the cupboard, over, onto the coffee table, #)
      (self   "You shove $hcname$ $arg$.")
      (target "$mcname$ shove$s$ you $arg$.")
      (rest  "$mcname$ shove$s$ $hcname$ $arg$.")
    )
  )
)
