/*
  SNMP management daemon for the Discworld Mudlib.
  Technically this daemon responds with v1 as it's protocol.
  However it does not support the full MIB-II spec mainly because a lot
  of it is not relevant to the information I wanted to advertise.
  Everything is advertised via the system entity in MIB-II using the
  identifiers 101 and up.  OID 100 is there as a dummy, it returns 0 and is
  for mrtg which requires 2 mibs to graph.
  
  So, for example, number of players on is numPlayers or 1.3.6.1.2.1.1.101

*/

#include "socket.h"
#include "socket_errors.h"
#include "snmp.h"

int s;

class SnmpMessage {
        int version;
        string community;
        string SourceAddress;
        int RequestType;
        int *PacketID;
        int ErrorStatus;
        int ErrorIndex;
        buffer *ObjectData;
        }

void WriteBuffer(string what, buffer tbuf)
{
  string tstr;
  int ti;
  tstr="";
  for(ti=0;ti<sizeof(tbuf);ti++)
  {
    tstr+=sprintf("%X ", tbuf[ti]);
  }
  tell_creator("sojan", "Buffer %s: %s\n", what, tstr);
}

void create()
{
  int tmp;
  s=socket_create(DATAGRAM_BINARY, "ReceiveData");
  tmp=socket_bind(s, LISTEN_PORT);
}

void SendReply(class SnmpMessage Reply)
{
  buffer ReplyBuf, RealReplyBuf, tmpbuf;
  int i,j,k,l,m;
  ReplyBuf=allocate_buffer(500);
  ReplyBuf[0]=SEQUENCE;
  ReplyBuf[1]=0;
  ReplyBuf[2]=INTEGER;
  ReplyBuf[3]=1;
  ReplyBuf[4]=Reply->version;
  ReplyBuf[5]=OCTET_STRING;
  ReplyBuf[6]=sizeof(Reply->community);
  i=write_buffer(ReplyBuf, 7, Reply->community);
  i=sizeof(Reply->community)+7;
  ReplyBuf[i]=Reply->RequestType;
  ReplyBuf[i+1]=0;
  ReplyBuf[i+2]=2;
  ReplyBuf[i+3]=sizeof(Reply->PacketID);
  k=i+4;
  for(j=0;j<sizeof(Reply->PacketID);j++)
  {
    ReplyBuf[k]=Reply->PacketID[j];
    k++;
  }
  ReplyBuf[k]=2;
  ReplyBuf[k+1]=1;
  ReplyBuf[k+2]=0;
  ReplyBuf[k+3]=2;
  ReplyBuf[k+4]=1;
  ReplyBuf[k+5]=0;
  ReplyBuf[k+6]=SEQUENCE;
  ReplyBuf[k+7]=0;
  l=k+8;
  for(j=0;j<sizeof(Reply->ObjectData);j++)
  {
    m=write_buffer(ReplyBuf, l, Reply->ObjectData[j]);
    l=l+sizeof(Reply->ObjectData[j]);
  }
  ReplyBuf[1]=l-2;
  ReplyBuf[k+7]=l-(k+8);
  RealReplyBuf=allocate_buffer(l);
  RealReplyBuf=ReplyBuf[0..l-1];
  RealReplyBuf[i+1]=sizeof(RealReplyBuf)-i-2;
  socket_write(s, RealReplyBuf, Reply->SourceAddress);
  return;
}

void ParseObjects(class SnmpMessage CurrentMessage)
{
  int i,j;
  class SnmpMessage MyReply;
  buffer mibdata;
  MyReply=new(
        class SnmpMessage,
        version:CurrentMessage->version,
        community:CurrentMessage->community,
        RequestType:GETRESPONSEPDU,
        SourceAddress:CurrentMessage->SourceAddress,
        PacketID:CurrentMessage->PacketID,
        ErrorStatus:0,
        ErrorIndex:0,
        ObjectData:({})
        );
  for(i=0;i<sizeof(CurrentMessage->ObjectData);i++)
  {
    j=CurrentMessage->ObjectData[i][1];
    mibdata=SNMPHANDLER->GetData(CurrentMessage->ObjectData[i][2..j+1]);
    if(mibdata[0]==SEQUENCE) MyReply->ObjectData+=({mibdata,}); 
  }
  SendReply(MyReply);
  return;
}



void ReceiveData(int fd, buffer message, string address)
{
  int i,j,k,ss,is,id,es,ei;
  class SnmpMessage NewMessage;
  id=0;
  es=0;
  ei=0;
  is=0;
  ss=0;
  NewMessage=new(
        class SnmpMessage, 
        version:message[4], 
        community:read_buffer(message, 7, message[6]),
        RequestType:message[message[6]+7],
        SourceAddress:address,
        PacketID:({}),
        ErrorStatus:0,
        ErrorIndex:0,
        ObjectData:({})
        );
  for(i=message[6]+9;i<message[1]+2;i++)
  {
    switch(message[i])
    {
      case INTEGER :
        switch(is)
        {
          case ID :
            i++;
            k=i;
            is++;
            for(j=0;j<message[k];j++)
            {
              i++;
              NewMessage->PacketID+=({message[i],});
            }
            break; 
          case ES :
            i++;
            i++;
            is++;
            NewMessage->ErrorStatus=message[i];
            break;
          case EI :
            i++;
            i++;
            is++;
            NewMessage->ErrorIndex=message[i];
            break;
        }  
        break;
      case SEQUENCE :
        if(!ss)
        {
          ss++;
          i++;
          break;
        }
        i++;
        j=message[i];
        i++;
        NewMessage->ObjectData+=({message[i..(i+j-1)],});
        break;
    }
  }
  ParseObjects(NewMessage);          
  return;
}
