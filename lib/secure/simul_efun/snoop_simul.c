int snoop(object sno, object snop) {
  if ("/secure/master"->valid_snoop(sno, snop, previous_object())) {
    if (!snop)
    efun::snoop(sno);
    else
      efun::snoop(sno, snop);
    return 1;
  }
  return 0;
} /* snoop() */
