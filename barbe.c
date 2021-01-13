#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>


// ====================== from TP2 ===============================
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
	printf("---- Array of size %i ----\n", tmp->size);
	int size = tmp->size;
	int i;
	for (i = 0; i < size; ++i){
		printf("%i ", tmp->tab[i]);
	}
	printf("\n");
}

void montee(struct tablo * source, struct tablo * destination){
	// Copie du tableau source dans le tableau destination
	// de taille 2^source->size
	for(int i = 0; i < destination->size; i++){
		if(i < source->size){
			destination->tab[i] = 0;
		}else{
			destination->tab[i] = source->tab[i - source->size];
		}
	}

	for(int l = log(destination->size/2)/log(2) - 1; l >= 0; l--){
		int inf = pow(2,l);
		int sup = pow(2, l+1)-1;

		#pragma omp parallel for
		for(int j = inf; j <= sup-1; j++){
			destination->tab[j] = destination->tab[2 * j] + destination->tab[(2 * j) + 1];
		}
	}
}

void descente(struct tablo * a, struct tablo * b){
	b->tab[1] = 0;

	for(int l = 1; l <= log(a->size/2) / log(2); l++){
		int inf = pow(2, l);
		int sup = pow(2, l+1)-1;

		#pragma omp parallel for
		for(int j = inf; j <= sup; j++){
			if(j%2 == 0){
				b->tab[j] = b->tab[j/2];
			}else{
				b->tab[j] = b->tab[(j-1)/2] + a->tab[j-1];
			}
		}
	}
}

void final(struct tablo * a, struct tablo *b){
	int inf = pow(2, ceil(log(a->size/2)));
	int sup = pow(2, ceil(log(a->size/2)) +1) -1;

	#pragma omp parallel for
	for(int j = inf; j <= sup; j++){
		b->tab[j] = b->tab[j] + a->tab[j];
	}
}
// ==============================================================

// Reverse le tableau source et le sauvegarde dans destination
void reverse(struct tablo * source, struct tablo * destination){
	destination->size = source->size;
	destination->tab = malloc(destination->size * sizeof(int));
	for(int j = destination->size; j >= 0; j--){
		destination->tab[destination->size - j] = source->tab[j-1];
	}
}

void generateArrayTest(struct tablo * s, int t[], int size){
	s->size = size;
	s->tab = malloc(s->size * sizeof(int));
	for(int i = 0; i < size; i++){
		s->tab[i] = t[i];
	}

}

void printResult(struct tablo * s, int size){
	printf("%d",s->tab[size]);
	for(int i = size+1; i < size*2; i ++){
		printf(" %d",s->tab[i]);
	}
	printf("\n");
}

int main(int argc, char **argv){
	struct tablo source;
	struct tablo rev_source;

	if(argc == 2){
		FILE *f = fopen(argv[1], "r");
		int t[100];
		int i = 0;
		int d = 0;

		fscanf(f, "%d", &d);
		while(!feof(f)){
			t[i] = d;
			fscanf(f, "%d", &d);
			i = i + 1;

		}
		fclose(f);
		printf("input size = %d\n", i);
		generateArrayTest(&source, t, i);
		reverse(&source, &rev_source);
	}

	// ============== PSUM phase =========================
	struct tablo * a = malloc(sizeof(struct tablo));
	a->tab = malloc(source.size*2*sizeof(int));
	a->size = source.size * 2;
	a->tab[0] = 0;
	montee(&source, a);

	struct tablo * psum = malloc(sizeof(struct tablo));
	psum->tab = malloc(source.size * 2 * sizeof(int));
	psum->size = source.size * 2;
	psum->tab[0] = 0;
	descente(a,psum);
	printResult(psum, source.size);

	final(a,psum);

	// ============== SSUM phase =========================

	struct tablo * b = malloc(sizeof(struct tablo));
	b->tab = malloc(rev_source.size*2*sizeof(int));
	b->size = rev_source.size * 2;
	b->tab[0] = 0;
	montee(&rev_source, b);

	struct tablo * ssum = malloc(sizeof(struct tablo));
	ssum->tab = malloc(rev_source.size * 2 * sizeof(int));
	ssum->size = rev_source.size * 2;
	ssum->tab[0] = 0;
	descente(b,ssum);
	printResult(ssum, rev_source.size);

	final(b,ssum);

	free(a);
	free(b);
	free(psum);
	free(ssum);

	return 0;
}
