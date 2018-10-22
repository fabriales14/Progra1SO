#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>

#define SEMAPHORES 1

int resolverTarea();
int resolver();
void enviarMensaje();
void recibirMensaje();
//variables utilizadas por los hijos,padres..
int *resuelto = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_ANONYMOUS, -1, 0);
resuelto=0;
int dificultad, contador=0, instancia=1, p[2], readbytes;
float tiempo;
char tarea[100], c, buffer[100];

int main() {

  sem_t *semaforo = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
             MAP_SHARED | MAP_ANONYMOUS, -1, 0);   //semaforo
  sem_init (semaforo, 1, 1); //inicio semaforo

  srand(time(NULL)); // se necesita para generar randoms diferentes
  pipe(p);
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
    if (fork()){
      contador++;
      recibirMensaje();
      printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d, %d, %d, %d)\n", getpid(), getppid(), contador, instancia);
      printf("Resolving request (%s)\n", buffer);
      sleep(tiempo);
      printf("The request has been solved... It has been a pleasure to assist you, BYE!\n");
    } else {
      enviarMensaje();
    }
    wait(NULL);
  } else if (dificultad > 45 && dificultad <= 85){
    resolverTarea();
  } else {
    resolverTarea();
  }
  return 0;
}

int resolverTarea(){
  int p1,p2;
  while (resuelto < 100){
    p1 = fork();
    contador++;
    if (p1 != 0){

      enviarMensaje();
      p2 = fork();
      if (p2 == 0){
        recibirMensaje();
        printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d, %d, %d, %d)\n", getpid(), getppid(), contador, instancia+1);
        printf("Start resolving the request (%s)\n", tarea);
        printf("Mr. Meeseeks N.%d is working on the request\n", getpid());
        sleep(tiempo);

        int resultado = resolver();
        if (resultado)
          printf("This Mr. Meeseeks N.%d resolved the request\n", getpid());
      } else {
        enviarMensaje();
      }
    }
    if (p1 != 0 && p2 != 0)
      break;
    if (p1 == 0){
      recibirMensaje();
      printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d, %d, %d, %d)\n", getpid(), getppid(), contador, instancia);
      printf("Start resolving the request (%s)\n", tarea);
      printf("Mr. Meeseeks N.%d is working on the request\n", getpid());
      sleep(tiempo);
      int resultado = resolver();
      if (resultado)
        printf("This Mr. Meeseeks N.%d resolved the request\n", getpid());
    }
  }
  waitpid(p1, NULL, 0);
  waitpid(p2, NULL, 0);
  return 0;
}

void enviarMensaje(){
  printf("Parent PPID:%d sending request with pipe\n", getpid());
  close(p[0]); //cerrar lado de lectura del pipe
  write(p[1], tarea, sizeof(tarea)); // escritura de la tarea
  close(p[1]);

}

void recibirMensaje(){
  printf("PID:%d receiving request with pipe\n", getpid());
  close(p[1]); // cerrar lado de escritura del pipe
  readbytes = read(p[0], buffer, sizeof(buffer)); // lee la tarea
  close(p[0]);
}

int resolver(){
  if (SEMAPHORES)
      sem_wait(semaforo);// Decrementa el semáforo
  int prob = rand() % 101;
  if (prob < dificultad){
    if (resuelto > 99)
      return 0;
    resuelto += dificultad;
    if (resuelto > 99){
      return 1;
    }
  }
  if (SEMAPHORES)
    sem_post(semaforo); // incrementa el semáforo
  return 0;
}
