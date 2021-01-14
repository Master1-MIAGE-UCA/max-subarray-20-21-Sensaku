Here the documentation about wich functions are parallel or not:

1. struct tablo * allocaTablo(int) : Non parallel
2. void printArray(struct tablo *): Non parallel

About prefix algorithm

3. void monteeSum(struct tablo *, struct tablo *): parallel
4. void monteeMax(struct tablo *, struct tablo *): parallel
5. void descenteSum(struct tablo *, struct tablo *): parallel
6. void descenteMax(struct tablo *, struct tablo *): parallel
7. void finalSum(struct tablo *, struct tablo *): parallel
8. void finalMax(struct tablo *, struct tablo *): parallel

9. void reverse(struct tablo *, struct tablo *): non parallel
10. void generateArrayTest(struct tablo *, int[], int): non parallel
11. void reverseLarge(struct tablo *, struct tablo * ): non parallel
12. void reduce(struct tablo * , struct tablo *): non parallel
13. void findSubArray(struct tablo *, struct tablo *): non parallel

Line 312 to 317 - step 5 : parallel

