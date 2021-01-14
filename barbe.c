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

void monteeSum(struct tablo * source, struct tablo * destination){
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

void monteeMax(struct tablo * source, struct tablo * destination){
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
			destination->tab[j] = fmax(destination->tab[2 * j], destination->tab[(2 * j) + 1]);
		}
	}
}

void descenteSum(struct tablo * a, struct tablo * b){
	b->tab[1] = 0;

	for(int l = 0; l <= ceil(log(a->size/2) / log(2)); l++){
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

void descenteMax(struct tablo * a, struct tablo * b){
	b->tab[1] = 0;

	for(int l = 0; l <= ceil(log(a->size/2) / log(2)); l++){
		int inf = pow(2, l);
		int sup = pow(2, l+1)-1;

		#pragma omp parallel for
		for(int j = inf; j <= sup; j++){
			if(j%2 == 0){
				b->tab[j] = b->tab[j/2];
			}else{
				b->tab[j] = fmax(b->tab[(j-1)/2], a->tab[j-1]);
			}
		}
	}
}
void finalSum(struct tablo * a, struct tablo *b){
	int inf = pow(2, log2(a->size/2));
	int sup = pow(2, log2(a->size/2) +1) -1;

	#pragma omp parallel for
	for(int j = inf; j <= sup; j++){
		b->tab[j] = b->tab[j] + a->tab[j];
	}
}
void finalMax(struct tablo * a, struct tablo *b){
	int inf = pow(2, log2(a->size/2));
	int sup = pow(2, log2(a->size/2) +1) -1;

	#pragma omp parallel for
	for(int j = inf; j <= sup; j++){
		b->tab[j] = fmax(b->tab[j], a->tab[j]);
	}
}
// ==============================================================

// Reverse le tableau source et le sauvegarde dans destination
void reverse(struct tablo * source, struct tablo * destination){
	destination->size = source->size;
	destination->tab = malloc(destination->size * sizeof(int));
	for(int j = destination->size; j > 0; j--){
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

void reverseLarge(struct tablo * s, struct tablo * f){
	for(int i = s->size/2; i < f->size; i++){
		f->tab[i] = s->tab[f->size - (i - (f->size/2))-1];
	}

}

void reduce(struct tablo * l, struct tablo * t){
	for(int i = 0; i < t->size; i++){
		t->tab[i] = l->tab[t->size + i];
	}
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
	monteeSum(&source, a);

	struct tablo * a2 = malloc(sizeof(struct tablo));
	a2->size = a->size;
	a2->tab = malloc(a->size * (sizeof(int)));
	a2->tab[0] = 0;
	descenteSum(a,a2);

	finalSum(a,a2);
	struct tablo * psum = malloc(sizeof(struct tablo));
	psum->tab = malloc(source.size * sizeof(int));
	psum->size = source.size;
	reduce(a2,psum);
	printArray(psum);
	// ============== SSUM phase =========================

	struct tablo * b = malloc(sizeof(struct tablo));
	b->tab = malloc(rev_source.size*2*sizeof(int));
	b->size = rev_source.size * 2;
	b->tab[0] = 0;
	monteeSum(&rev_source, b);

	struct tablo * c = malloc(sizeof(struct tablo));
	c->tab = malloc(rev_source.size * 2 * sizeof(int));
	c->size = rev_source.size * 2;
	c->tab[0] = 0;
	descenteSum(b,c);


	finalSum(b,c);
	struct tablo * c2 = malloc(sizeof(struct tablo));
	c2->size = rev_source.size*2;
	c2->tab = malloc(rev_source.size*2 * sizeof(int));
	c2->tab[0] = 0;

	reverseLarge(c,c2);
	struct tablo * ssum = malloc(sizeof(struct tablo));
	ssum->size = rev_source.size;
	ssum->tab = malloc(rev_source.size * sizeof(int));
	reduce(c2,ssum);
	printArray(ssum);

	// ============== SMAX phase =========================
	struct tablo * rev_psum = malloc(sizeof(struct tablo));
	rev_psum->size = psum->size;
	rev_psum->tab = malloc(rev_psum->size * sizeof(int));
	reverse(psum, rev_psum);

	struct tablo * maxMont = malloc(sizeof(struct tablo));
	maxMont->size = rev_psum->size * 2;
	maxMont->tab = malloc(rev_psum->size * 2 * sizeof(int));
	maxMont->tab[0] = 0;
	monteeMax(rev_psum, maxMont);

	struct tablo * maxDesc = malloc(sizeof(struct tablo));
	maxDesc->size = rev_psum->size * 2;
	maxDesc->tab = malloc(rev_psum->size * 2 * sizeof(int));
	maxDesc->tab[0] = 0;
	descenteMax(maxMont, maxDesc);
	finalMax(maxMont, maxDesc);

	struct tablo * maxTemp = malloc(sizeof(struct tablo));
	maxTemp->size = rev_psum->size*2;
	maxTemp->tab = malloc(rev_psum->size * 2 * sizeof(int));
	reverseLarge(maxDesc, maxTemp);

	struct tablo * smax = malloc(sizeof(struct tablo));
	smax->size = rev_psum->size;
	smax->tab = malloc(rev_psum->size * sizeof(int));
	reduce(maxTemp, smax);
	printArray(smax);

	// ============== PMAX phase =========================
	struct tablo * d = malloc(sizeof(struct tablo));
	d->size = ssum->size * 2;
	d->tab = malloc(d->size * sizeof(int));
	d->tab[0] = 0;
	monteeMax(ssum, d);

	struct tablo * e = malloc(sizeof(struct tablo));
	e->size = d->size;
	e->tab = malloc(e->size * sizeof(int));
	e->tab[0] = 0;
	descenteMax(d, e);

	finalMax(d, e);

	struct tablo * pmax = malloc(sizeof(struct tablo));
	pmax->size = ssum->size;
	pmax->tab = malloc(pmax->size * sizeof(int));
	reduce(e,pmax);
	printArray(pmax);

	struct tablo * m = malloc(sizeof(struct tablo));
	m->size = source.size;
	m->tab = malloc(source.size * sizeof(int));

	struct tablo * ms = malloc(sizeof(struct tablo));
	ms->size = source.size;
	ms->tab = malloc(source.size * sizeof(int));

	struct tablo * mp = malloc(sizeof(struct tablo));
	mp->size = source.size;
	mp->tab = malloc(source.size * sizeof(int));


	#pragma omp parallel for
	for(int i = 0; i < source.size; i ++){
		ms->tab[i] = pmax->tab[i] - ssum->tab[i] + source.tab[i];
		mp->tab[i] = smax->tab[i] - psum->tab[i] + source.tab[i];
		m->tab[i] = ms->tab[i] + mp->tab[i] - source.tab[i];
	}

	printArray(m);

	free(a);
	free(b);
	free(c);
	free(c2);
	free(d);
	free(e);
	free(maxMont);
	free(maxDesc);
	free(maxTemp);
	free(psum);
	free(ssum);
	free(smax);
	free(pmax);
	free(ms);
	free(mp);
	free(m);

	return 0;
}
