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

char cipherKey[] = "/ !#$%&'()*+,-.0123456789;:<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
char buffer[5000], temp[5000], concatStore[5000], filePath[256];
int shift;
long fileSize;
FILE *fp;

void concatString(char string[]) {
    ((concatStore[0] == '\0')) ? strcpy(concatStore, string) : strcat(concatStore, string);
}

void setFileSize() {
    fseek(fp, 0, SEEK_END); // seek to end of file
    fileSize = ftell(fp); // get current file pointer
    fseek(fp, 0, SEEK_SET); // seek back to beginning of file
    printf("fileSize: %d\n", fileSize);
}

void setFilePath() {
    fflush(stdin);
    int len;
	printf("\nPlease, input file path: ");
    // notice stdin being passed in
    fgets(filePath, 256, stdin);
    len = strlen(filePath);
    if(filePath[len-1] == '\n') filePath[len-1] = 0;  //removes \n from end of fgets string
}

void readFile() {
    int i = 0;
    if(fp) {
        while (i < (fileSize - 1) && (!(feof(fp)))) {
             buffer[i] = fgetc(fp);
             i++;
        }
        printf("\nfile contents:\n%s\n", buffer);
    }
    else {
        printf("\nerror: %s is not a valid file path\n\n", filePath);
        int select;
        printf("Input 1 to enter valid file.\n");
        printf("Input any other key to exit.\n");
        scanf("%d", &select);
        (select == 1) ? readFile() : exit(1);
    }
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
    return cipherOutput;
}

void cipherByLine(char longString[], char tempStore[], int shift) {
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
        }
        else printf("\nerror: malloc() failed\n");

        curLine = nextLine ? (nextLine+1) : NULL;
    }
}

void cipherAction() {
    cipherByLine(buffer, temp, shift);  //encrypt
    printf("\nencrypt:\n%s\n", concatStore);
    memset(buffer, 0, sizeof(buffer)); //clear buffer
    strcpy(buffer, concatStore); //strcpy concatStore to buffer
    memset(concatStore, 0, sizeof(concatStore)); //clear buffer
    cipherByLine(buffer, temp, (0 - shift));
    printf("\ndecrypt:\n%s\n", concatStore);
}

void shiftMenu() {
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
}

int main() {
    setFilePath();
    fp = fopen(filePath, "r");
    setFileSize();
    readFile();
    fclose(fp);
    shiftMenu();
    cipherAction();

    return 0;
}
