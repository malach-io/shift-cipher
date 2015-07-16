//8. File’s shift encryption and decryption ( 1 person )
//Implementation:
//1）File encryption
//2）File decryption
//Note:
//-Read the contents(plain text) from a file (only English letter allowed) as characters
//-in terms of the KEY (input from keyboard by user ) shift the corresponding character(get cipher text)
//-do the reverse operation when decrypting the file.
//For example: Assume the plain text is “abcdef”, KEY is 5, then every single letter in the plain text will shift right by 5 positions according to the alphabet( the following letter after z is a), get the cipher text (garbled)“fghijkl”.
//Now decrypt the cipher text: fghijkl, KEY is 5, and then every single letter in the cipher text will be shift left by 5 positions according to the alphabet( the following letter after a is z), get the original text “abcdef”.
//report need a cover page and main contents should include objectives, design requirements, design architecture, flowchart, function description, debugging problems, test analysis, etc.
//report page size: A4, page number size: 10, line space: 18 pound and standard character spacing.

#include <stdio.h>
#include <stdlib.h>

char cipherKey[] = " !#$%&'()*+,-./123456789;:<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
size_t MAX_FILE_SIZE = 5000;
char buffer[5000];
char temp[5000];
char tempReverse[5000];
char concatStore[5000] = {'\0'};

void stringConcat(char* first, char* second) {
    size_t len1 = strlen(first);
    size_t len2 = strlen(second);

    char * s = malloc(len1 + len2 + 2);
    memcpy(s, first, len1);
    s[len1] = ' ';
    memcpy(s + len1 + 1, second, len2 + 1); // includes terminating null
    memset(first, '\0', sizeof(first));
    strcpy(first, s);
    printf("string concat: %s\n", s);
    free(s);
}

void concatString(char string[]) {
    ((concatStore[0] == '\0')) ? strcpy(concatStore, string) : strcat(concatStore, string);
}

void readFile() {
    fflush(stdin);
    char string[MAX_FILE_SIZE];
	int len, i = 0;
	printf("\nPlease, input file path: ");

    // notice stdin being passed in
    fgets(string, MAX_FILE_SIZE, stdin);
    len = strlen(string);
    if(string[len-1] == '\n') string[len-1] = 0;  //removes \n from end of fgets string

    FILE *fp;
    fp = fopen(string,"r");

    if(fp) {
        while (i < (MAX_FILE_SIZE - 1) && (!(feof(fp)))) {
             buffer[i] = fgetc(fp);
             i++;
        }
        //buffer[i] = '\0';  // terminating character
        printf("\nfile contents:\n%s\n", buffer);
        shiftMenu();
    }
    else {
        printf("\nerror: %s is not a valid file path\n\n", string);
        int select;
        printf("Input 1 to enter valid file.\n");
        printf("Input any other key to exit.\n");
        scanf("%d", &select);
        (select == 1) ? readFile() : exit(1);
    }

    fclose(fp);
}

void switchChar(char original[], char temp[], int i, int j, int shift) {
    if (cipherKey[i] == original[j] && !feof(stdin)) temp[j] = cipherKey[shiftEdges(i, shift)];
    else {
        i++;
        switchChar(original, temp, i, j, shift);
    }
}

int shiftEdges(int i, int shift) {
	int cipherKeyLength = strlen(cipherKey) - 1;
	int newshift = shift + i;
    if (newshift > cipherKeyLength) newshift = newshift % cipherKeyLength;
    else if (newshift < 0) newshift = cipherKeyLength + (newshift % cipherKeyLength);
	return newshift;
}

char* switchCharLoop(char original[], char temp[], int i, int j, int shift) {
    char cipherOutput[256];
    if (j < strlen(original)) {
        switchChar(original, temp, i, j, shift);
        j++;
        switchCharLoop(original, temp, i, j, shift);
    }
    strcpy(cipherOutput, temp);
    //printf("ciphOut -            : %s\n", cipherOutput);
    return cipherOutput;
}

void encryptLineByLine(char longString[], char tempStore[], int shift) {
    int oLen = strlen(longString);
    longString[oLen - 1] = 0; //removes crap from end of string
    const char * curLine = longString;
    char newTempStr[256];

    while (curLine) {
        const char * nextLine = strchr(curLine, '\n');

        int curLineLen = nextLine ? (nextLine-curLine) : strlen(curLine);
        char * tempStr = (char *) malloc(curLineLen+1);

        if (tempStr) {
            memcpy(tempStr, curLine, curLineLen);
            tempStr[curLineLen] = '\0';  //NUL-terminate!
            if(tempStr[curLineLen-1] == ' ') tempStr[curLineLen-1] = 0;  //removes \n from end of string
            strcpy(newTempStr, switchCharLoop(tempStr, tempStore, 0, 0, shift));
            newTempStr[curLineLen] = '\n';  //adds \n to end of string
            concatString(newTempStr);
            free(tempStr);

            memset(newTempStr, 0, sizeof(newTempStr)); //clear buffer
            memset(temp, 0, sizeof(temp)); //clear buffer
            memset(tempReverse,0, sizeof(tempReverse)); //clear buffer
        }
        else printf("\nerror: malloc() failed\n");

        curLine = nextLine ? (nextLine+1) : NULL;
    }
}

void shiftMenu() {
    int shift;
    printf("\ninput a shift value: \n(note: the shift value must be between 1 and 90)\n");
    scanf("%d", &shift);

    if (shift < 1 || shift > strlen(cipherKey)) {
        printf("\nerror: %d is not a valid shift value.\n", shift);
        int select;
        printf("\nInput 1 to input valid shift value.\n");
        printf("Input any other key to exit.\n");
        scanf("%d", &select);
        (select == 1) ? shiftMenu() : exit(1);
        shiftMenu();
    }
    printf("\nencrypt:\n");
    encryptLineByLine(buffer, temp, shift);  //encrypt
    printf("**concat:\n%s\n", concatStore);
    memset(buffer, 0, sizeof(buffer)); //clear buffer
    strcpy(buffer, concatStore); //strcpy concatStore to buffer
    memset(concatStore, 0, sizeof(concatStore)); //clear buffer
    printf("\ndecrypt:\n");
    encryptLineByLine(buffer, temp, (0 - shift));
    printf("**concat:\n%s\n", concatStore);
}

int main() {
    readFile();

    return 0;
}
