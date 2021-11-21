#include "../common.h"
#include "../utils.h"

#define BACKLOG 1024

void* handle_connection(void *arg);
void* thread_function(void*);

pthread_mutex_t pool_lock, print_lock;
pthread_cond_t pool_ready;

typedef struct {
    int *fd;
} Connection;

std::queue<Connection*> connections;
std::string dictionary[DICTIONARY_SIZE];
pthread_mutex_t dictionary_lock[DICTIONARY_SIZE];

std::string dict_insert(int key, std::string value);
std::string dict_delete(int key);
std::string dict_update(int key, std::string value);
std::string dict_concat(int key1, int key2);
std::string dict_fetch(int key);

int main(int argc, char **argv)
{
    int num_pool_threads = 0;
    int                     listenfd, connfd, n;
    int                     sendbytes;
    struct sockaddr_in      servaddr;

    if (argc != 2)
        err_n_die("usage: %s <number of workers in thread pool>", argv[0]);

    num_pool_threads = atoi(argv[1]);

    if (num_pool_threads == 0)
        err_n_die("error: number of threads in thread pool must be greater than 0");

    pthread_t thread_pool[num_pool_threads];

    pthread_mutex_init(&pool_lock, NULL);
    pthread_mutex_init(&print_lock, NULL);
    pthread_cond_init(&pool_ready, NULL);

    for (int i = 0; i < DICTIONARY_SIZE; i++) {
        dictionary[i] = "";
        pthread_mutex_init(&dictionary_lock[i], NULL);
    }

    for (int thread_num=0; thread_num<num_pool_threads; thread_num++)
    {
        pthread_create(&thread_pool[thread_num], NULL, thread_function, NULL);
    }

    if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("Error while creating the socket!");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    if ( (bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0)
        err_n_die("Error while binding the socket!");

    if (listen(listenfd, BACKLOG) < 0)
        err_n_die("Error while listening to the socket!");

    std::cout << COLOR_GREEN;
    std::cout << "Server is running on port " << SERVER_PORT;
    std::cout << " with a pool of " << num_pool_threads << " threads" << std::endl;
    std::cout << COLOR_RESET;

    for ( ; ; )
    {
        struct sockaddr_in addr;
        socklen_t addr_len;
        char client_addr[MAXLINE+1];

        printf(COLOR_YELLOW);
        printf("Waiting for connections on port %d...\n", SERVER_PORT);
        printf(COLOR_RESET);
        fflush(stdout);

        connfd = accept(listenfd, (SA *) &addr, &addr_len);

        // pthread_t conn_thread;
        pthread_mutex_lock(&pool_lock);
        int *client_pntr = (int *)malloc(sizeof(int));
        *client_pntr = connfd;
        Connection *conn = new Connection();
        conn->fd = client_pntr;
        connections.push(conn);
        pthread_mutex_unlock(&pool_lock);
        pthread_cond_signal(&pool_ready);
        // pthread_create(&conn_thread, NULL, handle_connection, (void*)client_pntr);

    }

    return 0;
}

void* thread_function(void* arg)
{
    while (true)
    {
        pthread_mutex_lock(&pool_lock);
        while (connections.size() == 0)
        {
            pthread_cond_wait(&pool_ready, &pool_lock);
        }
        Connection *conn = connections.front();
        connections.pop();
        pthread_mutex_unlock(&pool_lock);
        handle_connection(conn->fd);
    }

    return NULL;
}

void* handle_connection(void* arg)
{
    int connfd = *((int*)arg);
    free(arg);
    size_t bytes_read;
    char recvline[MAXLINE+1];
    char sendline[MAXLINE+1];

    memset(recvline, 0, MAXLINE);
    while (( bytes_read = read(connfd, recvline, MAXLINE-1) ) > 0)
    {
        pthread_mutex_lock(&print_lock);
        printf("%sReceived new message: %s%s", COLOR_CYAN, recvline, COLOR_RESET);
        pthread_mutex_unlock(&print_lock);
        if (recvline[bytes_read-1] == '\n')
        {
            break;
        }
        memset(recvline, 0, MAXLINE);
    }

    if (bytes_read < 0)
        err_n_die("Error while reading from the socket!");

    std::stringstream tokenizer(recvline);
    std::string command;
    int key1, key2;
    std::string value;

    tokenizer >> command;
    tokenizer >> key1;

    if (command == "concat")
    {
        tokenizer >> key2;
    }
    else if (command != "delete" && command != "fetch")
    {
        tokenizer >> value;
    }

    std::string response;

    if (command == "insert")
        response = dict_insert(key1, value);
    else if (command == "delete")
        response = dict_delete(key1);
    else if (command == "update")
        response = dict_update(key1, value);
    else if (command == "concat")
        response = dict_concat(key1, key2);
    else if (command == "fetch")
        response = dict_fetch(key1);


    snprintf(sendline, MAXLINE, "%s", response.c_str());

    sleep(2);

    write(connfd, sendline, strlen(sendline));
    close(connfd);
    return NULL;
}

std::string dict_insert(int key, std::string value)
{
    pthread_mutex_lock(&dictionary_lock[key]);
    if (dictionary[key] != "")
    {
        pthread_mutex_unlock(&dictionary_lock[key]);
        return "Key already exists";
    }
    dictionary[key] = value;
    pthread_mutex_unlock(&dictionary_lock[key]);
    return "Insertion successful";
}

std::string dict_delete(int key)
{
    pthread_mutex_lock(&dictionary_lock[key]);
    if (dictionary[key] == "")
    {
        pthread_mutex_unlock(&dictionary_lock[key]);
        return "No such key exists";
    }
    dictionary[key] = "";
    pthread_mutex_unlock(&dictionary_lock[key]);
    return "Deletion successful";
}

std::string dict_update(int key, std::string value)
{
    pthread_mutex_lock(&dictionary_lock[key]);
    if (dictionary[key] == "")
    {
        pthread_mutex_unlock(&dictionary_lock[key]);
        return "Key does not exist";
    }
    dictionary[key] = value;
    std::string ret = dictionary[key];
    pthread_mutex_unlock(&dictionary_lock[key]);
    return ret;
}

std::string dict_concat(int key1, int key2)
{
    pthread_mutex_lock(&dictionary_lock[key1]);
    pthread_mutex_lock(&dictionary_lock[key2]);
    if (dictionary[key1] == "" || dictionary[key2] == "")
    {
        pthread_mutex_unlock(&dictionary_lock[key1]);
        pthread_mutex_unlock(&dictionary_lock[key2]);
        return "Concat failed as at least one of the keys does not exist";
    }
    std::string val1 = dictionary[key1], val2 = dictionary[key2];
    dictionary[key1] = val1 + val2;
    dictionary[key2] = val2 + val1;
    std::string ret = dictionary[key2];
    pthread_mutex_unlock(&dictionary_lock[key1]);
    pthread_mutex_unlock(&dictionary_lock[key2]);
    return ret;
}

std::string dict_fetch(int key)
{
    pthread_mutex_lock(&dictionary_lock[key]);
    if (dictionary[key] == "")
    {
        pthread_mutex_unlock(&dictionary_lock[key]);
        return "Key does not exist";
    }
    std::string ret = dictionary[key];
    pthread_mutex_unlock(&dictionary_lock[key]);
    return ret;
}
