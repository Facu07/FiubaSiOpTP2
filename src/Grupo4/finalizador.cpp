#include <iostream>
#include <queue>
#include "sv_sem.h"
#include "sv_shm.h"
#include "defs.h"
#include <pthread.h>
using namespace std;

int main(int argc, char *argv[]) {
   
	//ferry
	sv_sem mutexFerry("mutexFerry",1);

	//orillas
	sv_sem mutexQueueNorte("mutexQueueNorte",1);
	sv_sem mutexQueueSur("mutexQueueSur",1);
	sv_sem mutexQueueEste("mutexQueueEste",1);
	sv_sem mutexQueueOeste("mutexQueueOeste",1);
	
	//vehiculo
        sv_sem vehiculo_id("vehiculo_id",1);

	vehiculo_id.post();
	mutexFerry.post();
	mutexQueueNorte.post();
	mutexQueueSur.post();
	mutexQueueEste.post();
	mutexQueueOeste.post();

	return 0;
}
