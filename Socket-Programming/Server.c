#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<unistd.h>

void main()
{
	int s, r, send_flag, recv_flag, port_num, ns, b;
	printf("enter the port number : ");
	scanf("%d", &port_num);
	socklen_t len;
	struct sockaddr_in client, server;
	char buff[50];
	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s == -1)
	{
		printf("\nServer creation failed");
		exit(0);
	}
	printf("\nsocket sucessfully created");
	server.sin_family = AF_INET;
	server.sin_port = htons(port_num);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	len = sizeof(client);
	b = bind(s, (struct sockaddr*)&server, sizeof(server));
	if(b == -1)
	{
		printf("\nBinding Failed");
		exit(0);
	}
	printf("\nBinded sucessfully");
	r = listen(s, 1);
	if(r == -1)
	{
		printf("\nsocket listen failed");
		close(s);
		exit(0);
	}
	printf("\nsocket listening");
	ns = accept(s, (struct sockaddr*)& client, &len);
	if(r == -1)
	{
		printf("\nsocket accept failed");
		close(s);
		exit(0);
	}
	printf("\nsocket accepting");
	recv_flag = recv(ns, buff, sizeof(buff), 0);
	if(recv_flag == -1)
	{
		printf("\nmessege receive failed");
		close(s);
		close(ns);
		exit(0);
	}
	printf("\nMessege Received\n");
	/*
	*/
	int arr[1];
	if( access( buff, F_OK ) == 0 ) 
    		arr[0] = 1;
	else 
		arr[0] = 0;
	send_flag = send(ns, arr, sizeof(arr), 0);
	if(send_flag == -1)
	{
		printf("\nmessege not sent");
		close(s);
		close(ns);
		exit(0);
	}
	printf("\nmessege succesfully sent\n");
	
	char buff2[100];
	recv_flag = recv(ns, buff2, sizeof(buff2), 0);
	if(recv_flag == -1)
	{
		printf("\nmessege receive failed");
		close(s);
		close(ns);
		exit(0);
	}
	printf("\nMessege Received\n");
	if(buff2[0] == '1')
	{
		FILE *f;
		f = fopen(buff, "r");
		char str[50];
		int i, j;

		for(i = 1; buff2[i] != '\0'; i++)
			str[i-1] = buff2[i];
		str[i-1] = '\0';
		char file_chars[1000];
		
		i = 0;
		while ( 1 )  
		{  
			char ch = fgetc ( f ) ;  
			file_chars[i++] = ch;
			if ( ch == EOF )  
			break ;   
		}
		fclose(f);
		file_chars[i] = '\0';
		
		int count = 0;
		for(int i = 0; file_chars[i] != '\0'; i++)
		{  
			int j = 0;
			while(file_chars[i+j] != '\0' && str[j] != '\0' && file_chars[i+j] == str[j])
				j++;
			if(str[j] == '\0')
				count++;
		}
		
		arr[0] = count;
	
		send_flag = send(ns, arr, sizeof(arr), 0);
		if(send_flag == -1)
		{
			printf("\nmessege not sent");
			close(s);
			close(ns);
			exit(0);
		}
		printf("\nmessege succesfully sent\n");
	}
	else if(buff2[0] == '2')
	{
		FILE *f;
		f = fopen(buff, "r");
		char file_chars[1000];
		int i = 0;
		while( 1 )
		{
			char ch = fgetc( f );
			if(ch == EOF)
				break;
			file_chars[i++] = ch;
		}
		file_chars[i] = '\0';
		int n = 0;
		for(int i = 0; file_chars[i] !=  '\0'; i++)
			n++;
		for(int i = 0; i < n-1; i++)
		{
			for(int j = 0; j < n-i-1; j++)
			{
				if(file_chars[j] > file_chars[j+1])
				{
					char temp = file_chars[j];
					file_chars[j] = file_chars[j+1];
					file_chars[j+1] = temp;
				}
			}
		}
		fclose(f);
		f = fopen(buff, "w");
		int index = 0;
		printf("\nReordering......\n")
		while(file_chars[index] != '\0')
		{
			char ch = file_chars[index];
			fputc(ch, f);
			index++;
		}
		fclose(f);
		
		int arr[1];
		arr[0] = 1;
		send_flag = send(ns, arr, sizeof(arr), 0);
		if(send_flag == -1)
		{
			printf("\nmessege not sent");
			close(s);
			close(ns);
			exit(0);
		}
		printf("\nmessege succesfully sent\n");
 	}
	else if(buff2[0] == '3')
	{
		FILE *f;
		f = fopen(buff, "r");
		char str[50], str2[50];
		int i, j;
		for(i = 1; buff2[i] != '\0'; i++)
			str[i-1] = buff2[i];
		str[i-1] = '\0';
		int index = 0;
		for(j = i+1; buff2[j] != '\0'; j++)
			str2[index++] = buff2[j];
		str2[index] = '\0';

		char file_chars[1000];
		
		i = 0;
		while ( 1 )  
		{  
			char ch = fgetc ( f ) ;  
			file_chars[i++] = ch;
			if ( ch == EOF )  
			break ;   
		}
		fclose(f);
		file_chars[i-1] = '\0';
		
		f = fopen(buff, "w");
		char after[1000];
		int count = 0;
		
		
		index = 0;
		for(int i = 0; file_chars[i] != '\0'; i++)
		{  
			int j = 0;
			while(file_chars[i+j] != '\0' && str[j] != '\0' && file_chars[i+j] == str[j])
				j++;
			if(str[j] == '\0')
			{
				index = 0;
				for(int k = 0; k < i; k++)
					after[index++] = file_chars[k];
				for(int k = 0; str2[k] != '\0'; k++)
					after[index++] = str2[k];
				for(int k = i+j; file_chars[k] != '\0'; k++)
					after[index++] = file_chars[k];
				after[index] = '\0';
				count++;
				break;
			}
		}
		index = 0;
		printf("\n");
		while(after[index] != '\0')
		{
			fputc(after[index], f);
			index++;
		}
		fclose(f);
		arr[0] = count;
	
		send_flag = send(ns, arr, sizeof(arr), 0);
		if(send_flag == -1)
		{
			printf("\nmessege not sent");
			close(s);
			close(ns);
			exit(0);
		}
		printf("\nmessege succesfully sent\n");
	}
	
	printf("\nmessege succesfully sent\n\n");
	close(ns);
	close(s);
}
