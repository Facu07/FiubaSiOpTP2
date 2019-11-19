#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>
#include "./sv_sem.h"
#include "./sv_shm.h"
#include "vehiculo.h"
#define CLAVE "/home"
using namespace std;
