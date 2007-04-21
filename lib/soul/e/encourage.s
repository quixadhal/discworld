(encourage
   (pattern "<string>")
   (pattern "[at] <indirect:any-living>")
   (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You encourage yourself.")
      (rest "$mcname$ encourage$s$ $mobj$self.")
    )
    (arguments
      (arguments because no one else will, #)
      (self "You encourage yourself $arg$.")
      (rest "$mcname$ encourage$s$ $mobj$self $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You encourage $hcname$.")
      (target "$mcname$ encourage$s$ you.")
      (rest  "$mcname$ encourage$s$ $hcname$.")
    )
    (arguments
      (arguments because no one else will, to nibble your toes, 
                 to be quiet now, to make you dinner, to do mighty deeds, #)
      (self   "You encourage $hcname$ $arg$.")
      (target "$mcname$ encourage$s$ you $arg$.")
      (rest  "$mcname$ encourage$s$ $hcname$ $arg$.")
    )
  )
)

/* Saffra 9-10-00.  Because Archana asked me to. */