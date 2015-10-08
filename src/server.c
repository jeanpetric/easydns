/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
 *    Description:  Get an IP address and update the server with it.
 *
 *        Created:  07/10/15 20:53:20
 *       Compiler:  gcc
 *
 *         Author:  Jean Petric (petricomp@gmail.com), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <curl/curl.h>

static const char my_ip_url[] = "http://ipinfo.io/ip";

typedef struct Configuration
{
  long unsigned int update_interval;
  char *url_destination;
} Configuration;

typedef struct string
{
  char *ptr;
  size_t len;

} String;

void url_upload(String *ip, void *arg)
{
  CURL *curl;
  CURLcode response;
  Configuration *conf = (Configuration*) arg;
  char url[21] = "url=";

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, conf->url_destination);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strcat(url, ip->ptr));
    response = curl_easy_perform(curl);

    if(response != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
          curl_easy_strerror(response));
    curl_easy_cleanup(curl);
  }
}

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);

  }
  s->ptr[0] = '\0';
}

size_t callback(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);

  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

String ip_address()
{
  CURL *curl;
  CURLcode response;
  String url_data;

  curl = curl_easy_init();

  if (curl) {
    init_string(&url_data);
    curl_easy_setopt(curl, CURLOPT_URL, my_ip_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &url_data);

    response = curl_easy_perform(curl);

    if (response != CURLE_OK) {
      printf("something went wrong with curl...");
      exit(EXIT_FAILURE);
    }

    curl_easy_cleanup(curl);
  }

  return url_data;
}

void update(void *arg)
{
  printf("Updating an IP address...\n");
  String ip = ip_address();

  printf("url_upload\n");
  url_upload(&ip, arg);

  printf("%s\n", ip.ptr);
  free(ip.ptr);
}

void *update_thread(void *arg)
{
  Configuration *conf = (Configuration*) arg;

  while (1)
  {
    sleep(conf->update_interval);
    update(arg);
  }
}


int main(int argc, char *argv[])
{
  Configuration conf;
  conf.update_interval = 10;
  char url_destination[] = "http://petric.esy.es/addressbook.php";
  conf.url_destination = url_destination;

  pthread_t tid;
  pthread_create(&tid, NULL, &update_thread, &conf);
  pthread_join(tid, NULL);
  return 0;
}
