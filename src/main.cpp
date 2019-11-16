#include <iostream>
#include <pthread.h>
#include <queue>
#include "sv_sem.h"
#include "sv_shm.h"
#include "defs.h"
using namespace std;

struct Ferry{
	int kmax, k, n, cantVehiculos;
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
	case NORTE:
		pthread_mutex_lock(&mutexQueueNorte);
		Norte.colaVehiculos.push(vehiculo);
		pthread_mutex_unlock(&mutexQueueNorte);
	break;
	case SUR:
		pthread_mutex_lock(&mutexQueueSur);
		Sur.colaVehiculos.push(vehiculo);
		pthread_mutex_unlock(&mutexQueueSur);
	break;
	case ESTE:
		pthread_mutex_lock(&mutexQueueEste);
		Este.colaVehiculos.push(vehiculo);
		pthread_mutex_unlock(&mutexQueueEste);
	break;
	case OESTE:
		pthread_mutex_lock(&mutexQueueOeste);
		Oeste.colaVehiculos.push(vehiculo);
		pthread_mutex_unlock(&mutexQueueOeste);
	break;
	}
}

void* carga_descarga(void*arg){
	Vehiculo * arg_ptr = (Vehiculo*)arg;
	int contador = 0;
	bool salir = false;
	while(ferry.cantVehiculos > contador){
		switch(ferry.rutaActual){
		case NORTE:
			while(!Norte.colaVehiculos.empty() || !salir){
				pthread_mutex_lock(&mutexQueueNorte);
				printf("Ferry anclando en Orilla Norte");
				struct Vehiculo vehiculoActual = Norte.colaVehiculos.front();
				ferry.k += vehiculoActual.k;
				Norte.colaVehiculos.pop();
				contador ++;
				pthread_mutex_unlock(&mutexQueueNorte);
				if(ferry.n == ferry.k){
					salir = true;
				}
			}
		break;
		case SUR:
			while(!Sur.colaVehiculos.empty() || !salir){
				pthread_mutex_lock(&mutexQueueSur);
				printf("Ferry anclando en Orilla Sur");
				struct Vehiculo vehiculoActual = Sur.colaVehiculos.front();
				ferry.k += vehiculoActual.k;
				Sur.colaVehiculos.pop();
				contador ++;
				pthread_mutex_unlock(&mutexQueueSur);
				if(ferry.n == ferry.k){
					salir = true;
				}
			}
		break;
		case ESTE:
			while(!Este.colaVehiculos.empty() || !salir){
				pthread_mutex_lock(&mutexQueueEste);
				printf("Ferry anclando en Orilla Este");
				struct Vehiculo vehiculoActual = Este.colaVehiculos.front();
				ferry.k += vehiculoActual.k;
				Este.colaVehiculos.pop();
				contador ++;
				pthread_mutex_unlock(&mutexQueueEste);
				if(ferry.n == ferry.k){
					salir = true;
				}
			}
		break;
		case OESTE:
			while(!Oeste.colaVehiculos.empty() || !salir){
				pthread_mutex_lock(&mutexQueueOeste);
				printf("Ferry anclando en Orilla Oeste");
				struct Vehiculo vehiculoActual = Oeste.colaVehiculos.front();
				ferry.k += vehiculoActual.k;
				Oeste.colaVehiculos.pop();
				contador ++;
				pthread_mutex_unlock(&mutexQueueOeste);
				if(ferry.n == ferry.k){
					salir = true;
				}
			}
		break;
	}
}

int main(int argc, char *argv[]) {

	ferry.kmax = atoi(argv[0]);
	ferry.n = atoi(argv[1]);
	ferry.cantVehiculos = atoi(argv[2]);
	ferry.rutaActual = NORTE; 				// Empieza por el Norte

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
