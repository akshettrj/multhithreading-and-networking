#include "../common.h"
#include "../utils.h"

char SERVER_ADDRESS[] = "127.0.0.1";

typedef struct Request
{
    int req_num;
    int time_delay;
    std::string command;
    int key1;
    int key2;
    std::string value;
} Request;

Request* create_new_request()
{
    Request* request = new Request();
    request->req_num = 0;
    request->time_delay = 0;
    request->key1 = 0;
    request->key2 = 0;
    return request;
}

void *client_thread(void *data)
{
    Request *request = (Request*)(data);
    sleep(request->time_delay);

    pthread_t thread_id = pthread_self();

    int sockfd, bytes_read, send_bytes;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("Error while creating the socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_ADDRESS, &servaddr.sin_addr) <= 0)
        err_n_die("inet_pton error for %s\n", SERVER_ADDRESS);

    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        err_n_die("Error while connecting to the server");

    if (request->command == "concat")
        sprintf(sendline, "%s %d %d\n",\
                request->command.c_str(),\
                request->key1,\
                request->key2\
               );

    else if (request->command == "delete" || request->command == "fetch")
        sprintf(sendline, "%s %d\n",\
                request->command.c_str(),\
                request->key1\
               );

    else
        sprintf(sendline, "%s %d %s\n",\
                request->command.c_str(),\
                request->key1,\
                request->value.c_str()\
               );

    send_bytes = strlen(sendline);

    if (write(sockfd, sendline, send_bytes) != send_bytes)
        err_n_die("Error while sending data to the server");

    memset(recvline, 0, MAXLINE);

    while ( (bytes_read = read(sockfd, recvline, MAXLINE-1)) > 0)
    {
        printf("%d:%ld:%s\n", request->req_num, thread_id, recvline);
        memset(recvline, 0, MAXLINE);
    }

    if (bytes_read < 0)
        err_n_die("Error while reading data from the server");

    delete request;
    return NULL;
}

int main(int argc, char **argv)
{
    int sockfd, bytes_read;
    int send_bytes;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];

    int num_requests = 0;
    std::cin >> num_requests;

    pthread_t client_threads[num_requests];

    for (int req_num=0; req_num<num_requests; req_num++)
    {
        Request *request = create_new_request();
        request->req_num = req_num;

        std::cin >> request->time_delay;
        std::cin >> request->command;
        std::cin >> request->key1;

        if (request->command == "concat")
        {
            std::cin >> request->key2;
        }
        else if (request->command != "delete" && request->command != "fetch")
        {
            std::cin >> request->value;
        }

        pthread_create(&client_threads[req_num], NULL, client_thread, (void*)request);
    }

    for (int req_num=0; req_num<num_requests; req_num++)
    {
        pthread_join(client_threads[req_num], NULL);
    }

    return 0;
}
