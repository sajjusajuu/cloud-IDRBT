/*
 * IDRBT - Cloud Retrieval
 * Webpage: https://www.idrbt.ac.in
 * Contact: hussainsajid@acm.org
 * Institute of Development and Research in Banking Technology
 * 
 * The Original Code is generate_keys.c
 * This program will generate an n bit random vector(s) and also two nXn random invertible matrices m1 and m2
 * 
 * Note : Install GNU Scientific Library(GSL) before compiling this file
 *
 * Compiling : $ gcc -Wall -I/usr/local/include -c generate_keys.c
 *			   $ gcc -L/usr/local/lib generating_keys.o -lgsl -lgslcblas -lm -o generate_keys.out
 * Running : $ ./generating_keys.out <n> <seed>
 * Output :This program generates  s.key, m1.key, m2.key files in the current directory
 *
 * Contributor(s):
 *  Sajid Hussain
 */


#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_rng.h>

/* Creating a global random_number_generator(rng) instance 
*/
gsl_rng *r;		
extern unsigned long int gsl_rng_default_seed;								

void init_rng(int seed) {

	const gsl_rng_type *T;
	gsl_rng_default_seed = seed;
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

}

void generate_s(gsl_vector *s, size_t n) {
	size_t i = 0;
	for(i = 0; i<n; i++) {
		gsl_vector_set(s, i, gsl_rng_get(r));
	}
}

void generate_mat(gsl_matrix *m, size_t n) { 


	size_t i, j;
	gsl_matrix *t_m = gsl_matrix_alloc(n,n);
	gsl_matrix *v = gsl_matrix_alloc(n,n);
	gsl_vector *singular = gsl_vector_alloc(n);
	gsl_vector *work = gsl_vector_alloc(n);


	while(1) {
		/* Generating a random nXn matrix
		*/


		for(i = 0; i<n; i++)
			for(j = 0; j<n; j++)
				gsl_matrix_set(m,i,j,gsl_rng_get(r));
			
		gsl_matrix_memcpy(t_m , m);
		
		/* Singular Value Decomposition is applied to the random generated matrix 'm' and 
		   a singular vector 'singular' is computed which is in non incresing order.
		   If the matrix is invertible then the minimum value of the singular vector is greater 
		   than zero.
		  */
		gsl_linalg_SV_decomp(t_m , v , singular , work );
		int l_singular = (int ) gsl_vector_get(singular , n-1);
		if(l_singular > 0)
			return;
	}

}

 int main(int argc, char *argv[]) {

 	if(argc != 3) {
 		fprintf(stderr , "%s\n", "wrong number of arguments");
 		return 1;
 	}
 	/* The n stores the dimensions of the random bit vector (S).
 	*/
	size_t n = atoi(argv[1]); 					
	int seed = atoi(argv[2]);
	/*  Initializing the global rng instance
	*/
	init_rng(seed);               					

 	gsl_vector *s = gsl_vector_alloc(n);
 	generate_s(s, n);
 	gsl_matrix *m1 = gsl_matrix_alloc(n,n);
 	gsl_matrix *m2 = gsl_matrix_alloc(n,n);
 	generate_mat(m1, n);
 	generate_mat(m2, n);

 	FILE *fps = fopen("s.key", "w+b");
	FILE *fpm1 = fopen("m1.key", "w+b");
	FILE *fpm2 = fopen("m2.key", "w+b");
 	gsl_vector_fwrite(fps , s);
 	gsl_matrix_fwrite (fpm1 , m1);
 	gsl_matrix_fwrite(fpm2, m2);

 	return 0;
 }