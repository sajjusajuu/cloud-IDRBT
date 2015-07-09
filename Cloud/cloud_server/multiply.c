/*
 * IDRBT - Cloud Retrieval
 * Webpage: https://www.idrbt.ac.in
 * Contact: hussainsajid@acm.org
 * Institute of Development and Research in Banking Technology
 * 
 * The Original Code is multiply.c
 * 
 * Note : Install GNU Scientific Library(GSL) before compiling this file
 *
 * Compiling : $gcc -Wall -I/usr/local/include -c -fPIC multiply.c
 *			   $gcc -L/usr/local/lib multiply.o -lgsl -lgslcblas -lm -shared -o multiply.so
 * Running : $ multiply(add_1, add_2, n)
 * Output : A integer giving the score
 *
 * Contributor(s):
 *  Sajid Hussain
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_rng.h>

double vector_multiply(gsl_vector *a, gsl_vector* b, int n) {

 	gsl_vector_mul(a, b);
 	double sum = 0;
 	int i = 0;
 	for(i=0; i<n; i++)
 		sum += gsl_vector_get(a, i);
 	return sum;
 }


int multiply(char *add_1, char *add_2, int n) { 

	FILE *ind_1 = fopen("crypt_1.key", "r");
	FILE* ind_2 = fopen("crypt_2.key", "r");
	FILE* td_1 = fopen(add_1, "r");
	FILE* td_2 = fopen(add_2, "r");



	gsl_vector *ind_vec_1 = gsl_vector_alloc(n);
	gsl_vector *ind_vec_2 = gsl_vector_alloc(n);
	gsl_vector *td_vec_1 = gsl_vector_alloc(n);
	gsl_vector *td_vec_2 = gsl_vector_alloc(n);


	gsl_vector_fread(ind_1, ind_vec_1);
	gsl_vector_fread(ind_2, ind_vec_2);
	gsl_vector_fread(td_1, td_vec_1);
	gsl_vector_fread(td_2, td_vec_2);

	double score_1 = vector_multiply(ind_vec_1, td_vec_1, n);
	double score_2 = vector_multiply(ind_vec_2, td_vec_2, n);
	
    double score = score_1 + score_2;

    gsl_vector_free(ind_vec_1);
    gsl_vector_free(ind_vec_2);
    gsl_vector_free(td_vec_1);
    gsl_vector_free(td_vec_2);

    fclose(ind_1);
    fclose(ind_2);
    fclose(td_1);
    fclose(td_2);

    return score;
}
