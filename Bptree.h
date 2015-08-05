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
				int maxKeys;
				int currentSize;
				int* keyArray;
				Node* parent; // pointer to a parent node
				Node* next; //pointer to the next neighboring node (null if not a leaf node)
                Node* previous;
				Node** nodePointers;
				string* valuePointers;

				Node ();
				Node (int keySize) : maxKeys(keySize), currentSize(0), keyArray(new int[keySize]), 
					parent(NULL), next(NULL), nodePointers(NULL),
					valuePointers(NULL) {};

				// return true if insert successfully, else returns false
				/*
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
				*/
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
					if (currentSize < maxKeys) {
						return false;
					} else {
						return true;
					}
				}
				
				bool isLeaf() {
					if (nodePointers == NULL) {
						return true;
					} else {
						return false;
					}
				}
				
				void shuffleDown(int startIndex) {
                    /*
					// "-1" because we should not shift beyond the last element
					for(int i=startIndex; i<currentSize-1; i++) {
						keyArray[i] = keyArray[i+1];
					}
                     */
				}
				
				void shuffleUp(int startIndex) {
					for(int i=currentSize-1; i>=startIndex; i--) {
						keyArray[i+1] = keyArray[i];
						if (valuePointers != NULL) {
							valuePointers[i+1] = valuePointers[i];
						}
					}
					if (nodePointers != NULL) {
						for(int i=currentSize; i>=startIndex; i--) {
							nodePointers[i+1] = nodePointers[i];
						}
					}
				}
       
        
        
        
        
        
        
		};
		
class Bptree {
	//class Node; 
	
	// make this private
	private:
		
		Node* root;
		int keySize;
		int interiorReq;
		int leafReq;

		void findAndInsert(int key, string value, Node* nd);
		Node* getLeaf(int key, Node* nd);
		void insertToLeaf(int key, string value, Node* leaf);
		void insertToInterior(int key, Node* nd, Node* child);
		Node* makeSortedKeysNd(int key, string value, Node* nd, Node* child);
		int* mergeToSortedArray(int key, int* oldArr);
		void split(int key, string value, Node* nd, Node* child);
		void printKeysRow(Node* nd);
		void printValuesLeaf(Node* leaf);
        Node* findnode(int key, Node* node);

	public:
		Bptree(int keySize);
    
		//int holder;
		//Node x;
	    void insert(int key, string value);
		//int remove(int parameters);
		//string find(int parameters);
		void printKeys();
		void printValues();
        void remove(int key);
        string find(int key);

   
};
//some changes
#endif
