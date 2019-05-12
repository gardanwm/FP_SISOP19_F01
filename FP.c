#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define INTERVAL 3
#define FILEPATH "/dats/Documents/Putri/FP/crontab.data"
#define DEBUG 1

int gettimeofday(struct timeval *__restrict__ tp);

void readFile(){
    FILE *cronfile;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    // open cronfile
    cronfile= fopen(FILEPATH, "r");
    // exit if cronfile doesnt exist
    if (cronfile == NULL) exit(EXIT_FAILURE);

    // loop read file until EOF
    while ((read = getline(&line, &len, cronfile)) != -1) {
        if(DEBUG){
            printf("Retrieved line of length %zu:\n", read);
            printf("%s", line);
        }
        // do schedulling
    }

    // close cronfile
    fclose(cronfile);

    // empty array line
    if (line) free(line);

    return;
}

void* isFileModified(void *ptr){
    struct stat file_details;
    struct timeval tp;
    time_t modify_time;
    int FIRST=1;

    while(1){
        // get file details 
        stat(FILEPATH, &file_details);
        // get last modified file in epoch
        modify_time= file_details.st_mtime;

        // get current time
        gettimeofday(&tp);
        // convert to epoch in second
        long int now = tp.tv_sec;

        // debug
        if(DEBUG){
            printf("now \t\t: %lld\n",now);
            printf("Last modified \t: %lld\n",modify_time);
        }

        // file not modified;
        if (modify_time < (now - INTERVAL) && !FIRST){
            if(DEBUG) printf("file not modified\n");

            // next check on INTERVAL later
            sleep(INTERVAL);

            // next loop
            continue;
        }
        // file modified or file will read for the first time
        else {
            if(DEBUG && FIRST) printf("do read file for the first time\n");
            else if (DEBUG) printf("do read file\n");

            // declare first is false
            FIRST=0;

            // read and process file
            readFile();

            // next check on INTERVAL later
            sleep(INTERVAL);

            // next loop
            continue;
        }
    }

    return NULL;
}

int main(){
    pthread_t tid1;
    (void) pthread_create(&tid1,NULL,isFileModified,NULL);
    (void) pthread_join(tid1,NULL);
    return 0;
}