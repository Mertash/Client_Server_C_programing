#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 1024



static double x = 0;
static double y = 0;
static double c = 0; 


typedef struct {
    double x;
    double y;
    double c;
} MsgStruct;


MsgStruct msgStruct;
MsgStruct *msgStruct_ptr;



 //addition, subtraction, multiplication and division
double Addition(double *First, double *Second)
{
  double a = *First;
  double b = *Second;

  return a+b;        
}

double Subtraction(double *First, double *Second)
{
  double a = *First;
  double b = *Second;

  return a-b;        
}
double Multiplication(double *First, double *Second)
{
  double a = *First;
  double b = *Second;

  return a*b;        
}

double Division(double *First, double *Second)
{
  double a = *First;
  double b = *Second;
  
  return a/b;        
}


// Function designed for chat between client and server.
void func(int client_sock)
{
  //
  char buff[MAX];
  int n;
  // infinite loop for chat
  for (;;)
  {
    
    bzero(buff, MAX);
    // read the message from client and copy it in buffer
    read(client_sock, buff, sizeof(buff));
        
    if (strncmp("ClientParam", buff, 11) == 0) 
    {
      printf("Server param x = %f, y = %f, c = %f\n",x,y,c);
      printf("Ready read client param\n");
      bzero(buff, MAX);
      strcpy(buff,"WaitYourParam\n");
      write(client_sock, buff, sizeof(buff));

      recv(client_sock, &msgStruct, sizeof(msgStruct),0);
      read(client_sock, buff, sizeof(buff));
      x = msgStruct.x; y = msgStruct.y;
      
////////////////////////////////////////

      if (strncmp("addition", buff, 8) == 0)
      {
        printf("Start Addition\n");
       c = Addition(&x,&y);
        
      }

      if (strncmp("subtraction", buff, 11) == 0)
      {
        printf("Start Subtraction\n");
       c = Subtraction(&x,&y);
        
      }
      
      if (strncmp("multiplication", buff, 14) == 0)
      {
        printf("Start Multiplication\n");
        c = Multiplication(&x,&y);

        
      }
      
      if (strncmp("division", buff, 8) == 0)
      {
        printf("Start DSivision\n");
        c = Division(&x,&y);        
      }

 //////////////////////////
      msgStruct.c = c;
      printf ("New server data: x = %f, y = %f, c = %f\n",x,y,c);
      printf("Send ansver\n");
      bzero(buff, MAX);
      strcpy(buff,"TakeAnsver");
      write(client_sock, buff, sizeof(buff));
          
      send(client_sock, &msgStruct, sizeof(msgStruct),0);
      break;

    }

    
  }
}




int main(){
 

  char IPsave[20] = {0};


  printf("Enter Ip address (Defaut 127.0.0.1)\n");
  scanf("%s",IPsave);
  char *ip = IPsave;
  //char *ip = "127.0.0.1";
  
  printf("Enter Port (Defaut enter 5566)\n");
  int port = 0;
  scanf("%d",&port);
  //int port = 5566;

 
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[1024];
  int n;
 
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0){
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");
 
  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);
 
  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0){
    perror("[-]Bind error");
    exit(1);
  }
  printf("[+]Bind to the port number: %d\n", port);
 
  listen(server_sock, 5);
  printf("Listening...\n");
 
  while(1){
    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
    printf("[+]Client connected.\n");
 
    func(client_sock);

    close(client_sock);
    printf("[+]Client disconnected.\n\n");

 
  }

  
  return 0;
}
