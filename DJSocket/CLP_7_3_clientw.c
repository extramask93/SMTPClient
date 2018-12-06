#include <stdio.h>
#include <Winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define CALL(x) if(counter++ && !(x))error(#x,__LINE__,counter)

int counter = 2;

void error(char *action, int line, int retc) {
	fprintf(stderr, "'%s' in line #%d failed - client terminated\n", action, line);
	exit(retc);
}

int main(int argc, char *argv[]) {
	if(argc != 4) {
		fprintf(stderr, "usage %s server_addr service_no number_no\n", argv[0]);
		return 1;
	}

	WSADATA wsa;
	CALL( WSAStartup(MAKEWORD(2,2), &wsa) == 0 );

	int port = 0;
	CALL( sscanf(argv[2], "%d", &port) == 1 && port >= 1 && port <= 65535 );

	int numno = 0;
	CALL( sscanf(argv[3], "%d", &numno) == 1 && numno >= 0 && numno <= 10 );

	struct hostent *hent = NULL;
	CALL( (hent = gethostbyname(argv[1])) != NULL );

	struct protoent *pent;
	CALL( (pent = getprotobyname("tcp")) != NULL );

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr = *((struct in_addr *) hent->h_addr);
	memset(addr.sin_zero, 0, 8);

	int sock;
	CALL( (sock = socket(AF_INET, SOCK_STREAM, pent->p_proto)) > 0 );

	CALL( (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr))) == 0 );

	char *draw_cmd = "DRAW\r\n", *quit_cmd = "QUIT\r\n";

	if(numno == 0) {
		char *exitcmd = "EXIT\r\n";
		CALL ( send(sock, exitcmd, strlen(exitcmd), 0) == strlen(exitcmd) );	/***/
		closesocket(sock);							/***/
		return 0;
	}

	while(numno--) {
		CALL ( send(sock, draw_cmd, strlen(draw_cmd), 0) == strlen(draw_cmd) );
		char buff[16];
		int readin = 0;
		CALL( (readin = recv(sock, buff, sizeof(buff) - 1, 0)) > 0 );		/***/
		buff[readin] = '\0';
		printf("%s\n", buff);
		if(numno)
			Sleep(1000);
	}

	CALL ( send(sock, quit_cmd, strlen(quit_cmd), 0) == strlen(quit_cmd) );		/***/

	closesocket(sock);
	WSACleanup();

	return 0;
}