#include"testSqrtAccess.h"
#include"util.h"
#include<stdio.h>
#include<stdlib.h>
#include<obliv.h>

void showUsage(const char* exec)
{
  fprintf(stderr,"Usage: %s -- <port> <initial data>\n"
                 "   or: %s <remote-server> <port> <indices...>\n",exec,exec);
  exit(1);
}
int main(int argc,char* argv[])
{
  ProtocolDesc pd;
  TestSqrtAccessIO io;
  int i,me;
  if(argc<4) showUsage(argv[0]);
  else if(strcmp(argv[1],"--")==0)
  { io.size=argc-3;
    io.content=malloc(sizeof(int)*io.size);
    for(i=0;i<io.size;++i)
      if(sscanf(argv[i+3],"%d",io.content+i)!=1) showUsage(argv[0]);
    me=1;
  }else
  { io.size=argc-3;
    io.indices=malloc(sizeof(int)*io.size);
    for(i=0;i<io.size;++i) if(sscanf(argv[i+3],"%d",io.indices+i)!=1)
      showUsage(argv[0]);
    me=2;
  }
  connectOrDie(&pd,argv[1],argv[2]);
  setCurrentParty(&pd,me);
  execYaoProtocol(&pd,testSqrtAccess,&io);
  if(me==2) free(io.indices);
  for(i=0;i<io.size;++i) printf("%d ",io.outputs[i]);
  printf("\n");
  free(io.outputs);
  if(me==1) free(io.content);
  cleanupProtocol(&pd);
  return 0;
}
