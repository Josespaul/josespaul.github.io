#include <stdio.h>
#include <sys/un.h> 
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define SV_SOCK_PATH "/tmp/server"
#define CL_SOCK_PATH "/tmp/client"
#define BUF_SIZE 100

int main (int argc, char *argv[])
{
  int sfd, i;
  int rerror, werror, berror;
  struct sockaddr_un addr;
  struct sockaddr_un baddr;
  char buf[BUF_SIZE]={'\0'};

  remove(CL_SOCK_PATH);
  memset(buf, 0, BUF_SIZE);

  /*---------- Create Socket ------------*/
  sfd = socket(AF_UNIX,SOCK_STREAM,0);
  if(sfd == -1){
    perror("ERROR CREATING SOCKET");
    exit(1);
  }

  /*---------- Bind Address to client ----- */
  /*(1) Construct Address */
  memset(&baddr, 0 , sizeof(struct sockaddr_un));
  baddr.sun_family = AF_UNIX;
  strcpy(baddr.sun_path , CL_SOCK_PATH);
  /*(2) Bind Address*/
  berror = bind(sfd, (struct sockaddr *)&baddr, sizeof(struct sockaddr_un));
  if(berror == -1){
    perror("BIND ERROR");
    exit(1);
  }

  
  /*------------------------ Connect ---------------------------- */
  /* Construct SERVER Address  */
  memset(&addr, 0 , sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path , SV_SOCK_PATH);
  /* Connect */
  connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un));


  /* --------------Send/ Recv using Read -  Write  -------------------*/
  do {
    printf("Send(1) or Recv(0): ");  
    scanf("%d",&i);

    if (i == 0) {                          //Receive
      rerror = read(sfd, buf, BUF_SIZE);
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


    if (i == 1) {                            //Send
      rerror = read(0, buf, BUF_SIZE);
      if(rerror == -1){
	perror("READ ERROR");
	exit(1);
      }
  
      werror = write(sfd, buf, BUF_SIZE);
      if(werror == -1){
	perror("WRITE ERROR");
	exit(1);
      }
  
    }
 
  }while(i < 2);

  if(close(sfd) == -1){
    perror("CLOSE ERROR");
    exit(1);
  }

  return 0;
}

	  
