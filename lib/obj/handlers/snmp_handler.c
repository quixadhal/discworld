#include "snmp.h"

void create()
{
}

buffer BufferiseInt(int p)
{
  buffer buf1;
  buf1=allocate_buffer(4);
  if(p<0)
  {
    buf1[0]=(4294967296+p)/16777216;
  } else {
    buf1[0]=p/16777216;
  }
  buf1[1]=p/65536;
  buf1[2]=p/256;
  buf1[3]=p%256;
  return buf1;
}

buffer GetData(buffer MIB)
{
  int i,j,k;
  object *tmp;
  
  buffer mibreply,mibdata,tmpbuf;
  mibreply=allocate_buffer(4);
  tmpbuf=allocate_buffer(1);
  mibreply[0]=48;
  mibreply[1]=0;
  mibreply[2]=OBJECT_IDENT;
  mibreply[3]=0;
  mibreply+=MIB;
  i=sizeof(mibreply);
  switch(MIB[6])
  {
    case 101 :
      j=sizeof(users());
      tmpbuf[0]=2;
      mibreply+=tmpbuf;
      tmpbuf[0]=0;
      mibreply+=tmpbuf;
      tmpbuf=BufferiseInt(j);
      mibreply+=tmpbuf;
      mibreply[i+1]=sizeof(tmpbuf); 
      break;
    case 102 :
      j=0;
      tmp=users();
      for(k=0;k<sizeof(tmp);k++)
        if(tmp[k] && (tmp[k])->query_creator())
          j++;
      tmpbuf[0]=2;
      mibreply+=tmpbuf;
      tmpbuf[0]=0;
      mibreply+=tmpbuf;
      tmpbuf=BufferiseInt(j);
      mibreply+=tmpbuf;
      mibreply[i+1]=sizeof(tmpbuf);
      break;
    case 100 :
      tmpbuf[0]=2;
      mibreply+=tmpbuf;
      tmpbuf[0]=1;
      mibreply+=tmpbuf;
      tmpbuf[0]=0;
      mibreply+=tmpbuf;
      break;
    default :
      mibreply[0]=255;
      break;
  }
  mibreply[1]=sizeof(mibreply)-2;
  mibreply[3]=sizeof(MIB);
  return mibreply;
}     
