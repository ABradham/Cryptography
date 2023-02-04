#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>

#define DEBUG 0
#define NUM_BUCKETS 5000
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
        unsigned int cs10_index = md5_bhash(word, len, NUM_BUCKETS);
        unsigned int bad_index = md5_bhash(word, len, NUM_BUCKETS);

        md5_buckets[md5_index] += 1;
        cs10_buckets[cs10_index] += 1;
        bad_buckets[bad_index] += 1;

        num_words++;
    }
    
    // Close the file
    fclose(file);

    // Print results in a nice table
    for(int i = 0; i<NUM_BUCKETS; i++){
        printf("[%3d]: %8d\n", i, md5_buckets[i]);
    }
    return 0;
}