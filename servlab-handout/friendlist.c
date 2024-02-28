/*
 * friendlist.c - [Starting code for] a web-based friend-graph manager.
 *
 * Based on:
 *  tiny.c - A simple, iterative HTTP/1.0 Web server that uses the 
 *      GET method to serve static and dynamic content.
 *   Tiny Web server
 *   Dave O'Hallaron
 *   Carnegie Mellon University
 */
#include "csapp.h"
#include "dictionary.h"
#include "more_string.h"

static void doit(int fd);
static dictionary_t *read_requesthdrs(rio_t *rp);
static void read_postquery(rio_t *rp, dictionary_t *headers, dictionary_t *d);
static void clienterror(int fd, char *cause, char *errnum, 
                        char *shortmsg, char *longmsg);
static void print_stringdictionary(dictionary_t *d);
static void serve_friend_request(int fd, dictionary_t *query);
static void serve_friendlist_request(int fd, dictionary_t * query);
static void serve_unfriend_request(int fd, dictionary_t *query);
static void serve_introduce_request(int fd, dictionary_t *query);
static void serve_request(int fd, dictionary_t *query);

static void send_friendlist(int fd, dictionary_t *list);
static void add_friends(char** friends, char* user);
static void *request_handler(void* connfd_p);

static dictionary_t *users;
sem_t user_sem;

int main(int argc, char **argv) {
  int listenfd, connfd, *connfd_p;
  // int* connfd_p;
  // pthread_t thread;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  pthread_t th;
  /* Check command line args */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  sem_init(&user_sem, 0, 1);
  

  // make a dictionary 
  users = make_dictionary(COMPARE_CASE_SENS, free); // general dictionary for all users

  listenfd = Open_listenfd(argv[1]);

  /* Don't kill the server if there's an error, because
     we want to survive errors due to a client. But we
     do want to report errors. */
  exit_on_error(0);

  /* Also, don't stop on broken connections: */
  Signal(SIGPIPE, SIG_IGN);

  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    if (connfd >= 0) {
      Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                  port, MAXLINE, 0);
      // printf("Accepted connection from (%s, %s)\n", hostname, port);
      connfd_p = malloc(sizeof(int));
      *connfd_p = connfd;
      // make thread to handle request
      // pthread_create(&th, NULL, request_handler, connfd_p);
      // pthread_detach(th);

      Pthread_create(&th, NULL, request_handler, connfd_p);
      Pthread_detach(th);
    }
  }
}

void *request_handler(void* connfd_p) {
  int conn_fd = *(int*)connfd_p;
  free(connfd_p);

  doit(conn_fd);

  Close(conn_fd); 

  return NULL;
}

/*
 * doit - handle one HTTP request/response transaction
 */
void doit(int fd) {
  char buf[MAXLINE], *method, *uri, *version;
  rio_t rio;
  dictionary_t *headers, *query;


  /* Read request line and headers */
  Rio_readinitb(&rio, fd);
  if (Rio_readlineb(&rio, buf, MAXLINE) <= 0)
    return;
  // printf("%s", buf);
  
  if (!parse_request_line(buf, &method, &uri, &version)) {
    clienterror(fd, method, "400", "Bad Request",
                "Friendlist did not recognize the request");
  } else {
    if (strcasecmp(version, "HTTP/1.0")
        && strcasecmp(version, "HTTP/1.1")) {
      clienterror(fd, version, "501", "Not Implemented",
                  "Friendlist does not implement that version");
    } else if (strcasecmp(method, "GET")
               && strcasecmp(method, "POST")) {
      clienterror(fd, method, "501", "Not Implemented",
                  "Friendlist does not implement that method");
    } else {
      headers = read_requesthdrs(&rio);

      // printf("%s \n", (char*)dictionary_get(headers, "Content-Length:"));
      /* Parse all query arguments into a dictionary */
      query = make_dictionary(COMPARE_CASE_SENS, free);
      parse_uriquery(uri, query);
      if (!strcasecmp(method, "POST"))
        read_postquery(&rio, headers, query);

      /* For debugging, print the dictionary */
      // print_stringdictionary(query);

      /* You'll want to handle different queries here,
         but the intial implementation always returns
         nothing: */
      // serve_request(fd, query);

      if (starts_with("/befriend", uri)) {
        serve_friend_request(fd, query);
      } else if (starts_with("/unfriend", uri)) {
        serve_unfriend_request(fd, query);
      } else if (starts_with("/friends", uri)) {
        serve_friendlist_request(fd, query);
      } else if (starts_with("/introduce", uri)) {
        serve_introduce_request(fd, query);
      }

      /* Clean up */
      free_dictionary(query);
      free_dictionary(headers);
    }

    /* Clean up status line */
    free(method);
    free(uri);
    free(version);
  }
}


/*
 * read_requesthdrs - read HTTP request headers
 */
dictionary_t *read_requesthdrs(rio_t *rp) {
  char buf[MAXLINE];
  dictionary_t *d = make_dictionary(COMPARE_CASE_INSENS, free);

  Rio_readlineb(rp, buf, MAXLINE);

  while(strcmp(buf, "\r\n")) {
    Rio_readlineb(rp, buf, MAXLINE);
    // printf("%s", buf);
    parse_header_line(buf, d);
  }

  return d;
}

void read_postquery(rio_t *rp, dictionary_t *headers, dictionary_t *dest) {
  char *len_str, *type, *buffer;
  int len;
  
  len_str = dictionary_get(headers, "Content-Length");
  len = (len_str ? atoi(len_str) : 0);

  type = dictionary_get(headers, "Content-Type");
  
  buffer = malloc(len+1);
  Rio_readnb(rp, buffer, len);
  buffer[len] = 0;

  if (!strcasecmp(type, "application/x-www-form-urlencoded")) {
    parse_query(buffer, dest);
  }

  free(buffer);
}

static char *ok_header(size_t len, const char *content_type) {
  char *len_str, *header;
  
  header = append_strings("HTTP/1.0 200 OK\r\n",
                          "Server: Friendlist Web Server\r\n",
                          "Connection: close\r\n",
                          "Content-length: ", len_str = to_string(len), "\r\n",
                          "Content-type: ", content_type, "\r\n\r\n",
                          NULL);
  free(len_str);

  return header;
}

/*
 * serve_request - example request handler
 */
static void serve_request(int fd, dictionary_t *query) {
  size_t len;
  char *body, *header;

  body = strdup("alice\nbob");

  len = strlen(body);

  /* Send response headers to client */
  header = ok_header(len, "text/html; charset=utf-8");
  Rio_writen(fd, header, strlen(header));
  // printf("Response headers:\n");
  // printf("%s", header);

  free(header);

  /* Send response body to client */
  Rio_writen(fd, body, len);

  free(body);
}

/// @brief 
/// @param fd 
/// @param query - a dictionary of each part of the query -- i.e user = "alice" 
static void serve_friend_request(int fd, dictionary_t *query) {
  // user
  char* user = dictionary_get(query, "user");
  char* friends_to_add = (char*)(dictionary_get(query, "friends"));
  char* decoded = query_decode((char*)dictionary_get(query, "friends"));
  char** friends = split_string(decoded, '\n');
  free(decoded);


  if (!user || !friends_to_add) {
    // null
    clienterror(fd, "Bad Request", "400", "Friendlist did not recognize the request", "Not enough arguments supplied to URL or formatted poorly");
    return;
  }

  P(&user_sem);
  dictionary_t* user_friend_list = (dictionary_t*)dictionary_get(users, user);
  V(&user_sem);
  if (user_friend_list == NULL) {
    user_friend_list = make_dictionary(COMPARE_CASE_SENS, free);

    // insert user into the users dictionary
    P(&user_sem);
    dictionary_set(users, user, user_friend_list);
    V(&user_sem);
  }

  int index = 0;
  // while friends[index] != NULL
  while (friends[index]) {
    P(&user_sem);
    dictionary_t* friend = dictionary_get(user_friend_list, friends[index]);
    V(&user_sem);

    if (!friend) {
      if (!strcmp(user, friends[index])) {
        // do not add user to user friend_list
        continue;
      }

      // only add unique friends
      P(&user_sem);
      dictionary_set(user_friend_list, friends[index], NULL); // only interested in the friend string as a key
      V(&user_sem);

      dictionary_t* friend_friendlist = (dictionary_t*)dictionary_get(users, friends[index]); // get friends friend list
      if (friend_friendlist == NULL) {
        friend_friendlist = make_dictionary(COMPARE_CASE_SENS, free); // make a new dictionary if no value

        P(&user_sem);
        dictionary_set(users, friends[index], friend_friendlist);
        V(&user_sem);
      }

      // friends should be unique for other friend as well
      P(&user_sem);
      dictionary_set(friend_friendlist, user, NULL);
      V(&user_sem);
    }

    free(friends[index]);

    index++;
  }
  free(friends);

  send_friendlist(fd, user_friend_list);
}

static void send_friendlist(int fd, dictionary_t *list) {
  size_t length = 0;
  char *header;//, *body;

  P(&user_sem);
  int count = dictionary_count(list);
  V(&user_sem);

  if (count != 0) {
    P(&user_sem);
    const char** all_friends = dictionary_keys(list);
    V(&user_sem);

    char* friends = join_strings(all_friends, '\n');
    // body = (char*)malloc(strlen(friends));
    // strcpy(body, friends); 

    length = strlen(friends);

    header = ok_header(length, "text/html; charset=utf-8");

    Rio_writen(fd, header, strlen(header));
    free(header);

    Rio_writen(fd, friends, length);

    free(friends);

    free((void*)all_friends);
    // free(body);
  } else {
    // no friends to display return empty response
    header = ok_header(length, "text/html; charset=utf-8");

    Rio_writen(fd, header, strlen(header));
    free(header);
  }
}


static void serve_friendlist_request(int fd, dictionary_t * query) {
  char* user = dictionary_get(query, "user");

  if (!user) {
    clienterror(fd, "Bad Request", "400", "Friendlist doesn't recognize that request", "Not enough arguments supplied to the URL");
    return;
  }

  P(&user_sem);
  dictionary_t *user_friend_list = (dictionary_t*)dictionary_get(users, user);
  V(&user_sem);

  if (!user_friend_list) {
    // make a entry for user
    user_friend_list = make_dictionary(COMPARE_CASE_SENS, free);

    P(&user_sem);
    dictionary_set(users, user, user_friend_list);
    V(&user_sem);
  }

  send_friendlist(fd, user_friend_list);
}

static void serve_unfriend_request(int fd, dictionary_t *query) {
  // user
  char* user = dictionary_get(query, "user");
  char* friend = (char*)dictionary_get(query, "friends");

  if (!user || !friend) {
    clienterror(fd, "Bad Request", "400", "Friendlist doesn't recognize that request", "Not enough arguments supplied or arguments formated poorly to URL");
    return;
  }
  char* decoded = query_decode((char*)dictionary_get(query, "friends"));
  char** friends = split_string(decoded, '\n');
  free(decoded);

  P(&user_sem);
  dictionary_t* user_friend_list = (dictionary_t*)dictionary_get(users, user);
  V(&user_sem);

  if (user_friend_list == NULL) {
    user_friend_list = make_dictionary(COMPARE_CASE_SENS, free);

    // insert user into the users dictionary
    P(&user_sem);
    dictionary_set(users, user, user_friend_list);
    V(&user_sem);
  }

  int index = 0;
  // while friends[index] != NULL
  while (friends[index]) {
    P(&user_sem);
    dictionary_remove(user_friend_list, friends[index]); // removes even if there isn't an entry
    V(&user_sem);

    dictionary_t* friend_friendlist = (dictionary_t*)dictionary_get(users, friends[index]);
    if (!friend_friendlist) {
      // might be redundant
      friend_friendlist = make_dictionary(COMPARE_CASE_SENS, free);

      P(&user_sem);
      dictionary_set(users, user, friend_friendlist);
      V(&user_sem);
    }

    P(&user_sem);
    dictionary_remove(friend_friendlist, user);
    V(&user_sem);

    free(friends[index]);

    index++;
  }
  free(friends);
  
  send_friendlist(fd, user_friend_list);
}

/// @brief 
/// @param fd 
/// @param query - Assumes the friend is a singular non Null terminating string
static void serve_introduce_request(int fd, dictionary_t *query) {
  // prepare request
  int s_fd;
  char* hostName, *portno, *user, *friend, *request;

  hostName = dictionary_get(query, "host");
  portno = dictionary_get(query, "port");
  friend = dictionary_get(query, "friend");
  user = dictionary_get(query, "user");

  if (!hostName || !portno || !friend || !user) {
    clienterror(fd, "Bad Request", "400", "Friendlist doesn't recognize that request", "Not enough arguments or arguments are poorly formated in URL");
    return;
  }
  // connect
  s_fd = Open_clientfd(hostName, portno);

  if (s_fd != -1) {
    // request line for the server
    request = append_strings("GET /friends?user=", friend, " HTTP/1.1\r\nHost: ", hostName, "\r\n\r\n", NULL);

    // socket is valid -- send request to server
    Send(s_fd, request, strlen(request), 0);
    free(request);

    // read the response -- Check the status
    rio_t rio;
    char buf[MAXLINE] = {""};
    
    // read header
    Rio_readinitb(&rio, s_fd);
    Rio_readlineb(&rio, buf, MAXLINE);
    
    char** status_line = split_string(buf, ' ');
    if (strcmp("200", status_line[1])) {
      // status is not 200
      // char* header = append_strings("Failed to retrieve data from server ", status_line[1], " Please try again \n", NULL);
      // Rio_writen(fd, header, sizeof(header));
      // free(header);

      return;
    }

    // free status_line data
    int i = 0;
    while(status_line[i] != NULL) {
      free(status_line[i]);
      i++;
    }
    free(status_line);

    int body_length = 0;
    // read the rest of the header
    while(strcmp(buf, "\r\n")) {
      Rio_readlineb(&rio, buf, MAXLINE);

      if (starts_with("Content-length:", buf)) {
        char** strings = split_string(buf, ' ');
        body_length = atoi(strings[1]);
        free(strings[0]);
        free(strings[1]);
        free(strings[2]);
        free(strings);
      }
    }

    char* all_char = calloc(body_length + sizeof(friend), sizeof(char));
    while(Rio_readlineb(&rio, buf, MAXLINE) != 0) {
      strcat(all_char, buf);
    }

    strcat(all_char, friend);

    char** friends = split_string(all_char, '\n');
    free(all_char);

    add_friends(friends, user);
    i = 0;
    while(friends[i] != NULL) {
      free(friends[i]);
      i++;
    }
    free(friends);

    // close connection
    close(s_fd);

    //send response to client
    char* header = ok_header(0, "text/html; charset=utf-8");
    Rio_writen(fd, header, strlen(header));

    free(header);
  }  else {
    clienterror(fd, "Failed to connect to server", "400", "Ensure <Host> and <Port> are valid location", "Filler");
  }
}

void add_friends(char** friends, char* user) {
    P(&user_sem);
    dictionary_t* user_friend_list = (dictionary_t*)dictionary_get(users, user);
    V(&user_sem);

    // add friend
    int i = 0;
    while(friends[i]) {
      // friends != null
      P(&user_sem);
      dictionary_t* friend = dictionary_get(user_friend_list, friends[i]);
      V(&user_sem);
      if (!friend) {
        if (!strcmp(user, friends[i])) {
          // do not add user to user friend_list
          i++;
          continue;
        }

        // only add unique friends
        P(&user_sem);
        dictionary_set(user_friend_list, friends[i], NULL); // only interested in the friend string as a key
        V(&user_sem);


        dictionary_t* friend_friendlist = (dictionary_t*)dictionary_get(users, friends[i]); // get friends friend list
        if (friend_friendlist == NULL) {
          friend_friendlist = make_dictionary(COMPARE_CASE_SENS, free); // make a new dictionary if no value

          P(&user_sem);
          dictionary_set(users, friends[i], friend_friendlist);
          V(&user_sem);
        }

        // friends should be unique for other friend as well
        P(&user_sem);
        dictionary_set(friend_friendlist, user, NULL);
        V(&user_sem);
      }
      i++;
    }
}

/*
 * clienterror - returns an error message to the client
 */
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) {
  size_t len;
  char *header, *body, *len_str;

  body = append_strings("<html><title>Friendlist Error</title>",
                        "<body bgcolor=""ffffff"">\r\n",
                        errnum, " ", shortmsg,
                        "<p>", longmsg, ": ", cause,
                        "<hr><em>Friendlist Server</em>\r\n",
                        NULL);
  len = strlen(body);

  /* Print the HTTP response */
  header = append_strings("HTTP/1.0 ", errnum, " ", shortmsg, "\r\n",
                          "Content-type: text/html; charset=utf-8\r\n",
                          "Content-length: ", len_str = to_string(len), "\r\n\r\n",
                          NULL);
  free(len_str);
  
  Rio_writen(fd, header, strlen(header));
  Rio_writen(fd, body, len);

  free(header);
  free(body);
}

static void print_stringdictionary(dictionary_t *d) {
  int i, count;

  count = dictionary_count(d);
  for (i = 0; i < count; i++) {
    printf("%s=%s\n",
           dictionary_key(d, i),
           (const char *)dictionary_value(d, i));
  }
  printf("\n");
}
