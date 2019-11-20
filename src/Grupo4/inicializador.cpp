#include <iostream>
#include <queue>
#include "sv_sem.h"
#include "sv_shm.h"
#include "defs.h"
#include <pthread.h>
#include "ferry.h"
#include "vehiculo.h"
using namespace std;

struct Orilla{
        int nombre;
        queue <Vehiculo> colaVehiculos;
};
Orilla Norte, Sur, Este, Oeste;

void* thread_cargar_orilla(void*arg){

        //orillas
        sv_sem mutexQueueNorte("mutexQueueNorte",1);
        sv_sem mutexQueueSur("mutexQueueSur",1);
        sv_sem mutexQueueEste("mutexQueueEste",1);
        sv_sem mutexQueueOeste("mutexQueueOeste",1);

	Vehiculo tempVehiculo;
	tempVehiculo.ID = rand();					// Random para el nombre de los vehiculos
	tempVehiculo.k = 1 + (rand()% ferry.n);			// Random para el peso que no sea mayor a kmax
	tempVehiculo.orilla = 1 + (rand()% 4); 			// Numeros randoms entre 1 y 4 para determinar a que orilla van
	switch(tempVehiculo.orilla){
	case NORTE:
		mutexQueueNorte.wait();
		Norte.colaVehiculos.push(tempVehiculo);
		mutexQueueNorte.post();
	break;
	case SUR:
		mutexQueueSur.wait();
		Sur.colaVehiculos.push(tempVehiculo);
		mutexQueueSur.post();
	break;
	case ESTE:
		mutexQueueEste.wait();
		Este.colaVehiculos.push(tempVehiculo);
		mutexQueueEste.post();
	break;
	case OESTE:
		mutexQueueEste.wait();
		Oeste.colaVehiculos.push(tempVehiculo);
		mutexQueueEste.post();
	break;
	}
}

void* carga_descarga(void*arg){

	//ferry
        sv_sem mutexFerry("mutexFerry",1);

        //orillas
        sv_sem mutexQueueNorte("mutexQueueNorte",1);
        sv_sem mutexQueueSur("mutexQueueSur",1);
        sv_sem mutexQueueEste("mutexQueueEste",1);
        sv_sem mutexQueueOeste("mutexQueueOeste",1);

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
				mutexFerry.wait();

				mutexQueueNorte.wait();
				struct Vehiculo vehiculoActual = Norte.colaVehiculos.front();
				ferry.k += vehiculoActual.k;
				Norte.colaVehiculos.pop();
				contador ++;
				mutexQueueNorte.post();

				mutexFerry.post();

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
				mutexFerry.wait();

				mutexQueueSur.wait();
				struct Vehiculo vehiculoActual = Sur.colaVehiculos.front();
				ferry.k += vehiculoActual.k;
				Sur.colaVehiculos.pop();
				contador ++;
				mutexQueueSur.post();

				mutexFerry.post();

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
				mutexFerry.wait();

				mutexQueueEste.wait();
				struct Vehiculo vehiculoActual = Este.colaVehiculos.front();
				ferry.k += vehiculoActual.k;
				Este.colaVehiculos.pop();
				contador ++;
				mutexQueueEste.post();

				mutexFerry.post();

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
				mutexFerry.wait();

				mutexQueueOeste.wait();
				struct Vehiculo vehiculoActual = Oeste.colaVehiculos.front();
				ferry.k += vehiculoActual.k;
				Oeste.colaVehiculos.pop();
				contador ++;
				mutexQueueOeste.post();

				mutexFerry.post();

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

  //imprimo los numeros de padron
  cout << 93644 << " Santiago Aso" << endl;
  cout << 96029 << " Facundo Walter" << endl;
  cout << 91561 << " Pablo Prieto"  << endl;
  cout << 68180 << " Adrian Bonino" << endl;
  cout << " " << endl;

  	ferry.n = atoi(argv[1]);
	ferry.kmax = atoi(argv[2]);
	ferry.cantVehiculos = atoi(argv[3]);
	ferry.rutaActual = NORTE; 				// Empieza por el Norte
	ferry.k = 0;

	Norte.nombre = NORTE;
	Sur.nombre = SUR;
	Este.nombre = ESTE;
	Oeste.nombre = OESTE;
       
	//ferry
	sv_sem mutexFerry("mutexFerry",1);

	//orillas
	sv_sem mutexQueueNorte("mutexQueueNorte",1);
	sv_sem mutexQueueSur("mutexQueueSur",1);
	sv_sem mutexQueueEste("mutexQueueEste",1);
	sv_sem mutexQueueOeste("mutexQueueOeste",1);
	
	//sirve para generar los ids de los vehiculos
	sv_sem vehiculo_id("vehiculo_id",1);

	pthread_t threads_vehiculos[ferry.cantVehiculos];
	for(int i = 0; i < ferry.cantVehiculos; i++){
            vehiculo_id.wait();
            pthread_create(&threads_vehiculos[i],NULL,thread_cargar_orilla,NULL);
            vehiculo_id.post();
	}

	pthread_t thread_ferry;
	pthread_create(&thread_ferry,NULL,carga_descarga,NULL);

	for(int i = 0; i < ferry.cantVehiculos; i++){
		pthread_join(threads_vehiculos[i], NULL);
	}

	pthread_join(thread_ferry, NULL);

	return 0;
}
