#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LEN 8192

// sample code for 62/162 randomness analysis hw
// sws, feb 2023

/**
 * Tests if a given line contains a randomly generated string of 1s and 0s.
 * Uses the `Poker` probabalistic random number test as described by FIPS in:
 * https://csrc.nist.gov/csrc/media/publications/fips/140/1/archive/1994-01-11/documents/fips1401.pdf
*/
void test_randomness(int *array, int len, int line_index) {

  // array is a sequence len integers, each 0 or 1
  // test if it's random

  int frequencies[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  // Group bits into 4 bit subsequences
  for(int i = 0; i+4 < len; i+=4){
    int val = 0;
    for(int j = 0; j < 4; j++){
      // Set nth bit of current value to be 1
      val = (val << 1) | array[i + j];
    }
    frequencies[val]++;
  }

  // Calculate an X value
  double sum = 0;
  for(int i = 0; i < 16; i++){
    double curr_val = (double)(frequencies[i] * frequencies[i]);
    sum += curr_val;
  }
  sum -= 5000;

  double scale_fact = 16.0/5000.0;
  sum *= scale_fact;

  // Random (machine) if X is in range, Human otherwise
  if(sum > 1.03 && sum < 57.4){
    printf("S%d MACHINE\n", line_index);
  }else{
    printf("S%d HUMAN\n", line_index);
  }
}

int main(int argc, char **argv)
{

  int i = 0, j;
  int len;
  int *array;

  
  char *buff = malloc(MAX_LINE_LEN);
  if ( (!buff) || (argc < 2))
    return -1;
  FILE *f = fopen(argv[1], "r");
  while (fgets(buff, MAX_LINE_LEN, f)) {
    len = strlen(buff);
    
    if (len > MAX_LINE_LEN - 2)
      printf("Warning: MAX_LINE_LEN may be too short\n");
    
    if (len > 1) {
      len--; // remove the newline
      buff[len] = 0x00;
      //printf("h%d len=%d\n", i, len);
      //printf("String read: [%s]\n\n", buff);

      array = calloc(len, sizeof(int));  // so it's all zeros
      if (NULL == array) {
	      printf("calloc failed\n");
	      return -1;
      }
      for (j = 0; j < len; j++) {
	      if ('1'==buff[j])
	        array[j] = 1;
      }

      // Test randomness on each line and print result to stdout
      test_randomness(array, len, i);
      free(array);
      i++;
    }
  }


  fclose(f);
  return 0;
}
