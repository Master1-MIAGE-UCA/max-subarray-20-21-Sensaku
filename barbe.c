#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

struct tablo{
	int * tab;
	int size;
};

struct tablo * allocateTablo(int size){
	struct tablo * tmp = malloc(sizeof(struct tablo));
	tmp->size = size;
	tmp->tab = malloc(size * sizeof(int));
	tmp->tab[0] = 0;
	return tmp;
}

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
	struct tablo source;


	return 0;
}
