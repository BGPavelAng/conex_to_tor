//https://datatracker.ietf.org/doc/html/rfc1928
//https://linux.die.net/man/3/getaddrinfo
//https://linux.die.net/man/3/inet_ntop
//https://en.wikipedia.org/wiki/Endianness#Networking

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define IP_P "127.0.0.1"
#define PORT_tor "9050"



/*int getaddrinfo(const char *node,
const char *service,
const struct addrinfo *hints,
struct addrinfo **servinfo)
servinfo : populated by getaddrinfo() call 

–  contiene información por separado de addrinfo 
*/


int main(int argc, char* argv[]){
	
	int sock, cif;
	struct addrinfo cli, *servinfo, *sec;
	char viewip_port[INET_ADDRSTRLEN];

	//inicio de familia y tipo

   /*domain 
      - AF_INET/AF_INET6 
      - (*servinfo)‐>ai_family 

     type 
      - SOCK_STREAM/SOCK_DGRAM 
      - (*servinfo)‐>ai_socktype 

     protocol 
      - (*servinfo)‐>ai_protocol */


	memset(&cli, 0, sizeof(cli));

	cli.ai_family = AF_UNSPEC;       //0 Permite IPv4 or IPv6
	cli.ai_socktype = SOCK_STREAM;   //1 sock_stream
	cli.ai_protocol = 0;             //0 servicio escogera protocolo

	short port_w = htons(PORT_http_n);


	/*
	- node : domain name / ip address 
	- service : ports
	- hints : socket  
	          - hints.ai_family, hints.ai_socktype, hints.ai_protocol*/

	//const char *gai_strerror(int errcode);


	if(cif = getaddrinfo(IP_P, PORT_tor, &cli, &servinfo) != 0){

		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(cif));

	}

	//creamos el socket(0, 1, 0) y hacemos un loop con ai_next para buscar en las estructuras con la conexión correcta

	for(sec = servinfo; sec != NULL; sec = sec->ai_next){

	if((sock = socket(sec->ai_family, sec->ai_socktype, sec->ai_protocol)) == -1){
		perror("Error en socket");
		continue;
	}

	if((connect(sock, sec->ai_addr, sec->ai_addrlen)) == -1){
		perror("Error en connect");
		close(sock);
		continue;
	}

	break;

	}

	//Verificar que se realizó con exito la conexión AF_INET/AF_INET6

	switch(sec->ai_family){
		case 2: inet_ntop(sec->ai_family, &(((struct sockaddr_in*)sec->ai_addr)->sin_addr), viewip_port, sizeof(viewip_port));
		        printf("[+]Conexión exitosa (Proxy Tor): AF = %x IP = %s PUERTO = %s\n", sec->ai_family, viewip_port, PORT_tor);
		break;

		case 23: inet_ntop(sec->ai_family, &(((struct sockaddr_in*)sec->ai_addr)->sin_addr), viewip_port, sizeof(viewip_port));
		        printf("[+]Conexión exitosa (Proxy Tor): AF = %x IP = %s PUERTO = %s\n", sec->ai_family, viewip_port, PORT_tor);
		break;
	}

	//liberamos getaddrinfo
	freeaddrinfo(servinfo);


	//Comenzar el HandShake con la red Tor

	//Se utiliza Versión, NMetodo y Autenticación
	// 5 <- es para dar a entender que usaremos la versión socket5
	// 1 <- numero de petición
	// 0 <- sin autenticación, si se necesita usuario y contraseña se utilizaría 2, etc... para más en ietf.org 

	char buffToSend[3];  //510

	buffToSend[0] = 0x05;
	buffToSend[1] = 0x01;
	buffToSend[2] = 0x00;

	//int send(int sockfd,const void *msg,int len,int flags);

	send(sock, buffToSend, 3, 0);

	//Si la conexión es exitosa, mandará de regreso 2 bytes, uno con la misma version de socket y aceptando la no autenticación en caso de Tor

	char buffToRecive[2];

	//int recv(int sockfd,void *buf,int len,int flags);

	recv(sock, buffToRecive, 2, 0);

	if(buffToRecive[0] == 0x05 && buffToRecive[1] == 0x00){

		printf("%s\n", "[+]HandShake realizado");
	}


	//Mandemos una dirección para verificar que ya se puede navegar por Tor

	//Version sock5 0x05
	//Commando connect 0x01
	//reservado 0x00
	//Atyp dominio 0x03
	//Destino dirección + 1
	//Destino puerto
  
  
  full source code on my patreon
  código fuente completo en mi patreon
  
  https://www.patreon.com/BGPavelAng
