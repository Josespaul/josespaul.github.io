#include <stdio.h>
#include <sys/un.h> 
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define SV_SOCK_PATH "/tmp/server"
#define BUF_SIZE 100
#define BACKLOG 5

int main (int argc, char *argv[])
{
  
  int sfd, asfd, clilen;
  int i;
  int berror, lerror, rerror, werror;
  struct sockaddr_un server_addr, client_addr;
  char buf[BUF_SIZE]={'\0'};
  
  remove(SV_SOCK_PATH);
  memset(buf, 0, BUF_SIZE);
  
  
  /*----------------Create Socket --------------------- */ 
  sfd = socket(AF_LOCAL,SOCK_STREAM,0);
  if(sfd == -1){
    perror("ERROR CREATING SOCKET");
    exit(1);
  }

  /*--------------- Bind Address to socket ------------- */
  /*(1) Construct address */
  memset(&server_addr, 0 , sizeof(struct sockaddr_un));
  server_addr.sun_family = AF_UNIX;
  strcpy(server_addr.sun_path , SV_SOCK_PATH);

  /*(2) Bind */
  berror = bind(sfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un));
  if(berror == -1){
    perror("BIND ERROR");
    exit(1);
  }


  /*------------------------------- listen -----------------------------*/
  lerror = listen(sfd, BACKLOG);
  if(lerror == -1){
    perror("ERROR LISTENING");
    exit(1);
  }


  /* ------------------------------ Accept ------------------------------*/
  /*(1) Prepare for getting client Address */
  memset(&client_addr, 0 , sizeof(struct sockaddr_un));
  clilen = sizeof(client_addr);
  
  /*(2) Accept */
  asfd = accept(sfd, (struct sockaddr *)&client_addr, (socklen_t *)&clilen);
  if (asfd == -1){
    perror("ACCEPT FAILED");
    exit(1);
  }


  /* -------------SEND / RECV using READ / WRITE ------------------------*/
  do {
    
    printf("Send(1) or Recv(0): ");
    scanf("%d",&i);
  
    if(i == 0) {
      rerror = read(asfd, buf, BUF_SIZE);
      if(rerror == -1){
	perror("READ ERROR");
	exit(1);
      }
    
      werror = write(1, buf, BUF_SIZE);
      if(werror == -1){
	perror("WRITE ERROR");
	exit(1);
      }
    }
  
    else if (i == 1) { //send

      rerror = read(0, buf, BUF_SIZE);
      if(rerror == -1){
	perror("READ ERROR");
	exit(1);
      }
    
      werror = write(asfd, buf, BUF_SIZE);
      if(werror == -1){
	perror("WRITE ERROR");
	exit(1);
      }
    

    }
  }while(i < 2);  
    
  if(close(asfd) == -1){
    perror("CLOSE ERROR");
    exit(1);
  }   
  return 0;
}
