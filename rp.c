/* assert */
#include <assert.h>
/* fabs */
#include <math.h>
/* printf, fopen, fclose, fscanf, scanf */
#include <stdio.h>
/* EXIT_SUCCESS, malloc, calloc, free, qsort */
#include <stdlib.h>

struct distance_metric {
  size_t viewer_id;
  double distance;
};

int
main(int argc, char * argv[])
{
  size_t n, m;

  /* Validate command line arguments. */
  assert(2 == argc);

  /* ... */
  char const * const fn = argv[1];

  /* Validate input. */
  assert(fn);

  /* Open file. */
  FILE * const fp = fopen(fn, "r");
  assert(fp);

  /* Read file. */
  fscanf(fp, "%zu %zu", &n, &m);

  /* Allocate memory. */
  double * const rating = malloc(n * m * sizeof(*rating));

  /* Check for success. */
  assert(rating);

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < m; j++) {
      fscanf(fp, "%lf", &rating[i * m + j]);
    }
  }

  /* Close file. */
  int const ret = fclose(fp);
  assert(!ret);

  /* Allocate more memory. */
  double * const urating = malloc(m * sizeof(*urating));

  /* Check for success. */
  assert(urating);

  /* Get user input. 
  for (size_t j = 0; j < m - 1; j++) {
    printf("Enter your rating for movie %zu: ", j + 1);
    scanf("%lf", &urating[j]);
  }
*/

//Auto import user values
  urating[0] = 5.0;
  urating[1] = 5.0;
  urating[2] = 5.0;
  urating[3] = 5.0;

  /* Allocate more memory. */
  double count[10] = { 0.0 };
  double prob[10] = { 0.0 };
  double uprob[40] = { 0.0 }; 

  /* Compute probabilities */
  for (int k = 0; k < 10; k++) {
    for (size_t i = 0; i < n; i++) {
      count[k] += (rating[i * m + 4] == (k + 1) / 2.0);
    }
  }

  for (int k = 0; k < 10; k++) {
    prob[k] = count[k]/n;
  }

  for (int k = 0; k < 10; k++) {
    printf("prob[%d] = %lf\n", k, prob[k]);
  }
  
  
  for (int i = 0; i < n; i++) {
    for (size_t k = 0; k < 10; k++){
      for (int x = 0; x < (m-1); x++) {
      //if rating[i] = urate[i] AND user [i] = 5.0 ++
        if(urating[x] == rating[i * m + x] && rating [i * m + 4] == (k + 1)/2.0){
	  uprob[(k * 4) + x] += 1;
      }
      }
    }
  }

  for (int i = 0; i < 40; i++){
    printf("uprob[%d] = %lf\n", i, uprob[i]);
  }

  for (int i=0; i<10; i++){
    printf("count[%d] = %lf\n", i, count[i]);
  } 

  double prediction[10] = {0.0};
  for(size_t k = 0; k<10; k++){
   if(prob[k] != 0){
    prediction[k] = prob[k] * (uprob[4*k] / count[k]) * (uprob[4*k+1] / count[k]) * 		(uprob[4*k+2] / count[k]) * (uprob[4*k+3] / count[k]);
    }
    
   
  } 

  for (int i = 0; i < 10; i++){
    printf("prediction[%d] = %lf\n", i, prediction[i]);
  }
  
  int i;
  double max = prediction[0];
  for (int k = 1; k < 10; k++){
    if (prediction[k] > max){
      i = k; 
      max = prediction[k];
    }
  }
	

  /* Output prediction. */
  printf("The predicted rating for movie five is %.1lf.\n", (i + 1)/2.0);

  /* Deallocate memory. */
  free(rating);
  free(urating);
  return EXIT_SUCCESS;
}
