#!/bin/sh
# Takes a file name, gets the relative dir out of it.
newdir="../dwnr"
if [ -d $1 ] ; then
  if [ -d $newdir/$1 ] ; then
    exit
  fi
  mkdir $newdir/$1
  exit
fi
name=`echo $1 | sed "s&$PWD/&&"`
# trans "$PWD" "./"`
dir=`dirname $name`
file=`basename $name`
if [ -d $newdir/$dir ] ; then
  bing="blue"
else
  mkdir $newdir/$dir
fi
newname=`echo $file | sed "s/\..$//"`
echo $newname
awk -f man2dwformat.awk < $1 > $newdir/$dir/$newname
