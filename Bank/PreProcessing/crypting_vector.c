/*
 * IDRBT - Cloud Retrieval
 * Webpage: https://www.idrbt.ac.in
 * Contact: hussainsajid@acm.org
 * Institute of Development and Research in Banking Technology
 * 
 * The Original Code is crypting_vector.c
 * 
 * Note : Install GNU Scientific Library(GSL) before compiling this file
 *
 * Compiling : $ gcc -Wall -I/usr/local/include -c -fPIC crypting_vector.c
 *			   $ gcc -L/usr/local/lib crypting_vector.o -lgsl -lgslcblas -lm -shared -o crypting_vector.so
 * Running : $ crypt_vector(arr, n , factor, seed)
 *
 *	factor = 1 if query and 0 if else
 *
 * Output :Two vector files.
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
#include <gsl/gsl_permutation.h>
#include <unistd.h>

gsl_rng *r;		
extern unsigned long int gsl_rng_default_seed;

void init_rng(int seed) {

	const gsl_rng_type *T;
	gsl_rng_default_seed = seed;
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

}

void split(gsl_vector *org_vector, gsl_vector *splt_vec_1, gsl_vector *splt_vec_2,gsl_vector *s, int n) {    
	int i;
	for(i = 0; i<n; i++) {
		int bit = (int) gsl_vector_get(org_vector, i);
		if( (int)gsl_vector_get(s, i) == 0) {
			gsl_vector_set(splt_vec_1, i, bit);
			gsl_vector_set(splt_vec_2, i, bit);
		}
		else {
		    int r_num;
		    if(bit > 0)
			    r_num = gsl_rng_uniform_int(r, bit);
			else
		        r_num = 0;
			gsl_vector_set(splt_vec_1, i, r_num);
			gsl_vector_set(splt_vec_2, i, bit - r_num);
		}
	}

}

void compute_crp_vector(gsl_vector *splt_vec, gsl_matrix *m, gsl_vector *crp_vector, int n) {

	int  i , j;
	for(i = 0; i<n; i++) {
		double val = 0;
		for(j = 0; j<n; j++) {
			val += gsl_matrix_get(m, i, j)*gsl_vector_get(splt_vec,j);
		}
		gsl_vector_set(crp_vector, i, val);
	}
}

void crypt_vector(int *a, int n, int factor, int seed) {

	/*
	Opening the coresponding files as streams
	*/

	int i = 0;
	FILE *fp_s;
	FILE *fp_m1;
	FILE *fp_m2;
	if(factor == 0) {
		fp_s = fopen("s.key", "r");
		fp_m1 = fopen("m1_t.key", "r");
		fp_m2 = fopen("m2_t.key", "r");
	}
	else {
		fp_s = fopen("s_i.key", "r");
		fp_m1 = fopen("m1_i.key", "r");
		fp_m2 = fopen("m2_i.key", "r");
	}
	/*
	Allocating memories for the datastructures
	*/
	gsl_vector *org_vector = gsl_vector_alloc(n);
	gsl_vector *s = gsl_vector_alloc(n);
	gsl_matrix *m1 = gsl_matrix_alloc(n,n);
	gsl_matrix *m2 = gsl_matrix_alloc(n,n);


	/* 
	Getting them into memory
	*/
	gsl_vector_fread(fp_s, s);
	gsl_matrix_fread(fp_m1, m1);
	gsl_matrix_fread(fp_m2, m2);



	for(i = 0; i<n; i++)
		gsl_vector_set(org_vector, i, a[i]);

	init_rng(seed);
	
	if(factor == 1) {
		int scale = gsl_rng_uniform_int(r, 100);
		gsl_vector_scale(org_vector, scale);
	}
	
	gsl_vector *splt_vec_1 = gsl_vector_alloc(n);
	gsl_vector *splt_vec_2 = gsl_vector_alloc(n);
    
	split(org_vector, splt_vec_1, splt_vec_2, s, n);
	
	
	/*{
        int j = 0;
        for(j = 0; j<n; j++)
            printf("%d",  (int) gsl_vector_get(org_vector, j));
	    
	    printf("\n\n\n");
        //int i = 0, j = 0;
        for(j = 0; j<n; j++)
            printf("%d",  (int) gsl_vector_get(splt_vec_1, j));
            
        printf("\n\n\n");
        for(j = 0; j<n; j++)
            printf("%d", (int) gsl_vector_get(splt_vec_2, j));
        printf("\n\n");
	}*/
	

	gsl_vector *crp_vector_1 = gsl_vector_alloc(n);
	gsl_vector *crp_vector_2 = gsl_vector_alloc(n);

	compute_crp_vector(splt_vec_1, m1, crp_vector_1, n);
	compute_crp_vector(splt_vec_2, m2, crp_vector_2, n);
	FILE *f_crpt_1 = fopen("crypt_1.key", "w");
	FILE *f_crpt_2 = fopen("crypt_2.key", "w");

	gsl_vector_fwrite(f_crpt_1, crp_vector_1);
	gsl_vector_fwrite(f_crpt_2, crp_vector_2);

	gsl_vector_free(org_vector);
	gsl_vector_free(s);
	gsl_vector_free(splt_vec_1);
	gsl_vector_free(splt_vec_2);
	gsl_vector_free(crp_vector_1);
	gsl_vector_free(crp_vector_2);
	gsl_matrix_free(m1);
	gsl_matrix_free(m2);
	gsl_rng_free(r);

	fclose(fp_s);
	fclose(fp_m1);
	fclose(fp_m2);
	fclose(f_crpt_1);
	fclose(f_crpt_2);

}
