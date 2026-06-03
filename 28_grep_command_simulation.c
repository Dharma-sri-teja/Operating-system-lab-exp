#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LINES 1000
#define MAX_LINE_LENGTH 1024
typedef struct {
    char line[MAX_LINE_LENGTH];
    int lineNumber;
} FileLine;
FileLine fileContent[MAX_LINES];
int lineCount = 0;
void loadFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", filename);
        return;
    }
    lineCount = 0;
    while (fgets(fileContent[lineCount].line, MAX_LINE_LENGTH, file) != NULL && lineCount < MAX_LINES) {
        fileContent[lineCount].lineNumber = lineCount + 1;
        lineCount++;
    }
    fclose();
    printf("Loaded %d lines from file.\n\n", lineCount);
}
void grepBasic(const char *pattern) {
    printf("===== grep '%s' =====\n", pattern);
    int matchCount = 0;
    for (int i = 0; i < lineCount; i++) {
        if (strstr(fileContent[i].line, pattern) != NULL) {
            printf("%d: %s", fileContent[i].lineNumber, fileContent[i].line);
            matchCount++;
        }
    }
    printf("===== Found %d matches =====\n\n", matchCount);
}
void grepIgnoreCase(const char *pattern) {
    printf("===== grep -i '%s' (Case Insensitive) =====\n", pattern);
    int matchCount = 0;
    char lowerLine[MAX_LINE_LENGTH];
    char lowerPattern[256];
    strcpy(lowerPattern, pattern);
    for (int i = 0; lowerPattern[i]; i++) {
        lowerPattern[i] = tolower((unsigned char)lowerPattern[i]);
    }
    for (int i = 0; i < lineCount; i++) {
        strcpy(lowerLine, fileContent[i].line);
        for (int j = 0; lowerLine[j]; j++) {
            lowerLine[j] = tolower((unsigned char)lowerLine[j]);
        }
        if (strstr(lowerLine, lowerPattern) != NULL) {
            printf("%d: %s", fileContent[i].lineNumber, fileContent[i].line);
            matchCount++;
        }
    }
    printf("===== Found %d matches =====\n\n", matchCount);
}
void grepInvert(const char *pattern) {
    printf("===== grep -v '%s' (Invert Match) =====\n", pattern);
    int matchCount = 0;
    for (int i = 0; i < lineCount; i++) {
        if (strstr(fileContent[i].line, pattern) == NULL) {
            printf("%d: %s", fileContent[i].lineNumber, fileContent[i].line);
            matchCount++;
        }
    }
    printf("===== Found %d non-matching lines =====\n\n", matchCount);
}
void grepCount(const char *pattern) {
    printf("===== grep -c '%s' (Count) =====\n", pattern);
    int matchCount = 0;
    for (int i = 0; i < lineCount; i++) {
        if (strstr(fileContent[i].line, pattern) != NULL) {
            matchCount++;
        }
    }
    printf("%d\n\n", matchCount);
}
void grepLineNumbers(const char *pattern) {
    printf("===== grep -n '%s' (With Line Numbers) =====\n", pattern);
    int matchCount = 0;
    for (int i = 0; i < lineCount; i++) {
        if (strstr(fileContent[i].line, pattern) != NULL) {
            printf("%d: %s", fileContent[i].lineNumber, fileContent[i].line);
            matchCount++;
        }
    }
    printf("===== Found %d matches =====\n\n", matchCount);
}
void grepWholeWord(const char *pattern) {
    printf("===== grep -w '%s' (Whole Word) =====\n", pattern);
    int matchCount = 0;
    for (int i = 0; i < lineCount; i++) {
        char *pos = fileContent[i].line;
        while ((pos = strstr(pos, pattern)) != NULL) {
            int beforeOk = (pos == fileContent[i].line) || !isalnum((unsigned char)*(pos - 1));
            int afterOk = (pos[strlen(pattern)] == '\0') || !isalnum((unsigned char)pos[strlen(pattern)]);
            if (beforeOk && afterOk) {
                printf("%d: %s", fileContent[i].lineNumber, fileContent[i].line);
                matchCount++;
                break;
            }
            pos++;
        }
    }
    printf("===== Found %d matches =====\n\n", matchCount);
}
int main() {
    printf("======= Simulated grep UNIX Command =====\n\n");
    FILE *sampleFile = fopen("sample.txt", "w");
    if (sampleFile) {
        fprintf(sampleFile, "The quick brown fox jumps over the lazy dog\n");
        fprintf(sampleFile, "A fox is a cunning animal\n");
        fprintf(sampleFile, "Dogs and foxes are different\n");
        fprintf(sampleFile, "The FOX is known for its intelligence\n");
        fprintf(sampleFile, "This line has no match\n");
        fprintf(sampleFile, "fox in lowercase\n");
        fprintf(sampleFile, "prefixed word foxes\n");
        fclose(sampleFile);
    }
    loadFile("sample.txt");
    grepBasic("fox");
    grepIgnoreCase("fox");
    grepInvert("fox");
    grepCount("fox");
    grepLineNumbers("fox");
    grepWholeWord("fox");
    return 0;
}
