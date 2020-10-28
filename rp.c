/*
Copyright (c) 2016-2020 Jeremy Iverson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

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

static int
cmp(void const *ap, void const *bp)
{
  struct distance_metric const a = *(struct distance_metric*)ap;
  struct distance_metric const b = *(struct distance_metric*)bp;

  return a.distance < b.distance ? -1 : 1;
}

int
main(int argc, char * argv[])
{
  size_t n, m, k;

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

  /* Get user input. */
  for (size_t j = 0; j < m - 1; j++) {
    printf("Enter your rating for movie %zu: ", j + 1);
    scanf("%lf", &urating[j]);
  }

  /* Allocate more memory. */
  struct distance_metric * const distance = calloc(n, sizeof(*distance));

  /* Check for success. */
  assert(distance);

  /* Compute distances. */
  for (size_t i = 0; i < n; i++) {
    distance[i].viewer_id = i;
    for (size_t j = 0; j < m - 1; j++) {
      distance[i].distance += fabs(urating[j] - rating[i * m + j]);
    }
  }

  /* Sort distances. */
  qsort(distance, n, sizeof(*distance), cmp);

  /* Get user input. */
  printf("Enter the number of similar viewers to report: ");
  scanf("%zu", &k);

  /* Output k viewers who are least different from the user. */
  printf("Viewer ID   Movie five   Distance\n");
  printf("---------------------------------\n");

  for (size_t i = 0; i < k; i++) {
    printf("%9zu   %10.1lf   %8.1lf\n", distance[i].viewer_id + 1,
      rating[distance[i].viewer_id * m + 4], distance[i].distance);
  }

  printf("---------------------------------\n");

  /* Compute the average to make the prediction. */
  double sum = 0.0;
  for (size_t i = 0; i < k; i++) {
    sum += rating[distance[i].viewer_id * m + 4];
  }

  /* Output prediction. */
  printf("The predicted rating for movie five is %.1lf.\n", sum / k);

  /* Deallocate memory. */
  free(rating);
  free(urating);
  free(distance);

  return EXIT_SUCCESS;
}
