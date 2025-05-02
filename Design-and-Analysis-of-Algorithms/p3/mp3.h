#ifndef MP3_H
#define MP3_H

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <string.h>
#include <bitset>
#include <algorithm>

using namespace std;

typedef struct node {
	char symbol;
	int freq;
	struct node *left;
	struct node *right;
} Node;

extern vector<string> hufCode;

Node *buildHuftree(char **argv);
void genHufCode(Node* root, std::string code);
void incoding(char **argv);
Node* rebuildHufftree(vector<string>& hufCode);
void convertToAscii(ofstream outputFile, string& binaryText, Node* root);
void decoding(char **argv);

#endif