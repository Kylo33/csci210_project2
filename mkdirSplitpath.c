#include "types.h"
#include <strings.h>
#include <stdbool.h>

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){
    if (strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64];
    char dirName[64];

    struct NODE* parent = splitPath(pathName, baseName, dirName);

    if (parent == NULL) {
        return;
    }

    struct NODE* child = parent->childPtr;
    while (child != NULL) {
        if (strcmp(baseName, child->name) == 0) {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        child = child->siblingPtr;
    }

    struct NODE* newNode = malloc(sizeof(struct NODE));
    newNode->parentPtr = parent;
    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';
    newNode->siblingPtr = NULL;
    
    child = parent->childPtr;
    if (parent->childPtr == NULL) {
        parent->childPtr = newNode;
    } else {
        while (child->siblingPtr != NULL) {
            child = child->siblingPtr;
        }
        child->siblingPtr = newNode;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
    
    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    size_t n = strlen(pathName);
    if (n == 1 && pathName[0] == '/') {
        strcpy(dirName, "/");
        strcpy(baseName, "");
    } else {
        bool slash_found = false;
        size_t chars_before_slash = 0;

        for (size_t i = 0; i < n; i++) {
            if (pathName[i] == '/') {
                slash_found = true;
                chars_before_slash = i;
            }
        }

        if (slash_found) {
            strncpy(dirName, pathName, chars_before_slash + 1);
            dirName[chars_before_slash] = '\0';
            strcpy(baseName, pathName + chars_before_slash + 1);
        } else {
            strcpy(dirName, "");
            strcpy(baseName, pathName);
        }
    }

    struct NODE* cursor;
    if (dirName[0] == '/') {
        cursor = root;
    } else {
        cursor = cwd;
    }

    char* token = strtok(dirName, "/");
    while (token != NULL) {
        struct NODE* child = cursor->childPtr;
        while (strcmp(token, child->name) != 0) {
            child = child->siblingPtr;
            if (child == NULL) {
                printf("ERROR: directory %s does not exist\n", dirName);
                return NULL;
            }
        }
        cursor = child;
        token = strtok(NULL, "/");
    }

    return cursor;
}
