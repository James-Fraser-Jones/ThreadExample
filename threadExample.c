#include <stdio.h>
#include <pthread.h> //for threading (POSIX)

#include <stdlib.h> //for rand
#include <unistd.h> //for sleep (POSIX)

#include <string.h> //for strcmp

int schedule[10] = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20};

pthread_t threads[2];
pthread_mutex_t mutex;

void printSchedule(){
  printf("Schedule: ");
  for (int i = 0; i<10; i++){
    printf("%d ", schedule[i]);
  }
  printf("\n");
}

void* updateFromWeb(){
  time_t t;
  srand((unsigned) time(&t));

  while(1){
    pthread_mutex_lock (&mutex);
    sleep(2);
    schedule[rand() % 10] = (rand() % 100);
    pthread_mutex_unlock (&mutex);
    sleep(1);
  }
}

void* commandLine(){
  char buffer[100];

  char command[20];
  int time;
  int temperature;

  while(1){
    printf("Command Line: ");

    fgets(buffer, 100, stdin);
    sscanf(buffer, "%s %d %d", command, &time, &temperature);

    if (strcmp(command, "setTemp") == 0){
      pthread_mutex_lock (&mutex);
      schedule[time-1] = temperature;
      pthread_mutex_unlock (&mutex);
    }
    printSchedule();
  }
}

int main(int argc, char *argv[]){
    printSchedule();

    pthread_mutex_init(&mutex, NULL);
    pthread_create(&threads[0], NULL, commandLine, NULL);
    pthread_create(&threads[1], NULL, updateFromWeb, NULL);

    pthread_exit(NULL); //wait for all threads to complete before ending the program via main

    return 0;
}
