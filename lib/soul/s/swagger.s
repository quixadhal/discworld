(swagger
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You swagger about.")
      (rest "$mcname$ swagger$s$ about.")
    )
    (arguments
      (arguments suavely, impressively, lustily, bravely, arrogantly, 
                 far too arrogantly, seductively, drunkenly, from side 
                 to side, like a drunken sailor, like a drunken creator, 
                 like Captain Bligh, like John Wayne, like an eighth level 
                 wizard, like a High Lord, #)
      (self "You swagger about $arg$.")
      (rest "$mcname$ swagger$s$ about $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You swagger at $hcname$.")
      (target "$mcname$ swagger$s$ at you.")
      (rest  "$mcname$ swagger$s$ at $hcname$.")
    )
    (arguments
      (arguments suavely, impressively, bravely, arrogantly, far too 
                 arrogantly, seductively, drunkenly, from side to side, 
                 like a drunken sailor, like a drunken creator, like 
                 Captain Bligh, like John Wayne, like an eighth level 
                 wizard, like a High Lord, #)
      (self   "You swagger $arg$ at $hcname$.")
       (target "$mcname$ swagger$s$ $arg$ at you.")
      (rest  "$mcname$ swagger$s$ $arg$ at $hcname$.")
    )
   )
)

/* Saffra June 4, 2000 */
