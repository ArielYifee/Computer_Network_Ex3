#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>
#include <netinet/tcp.h>
#include <stdlib.h>

void Get_File(int new_sock, int i) {
    char buff[1024];
    memset(buff, 0, sizeof(buff)); //make buffer values clean
    int read_size = 0;
    int total_size = 0;
    int fsize;
    int flag;
    flag = read(new_sock, &fsize, sizeof(fsize));
    if (flag == -1) {
        perror("[-]Failed getting file size.");
        exit(1);
    }

//    FILE *fp;
//    char *filename = "recv.txt";
//    char index[8];
//    sprintf(index, "%d", i);
//    fp = fopen(strncat(index,filename,8), "w");

    while ((read_size = recv(new_sock, buff, sizeof(buff), 0)) > 0) {
        if (read_size == 0) {
            perror("[-]Client disconnected");
            exit(1);
        } else if (read_size == -1) {
            perror("[-]Failed to receive ");
            exit(1);
        }
        total_size += read_size;
//        fprintf(fp, "%s", buff);
        memset(buff, 0, sizeof(buff));
    }
    printf("[+]File size is: %d Bytes, received: %d Bytes.\n", ntohl(fsize), total_size);
}


int main() {
    // create the server
    char *ip = "127.0.0.1";
    int port = 8888;
    int sockfd, new_sock;
    int e;
    socklen_t addr_size;
    struct sockaddr_in server_addr, clinet_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { //if the socket didn't create
        perror("[-]Error in socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    printf("[+]Server socket created successfully.\n");

    //bind the address and port to socket
    e = bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (e < 0) {
        perror("[-]Error in bind");
        exit(1);
    }
    printf("[+]Binding successfully.\n");

    //listen
    if (listen(sockfd, 10) == 0) {
        printf("[+]Listening....\n");
    } else {
        perror("[-]Error in listening");
        exit(1);
    }
    addr_size = sizeof(clinet_addr);

    struct timeval start1, end1, start2, end2, mid_start1, mid_end1, mid_start2, mid_end2;
    double time_taken1, time_taken2;
    double time_1, time_2;


    for (int i = 0; i < 5; ++i) {
        //connection request
        new_sock = accept(sockfd, (struct sockaddr *) &clinet_addr, &addr_size);
        if (new_sock < 0) {
            perror("[-]Accept failed.\n");
        } else {
            if (i == 0) {
                gettimeofday(&start1, 0);
                printf("[+]Cubic\n");
            }
            printf("[+]Connection accepted.\n");
        }
        gettimeofday(&mid_start1, 0);
        Get_File(new_sock, i);
        close(new_sock);
        gettimeofday(&mid_end1, 0);
        long seconds = mid_end1.tv_sec - mid_start1.tv_sec;
        long microseconds = mid_end1.tv_usec - mid_start1.tv_usec;
        time_1 = seconds + microseconds * 1e-6;
        printf("Cubic number %d time = %.5f\n", i + 1, time_1);
    }
    gettimeofday(&end1, 0);
    long seconds1 = end1.tv_sec - start1.tv_sec;
    long microseconds1 = end1.tv_usec - start1.tv_usec;
    time_taken1 = seconds1 + microseconds1 * 1e-6;
    printf("Total cubic time = %.5f\n", time_taken1);
    printf("Average cubic time = %.5f\n", time_taken1 / 5);

    for (int i = 0; i < 5; i++) {
        char buff[256];
        strcpy(buff, "reno");
        addr_size = strlen(buff);
        if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, buff, addr_size) != 0) {
            printf("Failed in reno socket.\n");
            exit(1);
        }
        new_sock = accept(sockfd, (struct sockaddr *) &clinet_addr, &addr_size);
        addr_size = sizeof(buff);
        if (getsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, buff, &addr_size) != 0) {
            perror("Reno failed");
            exit(1);
        }
        if (new_sock < 0) {
            perror("[-]Accept failed.\n");
        } else {
            if (i == 0) {
                gettimeofday(&start2, 0);
                printf("[+]Reno\n");
            }
            printf("[+]Connection accepted\n");
        }
        gettimeofday(&mid_start2, 0);
        Get_File(new_sock, i + 5);
        close(new_sock);
        gettimeofday(&mid_end2, 0);
        long seconds = mid_end2.tv_sec - mid_start2.tv_sec;
        long microseconds = mid_end2.tv_usec - mid_start2.tv_usec;
        time_2 = seconds + microseconds * 1e-6;
        printf("Reno number %d time = %.5f\n", i + 1, time_2);
    }
    gettimeofday(&end2, 0);
    long seconds2 = end2.tv_sec - start2.tv_sec;
    long microseconds2 = end2.tv_usec - start2.tv_usec;
    time_taken2 = seconds2 + microseconds2 * 1e-6;
    printf("Total reno time = %.5f\n", time_taken2);
    printf("Average reno time = %.5f\n", time_taken2 / 5);

    int def = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &def, sizeof(int));
    close(sockfd);
    return 0;
}

