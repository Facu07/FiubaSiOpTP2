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
//Vehiculo tempVehiculo;

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

void* thread_cargar_orilla(void*arg){

	Vehiculo tempVehiculo;
	tempVehiculo.ID = rand();						// Random para el nombre de los vehiculos
	tempVehiculo.k = 1 + (rand()% ferry.n);			// Random para el peso que no sea mayor a kmax
	tempVehiculo.orilla = 1 + (rand()% 4); 			// Numeros randoms entre 1 y 4 para determinar a que orilla van
	switch(tempVehiculo.orilla){
	case NORTE:
		pthread_mutex_lock(&mutexQueueNorte);
		Norte.colaVehiculos.push(tempVehiculo);
		pthread_mutex_unlock(&mutexQueueNorte);
	break;
	case SUR:
		pthread_mutex_lock(&mutexQueueSur);
		Sur.colaVehiculos.push(tempVehiculo);
		pthread_mutex_unlock(&mutexQueueSur);
	break;
	case ESTE:
		pthread_mutex_lock(&mutexQueueEste);
		Este.colaVehiculos.push(tempVehiculo);
		pthread_mutex_unlock(&mutexQueueEste);
	break;
	case OESTE:
		pthread_mutex_lock(&mutexQueueOeste);
		Oeste.colaVehiculos.push(tempVehiculo);
		pthread_mutex_unlock(&mutexQueueOeste);
	break;
	}
}

void* carga_descarga(void*arg){
	int contador = 0;
	bool salir = false;
	while(ferry.cantVehiculos > contador){
		switch(ferry.rutaActual){
		case NORTE:
			printf("Ferry anclando en Orilla Norte\n");
			printf("Descargando %d \n",ferry.k);
			ferry.k = 0;
			ferry.rutaActual = SUR;
			while(!(Norte.colaVehiculos.empty()) && !salir){
				pthread_mutex_lock(&mutexQueueNorte);
				struct Vehiculo vehiculoActual = Norte.colaVehiculos.front();
				ferry.k += vehiculoActual.k;
				Norte.colaVehiculos.pop();
				contador ++;
				pthread_mutex_unlock(&mutexQueueNorte);
				if(ferry.n <= ferry.k){
					salir = true;
				}
			}
			printf("Se cargo %d en el ferry \n", ferry.k);
			salir = false;
		break;
		case SUR:
			printf("Ferry anclando en Orilla Sur\n");
			printf("Descargando %d \n",ferry.k);
			ferry.k = 0;
			ferry.rutaActual = ESTE;
			while(!(Sur.colaVehiculos.empty()) && !salir){
				pthread_mutex_lock(&mutexQueueSur);
				struct Vehiculo vehiculoActual = Sur.colaVehiculos.front();
				ferry.k += vehiculoActual.k;
				Sur.colaVehiculos.pop();
				contador ++;
				pthread_mutex_unlock(&mutexQueueSur);
				if(ferry.n <= ferry.k){
					salir = true;
				}
			}
			printf("Se cargo %d en el ferry \n", ferry.k);
			salir = false;
		break;
		case ESTE:
			printf("Ferry anclando en Orilla Este\n");
			printf("Descargando %d \n",ferry.k);
			ferry.k = 0;
			ferry.rutaActual = OESTE;
			while(!(Este.colaVehiculos.empty()) && !salir){
				pthread_mutex_lock(&mutexQueueEste);
				struct Vehiculo vehiculoActual = Este.colaVehiculos.front();
				ferry.k += vehiculoActual.k;
				Este.colaVehiculos.pop();
				contador ++;
				pthread_mutex_unlock(&mutexQueueEste);
				if(ferry.n <= ferry.k){
					salir = true;
				}
			}
			printf("Se cargo %d en el ferry \n", ferry.k);
			salir = false;
		break;
		case OESTE:
			printf("Ferry anclando en Orilla Oeste\n");
			printf("Descargando %d \n",ferry.k);
			ferry.k = 0;
			ferry.rutaActual = NORTE;
			while(!(Oeste.colaVehiculos.empty()) && !salir){
				pthread_mutex_lock(&mutexQueueOeste);
				struct Vehiculo vehiculoActual = Oeste.colaVehiculos.front();
				ferry.k += vehiculoActual.k;
				Oeste.colaVehiculos.pop();
				contador ++;
				pthread_mutex_unlock(&mutexQueueOeste);
				if(ferry.n <= ferry.k){
					salir = true;
				}
			}
			printf("Se cargo %d en el ferry \n", ferry.k);
			salir = false;
		break;
		}
	}
}

int main(int argc, char *argv[]) {

	ferry.kmax = atoi(argv[1]);
	ferry.n = atoi(argv[2]);
	ferry.cantVehiculos = atoi(argv[3]);
	ferry.rutaActual = NORTE; 				// Empieza por el Norte
	ferry.k = 0;


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

	for(int i = 0; i < ferry.cantVehiculos; i++){
		pthread_create(&threads_vehiculos[i],NULL,thread_cargar_orilla,NULL);
	}

	pthread_create(&thread_ferry,NULL,carga_descarga,NULL);

	for(int i = 0; i < ferry.cantVehiculos; i++){
		pthread_join(threads_vehiculos[i], NULL);
	}

	pthread_join(thread_ferry, NULL);

	return 0;
}
