/*zaproponowane poprawki przyniosy dobre efekty */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#define PORT 1200
#include <stdio.h>
#include <stdlib.h>
#define MAX_FILE_SIZE 1024 
//	char buffer[MAX_FILE_SIZE];
char *loadFile(const char *filename) {
char* buffer;
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }
    size_t len = fread(buffer, 1, MAX_FILE_SIZE, file);
    if (ferror(file)) {
        perror("Error reading file");
        fclose(file);
        return NULL;
    }
    fclose(file);
    buffer[len] = '\0';
    return buffer;
}
/*data*/
void send_response2(int connfd, char *status, char *content_type, char *body) {
    char response[1024];

    // Dodanie daty i czasu
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
  char *timestamp = asctime(timeinfo);
  //  char *timestamp = popen("ls","ls");
    strcat(response, timestamp);
sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><body>Aktualny czas: %s</body></html>", timestamp);
    send(connfd, response, strlen(response), 0);
}

void parse_request_line(char *req, char *method, char *uri) {
    sscanf(req, "%s %s", method, uri);
}

void send_response(int connfd, char *status, char *content_type, char *body) {
    char response[1024];
    sprintf(response, "HTTP/1.1 %s\r\nContent-Type: %s\r\n\r\n%s", status, content_type, body);
    send(connfd, response, strlen(response), 0);
}
void handle_post(int connfd, char *data) {
    // Przetwarzanie danych z POST
    printf("Received POST data: %s\n", data);
    // ... (zapis do bazy danych, itp.)
    send_response(connfd, "201 Created", "text/plain", "Resource created");
}

void handle_put(int connfd, char *data) {
    // Przetwarzanie danych z PUT
    printf("Received PUT data: %s\n", data);
    // ... (aktualizacja zasobu)
    send_response(connfd, "200 OK", "text/plain", "Resource updated");
}
int main(int argc, char *argv[]) {
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen = sizeof(cliaddr);

    char buffer[1024];

    // Tworzenie gniazda
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully\n");

    // Konfiguracja adresu serwera
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Nasłuchuj na wszystkich interfejsach
    serv_addr.sin_port = htons(PORT);

    // Bindowanie gniazda do adresu
    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Binding successful\n");

    // Nasłuchiwanie połączeń
    if (listen(listenfd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Listening...\n");

    while (1) {
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddrlen);
        if (connfd < 0) {
            perror("accept");
            continue; 
        }
        if (recv(connfd, buffer, sizeof(buffer), 0) < 0) {
            perror("recv");
            close(connfd);
            continue; // Obsługa błędu: zamknij połączenie i kontynuuj pętlę
        }

        // Parsuj żądanie
        char method[20], uri[1024];
        parse_request_line(buffer, method, uri);

        // Obsługa żądania
  // ... (reszta kodu)

// Obsługa żądania
if (strcmp(method, "GET") == 0) {
    if (strcmp(uri, "/") == 0) {
        send_response2(connfd, "200 OK", "text/html", "<!DOCTYPE html><html><body>Hello from my server!</body></html>");
    } else if (strcmp(uri, "/coments") == 0) {
        char *content = loadFile("/sdcard/input.html");
        if (content) {
            send_response(connfd, "200 OK", "text/html", content);
        } else {
            send_response(connfd, "500 Internal Server Error", "text/plain", "Error loading file");
        }
    } else {
        send_response(connfd, "404 Not Found", "text/plain", "Page not found");
    }
} else if (strcmp(method, "POST") == 0) {
    handle_post(connfd, buffer);
} else if (strcmp(method, "PUT") == 0) {
    handle_put(connfd, buffer);
} else {
    send_response(connfd, "501 Not Implemented", "text/plain", "Method not implemented");
}


        close(connfd); // Zamknij połączenie po obsłużeniu żądania
    }

    close(listenfd); // Zamknij główne gniazdo
    return 0;
}
