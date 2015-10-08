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

#include "utils.h"

static const char my_ip_url[] = "http://ipinfo.io/ip";

struct config
{
    char *service;
    char *domain;
    char *hash;
    long unsigned int update_interval;
};
static struct config cg;

struct string
{
    char *ptr;
    size_t len;

};

void url_upload(struct string *ip)
{
    CURL *curl;
    CURLcode response;
    char post_req[128];

    strcpy(post_req, "ip=");
    strcat(post_req, ip->ptr);
    strcat(post_req, "&domain=");
    strcat(post_req, cg.domain);
    strcat(post_req, "&hash=");
    strcat(post_req, cg.hash);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, cg.service);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_req);
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

struct string ip_address()
{
    CURL *curl;
    CURLcode response;
    struct string url_data;

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

void update()
{
    struct string ip = ip_address();
    str_trim_right(&ip.ptr);

    url_upload(&ip);

    free(ip.ptr);
}

void *update_thread()
{
    while (1)
    {
        sleep(cg.update_interval);
        update();
    }
}

void set_config(const char *c_path)
{
    FILE *c_fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *tokens, *key, *value;

    c_fp = fopen(c_path, "r");
    if (c_fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, c_fp)) != -1) {
        key = strsep(&line, " ");
        value = strsep(&line, "\n");

        char *dest = malloc(sizeof(int)*str_size(value));
        char *p = strncpy(dest, value, str_size(value));

        if (strcmp(key, "service") == 0) {
            cg.service = p;
        } else if (strcmp(key, "domain") == 0) {
            cg.domain = p;
        } else if (strcmp(key, "hash") == 0) {
            cg.hash = p;
        } else if (strcmp(key, "interval") == 0) {
            cg.update_interval = atoi(p);
        }
    }

    fclose(c_fp);
    //TODO: causing SEGFAULT in some cases!??
    //if (line)
    //    free(line);
    return;
}

static int handle_options(const char ***argv, int *argc, int *envchanged)
{
    const char **orig_argv = *argv;

    while (*argc > 0) {
        const char *cmd = (*argv)[0];
        if (cmd[0] != '-')
            break;

        if (skip_prefix(cmd, "--config-path", &cmd)) {
            if (*cmd == '=')
                set_config(cmd + 1);
            else {
                exit(0);
            }
        }

        (*argv)++;
        (*argc)--;
    }
}

int main(int argc, char **av)
{
    const char **argv = (const char **) av;

    argv++;
    argc--;

    handle_options(&argv, &argc, NULL);
    //TODO: free cg at the end!

    pthread_t tid;
    pthread_create(&tid, NULL, &update_thread, &cg);
    pthread_join(tid, NULL);

    return 0;
}
