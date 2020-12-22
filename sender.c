#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <netinet/tcp.h>
#include <sys/stat.h>


void Send_File(int sockfd) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer)); //make buffer values clean
    FILE *file;
    file = fopen("1mb.txt", "r");
    if (file == NULL) {
        perror("[-]Error in reading file.");
        exit(1);
    }

    int fsize;
    fseek(file, 0, SEEK_END); // seek to end of file
    fsize = ftell(file); // get current file pointer
    fseek(file, 0, SEEK_SET);
    int converted_number = htonl(fsize);
    write(sockfd, &converted_number, sizeof(converted_number));

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (send(sockfd, buffer, sizeof(buffer), 0) == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
    }
    fclose(file);
    printf("[+]File (%d Bytes) sent successfully.\n", fsize);
}


int main() {
    char *ip = "127.0.0.1";
    int port = 8888;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; // set family type
    server_addr.sin_port = htons(port);// set port
    server_addr.sin_addr.s_addr = inet_addr(ip); //convert the string of ip to binary and set ip

    if (inet_pton(AF_INET, ip, &(server_addr.sin_addr)) == 1) { // check if network address was successfully converted
        printf("[+]connect to network\n");
    } else {
        perror("[-]network is disconnected \n");
        exit(1);
    }
    for (int i = 0; i < 5; ++i) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("[-]Error in socket");
            exit(1);
        }
        printf("[+]Server socket created successfully.\n");
        if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {// check connection
            perror("[-]Connection Error");
            exit(1);
        }
        printf("[+]Connected to Server.\n");
        if (i == 0) {
            printf("The current congestion control is: Cubic.\n");
        }
        Send_File(sockfd);
        close(sockfd);
    }

    for (int i = 0; i < 5; ++i) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        char bif[256];
        socklen_t len;
        strcpy(bif, "reno"); // copy the string
        len = strlen(bif);
        if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, bif, len) !=
            0) { // set the socket protocol and check if it's really sets.
            perror("[-]Error in set socket protocol");
            exit(1);
        }
        if (sockfd < 0) {
            perror("[-]Error in socket");
            exit(1);
        }
        printf("[+]Server socket created successfully.\n");
        if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {// check connection
            perror("[-]Connection Error");
            exit(1);
        }
        printf("[+]Connected to Server.\n");
        if (i == 0) {
            printf("The current congestion control is: Reno.\n");
        }
        Send_File(sockfd);
        close(sockfd);
    }
    return 0;
}

