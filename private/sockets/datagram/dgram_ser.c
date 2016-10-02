#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SV_SOCK_PATH  "/tmp/serversocket"
#define BUF_SIZE  512

int main(int argc, char *argv[])
{
  struct sockaddr_un svaddr, claddr;
  int sfd, j;
  ssize_t numBytes;
  socklen_t len;
  char buf[BUF_SIZE]={'\0'};

  remove("/tmp/serversocket");
  printf("-- INFINITY SERVER READY --\n");
  fflush(stdin);
  fflush(stdout);
  /* Create server socket */
  sfd = socket(AF_UNIX, SOCK_DGRAM, 0);

  
  /* Construct well-known address and bind server socket to it */
  /* (1) Construct Address - Structure*/
  remove(SV_SOCK_PATH);
  memset(&svaddr, 0, sizeof(struct sockaddr_un));
  svaddr.sun_family = AF_UNIX;
  strcpy(svaddr.sun_path, SV_SOCK_PATH);
  /* (2) Bind */
  if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1)
    perror("bind");
  
  /* Receive messages, convert to uppercase, and return to client */
  
  for (;;) { 
    len = sizeof(struct sockaddr_un);
    
    recvfrom(sfd, buf, BUF_SIZE, 0,(struct sockaddr *) &claddr, &len);
    printf("\n\nReqest accepted from%s \nSearching All 12 Realms. . ..for %s\n",claddr.sun_path,buf);
    sleep(7);

    /* Manuplation */
    if(strcmp(buf, "Kailash") == 0)
      strcpy(buf,"  -- (1) MATCH FOUND --\nOriginal Name: Thor \nOrigin        : Marvel Cinematic Universe\nStatus        : COMBAT READY"); 
    
    else if(strcmp(buf, "Muthu") == 0)
      strcpy(buf,"  -- (1) MATCH FOUND --\nOriginal Name: Electro \nOrigin        : Marvel Cinematic Universe \nStatus        : ACTIVE");
    
    else if(strcmp(buf, "Satish") == 0)
      strcpy(buf,"Satish Ram - INFO Classified");  
 
    else if(strcmp(buf, "Joses") == 0)
      strcpy(buf,"  -- (1) MATCH FOUND --\nOriginal Name: Joses Paul  \nOrigin        : Planet Earth \nStatus        : Busy Coding"); 
    
    else if(strcmp(buf, "Laki") == 0)
      strcpy(buf,"  -- (1) MATCH FOUND --\nOriginal Name: Mr.Multiverse \nOrigin        : Unknown \nStatus        : ON ACTIVE MISSION");  
 
    
    sendto(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &claddr, len);
    printf("%s",buf);
    fflush(stdin);
    fflush(stdout);
    }

    return 0;
}
