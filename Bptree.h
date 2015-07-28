//
//  bptree.h
//  bptree
//
//  Created by Tyler Wong on 2015-07-24.
//  Copyright (c) 2015 Tyler Wong. All rights reserved.
//

#ifndef _Bptree_
#define _Bptree_

#pragma once

//#include <stdio.h>
#include <iostream>
#include <string>
//#include "bptree.cpp"
using namespace std;
/*
class record{
    int key;
    string stringvalue;
    
public:
    record();
    record(int, string);//initializing a record with a key and a string.
    string getstring();
    int getkey();
};
*/

/*
class Node {
    int holder;
    int* keyarray;
    Node* parent; // pointer to a parent node
    Node* next; //pointer to the next neighboring node (null if not a leaf node)
public:
    Node ();
    Node (int,int);
    int someFunction(int);
};
*/
	class Node {
			public:
				//int holder;
				int keySize;
				int currentSize;
				int* keyArray;
				Node* parent; // pointer to a parent node
				Node* next; //pointer to the next neighboring node (null if not a leaf node)
				Node** nodePointers;
				string* valuePointers;

				Node ();
				Node (int keySize) : keySize(keySize), currentSize(0), keyArray(new int[keySize]), 
					parent(NULL), next(NULL), nodePointers(NULL),
					valuePointers(NULL) {};

				// return true if insert successfully, else returns false
				bool insert(int key, string value) {
					if (currentSize < keySize) {
						keyArray[currentSize];
						//currentSize++;
						return true;
					} else {
						return false;
					}
				}

				bool remove(int key) {
					int keyIndex = find(key);
					if (keyIndex != -1) {
						shuffleDown(keyIndex);
						currentSize--;
						return true;
					} else {
						return false;
					}
						
				}

				int find(int key) {
					for (int i=0; i<currentSize; i++) {
						if (key == keyArray[i]) {
							return i;
						}
					}
					return -1;
				}

				bool isEmpty() {
					if(currentSize == 0) {
						return true;
					} else {
						return false;
					}
				}

				bool isFull() {
					if (currentSize == keySize) {
						return true;
					} else {
						return false;
					}
				}

				void shuffleDown(int startIndex) {
					// "-1" because we should not shift beyond the last element
					for(int i=startIndex; i<currentSize-1; i++) {
						keyArray[i] = keyArray[i+1];
					}
				}
				
				void shuffleUp(int startIndex) {
					for(int i=currentSize-1; i>=startIndex; i--) {
						keyArray[i+1] = keyArray[i];
						if (valuePointers != NULL) {
							valuePointers[i+1] = valuePointers[i];
						}
					}
					currentSize++;
				}


				//Node (int,int);
				//int someFunction(int);
		};
		
class Bptree {
	//class Node; 
	
	// make this private
	private:
		/*
		class Node {
			public:
				//int holder;
				int keySize;
				int currentSize;
				int* keyArray;
				Node* parent; // pointer to a parent node
				Node* next; //pointer to the next neighboring node (null if not a leaf node)
				Node** nodePointers;
				string* valuePointers;

				Node ();
				Node (int keySize) : keySize(keySize), currentSize(0), keyArray(new int[keySize]), 
					parent(NULL), next(NULL), nodePointers(NULL),
					valuePointers(NULL) {};

				// return true if insert successfully, else returns false
				bool insert(int key, string value) {
					if (currentSize < keySize) {
						keyArray[currentSize];
						//currentSize++;
						return true;
					} else {
						return false;
					}
				}

				bool remove(int key) {
					int keyIndex = find(key);
					if (keyIndex != -1) {
						shuffleDown(keyIndex);
						currentSize--;
						return true;
					} else {
						return false;
					}
						
				}

				int find(int key) {
					for (int i=0; i<currentSize; i++) {
						if (key == keyArray[i]) {
							return i;
						}
					}
					return -1;
				}

				bool isEmpty() {
					if(currentSize == 0) {
						return true;
					} else {
						return false;
					}
				}

				bool isFull() {
					if (currentSize == keySize) {
						return true;
					} else {
						return false;
					}
				}

				void shuffleDown(int startIndex) {
					// "-1" because we should not shift beyond the last element
					for(int i=startIndex; i<currentSize-1; i++) {
						keyArray[i] = keyArray[i+1];
					}
				}
				
				void shuffleUp(int startIndex) {
					for(int i=currentSize-1; i>=startIndex; i--) {
						keyArray[i] = keyArray[i+1];
						if (valuePointers != NULL) {
							valuePointers[i] = valuePointers[i+1];
						}
					}
					currentSize++;
				}


				//Node (int,int);
				//int someFunction(int);
		};
		*/
		Node* root;
		int keySize;
		int interiorReq;
		int leafReq;

		void findAndInsert(int key, string value, Node* nd);
		Node* getNode(int key, Node* nd);

	public:
		Bptree(int keySize);
    
		//int holder;
		//Node x;
	    void insert(int key, string value);
		//int remove(int parameters);
		//string find(int parameters);
		void printKeys();
		int printvalues();

   
};

#endif
