/*
 * Jacob Pankey
 * Professor Tao
 * CS361: System Programming
 * Ceasar Cipher Decryption
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
void readFreq(float given[], char fname[]);
void calcFreq(float found[], char fname[]);
char rotate(char ch, int num);
int findKey(float given[], float found[]);
void decrypt(int key, char fname[]);

FILE *fp;
float freqsGiven[26], freqsObserved[26];

/* *****************************************
 * Read average letter frequencies for the
 * English language from a text file
 * provided to the program 
 * *****************************************/
void readFreq(float given[], char fname[])
{
	char line[50];
	char *token;
	int i = 0;	
	fp = fopen(fname, "r");

	if(!fp)
		printf("Something Went Wrong\n");
	
   	while(fgets(line, 50, fp)!=NULL)
	{	
		token = strtok(line, " ");
		token = strtok(NULL, "\n");
		given[i] = strtod(token,NULL);
		i++;
	}
	fclose(fp);
}
/******************************************
 * Read an encrypted text file and 
 * calculate the letter frequencies within
 * the file
 *****************************************/
void calcFreq(float found[], char fname[])
{
	char ch;
	int total, counts[26], i;
	total = 0;
	for(i = 0; i < 26; i++)
		counts[i] = 0;

	fp = fopen(fname, "r");
	if(!fp)
		printf("Something Went Wrong\n");
	
	while((ch = fgetc(fp)) != EOF)
	{
		if(isalpha(ch))
		{	
			ch = toupper(ch);
			counts[ch%26]++;
			total++;
		}
	}
	for(i = 'A'; i <= 'Z'; i++)
		found[i -65]  = (float)counts[i % 26] / (float)total;
	fclose(fp);
}
 /****************************************
 * Roatate characater ch down the alphabet
 * by 'num' positions
 *****************************************/
char rotate(char ch, int num)
{
	if(isupper(ch))
	{
		return(ch - 'A' + num) % 26 + 'A';
	}
	else if(islower(ch))
	{
		return(ch - 'a' + num) % 26 + 'a';
	}
	return ch;
}
/******************************************
 * Determine the key to the caesar cipher
 * used to encrypt the text file. Best key
 * is chosen using least squares fit.
 ******************************************/
int findKey(float given[], float found[])
{
	double sum, lowSum, dif;
	int key, j, i;
	key = -1;
	for(j = 0; j < 26; j++)
	{
		sum = 0;
		for(i = 0; i < 26; i++)
		{
			dif = given[i] - found[(i + j) % 26];
			sum += dif * dif;	
		}
		if(key == -1 || sum < lowSum)
		{
			key = j;
			lowSum = sum;
		}
	}	
	return key;
}
/*******************************************
 * Decrypt file 'fname' using the key passed
 * as a parameter The function writes to two
 * separate files as output to avoid 
 * overwriting before the results are checked
 * ****************************************/
void decrypt(int key, char fname[])
{
	char ch;
	FILE *out;
	
	if(strcmp("/home/pankeyj/Documents/CS361/Projects/data.out", fname) == 0)
		out = fopen("/home/pankeyj/Documents/CS361/Projects/results.txt", "w");
	else
		out = fopen("/home/pankeyj/Documents/CS361/Projects/results2.txt", "w");
	fp = fopen(fname, "r");
	while((ch = fgetc(fp)) != EOF)
	{
		if(isalpha(ch))
		{
			fprintf(out, "%c", rotate(ch, 26-key));
		}
		else
		{
			fprintf(out, "%c", ch);
		}	
	}
	fclose(fp);
	fclose(out);
}

int main()
{
	int key;
	readFreq(freqsGiven, "/home/pankeyj/Documents/CS361/Projects/letterfreq.txt");
	// Decrytp long text file
	calcFreq(freqsObserved, "/home/pankeyj/Documents/CS361/Projects/data2.out");
	key = findKey(freqsGiven, freqsObserved);
	decrypt(key, "/home/pankeyj/Documents/CS361/Projects/data2.out");
	// Decrypt short text file
	calcFreq(freqsObserved, "/home/pankeyj/Documents/CS361/Projects/data.out");
	key = findKey(freqsGiven, freqsObserved);
	decrypt(key, "/home/pankeyj/Documents/CS361/Projects/data.out");

	return 0;

}
