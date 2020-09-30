#include "../../include/client/client.h"
#include "../../include/util/Packet.hpp"



Client::Client(char *ip_address, char *port)
{
	sockfd = 0;

	serv_addr.sin_family = AF_INET;     
	serv_addr.sin_port = htons(atoi(port));    
	
	if(inet_pton(AF_INET, ip_address, &serv_addr.sin_addr)<=0) 
	{ 
		std::cout << "\nInvalid address/ Address not supported \n" << std::endl; 
	} 

	bzero(&(serv_addr.sin_zero), 8);  
}



int Client::ConnectToServer()
{
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "\n Socket creation error \n" << endl;
		return -1;
	}

	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	{
        cout << "ERROR connecting\n" << endl;
        return -1;
	}

	return 0;
}



int Client::clientCommunication(char* group)
{
	int n;
	char messageBuffer[256] = {0};
	char groupBuffer[256] = {0};
	Packet *mypacket = new Packet((char*)"", messageBuffer);
	int packetSize = sizeof(Packet);
	bool connectedToServer = true;
	string input;

	while(connectedToServer)
    {
    	cout << "Enter the message: ";
		cin >> input;

		strncpy(messageBuffer, input.c_str(), 255);
		strncpy(groupBuffer, group, 19);

		*mypacket = Packet(groupBuffer, messageBuffer);

		n = write(sockfd, mypacket, packetSize);

	    if (n < 0)
	    {
			cout << "ERROR writing to socket\n" << endl;
			return -1;
	    }

		Packet* receivedPacket = readPacket(sockfd, &connectedToServer);
		
		if (!connectedToServer)
		{
			// Free allocated memory for reading Packet
			free(receivedPacket);
			
			break;
		}

		cout << "[Server Message]: " << receivedPacket->message  << endl;

	    if (n < 0)
	    {
			cout << "ERROR reading from socket\n" << endl;
			return -1;
	    }

	    *receivedPacket = Packet();
	}

	free(mypacket);
	close(sockfd);

	return 0;
}
