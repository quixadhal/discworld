(flame
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments like Signe, like a dragon, with napalm, with propane,
                 fishily, with a high explosive womble, with glee, 
                 without remorse, froggily, forcefully, to 5000 degrees, #)
      (self "You flame $arg$.")
      (rest "$mcname$ flame$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments like Signe, like a dragon, with napalm, with propane,
                 fishily, with a high explosive womble, with glee, 
                 without remorse, froggily, forcefully, to 5000 degrees, #)
      (self   "You flame $hcname$ $arg$.")
      (target "$mcname$ flame$s$ you $arg$.")
      (rest  "$mcname$ flame$s$ $hcname$ $arg$.")
    )
  )
)

