(seduce
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You seduce $hcname$.")
      (target "$mcname$ seduce$s$ you.")
      (rest  "$mcname$ seduce$s$ $hcname$.")
    )
    (arguments
      (arguments  absentmindedly,with enthusiasm,casually,
         on a coffee table,with promises of chocolate,with love,
         lustfully,in a hurry,with a shovel,with your frog,#)
      (self   "You seduce $hcname$ $arg$.")
      (target "$mcname$ seduce$s$ you $arg$.")
      (rest  "$mcname$ seduce$s$ $hcname$ $arg$.")
    )
  )
)

