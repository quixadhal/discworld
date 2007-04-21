(chortle
  (pattern "[around] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [around] <indirect:any-living>")
  (pattern "[around] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You chortle in joy.")
      (rest "$mcname$ chortles in joy.")
    )
    (arguments
      (arguments  in joy, in glee, maniacally, wickedly, #)
      (self "You chortle $arg$.")
      (rest "$mcname$ chortles $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You chortle in joy at $hcname$.")
      (target "$mcname$ chortles in joy at you.")
      (rest   "$mcname$ chortles in joy at $hcname$.")
    )
    (arguments
      (arguments  in joy, in glee, maniacally, wickedly, #)
      (self   "You chortle $arg$ at $hcname$.")
      (target "$mcname$ chortles $arg$ at you.")
      (rest   "$mcname$ chortles $arg$ at $hcname$.")
    )
  )
)
/* Made targetable, added args. 11-28-00.  Saffra */
