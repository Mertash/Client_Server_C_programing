#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>




#define MAX 1024 

static double x = 2;
static double y = 2;
static double c = 2; 

static unsigned int Numeric_Operation = 0;

static const char *OperationName[] = {"", "addition", "subtraction","multiplication","division"};

typedef struct {
      double x;
      double y;
      double c;
  } MsgStruct;
 

MsgStruct msgStruct;
MsgStruct *msgStruct_ptr;

void func(int sock)
{
  char buff[MAX];
  int n;
  for (;;) 
  {

    strcpy(buff, "ClientParam");
    write(sock, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(sock, buff, sizeof(buff));
    

      if ((strncmp(buff,"WaitYourParam", 13)) == 0) 
        {
          bzero(buff, sizeof(buff));
          
          strcpy(buff,OperationName[Numeric_Operation]);  
          send(sock, &msgStruct, sizeof(msgStruct),0);
          write(sock, buff, sizeof(buff));
          printf("Parameters are send\n");

          bzero(buff, sizeof(buff));
              while(1)
              {

                read(sock, buff, sizeof(buff));
                  if ((strncmp(buff,"TakeAnsver", 10)) == 0)
                   {
                      printf("listenning Ansver\n");
              
                      recv(sock, &msgStruct, sizeof(msgStruct),0);
                      c = msgStruct.c;

                      printf ("\nAnsver of %s is %f\n\n",OperationName[Numeric_Operation],c);
                       
                    break;
                    }

              }
              
              bzero(buff, sizeof(buff));
              break;

        }    
  }
}





int main()
{
 
  
  char IPsave[20] = {0};


  printf("Enter Ip address (Defaut 127.0.0.1)\n");
  scanf("%s",IPsave);
  char *ip = IPsave;
  //char *ip = "127.0.0.1";
  
  printf("Enter Port (Defaut enter 5566)\n");
  int port = 0;
  scanf("%d",&port);
  //int port = 5566;

  printf("Enter first number\n");
  scanf("%lf",&x);
  
  printf("Enter second number\n");
  scanf("%lf",&y);

  printf("Enter number of mathematical operation (1:Addition, 2:Subtraction, 3:Multiplication, 4:Division)\n");
  scanf("%d",&Numeric_Operation);




  int sock;
  struct sockaddr_in addr;
  socklen_t addr_size;
  char buffer[1024]={0};
  int n;
    
  msgStruct.x = x;
  msgStruct.y = y;
  
   printf ("Your First number = %f, Second number = %f\n",x,y);
 
  sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
      perror("[-]Socket error");
      exit(1);
    }
  printf("[+]TCP server socket created.\n");
 
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  addr.sin_addr.s_addr = inet_addr(ip);
 
  
// connect the client socket to server socket
  if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0) 
  {
    printf("connection with the server failed...\n");
    exit(0);
  }
  else
    printf("connected to the server..\n");

  func(sock);
    
  close(sock);
  printf("Disconnected from the server.\n");
 
  return 0;
}
