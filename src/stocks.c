/**
 * @author Laura Viglioni
 * @lic GNU3
 * 2019
*/


#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define NUM_STOCKS 6

// strucks
typedef struct {
  char *name, *code;
  float  initial_val;
  int qtty;
} Stock;

// headers
void *getStock(void *arg);
void *printInfo(void *arg);
void createThreads();

// global variables
pthread_t th[NUM_STOCKS+1];
pthread_mutex_t mutex;
Stock stocks[NUM_STOCKS] = {
  {"Usiminas","USIM5",0,0},
  {"Petrobras","PETR4",0,0},
  {"Banestes", "BEES3",0,0},
  {"Kroton", "KROT3",0,0},
  {"Oi","OIBR3",0,0},
  {"Tim", "TIMP3",0,0}
};
volatile char *phrases[NUM_STOCKS];

// main
int main() {
  pthread_mutex_init(&mutex,NULL);
  system("echo Hello There! Loading...");
  createThreads();
  
  return 0;
}

void createThreads(){
  int err[NUM_STOCKS+1];
  for(int i=0; i<NUM_STOCKS; i++) {
    err[i] = pthread_create(&(th[i]),NULL,&getStock,i);
    if (err[i] != 0)
      printf("\ncan't create thread :[%s]", strerror(err[i]));
    else
      printf("\n Thread %d created successfully\n",i);
  }
  err[NUM_STOCKS] = pthread_create(&(th[NUM_STOCKS]),NULL,&printInfo,NULL);

  for(int i=0; i<NUM_STOCKS+1; i++)
    (void) pthread_join(th[i],NULL);
  
}

void *printInfo(void *arg){
  char output [360];
  
  while(1){
    sleep(10);
    sprintf(output, "echo '∵");
    for(int i = 0; i < NUM_STOCKS; i++) {
      sprintf(output, "%s %s ∵", output, phrases[i]);
    }
    sprintf(output, "%s'", output);
    //printf("%s\n", output);
    system(output);
  }
  return NULL;
}

void *getStock( void *arg) {
  FILE *fp = NULL;
  float val, var, per;
  char shell_command[115], phrase[60];
  int id = (int*)arg;
  float initial_budget = stocks[id].qtty * stocks[id].initial_val, actual_budget, robson, robson_per;
  sleep(id);
  while(1){
    sprintf(shell_command,"echo $(w3m 'https://finance.yahoo.com/quote/%s.SA' |grep BRL -A 2) | sed 's/.*BRL//'  | sed 's/[^-.0-9 ]*//g'", stocks[id].code);
  
    fp = popen(shell_command,"r");

    if(fp != NULL) {
      fscanf(fp, "%f %f %f", &val, &var, &per);
      actual_budget = stocks[id].qtty * val;
      robson = actual_budget - initial_budget;
      robson_per = 100*(robson)/(initial_budget);
      if(stocks[id].qtty > 0){
  	sprintf(phrase,"%s: R$ %.2f Δ = %.2f %.2f%% - R$ %.2f %.0f%%", stocks[id].name , val, var, per, robson, robson_per);
      }
      else
  	sprintf(phrase,"%s: R$ %.2f Δ: %.2f %.2f%%", stocks[id].name , val, var, per);
      fclose(fp);

      pthread_mutex_lock(&mutex);
      phrases[id] = phrase;
      pthread_mutex_unlock(&mutex);
    }
    fp = NULL;

    sleep(30);
  }
  return NULL;
}
