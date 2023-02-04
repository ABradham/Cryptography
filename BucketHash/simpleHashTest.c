#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/md5.h>

#define DEBUG 0
#define NUM_BUCKETS 100
#define FILENAME "words.txt"


unsigned int bad_bhash(char *data, int len, int buckets)
{

    unsigned int sum, j, rval;

    if ((0 == data) || (0 == len))
    {
        printf("illegal input\n");
        exit(-1);
    }

    sum = data[0];

    for (j = 1; j < len; j++)
        sum = sum ^ ((unsigned char)(data[j]));

    rval = sum % buckets;

    if (DEBUG)
    {
        printf("bad_bhash %s to %d\n", data, rval);
    }

    return rval;
}

unsigned int cs10_bhash(char *data, int len, int buckets)
{

    unsigned int sum, j, rval;

    if ((0 == data) || (0 == len))
    {
        printf("illegal input\n");
        exit(-1);
    }

    sum = data[0];

    for (j = 1; j < len; j++)
    {
        sum = 37 * sum + ((unsigned char)(data[j]));
    }

    rval = sum % buckets;

    if (DEBUG)
    {
        printf("cs10_bhash %s to %d\n", data, rval);
    }

    return rval;
}

int md5_bhash(char *data, int len, int buckets)
{

    unsigned char hval[128];
    unsigned int *ip = (unsigned int *)&hval[0], rval;

    MD5(data, len, &hval[0]);

    rval = (*ip) % buckets;

    if (DEBUG)
    {
        printf("md5_bhash %s to %d\n", data, rval);
    }

    return rval;
}

unsigned int find_max(unsigned int* arr, int len){
    unsigned long long max = 0;
    for(int i = 0; i<len; i++){
        if(arr[i] > max)
            max = arr[i];
    }
    return max;
}

unsigned int find_min(unsigned int* arr, int len){
    unsigned long long min = __LONG_MAX__;
    for(int i = 0; i<len; i++){
        if(arr[i] < min)
            min = arr[i];
    }
    return min;
}

int main()
{
    // Create integer array to hold number of words assigned to each bucket
    unsigned int* md5_buckets = calloc(NUM_BUCKETS, sizeof(unsigned int));
    unsigned int* cs10_buckets = calloc(NUM_BUCKETS, sizeof(unsigned int));
    unsigned int* bad_buckets = calloc(NUM_BUCKETS, sizeof(unsigned int));
    
    // Open file to get fp
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("Could not open file\n");
        exit(1);
    }

    // For every word in the file, hash and increment value of counter in bucket
    char word[100];
    int num_words = 0;
    int len;
    while (fscanf(file, "%s%n", word, &len) == 1)
    {
        unsigned int md5_index = md5_bhash(word, len, NUM_BUCKETS);
        unsigned int cs10_index = cs10_bhash(word, len, NUM_BUCKETS);
        unsigned int bad_index = bad_bhash(word, len, NUM_BUCKETS);

        md5_buckets[md5_index] += 1;
        cs10_buckets[cs10_index] += 1;
        bad_buckets[bad_index] += 1;

        num_words++;
    }
    
    // Close the file
    fclose(file);

    // Print experiment results
    printf("-----MD5 Bucket-----\n");
    printf("Min bucket size: %4d\n", find_min(md5_buckets, NUM_BUCKETS));
    printf("Max bucket size: %4d\n", find_max(md5_buckets, NUM_BUCKETS));
    printf("\n\n");

    printf("-----CS10 Bucket-----\n");
    printf("Min bucket size: %4d\n", find_min(cs10_buckets, NUM_BUCKETS));
    printf("Max bucket size: %4d\n", find_max(cs10_buckets, NUM_BUCKETS));
    printf("\n\n");

    printf("-----Bad Bucket-----\n");
    printf("Min bucket size: %4d\n", find_min(bad_buckets, NUM_BUCKETS));
    printf("Max bucket size: %4d\n", find_max(bad_buckets, NUM_BUCKETS));
    printf("\n\n");


    printf("As you can see, each hashing algorithm (absent the `bad` bucket hash) performs relatively equally on the task of bucketing.\n");
    printf("Now, let's see how much time each hash takes to bucket:\n\n");


   /**
    * ========MD5 TIME TEST BEGIN========
   */
    FILE *file_md5_time = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("Could not open file\n");
        exit(1);
    }

    clock_t md5_start, md5_end;
    md5_start = clock();

    char word_md5_time[100];
    while (fscanf(file_md5_time, "%s%n", word_md5_time, &len) == 1)
    {
        md5_bhash(word_md5_time, len, NUM_BUCKETS);
    }
    md5_end = clock();
    
    // Close the file
    fclose(file_md5_time);

   /**
    * ========MD5 TIME TEST END========
   */

  /**
    * ========CS10 TIME TEST BEGIN========
   */
    // Open file to get fp
    FILE *file_cs10_time = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("Could not open file\n");
        exit(1);
    }

    clock_t cs10_start, cs10_end;
    cs10_start = clock();

    char word_cs10_time[100];
    while (fscanf(file_cs10_time, "%s%n", word_cs10_time, &len) == 1)
    {
        cs10_bhash(word_cs10_time, len, NUM_BUCKETS);
    }
    cs10_end = clock();
    
    // Close the file
    fclose(file_cs10_time);

    /**
    * ========CS10 TIME TEST END========
    */


  /**
    * ========BAD TIME TEST BEGIN========
   */
    // Open file to get fp
    FILE *file_bad_time = fopen(FILENAME, "r");
    if (file_bad_time == NULL)
    {
        printf("Could not open file\n");
        exit(1);
    }

    clock_t bad_start, bad_end;
    bad_start = clock();

    char word_bad_time[100];
    while (fscanf(file_bad_time, "%s%n", word_bad_time, &len) == 1)
    {
        bad_bhash(word, len, NUM_BUCKETS);
    }
    bad_end = clock();
    
    // Close the file
    fclose(file_bad_time);

    /**
    * ========BAD TIME TEST END========
    */

   //Print time test results
   printf("====TIME TEST RESULTS====\n");
   printf("MD5: %10f\n", ((double) (md5_end - md5_start)) / CLOCKS_PER_SEC);
   printf("CS10: %10f\n", ((double) (cs10_end - cs10_start)) / CLOCKS_PER_SEC);
   printf("BAD: %10f\n\n", ((double) (bad_end - bad_start)) / CLOCKS_PER_SEC);

   printf("As you can see, MD5 takes ~2.5 times longer to hash when compared to cs10 and our `bad` hash.\n");

    return 0;
}