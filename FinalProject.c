#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

// Compare function for sorting words alphabetically, case-insensitive
int compareWords(const void* a, const void* b) {
    return strcasecmp(*(const char**)a, *(const char**)b);
}

// Check if a character is a special character
int isSpecialCharacter(char ch, const char* specialCharacters) {
    return strchr(specialCharacters, ch) != NULL;
}

// Check if a word is a stopword
int isStopword(const char* word, const char** stopwords, int numStopwords) {
    for (int i = 0; i < numStopwords; i++) {
        if (strcasecmp(word, stopwords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Clean a word by removing special characters and converting to lowercase
void cleanWord(char* word, const char* specialCharacters) {
    int len = strlen(word), index = 0;
    for (int i = 0; i < len; i++) {
        if (!isSpecialCharacter(word[i], specialCharacters) && isalpha(word[i])) {
            word[index++] = tolower(word[i]);
        }
    }
    word[index] = '\0';
}

int main() {
    // Read stopwords from a file
    FILE* stopwordsFile = fopen("stopwords.txt", "r");
    char** stopwords = (char**)malloc(1000 * sizeof(char*));
    int numStopwords = 0;
    char stopword[50];
    while (fscanf(stopwordsFile, "%49s", stopword) == 1) {
        stopwords[numStopwords++] = strdup(stopword);
    }
    fclose(stopwordsFile);

    // Read special characters from a file
    FILE* specialCharactersFile = fopen("specialcharacters.txt", "r");
    char specialCharacters[100];
    fgets(specialCharacters, 100, specialCharactersFile);
    fclose(specialCharactersFile);

    // Define arrays for input and output files
    const char* inputFiles[] = {"d1.txt", "d2.txt", "d3.txt", "d4.txt"};
    const char* outputFiles[] = {"Tokenizedd1.txt", "Tokennizedd2.txt", "Tokenizedd3.txt", "Tokenizedd4.txt"};

    // Process each input file
    for (int fileIndex = 0; fileIndex < 4; fileIndex++) {
        FILE* file = fopen(inputFiles[fileIndex], "r");
        char word[100];
        int wordCount = 0;
        char* wordsArray[1000];
        int frequencies[1000] = {0}; // Array for storing word frequencies

        // Read and process each word in the file
        while (fscanf(file, "%99s", word) == 1) {
            cleanWord(word, specialCharacters);
            if (!isStopword(word, (const char**)stopwords, numStopwords) && strlen(word) > 0) {
                int found = 0;
                for (int i = 0; i < wordCount; i++) {
                    if (strcmp(wordsArray[i], word) == 0) {
                        frequencies[i]++;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    wordsArray[wordCount] = strdup(word);
                    frequencies[wordCount] = 1;
                    wordCount++;
                }
            }
        }
        fclose(file);

        // Sort the words alphabetically
        qsort(wordsArray, wordCount, sizeof(char*), compareWords);

        // Find the highest frequency among the words
        int highestFrequency = 0;
        for (int i = 0; i < wordCount; i++) {
            if (frequencies[i] > highestFrequency) {
                highestFrequency = frequencies[i];
            }
        }

        // Write words with frequencies and weights to the output file
        FILE* outputFile = fopen(outputFiles[fileIndex], "w");
        for (int i = 0; i < wordCount; i++) {
            double weight = (double)frequencies[i] / highestFrequency;
            fprintf(outputFile, "%s %d %.4f\n", wordsArray[i], frequencies[i], weight);
        }
        fclose(outputFile);
    }



    return 0;
}
