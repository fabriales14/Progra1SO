#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include <stdint.h>
#include <sys/mman.h>

#define SEMAPHORES 1

int resolverTarea();
int resolver();
void *mmap(void *addr, size_t len, int prot, int flags, int fd,off_t off);
int *resuelto;
sem_t *semaforo;    //semaforo
int dificultad, contador=0, instancia=1, p[2], readbytes;
float tiempo;
char tarea[100], c, buffer[100];
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main() {
  resuelto = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  semaforo = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  sem_init (semaforo, 1, 1); //inicio semaforo

  srand(time(NULL)); // se necesita para generar randoms diferentes
  printf("Ingrese la tarea a realizar\n");
  scanf("%[^\n]s", tarea);
  printf("La tarea es: %s\n", tarea);
  printf("Desea ingresar la dificultad? (s/n):");
  scanf(" %c", &c);
  if (c == 's'){
    printf("Ingrese la dificultad de la solicitud:\n");
    scanf(" %d", &dificultad);
  } else {
    dificultad = rand() % 101;
  }
  tiempo = ((float)rand()/(float)(RAND_MAX)*5.0);
  if (tiempo < 0.5)
    tiempo++;
  printf("The difficulty is %d\n", dificultad);
  printf("The estimated time is %f\n", tiempo);
  if (dificultad > 85){
      contador++;
      printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d, %d, %d, %d)\n", getppid(), getpid(), contador, instancia);
      printf("Resolving request (%s)\n", tarea);
      sleep(tiempo);
      printf("The request has been solved... It has been a pleasure to assist you, BYE!\n");
    }
  } else if (dificultad > 45 && dificultad <= 85){
    resolverTarea();
  } else {
    resolverTarea();
  }
  return 0;
}

int resolverTarea(){
  pthread_mutex_lock( &mutex1 );
  while (*resuelto < 100){
    contador++;
        printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d, %d, %d, %d)\n", getppid(), getpid(), contador, instancia+1);
        printf("Start resolving the request (%s)\n", tarea);
        printf("Mr. Meeseeks N.%d is working on the request\n", getpid());
        sleep(tiempo);
        int resultado = resolver();
        if (resultado)
          printf("This Mr. Meeseeks N.%d resolved the request\n", getpid());
    }
    pthread_mutex_unlock( &mutex1 );
  }
  return 0;
}

int resolver(){
  pthread_mutex_lock( &mutex1 );
  if (SEMAPHORES)
      sem_wait(semaforo);// Decrementa el semáforo
  int prob = rand() % 101;
  if (prob < dificultad){
    if (*resuelto > 99){
      pthread_mutex_unlock( &mutex1 );
      return 0;
    }
      resuelto += dificultad;
    if (*resuelto > 99){
      pthread_mutex_unlock( &mutex1 );
      return 1;
    }
  }
  if (SEMAPHORES){
    sem_post(semaforo); // incrementa el semáforo
    pthread_unmutex_unlock( &mutex1 );
    return 0;
  }

  pthread_mutex_unlock( &mutex1 );

}
