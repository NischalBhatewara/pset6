/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

#define ALPHABETS 27

// Trie node
typedef struct trieNode {
	bool isLeaf;
	struct trieNode *child[ALPHABETS];
} trieNode;

// Prototypes
trieNode *getNode(void);
void insertInTrie(char*);
int getRelativePos(char);
bool inTrie(const char*);
bool deleteNode(trieNode*);

// Global variables
trieNode *rootNode;
size_t dictionarySize = 0;
int mc = 0;

/*
 * Returns true if word is in dictionary else false.
 */

bool
check(const char *word)
{
	return inTrie(word);
}


/*
 * Loads dictionary into memory.  Returns true if successful else false.
 */

bool
load(const char *dictionary)
{
	char *word = NULL;
	size_t len = 0;

	FILE *fp = fopen(dictionary, "r");

	if (fp == NULL) return false;

	rootNode = getNode();

	while (getline(&word, &len, fp) != -1) {
		if (word[strlen(word) - 1] == '\n') word[strlen(word) - 1] = '\0'; // remove new line character
		insertInTrie(word);
	}

	free(word);	
	fclose(fp);

	return true;
}


/*
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */

unsigned int
size(void)
{
	return dictionarySize;
}


/*
 * Unloads dictionary from memory.  Returns true if successful else false.
 */

bool
unload(void)
{
	return deleteNode(rootNode);
}

/*
 * Get new node
 */
trieNode *getNode() {
	trieNode *T = (trieNode *) malloc(sizeof(trieNode));
	T -> isLeaf = false;
	for (int i = 0; i < ALPHABETS; ++i)
		T -> child[i] = NULL;
	return T;
}

/*
 * returns the pos relative to 'a'
 */
int getRelativePos(char ch) {
	return (ch == '\'') ? 27 : ch % 'a';
}

void insertInTrie(char *word) {
	trieNode *temp, *node = rootNode;
	for (int i = 0; i < strlen(word); ++i)
	{
		int pos = getRelativePos(word[i]);
		if (node -> child[pos] == NULL) {
			temp = getNode();
			node -> child[pos] = temp;
			node = temp;
		} else {
			node = node -> child[pos];
		}
	}
	node -> isLeaf = true;
	++dictionarySize;
}

bool inTrie(const char *word) {
	trieNode *node = rootNode;
	for (int i = 0; i < strlen(word); ++i)
	{
		int pos = getRelativePos(word[i]);
		if (node -> child[pos] == NULL)
			return false;
		node = node -> child[pos];
	}
	if (node -> isLeaf) return true;
	else return false;
}

bool deleteNode(trieNode *node) {
	if (node == NULL) return true;
	for (int i = 0; i < ALPHABETS; ++i)
		if (node -> child[i] != NULL) deleteNode
			(node -> child[i]);
	free(node);
	return true;
}
