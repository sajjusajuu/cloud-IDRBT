/*
* Compiling : $ gcc -Wall -I/usr/local/include -c text.c
*			  $ gcc -L/usr/local/lib text.o -lgsl -lgslcblas -lm -o text.out
*/

#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

#define BUFFER 725

int main(int argc, char * argv[]) {
	FILE *fp = fopen(argv[1],  "r");
	printf("opening %s\n", argv[1]);
	
	int flag = atoi(argv[2]);
	int matrow = 0;
	if(argc == 4) {
		matrow = atoi(argv[3]);
	}

	if (flag == 1) {
		gsl_vector *s = gsl_vector_alloc(BUFFER);
		gsl_vector_fread(fp, s);
		int i = 0;
    	for(i = 0; i<BUFFER; i++)
        	printf("%f ",  gsl_vector_get(s,i));
	}
	else {
		gsl_matrix *m = gsl_matrix_alloc(BUFFER, BUFFER);
		gsl_matrix_fread(fp, m);
		int i = 0;
    	for(i = 0; i<BUFFER; i++)
        	printf("%f ",  gsl_matrix_get(m,matrow, i));
	}
    printf("\n\n");
    fclose(fp);
	return 0;
}

