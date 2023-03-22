#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/wait.h>
#define MAXSIZE 1000

struct student{
	char name[50];
	char mobile[50];
	char alphanum[50];
};

void to_string(struct student s1, char* arr)
{
	strcpy(arr, "");
	strcat(arr, s1.name);
	strcat(arr, ",");
	strcat(arr, s1.mobile);
	strcat(arr, ",");
	strcat(arr, s1.alphanum);
}

void from_string(struct student *s1, char *arr)
{
	char s_arr[3][50];
	int index = 0;
	char *token = strtok(arr, ",");
	
	while( token != NULL )
	{
		strcpy(s_arr[index], token);
		index++;
		token = strtok(NULL, ",");
	}
	strcpy(s1->name, s_arr[0]);
	strcpy(s1->mobile, s_arr[1]);
	strcpy(s1->alphanum, s_arr[2]);
}

void main()
{
	int sockfd,newsockfd,retval,i;
	socklen_t actuallen;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;
	char buff[MAXSIZE];
	struct student slst[4];
	char brr[100];
	strcpy(brr, "karthik,123456,1b2c3d");
	from_string(&slst[0], brr);
	strcpy(brr, "vasanth,234567,b2c3d4");
	from_string(&slst[1], brr);
	strcpy(brr, "vignesh,345678,2c3d4e5");
	from_string(&slst[2], brr);
	strcpy(brr, "pradeep,456789,c3d4e5");
	from_string(&slst[3], brr);
	int a=0;
	sockfd=socket(AF_INET,SOCK_STREAM,0);

	if(sockfd==-1)
	{
	printf("\nSocket creation error");
	}

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=htons(INADDR_ANY);
	retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==1)
	{
		printf("Binding error");
		close(sockfd);
	}

	retval=listen(sockfd,1);
	if(retval==-1)
	{
	close(sockfd);
	}
	actuallen=sizeof(clientaddr);
	newsockfd=accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);
	if(newsockfd==-1)
	{
		close(sockfd);
	}
	
	for (i = 0; ; i+=1)
	{
		
		memset(buff, '\0', sizeof(buff));
		recedbytes=recv(newsockfd,buff,sizeof(buff),0);
		if(recedbytes==-1)
		{
			close(sockfd);
			close(newsockfd);
		}
		
		if (buff[0] == '3')
		{
			break;
		}
		
		int pi[2];
		pipe(pi);
		
		int pid = fork();
		if(pid == 0)
		{
			char msg[50];
			strcpy(msg, "-1,-1");
			if(buff[0] == '2')
			{
				char values[3][50];
				int index = 0;
				char *token = strtok(buff, "-");
				while(token != NULL)
				{
					strcpy(values[index], token);
					index++;
					token = strtok(NULL, "-");
				}
				int flag = 0;
				
				for(int i = 0; i < 4; i++)
				{
					if(strcmp(slst[i].mobile,values[1]) == 0)
					{
						char x[2];
						x[0] = '0'+i;
						x[1] = '\0';
						
 						strcpy(msg, x);
 						strcat(msg, ",");
 						strcat(msg, values[2]);
 						
						flag = 1;
						break;
					}
				}
				memset(buff, '\0', sizeof(buff));
				if(flag)
					strcpy(buff, "sucessfull\n");
				else
					strcpy(buff, "unsucessfull\n");
				buff[strlen(buff)] = '\0';
				sentbytes=send(newsockfd,buff,sizeof(buff),0);
				if(sentbytes==-1)
				{
					close(sockfd);
					close(newsockfd);
				}
			}
			close(pi[0]);
			write(pi[1], msg, strlen(msg));
			return;
		}
		else
		{
			wait(NULL);
			close(pi[1]);
			//close(1);
			char msg[50];
			int nbytes = read(pi[0], msg, 40);
			char vals[2][50];
			char *tok = strtok(msg, ",");
			int index = 0;
			while(tok != NULL)
			{
				strcpy(vals[index], tok);
				index++;
				tok = strtok(NULL, ",");
			}
			index = atoi(vals[0]);
			strcpy(slst[index].alphanum, vals[1]);
			if(buff[0] == '1')
			{
				memset(buff, '\0', sizeof(buff));
				strcpy(buff, "");
				for(int i = 0; i < 3; i++)
				{
					char crr[100];
					to_string(slst[i], crr);
					strcat(buff, crr);
					strcat(buff, "-");
				}
				char crr[100];
				to_string(slst[3], crr);
				strcat(buff, crr);
				
				buff[strlen(buff)] = '\0';
				sentbytes=send(newsockfd,buff,sizeof(buff),0);
				if(sentbytes==-1)
				{
					close(sockfd);
					close(newsockfd);
				}
			}
		}
		
		
		
		
	}
	close(sockfd);
	close(newsockfd);
}

