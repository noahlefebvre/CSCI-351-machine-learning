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

  /* Get user input for ranking movies 1-4*/ 
  for (size_t j = 0; j < m - 1; j++) {
    printf("Enter your rating for movie %zu: ", j + 1);
    scanf("%lf", &urating[j]);
  }


/*Auto import user values
  urating[0] = 5.0;
  urating[1] = 5.0;
  urating[2] = 5.0;
  urating[3] = 5.0;*/

  /* Allocate memory for the three probability arrays. */
  double count[10] = { 0.0 };
  double prob[10] = { 0.0 };
  double uprob[40] = { 0.0 }; 

  /* Compute count for each movie rating(1-5) moviei = k+1/ 2.0 
     c(movie5 = 0.5), c(movie5 = 1.0, c(movie5 = 1.5) ... */
  for (int k = 0; k < 10; k++) {
    for (size_t i = 0; i < n; i++) {
      count[k] += (rating[i * m + (m-1)] == (k + 1) / 2.0);
    }
  }

  /*Compute probability for each movie rating (1.0-5.0)
     c(movie5 = 0.5)/ total viewers in database*/
  for (int k = 0; k < 10; k++) {
    prob[k] = count[k]/n;
  }
/*
  for (int k = 0; k < 10; k++) {
    printf("prob[%d] = %lf\n", k, prob[k]);
  }*/
  
  /*Find conditional prob for each movie and each of the ratings; uprob = count of people that rate movie 5 (1-5) and also rate movie 1,2..5 the same as user input 1,2..5
*/  
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

  /* probabilty that user will rate movie 5 (0.5-5.0)*/
  double prediction[10] = {0.0};
  for(size_t k = 0; k<10; k++){
    if(prob[k] != 0){
    prediction[k] = prob[k] * (uprob[4*k] / count[k]) * (uprob[4*k+1] / count[k]) * 		(uprob[4*k+2] / count[k]) * (uprob[4*k+3] / count[k]);
    }
    
   
  } 

  /*finds max prediction to show which rating is most likely to be given to 	movie 5 by user*/
  int i;
  double max = prediction[0];
  for (int k = 1; k < 10; k++){
    if (prediction[k] > max){
      i = k; 
      max = prediction[k];
    }
  }
	

  /* Prints the predicted value of user 5's rating for movie 5 */
  printf("The predicted rating for movie five is %.1lf.\n", (i + 1)/2.0);

  /* Deallocate memory. */
  free(rating);
  free(urating);
  return EXIT_SUCCESS;
}
