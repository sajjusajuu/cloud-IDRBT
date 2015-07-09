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
 * The lenght of the dictionary is 725
 *
 * Compiling : $ gcc -Wall -I/usr/local/include -c -fPIC generate_keys.c
 *			   $ gcc -L/usr/local/lib generate_keys.o -lgsl -lgslcblas -lm -shared -o generate_keys.so
 * Running : $ generate_keys(n, seed)
 * Output :This program generates  s.key, m1.key, m2.key files in the current directory
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

#define RANGE 1000

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

void generate_s(gsl_vector *s, int n) {
	int i = 0;
	for(i = 0; i<n; i++) {
		gsl_vector_set(s, i, gsl_rng_uniform_int(r, RANGE) % 2);
		printf("\rGenerating the vector S: %d%% Completed", ((i+1)*100/n));
	}
	printf("\n");
}

void generate_mat(gsl_matrix *m, int n, int n_m) { 


	int i, j;
	gsl_matrix *t_m = gsl_matrix_alloc(n,n);
	gsl_matrix *v = gsl_matrix_alloc(n,n);
	gsl_vector *singular = gsl_vector_alloc(n);
	gsl_vector *work = gsl_vector_alloc(n);

	//int count = 1;
	while(1) {
		/* Generating a random nXn matrix
		*/

		//printf("Generating for %d time\n", count);
		for(i = 0; i<n; i++) {
			for(j = 0; j<n; j++)
				gsl_matrix_set(m,i,j,gsl_rng_uniform_int(r, RANGE));
			printf("\rGenerating the Matrix M%d: %d%% Completed",n_m, ((i+1)*100/n));
		}
		printf("\n");
		printf("Checking if the matrix M%d is invertible: ", n_m);
		gsl_matrix_memcpy(t_m , m);
		
		/* Singular Value Decomposition is applied to the random generated matrix 'm' and 
		   a singular vector 'singular' is computed which is in non incresing order.
		   If the matrix is invertible then the minimum value of the singular vector is greater 
		   than zero.
		  */
		gsl_linalg_SV_decomp(t_m , v , singular , work );
		int l_singular = (int ) gsl_vector_get(singular , n-1);
		if(l_singular > 0){
			printf("Yes\n");
			gsl_vector_free(singular);
			gsl_vector_free(work);
			gsl_matrix_free(v);
			gsl_matrix_free(t_m);
			return;
		}
		printf("No, Regenarating M%d\n", n_m);
		//count++;
	}

}

 int generate_keys(int n, int seed) {
	/*  Initializing the global rng instance
	*/
	init_rng(seed);               					

 	gsl_vector *s = gsl_vector_alloc(n);
 	generate_s(s, n);
 	printf("Generating the inverse vector of S: ");
 	gsl_vector *s_i = gsl_vector_alloc(n);
 	int i = 0;
 	for(i =0; i<n; i++) 
 		gsl_vector_set(s_i, i, abs(gsl_vector_get(s, i)-1));
 	printf("Done\n");

 	gsl_matrix *m1 = gsl_matrix_alloc(n,n);
 	gsl_matrix *m2 = gsl_matrix_alloc(n,n);
 	gsl_matrix *m1_t = gsl_matrix_alloc(n, n);
 	gsl_matrix *m1_i = gsl_matrix_alloc(n, n);
 	gsl_matrix *m2_t = gsl_matrix_alloc(n, n);
 	gsl_matrix *m2_i = gsl_matrix_alloc(n, n);
 	gsl_matrix *t_m = gsl_matrix_alloc(n ,n);



 	generate_mat(m1, n, 1);
 	generate_mat(m2, n, 2);

 	printf("Generating the transpose of matrix M1 :");
 	gsl_matrix_memcpy(m1_t, m1);
 	gsl_matrix_transpose(m1_t);
 	printf("Done\n");
 	printf("Generating the transpose of matrix M2 :");
 	gsl_matrix_memcpy(m2_t, m2);
 	gsl_matrix_transpose(m2_t);
 	printf("Done\n");

 	gsl_permutation* p = gsl_permutation_calloc(n);
 	int sn = 1;

 	printf("Generating the inverse of matrix M1 :");
 	gsl_matrix_memcpy(t_m, m1);
 	gsl_linalg_LU_decomp(t_m ,p, &sn);
 	gsl_linalg_LU_invert (t_m, p ,m1_i);
 	printf("Done\n");
 	printf("Generating the inverse of matrix M2 :");
 	gsl_matrix_memcpy(t_m, m2);
 	gsl_linalg_LU_decomp(t_m ,p, &sn);
 	gsl_linalg_LU_invert (t_m, p ,m2_i);
 	printf("Done\n");

 	FILE *fps = fopen("s.key", "w+b");
 	FILE *fps_i = fopen("s_i.key", "w+b");
	FILE *fpm1 = fopen("m1.key", "w+b");
	FILE *fpm2 = fopen("m2.key", "w+b");
	FILE *fpm1_t = fopen("m1_t.key" , "w+b");
	FILE *fpm2_t = fopen("m2_t.key" , "w+b");
	FILE *fpm1_i = fopen("m1_i.key" , "w+b");
	FILE *fpm2_i = fopen("m2_i.key" , "w+b");

 	gsl_vector_fwrite(fps , s);
 	gsl_vector_fwrite(fps_i, s_i);
 	gsl_matrix_fwrite (fpm1 , m1);
 	gsl_matrix_fwrite(fpm2, m2);
 	gsl_matrix_fwrite(fpm1_t, m1_t);
 	gsl_matrix_fwrite(fpm2_t, m2_t);
 	gsl_matrix_fwrite(fpm1_i, m1_i);
 	gsl_matrix_fwrite(fpm2_i, m2_i);

 	gsl_vector_free(s);
 	gsl_vector_free(s_i);
 	gsl_matrix_free(m1);
 	gsl_matrix_free(m2);
 	gsl_matrix_free(m1_t);
 	gsl_matrix_free(m2_t);
 	gsl_matrix_free(m1_i);
 	gsl_matrix_free(m2_i);
 	gsl_permutation_free(p);
 	gsl_rng_free(r);

 	fclose(fps);
 	fclose(fpm1);
 	fclose(fpm2);
 	fclose(fpm1_t);
 	fclose(fpm2_t);
 	fclose(fpm1_i);
 	fclose(fpm2_i);
 	return 0;
 }
