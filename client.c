#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdbool.h>
#include"tools.h"
void main()
{
	struct sockaddr_in server; //store the information of server
	int socketdescripter;
	//socket descripter
	socketdescripter=socket(AF_INET,SOCK_STREAM,0);// (domain,type,protocol)
	if (socketdescripter < 0)
        printf("Error in client creating\n");
    else
        printf("client Created\n");
	if(socketdescripter!=-1)
	{
		server.sin_family=AF_INET;
		server.sin_addr.s_addr=INADDR_ANY;// client on same machine
		server.sin_port=htons(8888);
		connect(socketdescripter,(struct sockaddr *)&server,sizeof(server));
		do
		{
			printf("\t\t\t***   WELCOME TO BANKING SYSTEM   ***\n");
			printf("1. Sign up\n2. Sign in\n3. Exit\n\n");
			int choice,ch;
			printf("Enter your choice:");
			scanf("%d",&choice);
			switch(choice)
			{
			   case 1:
			   		new_account();
			   		break;
			   case 2:
			   		ch=existing_account(socketdescripter);
			   		printf("****Successfully logged in!!!!!*****\n\n");
			   		printf("ch:%d\n",ch);
			   		showMenu(socketdescripter,ch);
			   		break;
			   case 3:
			   		exit(0);
			   default:
			   		printf("Please select the appropiate choice\n");
			}
			
		}while(1);
	}

	close(socketdescripter);


}
