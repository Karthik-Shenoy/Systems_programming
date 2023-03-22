#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
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
	char buff[MAXSIZE];
	int sockfd,retval,i;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("\nSocket Creation Error");
		return;
	}

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		printf("Connection error");
		return;
	}

	for (i = 0; ; i+=1)
	{
	
		printf("1.Display\n2.replace\n3.exit");
		memset(buff, '\0', sizeof(buff));
		printf("enter the text\n");
		scanf(" %c",&buff[0]);
		char ch;
		ch = buff[0];
		buff[strlen(buff)] = '\0';
		if(buff[0] == '2')
		{
			char new_a[50], n_mobile[50];
			printf("Enter the mobile number : \n");
			scanf(" %[^\n]s", n_mobile);
			printf("Enter the new alnum value : \n");
			scanf(" %[^\n]s", new_a);
			strcat(buff, "-");
			strcat(buff, n_mobile);
			strcat(buff, "-");
			strcat(buff, new_a);
		}
		buff[strlen(buff)] = '\0';
		sentbytes=send(sockfd,buff,sizeof(buff),0);
		if(sentbytes==-1)
		{
			printf("!!");
			close(sockfd);
		}
		if (buff[0] == '3')
		{
			break;
		}
		memset(buff, '\0', sizeof(buff));
		recedbytes=recv(sockfd,buff,sizeof(buff),0);
		if(ch == '1')
		{
			printf("name, mobile_num, alpha_numeric\n");
			char* token = strtok(buff, "-");
			while(token != NULL)
			{
				printf("%s\n", token);
				token = strtok(NULL, "-");
			}
		}
		else
			printf ("%s\n", buff);
	}
	close(sockfd);
}
