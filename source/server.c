#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdbool.h>

int validateRecivedContent(int personnr, char *ip, char *contentType, char *accept, char *name)
{
    if (personnr > 9999 || personnr < 999)
    {
        printf("Invalid personnr\n");
        return 1;
    }

    // struntar i ip koll för tillfället
    if (strcmp(contentType, "application/json"))
    {
        printf("Invalid Content-Type\n");
        return 1;
    }

    if (strcmp(accept, "application/json"))
    {
        printf("Invalid Accept\n");
        return 1;
    }

    // struntar i namn för tillfället

    return 0;
}

int main()
{
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;

    int clientAddrLen, bytesRead;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to initialize winsock\n");
        return 1;
    }

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Failed to create socket\n");
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Failed to bind socket\n");
        return 1;
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR)
    {
        printf("Failed to listen on socket\n");
        return 1;
    }

    printf("Server listening on port 8080...\n");

    bool exit = false;
    while (!exit)
    {

        char buffer[1024];
        clientAddrLen = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET)
        {
            printf("Failed to accept client connection\n");
            return 1;
        }

        printf("Client connected: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        int recvPersonnummer = 0;
        char recvOmLabb;
        char recvIpAddress[16];
        char recvContentType[512];
        char recvAccept[512];
        char recvName[512];

        if ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0)
            ;
        sscanf(buffer, "POST /api/1.0/pass/%d%c HTTP/1.1\nHOST: %15s\nContent-Type: %511s\nAccept: %511s\n{name : %511[^}]%*c",
               &recvPersonnummer, &recvOmLabb, recvIpAddress, recvContentType, recvAccept, recvName);
        printf("Recived:\n\tPersonnummer:\t\t%d\n\tHost:\t\t\t%s\n\tContent-type:\t\t%s\n\tAccept:\t\t\t%s\n\tName:\t\t\t%s\n\n",
               recvPersonnummer, recvIpAddress, recvContentType, recvAccept, recvName);

        if (validateRecivedContent(recvPersonnummer, recvIpAddress, recvContentType, recvAccept, recvName) == 0)
        {
            strcpy(buffer, "Well done! B-)\n");
            printf("Message was correct\n");
            FILE *fp;
            fp = fopen("res/lab_4_done.txt", "a+");
            if(fp)
            {
                fprintf(fp,"Namn: %s, Personnummer: %d, Omlabb: %c\n", recvName, recvPersonnummer, recvOmLabb);
                fclose(fp);
            }
            
        }
        else
        {
            strcpy(buffer, "Try again, :-(\n");
            printf("Message was wrong\n");
        }

        send(clientSocket, buffer, sizeof(buffer), 0);
        printf("Informing client of result\n\n");

        closesocket(clientSocket);
        printf("Client disconnected: %s:%d\n\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
