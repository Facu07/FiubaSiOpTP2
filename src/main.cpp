#include <iostream>
#include <pthread.h>
#include <queue>
#include "sv_sem.h"
#include "sv_shm.h"
#include "defs.h"
using namespace std;

struct Ferry{
	int kmax, n, cantVehiculos;
	int rutaActual;
};
Ferry ferry;

struct Vehiculo{
	int ID;
	int k;
	int orilla;
};

struct Orilla{
	int nombre;
	queue <Vehiculo> colaVehiculos;
};
Orilla Norte, Sur, Este, Oeste;

pthread_mutex_t mutexQueueNorte;
pthread_mutex_t mutexQueueSur;
pthread_mutex_t mutexQueueEste;
pthread_mutex_t mutexQueueOeste;
pthread_mutex_t mutexFerry;

//sv_sem vehiculos;

void* thread_vehiculo(void*arg){
	Vehiculo * arg_ptr = (Vehiculo*)arg;
	Vehiculo vehiculo = *arg_ptr;
	switch(vehiculo.orilla){
	case NORTE: Norte.colaVehiculos.push(vehiculo);
	break;
	case SUR: Sur.colaVehiculos.push(vehiculo);
	break;
	case ESTE: Este.colaVehiculos.push(vehiculo);
	break;
	case OESTE: Oeste.colaVehiculos.push(vehiculo);
	break;
	}
}

void* carga_descarga(void*arg){
	Vehiculo * arg_ptr = (Vehiculo*)arg;
	int contador = 0;
	while(ferry.cantVehiculos > contador){

		contador ++;
	}
}

int main(int argc, char *argv[]) {

	ferry.kmax = atoi(argv[0]);
	ferry.n = atoi(argv[1]);
	ferry.cantVehiculos = atoi(argv[2]);
	ferry.rutaActual = NORTE;

	Norte.nombre = NORTE;
	Sur.nombre = SUR;
	Este.nombre = ESTE;
	Oeste.nombre = OESTE;

	pthread_t threads_vehiculos[ferry.cantVehiculos];
	pthread_t thread_ferry;
	pthread_mutex_init(&mutexQueueNorte,NULL);
	pthread_mutex_init(&mutexQueueSur,NULL);
	pthread_mutex_init(&mutexQueueEste,NULL);
	pthread_mutex_init(&mutexQueueOeste,NULL);
	pthread_mutex_init(&mutexFerry,NULL);

	Vehiculo tempVehiculo;

	for(int i = 0; i == ferry.cantVehiculos; i++){
		tempVehiculo.ID = rand();				// Random para el nombre de los vehiculos
		tempVehiculo.k = rand();				// Random para el peso
		tempVehiculo.orilla = 1 + rand()%5; 	// Numeros randoms entre 1 y 4 para determinar a que orilla van
		pthread_create(&threads_vehiculos[i],NULL,thread_vehiculo,&tempVehiculo);
	}

	pthread_create(&thread_ferry,NULL,carga_descarga,NULL);



	return 0;
}
