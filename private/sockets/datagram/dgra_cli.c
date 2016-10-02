//client
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SV_SOCK_PATH  "/tmp/serversocket"
#define CL_SOCK_PATH "/tmp/newclientsocket"
#define BUF_SIZE  512


int main(int argc, char *argv[])
{
  struct sockaddr_un svaddr, claddr;
  int sfd;
  char buf[BUF_SIZE]={'\0'};
  char read_buf[BUF_SIZE] ={'\0'};
 
  char got_buf[BUF_SIZE] ={'\0'};
  
  /* --------------create socket -----------------------------*/
  sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sfd == -1)
    perror("socket");

  /* ---------- Bind Address to client socket ----------------*/
  /*(1) construct client address structure */
  remove (CL_SOCK_PATH);
  memset(&claddr, 0, sizeof(struct sockaddr_un));
  claddr.sun_family = AF_UNIX;
  strcpy(claddr.sun_path,CL_SOCK_PATH);
  /*(2) Bind */
  if (bind(sfd, (struct sockaddr *) &claddr, sizeof(struct sockaddr_un)) == -1)
    perror("bind");
  
  
  /* ------------Construct server address ---------------------*/
  memset(&svaddr, 0, sizeof(struct sockaddr_un));
  svaddr.sun_family = AF_UNIX;
  strcpy(svaddr.sun_path, SV_SOCK_PATH);
  
  /* ------Send messages to server; echo responses on stdout --*/
  for(;;){
    printf("Search : ");
  scanf("%s",read_buf);
  sendto(sfd, read_buf, BUF_SIZE, 0, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un));
  recvfrom(sfd, buf, BUF_SIZE, 0, NULL, NULL);
  printf("Infinfty Server Response: \n%s \n\n", buf);
  fflush(stdin);
  fflush(stdout);
  }
  /* -------Remove client socket pathname ; exit---------------*/
  exit(EXIT_SUCCESS);
}
