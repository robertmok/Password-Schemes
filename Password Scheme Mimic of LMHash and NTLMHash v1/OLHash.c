#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//global variable
char tpw[13] = " "; //store pw in table to compare, 12 for pw + 1 for '\0'
int index = 1; //line number in file for replacement

//encrypt characters
void E(char *in, char *out)
{
	out[0] = (in[0]&0x80)^(((in[0]>>1)&0x7F)^((in[0])&0x7F));
	out[1] = ((in[1]&0x80)^((in[0]<<7)&0x80))^(((in[1]>>1)&0x7F)^((in[1])&0x7F));
	out[2] = ((in[2]&0x80)^((in[1]<<7)&0x80))^(((in[2]>>1)&0x7F)^((in[2])&0x7F));
	out[3] = ((in[3]&0x80)^((in[2]<<7)&0x80))^(((in[3]>>1)&0x7F)^((in[3])&0x7F));
}

//convert lowercase into uppercase
void convert(char* npw, char* cpw)
{
    int i;
    for(i = 0; i < 12; i++)
    {
        cpw[i] = toupper(npw[i]); //strupr() also works
    }
}

//find user input in hashtable.txt
int find(FILE* fp, char* id, int index)
{
    int found = 0;
    int length = 18; //ID = 4 + " " + hash pw = 12 + '\n'
    char line[18] = " ";
    int e = 0;
    int i;

    while(fgets(line, length, fp)!= NULL) //gets a line and store in variable line[length]
    {
        //compare first 4 chars in line[18] to given ID
        if(strncmp(line, id, 4) == 0)  //can compare entire string? yes = no need for loop, no = need for loop and count
        {
            found = 1;
            goto end; //need goto, if found
        }
        index++; //keeps count of line number in file
    }
    end: //goto here
    //store hash pw in table to compare later
        if(found == 1)
        {
            for(i = 0; i < 12; i++)
            {
                tpw[i] = line[i+5];
            }
        }
    return found;
}

//compare each character in string
int compare(char* str1, char* str2, int length)
{
	int i;
	int count = 0;
	int matched = 0;

	for (i = 0; i < length; i++)
	{
	    if (*str1++ != *str2++) //str1[i] != str2[i]
        {
			break;
        }
        count++;
	}
	if(count == length)
        matched = 1;
	return matched;
}

int main(int argc, char const *argv[])
{
	char id[5] = " "; //user input id
    char pw[13] = " "; //user input pw
    char npw[13] = " "; //user input new pw
    char cpw[13] = " "; //the whole converted pw
    char cpw1[5] = " "; //converted b1
    char cpw2[5] = " "; //converted b2
    char cpw3[5] = " "; //converted b3
    char hpw[13] = " "; //whole hashed pw
    char hpw1[5] = " "; //hashed block 1
    char hpw2[5] = " "; //hashed block 2
    char hpw3[5] = " "; //hashed block 3
    int a,b,c;
    int x, y, z;
    int tries = 0;

    //ask id
    printf("Enter User ID: ");
	fgets(id, 5, stdin);
	fgetc(stdin); //consume the newline character
	FILE *fp = fopen("hashtable.txt", "r+"); //open for reading and writing, startsat the beginning
    //find id in hashtable
    if (find(fp, id, index))
    {
		printf("\nUser ID found!\n");
		//lock account condition
        while (tries != 3) {
            printf("\nEnter password: ");
            fgets(pw, 13, stdin);
            fgetc(stdin); //consume the newline character
            convert(pw,cpw); //convert pw to upper case
            //split into 3 blocks
            for(a = 0; a < 4; a++)
            {
                cpw1[a] = cpw[a];
            }
            for(b = 0; b < 4; b++)
            {
                cpw2[b] = cpw[b+4];
            }
            for(c = 0; c < 4; c++)
            {
                cpw3[c] = cpw[c+8];
            }
            //hash pw for each block
            E(cpw1,hpw1);
            E(cpw2,hpw2);
            E(cpw3,hpw3);
            //combine all hashed blocks into hpw[12]
            for(x = 0; x < 4; x++)
            {
                hpw[x] = hpw1[x];
            }
            for(y = 0; y < 4; y++)
            {
                hpw[y+4] = hpw2[y];
            }
            for(z = 0; z < 4; z++)
            {
                hpw[z+8] = hpw3[z];
            }
            //compare password
            if(compare(hpw,tpw,12))
            {
                printf("\nPassword Matched!\n");
                printf("\nEnter new password: ");
                fgets(npw, 13, stdin);
                fgetc(stdin); //consume the newline character
                //change new pw into all upper case
                convert(npw,cpw);
                //separate converted pw into 3 blocks
                for(a = 0; a < 4; a++)
                {
                    cpw1[a] = cpw[a];
                }
                for(b = 0; b < 4; b++)
                {
                    cpw2[b] = cpw[b+4];
                }
                for(c = 0; c < 4; c++)
                {
                    cpw3[c] = cpw[c+8];
                }
                //hash new pw for each block
                E(cpw1,hpw1);
                E(cpw2,hpw2);
                E(cpw3,hpw3);
                //combine all hashed blocks into hpw[12]
                for(x = 0; x < 4; x++)
                {
                    hpw[x] = hpw1[x];
                }
                for(y = 0; y < 4; y++)
                {
                    hpw[y+4] = hpw2[y];
                }
                for(z = 0; z < 4; z++)
                {
                    hpw[z+8] = hpw3[z];
                }
                //replace old pw in hashtable, index of line is stored
                fclose(fp); //close file and reopen for replace
                char ofile[] = "hashtable.txt";
                FILE *input = fopen(ofile, "r");
                FILE *output = fopen("clone.txt", "w");
                char buffer[512];
                while (fgets(buffer, sizeof(buffer), input) != NULL)
                {
                    char *pos = strstr(buffer, tpw);
                    if (pos != NULL)
                    {
                        //allocate memory for temporary buffer
                        char *temp = calloc(strlen(buffer) - strlen(tpw) + strlen(hpw) + 1, 1);
                        //copy the text before the text to replace
                        memcpy(temp, buffer, pos - buffer);
                        //copy in the replacement text
                        memcpy(temp + (pos - buffer), hpw, strlen(hpw));
                        //copy the remaining text from after the replace text
                        memcpy(temp + (pos - buffer) + strlen(hpw), pos + strlen(tpw), 1 + strlen(buffer) - ((pos - buffer) + strlen(tpw)));
                        fputs(temp, output);
                        free(temp);
                    }
                    else
                        fputs(buffer, output);
                }
                fclose(output);
                fclose(input);
                //copy content from clone.txt to hashtable.txt
                FILE *fp1, *fp2;
                char stuff;
                fp1 = fopen("clone.txt", "r");
                fp2 = fopen(ofile, "w");
                while (1)
                {
                    stuff = fgetc(fp1);
                    if (stuff == EOF)
                        break;
                    else
                        putc(stuff, fp2);
                }
                fclose(fp1);
                fclose(fp2);
                break;
            }
            else
            {
                printf("\nPassword Incorrect!\n");
                tries++;
            }
        }// while
    }
	else
    {
		printf("\nUser ID not found!\n");
        //enter new password
        printf("\nEnter a password: ");
        fgets(npw, 13, stdin);
        fgetc(stdin); //consume the newline character
        //convert pw into upper case
        convert(npw,cpw);
        //separate into 3 blocks
        for(a = 0; a < 4; a++)
        {
            cpw1[a] = cpw[a];
        }
        for(b = 0; b < 4; b++)
        {
            cpw2[b] = cpw[b+4];
        }
        for(c = 0; c < 4; c++)
        {
            cpw3[c] = cpw[c+8];
        }
        //hash pw for each block
        E(cpw1,hpw1);
        E(cpw2,hpw2);
        E(cpw3,hpw3);
        //combine hashed pw blocks into hpw[12];
        for(x = 0; x < 4; x++)
        {
            hpw[x] = hpw1[x];
        }
        for(y = 0; y < 4; y++)
        {
            hpw[y+4] = hpw2[y];
        }
        for(z = 0; z < 4; z++)
        {
            hpw[z+8] = hpw3[z];
        }
        //add user id into hashtable, //add pw into hashtable
        fclose(fp); //close file and reopen for append
        fp = fopen("hashtable.txt", "a+");
        fprintf(fp, "\n%s %s", id, hpw);
    }
    if(tries == 3)
    {
        printf("\nToo many unsuccessful attempt - your account is locked!\n");
    }
	fclose(fp);

    printf("Press enter to continue...\n");
	getchar();
	return 0;
}

