/**
 * @author Laura Viglioni
 * @lic GNU3
 * 2016 - 2019
*/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define NUMTHREADS 7


// headers
void *printInfo(void *arg);
void *getDate(void *arg);
void *updateTime(void *arg);
void *getVolume(void *arg);
void *getMemory(void *arg);
void *getBattery(void *arg);
void *getBrightness(void *arg);
char *weekrussian(int num);
char *monthrussian(int num);
char *dayrussian(int num);

// structures



typedef struct {
  char *status;
  float var, perc, atual, lucro, full_perc;
} Stock;

// global variables


pthread_t th[NUMTHREADS];
pthread_mutex_t mutex;
volatile struct tm *current;
volatile char *vol = "";
volatile Stock stocks;
volatile Stock stocks2;
volatile float total;
volatile char *memory = "";
volatile char *battery = "";
volatile char *time_now = "";
volatile char *date = "";
volatile char *bright = "";
// main

int main() {
  int err[NUMTHREADS], i=0;
  pthread_mutex_init(&mutex,NULL);

  system("echo Hello There");

  total = 0;
  
    
  err[i] = pthread_create(&(th[i]),NULL,&printInfo,NULL);
  i++;
  err[i] = pthread_create(&(th[i]),NULL,&updateTime,NULL);
  i++;
  err[i] = pthread_create(&(th[i]),NULL,&getDate,NULL);
  i++;
  err[i] = pthread_create(&(th[i]),NULL,&getVolume,NULL);
  i++;
  err[i] = pthread_create(&(th[i]),NULL,&getMemory,NULL);
  i++;
  err[i] = pthread_create(&(th[i]),NULL,&getBattery,NULL);
  i++;
  err[i] = pthread_create(&(th[i]),NULL,&getBrightness,NULL);

  for (int i=0; i<NUMTHREADS; i++) {
    if (err[i] != 0)
      printf("\ncan't create thread :[%s]", strerror(err[i]));
    else
      printf("\n Thread %d created successfully\n",i);
  }

  for(int i=0; i<NUMTHREADS; i++)
    (void) pthread_join(th[i],NULL);

  return 0;
}


////////////////////////////////////// Functions

// print all info
void *printInfo(void *arg) {
  char output[270];
  sleep(4);
  while(1){
    pthread_mutex_lock(&mutex);

    // print com uma thread de stock
    sprintf(output,"echo '%s %s ∵ %s ∵ %s ∵ %s ∵ %s'", date, time_now,vol, bright, memory, battery);
    
    system(output);
    pthread_mutex_unlock(&mutex);
    usleep(200000);
  }
  return NULL;
}

void *getBattery(void *arg) {
  char phrase[27] , status[12];
  float bat, actual, total;
  FILE *f_actual = NULL, *f_total = NULL, *f_status=NULL;
  while(1) {
    bat = -1;
    
    f_actual = fopen("/sys/class/power_supply/BAT0/charge_now","r");
    f_total = fopen("/sys/class/power_supply/BAT0/charge_full","r");
    f_status = fopen("/sys/class/power_supply/BAT0/status","r");
    
    if(f_actual != NULL && f_total != NULL && f_status != NULL){
      fscanf(f_actual,"%f", &actual);
      fscanf(f_total, "%f", &total);
      fscanf(f_status, "%s", status);
      bat = actual/total*100;
      sprintf(phrase, "Battery: %.0f%% %s", bat, status);
      fclose(f_actual);
      fclose(f_total);
      fclose(f_status);
    }
    
    f_actual = NULL;
    f_total = NULL;
    f_status=NULL;
    
    pthread_mutex_lock(&mutex);
    battery = phrase;
    pthread_mutex_unlock(&mutex);
    
    usleep(200000);
      
  }
}

void *getBrightness(void *arg){
  float brightness;
  char phrase[8];
  FILE *fp = NULL;

  while(1) {
    brightness = -1;
    
    fp = popen("xrandr --verbose | awk '/Brightness/ { print $2; exit }'","r");
    
    if (fp != NULL ){
      fscanf(fp,"%f",&brightness);
      brightness *= 100;
      sprintf(phrase, "☼: %.0f%%", brightness);
      pclose(fp);
    }
    
    fp = NULL;
    pthread_mutex_lock(&mutex);
    bright = phrase;
    pthread_mutex_unlock(&mutex);

    usleep(100000);
  }
}

// get the system sound volume using shell script
void *getVolume(void *arg) {
  int volume;
  char phrase[8];
  FILE *fp = NULL;

  while(1) {
    volume = -1;
    
    //fp = popen("~/.status_bar/shell_scripts/getVolume.sh","r");

    fp = popen("amixer get Master |grep 'Mono: Playback' |awk '{print $4}'|sed 's/[^0-9\%]//g'","r");
    
    if (fp != NULL ){
      fscanf(fp,"%d",&volume);
      sprintf(phrase, "♫: %d%%", volume);
      pclose(fp);
    }
    
    fp = NULL;
    pthread_mutex_lock(&mutex);
    vol = phrase;
    pthread_mutex_unlock(&mutex);

    usleep(100000);
  }
}

void *getMemory(void *arg) {
  char mem[30];
  FILE *fp = NULL;

  while(1) {
    fp = popen("~/.status_bar/shell_scripts/memory.sh","r");

    if (fp != NULL ){
      fgets(mem,30,fp);
      pclose(fp);
    }
    fp = NULL;
    pthread_mutex_lock(&mutex);
    memory = mem;
    pthread_mutex_unlock(&mutex);
    sleep(1);
  }
}




// every 250000ns = 0.25s updates date and time variable (datetime.current) and .time
void *updateTime(void *arg){
  char phrase[10];
  time_t aux;
  int h,m,s;

  while(1){
   pthread_mutex_lock(&mutex);
    time(&aux);
    current = localtime(&aux);

    h = current->tm_hour;
    m = current->tm_min;
    s = current->tm_sec;
    
    sprintf(phrase, "%02d:%02d:%02d",h,m,s);
    
    time_now = phrase;
    pthread_mutex_unlock(&mutex);
    usleep(250000);
  }
  return NULL;
}

// Gets the .current info and put'em in .weekday .month .day
void *getDate(void *arg) {
  int day,week,month,year;
  char *aux_day, *aux_week, *aux_month, phrase[40];
  while(1){
    pthread_mutex_lock(&mutex);
    day = current->tm_mday;
    week = current->tm_wday;
    month = current->tm_mon;
    year = current->tm_year;
    pthread_mutex_unlock(&mutex);
  
    aux_day = dayrussian(day);
    aux_week = weekrussian(week);
    aux_month = monthrussian(month);

    
    

    sprintf(phrase, "%s - %s, %s, %d -", aux_week, aux_day,aux_month, year+1900);

    pthread_mutex_lock(&mutex);
    date = phrase;
    pthread_mutex_unlock(&mutex);

    sleep(60);
  }
  return NULL;
}


// The next 3 functions "translate" date infos to russian

char *weekrussian(int num){
  switch(num){
  case 0:
    return "Воскресенье";
  case 1:
    return "Понедельник";
  case 2:
    return "Вторник";
  case 3:
    return "Среда";
  case 4:
    return "Четверг";
  case 5:
    return "Пятница";
  case 6:
    return "Суббота";
  }
    return "ERROR DAYWEEK"; 
}

char *monthrussian (int num) {
  switch(num){
  case 0:
      return "января";
  case 1:
      return "февраля";
  case 2:
      return "марта";
  case 3:
      return "апреля";
  case 4:
      return "мая";
  case 5:
      return "июня";
  case 6:
      return "июля";
  case 7:
      return "августа";
  case 8:
      return "сентября";
  case 9:
      return "октября";
  case 10:
    return "ноября";
  case 11:
    return "декабря";
  }
  return "ERROR MONTH";
}

char *dayrussian(int num) {
  switch(num) {
  case 1:
    return "Первое";
  case 2:
    return "Второе";
  case 3:
    return "Третье";
  case 4:
    return "Четвертое";
  case 5:
    return "Пятое";
  case 6:
    return "Шестое";
  case 7:
    return "Седьмое";
  case 8:
    return "Восьмое";
  case 9:
    return "Девятое";
  case 10:
    return "Десятое";
  case 11:
    return "Одиннадцатое";
  case 12:
    return "Двенадцатое";
  case 13:
    return "Тринадцатое";
  case 14:
    return "Четырнадцатое";
  case 15:
    return "Пятнадцатое";
  case 16:
    return "Шестнадцатое";
  case 17:
    return "Семнадцатое";
  case 18:
    return "Восемнадцатое";
  case 19:
    return "Девятнадцатое";
  case 20:
    return "Двадцатое";
  case 21:
    return "Двадцать Первое";
  case 22:
    return "Двадцать Второе";
  case 23:
    return "Двадцать Третье";
  case 24:
    return "Двадцать Четвертое";
  case 25:
    return "Двадцать Пятое";
  case 26:
    return "Двадцать Шестое";
  case 27:
    return "Двадцать Седьмое";
  case 28:
    return "Двадцать Восьмое";
  case 29:
    return "Двадцать Девятое";
  case 30:
    return "Тридцатое";
  case 31:
    return "Тридцать Первое";
  }
  return "ERROR DAY IN RUSSIAN";
}
