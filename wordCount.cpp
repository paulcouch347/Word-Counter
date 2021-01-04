/*
	Author: Paul Couch
	Date: 4/5/2020
	Project: CS 315 Assignment 4
*/

#include <iostream>
#include <stdlib.h>

using namespace std;

const int S = 23;

typedef struct node_s {
	string word;
	int count;
	node_s* next;
} node;

int getHashVal(string word) {
	int hashVal = 0;
	for (int i = 0; i < (int)word.length(); i++) {
		hashVal += word[i];
	}//add up ASCII values of characters
	return hashVal % S;
}//end getHashVal

void insert(node* hashTable[], string word) {
	int hashVal = getHashVal(word);
	if (hashTable[hashVal] == NULL) {
		node* newNode = (node*)malloc(sizeof(node));
		newNode->word = word;
		newNode->count = 1;
		newNode->next = NULL;
		hashTable[hashVal] = newNode;
	}//check if hash table entry is empty
	else {
		bool isFound = false;
		node* current = hashTable[hashVal];
		while (current->next != NULL && !isFound) {
			if (current->word.compare(word) == 0) {
				current->count++;
				isFound = true;
			}
			current = current->next;
		}//check if word is already in table
		if (current->word.compare(word) == 0) {
			current->count++;
			isFound = true;
		}
		else if(!isFound){
			node* newNode = (node*)malloc(sizeof(node));
			newNode->word = word;
			newNode->count = 1;
			newNode->next = NULL;
			current->next = newNode;
		}//if word isn't in table link it to the last node in this entry of table
	}
}//end insert

int getNumWords(node* hashTable[]) {
	int numWords = 0;
	for (int i = 0; i < S; i++) {
		node* current = hashTable[i];
		while (current != NULL) {
			numWords++;
			current = current->next;
		}//add up number of nodes in each entry of hashtable
	}
	return numWords;
}//end getNumWords

void transferData(node* hashTable[], node* arr[]) {
	int indx = 0;
	for (int i = 0; i < S; i++) {
		node* current = hashTable[i];
		while (current != NULL) {
			arr[indx] = current;
			indx++;
			current = current->next;
		}//transfer data from each node into array
	}
}//end transferData

int lomutoPartition(node* arr[], int left, int right) {
	node* temp;
	int indx = left - 1; 
	int pivot = arr[right]->count;// pick rightmost value as pivot
	for (int i = left; i < right; i++) {
		if (arr[i]->count == pivot && arr[i]->word.compare(arr[right]->word) < 0) {
			indx++;
			temp = arr[i];
			arr[i] = arr[indx];
			arr[indx] = temp;
		}
		if (arr[i]->count > pivot) {
			indx++;
			temp = arr[i];
			arr[i] = arr[indx];
			arr[indx] = temp;
		}//move elements greater than the pivot in front of it
	}
	temp = arr[right];
	arr[right] = arr[indx + 1];//move pivot to final location
	arr[indx + 1] = temp;
	return indx + 1;
}//end lomutoPartition

void numSort(node* arr[], int left, int right) {
	if (right > left) {
		int pivotIndex = lomutoPartition(arr, left, right);
		numSort(arr, left, pivotIndex - 1);
		numSort(arr, pivotIndex + 1, right);
	}
}//end numSort

int alphaPartition(node* arr[], int left, int right) {
	node* temp;
	int indx = left - 1;
	for (int i = left; i < right; i++) {
		if (arr[i]->word.compare(arr[right]->word) < 0) {
			indx++;
			temp = arr[i];
			arr[i] = arr[indx];
			arr[indx] = temp;
		}//move words in front of pivot based on alphabetical order
	}
	temp = arr[right];
	arr[right] = arr[indx + 1];//move pivot to final location
	arr[indx + 1] = temp;
	return indx + 1;
}//end alphaPartition

void alphaSort(node* arr[], int left, int right) {
	if (right > left) {
		int pivotIndex = alphaPartition(arr, left, right);
		alphaSort(arr, left, pivotIndex - 1);
		alphaSort(arr, pivotIndex + 1, right);
	}
}//end alphaSort

int main() {
	node* hashTable[S] = { NULL };
	string line;
	while (getline(cin, line)) {
		if (line[0] != 13) {//check if line is empty
			string word = "";
			for (char c : line) {
				if (c == ' ' || c == '	' || c == 13) {
					if (word != "")
						insert(hashTable, word);
					word = "";
				}//check if the end of a word has been reached
				else
					word += c;
			}//add character to current word
			if (word != "")
				insert(hashTable, word);
		}
	}
	int numWords = getNumWords(hashTable);
	node* sorted[numWords] = { NULL };
	transferData(hashTable, sorted);//transfer hash table data to array
	alphaSort(sorted, 0, numWords - 1);
	for (int i = 0; i < numWords; i++) {
		cout << sorted[i]->word << " " << sorted[i]->count << endl;
	}//output alphabetically sorted list
	cout << endl;
	numSort(sorted, 0, numWords - 1);
	for (int i = 0; i < numWords; i++) {
		cout << sorted[i]->word << " " << sorted[i]->count << endl;
	}//output numerically sorted list
	return 0;
}//end main()