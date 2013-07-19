#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int create_tcp_socket();
void build_get_query(char *host, char *query);

#define HOST "Embed3 Tech"
#define PAGE "/"
#define PORT 80
#define USERAGENT "Embed3"
 
int main(int argc, char *argv[])
{
	struct sockaddr_in remote;
  	int sock;
  	int tmpres;
  	char ip[] = "74.125.136.141";
  	char get[200];
  	char buf[BUFSIZ+1];
  	char host[] = "crypto-class.appspot.com";
 
	if(argc > 1){
    		printf("Incorrect Arguments\n");
		return -1;
  	}

//	while(1) {
	strcpy(ip, "74.125.136.141");
	strcpy(host, "crypto-class.appspot.com");
  	sock = create_tcp_socket();
  	printf("IP is %s\n", ip);
  	remote.sin_family = AF_INET;
  	tmpres = inet_pton(AF_INET, ip, (void *)(&(remote.sin_addr.s_addr)));

  	if( tmpres < 0)  {
		perror("Can't set remote.sin_addr.s_addr");
    		exit(1);
  	}

	else if(tmpres == 0) {
    		printf("%s is not a valid IP address\n", ip);
    		exit(1);
  	}

  	remote.sin_port = htons(PORT);
 

 	 if(connect(sock, (struct sockaddr *)&remote, sizeof(struct sockaddr)) < 0){
    		perror("Could not connect");
    		exit(1);
  	}
  	
	build_get_query("crypto-class.appspot.com", get);
  	printf("Query is:\n<<START>>\n%s<<END>>\n", get);
 
  //Send the query to the server
  	int sent = 0;
  	while(sent < strlen(get)) {
    		tmpres = send(sock, get+sent, strlen(get)-sent, 0);
	    	if(tmpres == -1) {
			perror("Can't send query");
      			exit(1);
   		}

    		sent += tmpres;
  	}
  //now it is time to receive the page
  	memset(buf, 0, sizeof(buf));
  	int htmlstart = 0;
  	char * htmlcontent;
	while((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0) {
		printf("\n%s", buf);
		if(tmpres != BUFSIZ)
			break;
  	}

  	if(tmpres < 0) {
    		perror("Error receiving data");
  	}
 
  	close(sock);
//	sleep(500);

//}


 	return 0;
}
 
 
int create_tcp_socket()
{
 	int sock;
  	if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    		perror("Can't create TCP socket");
    		exit(1);
  	}
  	return sock;
}
 
 
 
void build_get_query(char *host, char *query)
{
	char c[] = "f20bdba6ff29eed7b046d1df9fb7000058b1ffb4210a580f748b4ac714c001bd4a61044426fb515dad3f21f18aa577c0bdf302936266926ff37dbf7035d5eeb4";
  	const char tpl[] = "GET /%s%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
  // -5 is to consider the %s %s %s in tpl and the ending \0
  	sprintf(query, tpl, "po?er=", "HELLO", host, USERAGENT);
}

