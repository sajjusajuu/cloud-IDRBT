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
 * Compiling : $
 *			   $ 
 * Running : $ ./crpting_vector <name_of_file>, <file_s>, <file_m1>, <file_m2>, <dim>, <scaling>,
 				<splitingfactor>, <mulfactor>, <seed>
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

gsl_rng *r;		
extern unsigned long int gsl_rng_default_seed;

void init_rng(int seed) {

	const gsl_rng_type *T;
	gsl_rng_default_seed = seed;
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

}

void split(gsl_vector *org_vector, gsl_vector *splt_vec_1, gsl_vector *splt_vec_2,gsl_vector *s, int sf,int n) {

	gsl_vector *s_m = gsl_vector_alloc(n);
	gsl_vector_memcpy(s_m, s);
	int i = 0;
	
	for(; i<n; i++) {
		int bit = gsl_vector_get(s_m, i);
		if(sf == 1) {
			if(bit == 1) bit = 0;
			else bit = 1;
		}
		gsl_vector_set(s_m, i, bit);
	}

	for(i = 0; i<n; i++) {
		int bit = gsl_vector_get(org_vector, i);
		if(gsl_vector_get(s_m, i) == 0) {
			gsl_vector_set(splt_vec_1, i, bit);
			gsl_vector_set(splt_vec_2, i, bit);
		}
		else {
			int r_num = gsl_rng_uniform_int(r, bit);
			gsl_vector_set(splt_vec_1, i, r_num);
			gsl_vector_set(splt_vec_2, i, bit - r_num);
		}
	}

}

void compute_crp_vector(gsl_vector *splt_vec, gsl_matrix *m, gsl_vector *crp_vector, int mf, int n) {

	gsl_matrix *m_mul = gsl_matrix_alloc(n,n);
	gsl_matrix_memcpy(m_mul, m);

	/* 
	mf == 0 is for transpose of the matrix
	and mf == 1 is for inverse of the matrix
	*/
	if(mf == 0)
		gsl_matrix_transpose(m_mul);
	else {
		gsl_linalg_cholesky_decomp (m_mul);
		gsl_linalg_complex_cholesky_invert(m_mul);
	}
	int i = 0, j = 0;
	for(; i<n; i++) {
		int val = 0;
		for(j = 0; j<n; j++) {
			val += gsl_matrix_get(m_mul, i, j)*gsl_vector_get(splt_vec,j);
		}
		gsl_vector_set(crp_vector, i, val);
	}
}

int main(int argc, char ** argv) {
	if( argc != 10) {
		fprintf(stderr, "%s\n", "wrong number of arguments in crpting_vector");
		return 1;
	}

	/*
	Opening the coresponding files as streams
	*/
	FILE *fp_org_vec = fopen(argv[1], 'r');
	FILE *fp_s = fopen(argv[2], 'r');
	FILE *fp_m1 = fopen(argv[3], 'r');
	FILE *fp_m2 = fopen(argv[4], 'r');
	int n = atoi(argv[5]);
	int scale = atoi(argv[6]);
	int splitingfactor atoi(argv[7]);
	int mulfactor = atoi(argv[8]);
	int seed = atoi(argv[9]);
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

	gsl_vector_fread(fp_org_vec, org_vector);
	gsl_vector_fread(fp_s, s);
	gsl_matrix_fread(fp_m1, m1);
	gsl_matrix_fread(fp_m2, m2);

	if(scale > 1) {
		int i = 0
		for(; i<n; i++) {
			gsl_vector_set(org_vector, i, gsl_vector_get(org_vector, i)*scale);
		}
	}

	init_rng(seed);

	gsl_vector *splt_vec_1 = gsl_vector_alloc(n);
	gsl_vector *splt_vec_2 = gsl_vector_alloc(n);

	split(org_vector, splt_vec_1, splt_vec_2, s, splitingfactor, n);

	gsl_vector *crp_vector_1 = gsl_vector_alloc(n);
	gsl_vector *crp_vector_2 = gsl_vector_alloc(n);

	compute_crp_vector(splt_vec_1, m1, crp_vector_1, mulfactor, n);
	compute_crp_vector(splt_vec_2, m2, crp_vector_2, mulfactor, n);

	return 0;

}