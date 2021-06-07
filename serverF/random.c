// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "random.h"

double randNormalize (void){
	return (rand() / ( (double)RAND_MAX + 1));
}
int randInt(int izq, int der){
	return (int)(randNormalize() * (der - izq + 1) + izq);
}

void randomize(void){
	srand((int)time(NULL));
}