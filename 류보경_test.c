#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[])
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    char buf[BUF_SIZE];
    
    char webpage[] =
                "HTTP/1.1 200 OK\r\n"
                "Server: Linux Web Server\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                "<!DOCTYPE html>\r\n"
                "<html><head><title> My Web Page </title>\r\n"
                "<style>body {background-color: #FFFF00 }</style></head>\r\n"
                "<body><center><h1>Hello world!!</h1><br>\r\n"
                "<img src=\"sheep.jpg\"></center></body></html>\r\n";

    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    server_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(server_fd == -1){
        perror("socket failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        perror("bind failed");
        close(server_fd);
        exit(1);
    }

    if(listen(server_fd, 5) == -1){
        perror("listen failed");
        close(server_fd);
        exit(1);
    }

    while(1) {
        client_addr_size = sizeof(client_addr);
        if((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_size)) == -1){
            perror("accept failed");
            exit(1);
        }

        int len = read(client_fd, buf, BUF_SIZE - 1);
        if(len <= 0) {
            close(client_fd);
            exit(1);
        }
        buf[len] = 0;

        if(strstr(buf, "GET /sheep.jpg ") != NULL) {
            int img_fd = open("sheep.jpg", O_RDONLY);
            if(img_fd == -1){
                perror("image open failed");
                close(client_fd);
            }

            struct stat st;
            fstat(img_fd, &st);
            int img_size = st.st_size;

            char header[BUF_SIZE];
            sprintf(header,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: image/jpeg\r\n"
                "Content-Length: %d\r\n"
                "Connection: close\r\n\r\n",
                img_size);
            write(client_fd, header, strlen(header));

            int n;
            while((n = read(img_fd, buf, BUF_SIZE)) > 0) {
                write(client_fd, buf, n);
            }
            close(img_fd);
        }
        else {
            write(client_fd, webpage, strlen(webpage));
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
