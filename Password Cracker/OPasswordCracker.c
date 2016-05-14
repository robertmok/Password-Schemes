#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//global variable
char apw[5] = " ";

//creates hashes of all possible 4 uppercase letter passwords
int create(FILE *fp) {
	#define ASCII_Z (int)'Z' //value of letter Z
	#define ASCII_A (int)'A' //value of letter A
	char letter[5]; //4 letter pw
	char temp[5]; //4 letter hashed pw
	//removing junk data
	memset(temp, 0, 5);
	memset(letter, 0, 5);
	int i, j, k, l;
	i = j = ASCII_A; //i and j is set to A
	k = l = ASCII_A; //k and l is set to A
	int counting = 0;
	for( i; i <= ASCII_Z; i++) {  //looping AAAA to ZZZZ, 1st char
		for( j; j <= ASCII_Z; j++) { //2nd char
			for( k; k <= ASCII_Z; k++) { //3rd char
				for( l; l <= ASCII_Z; l++) { //4th char
					letter[0] = (char)i; //setting a character to char
					letter[1] = (char)j;
					letter[2] = (char)k;
					letter[3] = (char)l;
					//avoiding newline at the end of file
					if (i == ASCII_Z && j == ASCII_Z && k == ASCII_Z && l == ASCII_Z)
                    {
						E(letter, temp);
						fprintf(fp, "%s %s", temp, letter); //delimited by a space
					}
					else
					{
						E(letter, temp); //hash
						fprintf(fp, "%s %s\n", temp, letter); //print to file
					}
					counting++;
				}
				l = ASCII_A;
			}
			k = ASCII_A;
		}
		j = ASCII_A;
	}
	return counting;
}

//find the hash that user has inputted in the hashedpws.txt, return 1 if found, 0 otherwise
int find(FILE *fp, char* input, char* output) {
    int found = 0;
    int length = 10; //hash = 4 + " " + pw = 4 + '\n' = 10
    char line[10] = " ";
    char thing[5] = " ";
    int k = 0;
    int i;

    while(fgets(line, length, fp)!= NULL) //read line from file until null
    {
        for(k = 0; k < 4; k++) //copy 1st 4 letter of line into thing
        {
            thing[k] = line[k];
        }
        if(compare(thing, input, 4)) //copied 4 letter with the user input
        {
            found = 1;
            goto end; //quit while loop if hash is found
        }
        else
        {
            found = 0;
        }
    }
    end:
    //save actual password from hashedpws.txt to print out later
        if(found == 1)
        {
            for(i = 0; i < 10; i++)
            {
                apw[i] = line[i+5];
            }
        }
    return found;
}

//comparing each character in string to see if they match
int compare(char* str1, char* str2, int length)
{
	int i;
	int count = 0;
	int matched = 0;
    count = 0;
	for (i = 0; i < length; i++)
	{
	    if (*str1++ != *str2++)
        {
			break;
        }
        count++;
	}
	if(count == length)
    {
        matched = 1;
    }
	return matched;
}

//encrypt characters
void E(char *in, char *out){
	out[0] = (in[0]&0x80)^(((in[0]>>1)&0x7F)^((in[0])&0x7F));
	out[1] = ((in[1]&0x80)^((in[0]<<7)&0x80))^(((in[1]>>1)&0x7F)^((in[1])&0x7F));
	out[2] = ((in[2]&0x80)^((in[1]<<7)&0x80))^(((in[2]>>1)&0x7F)^((in[2])&0x7F));
	out[3] = ((in[3]&0x80)^((in[2]<<7)&0x80))^(((in[3]>>1)&0x7F)^((in[3])&0x7F));
}

int main(int argc, char const *argv[])
{
	char input[5] = " ";
	FILE *fp = fopen("hashedpws.txt", "r+");
	//creating file if not exist
	if (!fp) {
		fp = fopen("hashedpws.txt", "a+");
		create(fp);
		rewind(fp); //go back to beginning of file
	}
	//input a 4 character hash password
	printf("Enter a hash of a 4 character password: ");
	fgets(input, 5, stdin);
    fgetc(stdin); //get rid of newline character
	//searching hashtable and returning actual password as output if found
	if (find(fp, input, apw))
		printf("\nPassword: %s\n", apw);
	else
		printf("\nPassword not found!\n");
	fclose(fp);

	printf("Press enter to continue...\n");
    getchar();
	return 0;
}
