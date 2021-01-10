#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

struct tablo{
	int * tab;
	int size;
};

void printArray(struct tablo * tmp){
	print("---- Array of size %i ----\n", tmp->size);
	int size = tmp->size;
	int i;
	for (i = 0; i < size; ++i){
		printf("%i ", tmp->tab[i]);
	}
	print("\n");
}

int main(int argc, char **argv){
	return 0;
}
