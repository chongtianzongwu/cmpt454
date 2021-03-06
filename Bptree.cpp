//
//  Bptree.cpp
//  Bptree
//
//	Authors: Tyler Wong(301207988), Colin Qiao(301191776)
//  Created by Tyler Wong on 2015-07-24.
//  Copyright (c) 2015 Tyler Wong. All rights reserved.
//

#include <iostream>
#include <stdio.h>
//#include "Node.h"
#include "Bptree.h"
using namespace std;

#ifdef _Bptree_

Bptree::Bptree(int keySize) {
    this->keySize = keySize;
    root = NULL;

    if((keySize + 1) % 2 == 0) {
        interiorReq = leafReq = keySize;
    } else {
        interiorReq = ((keySize + 1) / 2) + 1;
        leafReq = (keySize + 1) / 2;
    }
}

Node* Bptree::getLeaf(int key, Node* nd) {
    int nextNdIndex = 0;
    // this is a leaf node
    if (nd->nodePointers == NULL) {
        return nd;
	} else {
        for(int i=0; i<nd->currentSize; i++) {
            if(key < nd->keyArray[i]) {
                break;
            }
            nextNdIndex++;
        }
        return (getLeaf(key, nd->nodePointers[nextNdIndex]));
    }
}

void Bptree::insert(int key, string value) {
    if(root == NULL) {
        root = new Node(keySize);
    }
    Node* targetLeaf = getLeaf(key, root);
	insertToLeaf(key, value, targetLeaf);
}

void Bptree::insertToLeaf(int key, string value, Node* leaf) {
		if (leaf->valuePointers == NULL) {
			leaf->valuePointers = new string[keySize];
		}
		if (!leaf->isFull()) {
			int insertIndex = 0;
			// find the correct position to be inserted
			for (;insertIndex < leaf->currentSize; insertIndex++) {
				if (key < leaf->keyArray[insertIndex]) {
					break;
				}
			}
			if (insertIndex < leaf->currentSize) {
				leaf->shuffleUp(insertIndex);
			}
			leaf->keyArray[insertIndex] = key;
			leaf->valuePointers[insertIndex] = value;
			leaf->currentSize++;
		} else {
			split(key, value, leaf, NULL);
		}
}

void Bptree::split(int key, string value, Node* nd, Node* child) {
	Node* sibling = new Node(keySize);
	Node* keyNd = makeSortedKeysNd(key, value, nd, child);
	int leftOver = -1;
	
	if (nd->isLeaf()) {
		int splitSz = keyNd->currentSize - leafReq;
		
		for (int i=0; i<splitSz; i++) {
			nd->keyArray[i] = keyNd->keyArray[i];
			nd->valuePointers[i] = keyNd->valuePointers[i];
		}
		
		nd->currentSize = splitSz;
		sibling->valuePointers = new string[keySize];
		
		for (int i=0; i<leafReq; i++) {
			sibling->keyArray[i] = keyNd->keyArray[splitSz+i];
			sibling->valuePointers[i] = keyNd->valuePointers[splitSz+i];
		}
		sibling->currentSize = leafReq;		
		leftOver = sibling->keyArray[0];
	} else {
		int splitPointerSz = 0;
		int splitKeySz = 0;
		if (sibling->nodePointers == NULL) {
			sibling->nodePointers = new Node*[keySize+1];
		}
		if ((keySize + 2) % 2 == 0) {
			splitPointerSz = (keySize + 2) / 2;
		} else {
			splitPointerSz = (keySize + 2) / 2 + 1;
		}
		if (keySize % 2 == 0) {
			splitKeySz = keySize / 2;
		} else {
			splitKeySz = keySize / 2 + 1;
		}
		
		// distribute pointers between nd and its sibling 
		for (int i=0; i<splitPointerSz; i++) {
			nd->nodePointers[i] = keyNd->nodePointers[i];

 		}
		for (int i=0; i<(keyNd->currentSize+1)-splitPointerSz; i++) {
			sibling->nodePointers[i] = keyNd->nodePointers[i+splitPointerSz];
			sibling->nodePointers[i]->parent = sibling;
		}
		
		// distribute keys between nd and its sibling
		for (int i=0; i<splitKeySz; i++) {
			nd->keyArray[i] = keyNd->keyArray[i];
		}
		for (int i=0; i<keySize/2; i++) {
			sibling->keyArray[i] = keyNd->keyArray[i+splitKeySz+1]; // '+1' is because we need to leave the middle element out
		}
		nd->currentSize = splitKeySz;
		sibling->currentSize = keySize/2;
		leftOver = keyNd->keyArray[splitKeySz];
		cout << "\nleft over is: " << leftOver << endl;
	}
	delete keyNd;
	

	nd->next = sibling;
    sibling->previous = nd;
	if (nd->parent == NULL) {
		root = new Node(keySize);
		root->nodePointers = new Node*[keySize+1];
		root->nodePointers[0] = nd;
		root->nodePointers[1] = sibling;
		root->keyArray[0] = leftOver;
		root->currentSize++;
		nd->parent = root;
		sibling->parent = root;
	} else {
		sibling->parent = nd->parent;
        //nd->parent->currentSize++;

		insertToInterior(leftOver, nd->parent, sibling);
	}
}

void Bptree::insertToInterior(int key, Node* nd, Node* child) {
	if (nd->nodePointers == NULL) {
		nd->nodePointers = new Node*[keySize+1];
	}
	if (!nd->isFull()) {
		int i = 0;
		for (; i<nd->currentSize; i++) {
			if (key < nd->keyArray[i]) {
				break;
			}
		}
		if (i < nd->currentSize) {
			nd->shuffleUp(i);
			nd->shuffleUpNodePointers(i);
			nd->nodePointers[i] = child;
		} else {
			nd->nodePointers[i+1] = child;
		}
		nd->keyArray[i] = key;
		nd->currentSize++;
	} else {
		split(key, "", nd, child);
	}
}

// 
Node* Bptree::makeSortedKeysNd(int key, string value, Node* nd, Node* child) {
	int totalSize = keySize + 1;
	Node* keyNd = new Node(totalSize);
	keyNd->keyArray = mergeToSortedArray(key, nd->keyArray);
	int oldValuesInserted = 0;
	
	if (nd->isLeaf()) {
		keyNd->valuePointers = new string[totalSize];
		for (int i=0; i<keySize; i++) {
			if (keyNd->keyArray[i] == key) {
				keyNd->valuePointers[i] = value;
			} else {
				keyNd->valuePointers[i] = nd->valuePointers[oldValuesInserted];
				oldValuesInserted++;
			}
		}
		if (oldValuesInserted < keySize) {
			keyNd->valuePointers[keySize] = nd->valuePointers[keySize-1];
		} else {
			keyNd->valuePointers[keySize] = value;
		}
	} else {
		keyNd->nodePointers = new Node*[totalSize+1];
		bool isChildInserted = false;
		for (int i=0; i<keySize; i++) {
			if (keyNd->keyArray[i] == key) {
				keyNd->nodePointers[i] = child;
				isChildInserted = true;
			} else {
				keyNd->nodePointers[i] = nd->nodePointers[oldValuesInserted];
				oldValuesInserted++;
			}
		}
		if (!isChildInserted) {
			keyNd->nodePointers[keySize] = nd->nodePointers[oldValuesInserted];
			keyNd->nodePointers[keySize+1] = child;
		} else {
			while (oldValuesInserted <= keySize) {
				int i = 0;
				keyNd->nodePointers[keySize+i] = nd->nodePointers[oldValuesInserted];
				i++;
				oldValuesInserted++;
			} 
		}
	}
	keyNd->currentSize = totalSize;
	
	return keyNd;
}

int* Bptree::mergeToSortedArray(int key, int* oldArr) {
	int oldArrIndex = 0;
	int newArrIndex = 0;
	int keyIndex = 0;
	int* newArr = new int[keySize + 1];
	while(oldArrIndex < keySize && keyIndex < 1) {
		if(key < oldArr[oldArrIndex]) {
			newArr[newArrIndex] = key;
			keyIndex++;
		} else {
			newArr[newArrIndex] = oldArr[oldArrIndex];
			oldArrIndex++;
		}
		newArrIndex++;
	}
	while(oldArrIndex < keySize) {
		newArr[newArrIndex] = oldArr[oldArrIndex];
		oldArrIndex++;
		newArrIndex++;
	}
	if (keyIndex < 1) {
		newArr[newArrIndex] = key;
		keyIndex++;
		newArrIndex++;
	}
 	
	return newArr;
}

//a recursive function that will find the leaf node
Node* Bptree::findnode(int key, Node* node){
    int nextNdIndex=0;
    if (node->nodePointers == NULL){
        return node;
    }
    else {
        for(int i=0; i<node->currentSize; i++) {
            //for (int i=nd->currentSize;i>=0; i--){
            if(key < node->keyArray[i]) {
                break;
            }
            nextNdIndex++;
        }
        
        return(findnode(key, node->nodePointers[nextNdIndex]));

    }
}


string Bptree::find(int key){
    
    string returnvalue = "Node Not Found\n";
    if(root == NULL) {
        return "";
    }
    else{
        Node * node=findnode(key,root);
        for (int i=0; i<node->currentSize; i++){
            if (node->keyArray[i]==key){
                return (node->valuePointers[i]);
            }
        }
        return "";
    }
}


void Bptree::remove(int key){
    bool foundneighbour = false;
    if(root == NULL) {
        cout<<"Tree is empty";
    }
    else{
        Node* node = findnode(key, root);
        for (int i=0; i<node->currentSize; i++){
            if (node->keyArray[i]==key){
                printf("Found Node, Key is : %d, now deleting", key);
                node->shuffleDown(i);
                node->currentSize--;
            }
        }
        //chech if node has enough entries. if not, need to merge.
        if ((node->currentSize<leafReq)){
            //check for a sibling, not a cousin
            
            if (node->previous != NULL){
                if (node->previous->parent == node->parent && foundneighbour == false){
                    //then previous is a sibling and we need to coalese
                    if (node->previous->currentSize>leafReq){
                        foundneighbour = true;
                        node->shuffleUp(0);
                        node->valuePointers[0]=node->previous->valuePointers[node->previous->currentSize-1];
                        node->keyArray[0] = node->previous->keyArray[node->previous->currentSize-1];
                        node->previous->currentSize--;
                        node->currentSize++;
                        for (int i=0; i<node->parent->currentSize; i++){
                            if (node->parent->keyArray[i] > node ->keyArray[0]){
                                // if (node->parent->keyArray[i] ==removedkey){
                                
                                
                                node->parent->keyArray[i]=node->keyArray[0];
                                break;
                                
                                //node->parent->keyArray[i]=node->previous->keyArray[node->previous->currentSize-1];
                                //do something
                            }
                        }
                    }
                }
            }

            if (node->next!= NULL){
                if(node->next->parent == node->parent && foundneighbour == false ){
                    //then next is a sibling, and we can try to coalese
                    //check currentsize of sibling node, if that node has too little, we cannot coalsee
                    if(node->next->currentSize>leafReq){
                        foundneighbour = true;
                        node->valuePointers[node->currentSize] = node ->next->valuePointers[0];
                        node->keyArray[node->currentSize] = node ->next->keyArray[0];
                        node->next->shuffleDown(0);
                        node->next->currentSize--;
                        node->currentSize++;
                    
                    
                        for (int i=node->parent->currentSize-1; i>=0; i--){
                            if (node->parent->keyArray[i]<node->next->keyArray[0]){
                                node->parent->keyArray[i] = node->next->keyArray[0];
                                break;
                            }
                        }
                    }
                }
            }
            
            //otherwise, if there are no neighbours that can coalese, that means the tree shrinks
            //mess arouond with nodepointers in interior nodes
            
            
            if (foundneighbour == false){
                //the node must merge with its neighbour
                printf("\n merging");
                if (node->previous != NULL){
                    
                    
                    
                    
                }
                else if (node->next != NULL){
                    for (int i=0; i<node->next->currentSize; i++){
                        node->keyArray[node->currentSize+i]=node->next->keyArray[i];
                        node->valuePointers[node->currentSize+i]=node->next->valuePointers[i];
                        
                        
                        
                    }
                    node->currentSize = node->currentSize+node->next->currentSize;
                    node->next = node->next->next;
                    for (int i=node->parent->currentSize-1;i>=0; i--){
                        if (node->parent->keyArray[i]<node->keyArray[0]){
                            node->parent->shuffleDown(i+1);
                        }
                        
                        
                        
                        
                    }
                    
                    
                    
                    
                }
                
                
                
                    
                
            }
            
            
        }
        
        
        
    }
    //might also have to fix the interior nodes as well, chech the first value of the array and compare it to the parent interoior nodes
}


void Bptree::printKeys(){
    cout << "\n---------------- Displaying Keys ----------------\n" << endl;
    Node* tempNd = root;
	
	printKeysRow(tempNd);
	cout << endl;
	while (tempNd != NULL && tempNd->nodePointers != NULL) {
		tempNd = tempNd->nodePointers[0];
		printKeysRow(tempNd);
		cout << endl;
	}
}

void Bptree::printKeysRow(Node* nd) {
	if (nd != NULL) {
		cout << "[";
		for(int i=0; i<nd->currentSize; i++) {
			if (i < nd->currentSize-1) {
				cout << nd->keyArray[i] << ", ";
			} else {
				cout << nd->keyArray[i];
			}
		}
		cout << "] ";
		if (nd->next != NULL) {
			printKeysRow(nd->next);
		}
	}
}

void Bptree::printValues(){
    cout << "\n---------------- Displaying Values ----------------\n" << endl;
    Node* tempNd = root;

	
	// get the left-most leaf of the tree
	while (tempNd != NULL && tempNd->nodePointers != NULL) {
		tempNd = tempNd->nodePointers[0];
	}
	
	printValuesLeaf(tempNd);
	cout << ", ";
	
	while (tempNd->next != NULL) {
		tempNd = tempNd->next;
		printValuesLeaf(tempNd);
		if (tempNd->next != NULL) {
			cout << ", ";
		}
	}
	cout << endl;
}

void Bptree::printValuesLeaf(Node* leaf) {
	if (leaf != NULL) {
		for (int i=0; i<leaf->currentSize; i++) {
			if (i == leaf->currentSize-1) {
				cout << leaf->valuePointers[i];
			} else {
				cout << leaf->valuePointers[i] << ", ";
			}
		}
	}
}


/*
int selection(int parameters, Bptree *tree){ // this function is going to direct us to the function that we need to use, depending on what the user inputs
    int inputkey;
    string inputstring;
    //the string and inputkey are variables used to store user input
    switch (parameters){
        
        case 1:{//insert
            cout<<"please enter the key value of your new node: ";
            cin>>inputkey;
            cout<<"please enter the string value associated with the key: ";
            cin>>inputstring;
            
            //create a new record for that pair
            record newrecord = record(inputkey, inputstring);
            //cout<<newrecord.getstring() + "\n";
            
            //NOW WE NEED A METHOD THAT INSERTS IT INTO THE TREE
            
            tree->insert(inputkey, inputstring, tree);
           
            
            break;
        }
        case 2://remove
            cout<<"please enter the key value of the node: ";
            cin>>inputkey;
            tree->remove(inputkey);
            break;
        case 3://find
            cout<<"please enter the key value of the node: ";
            cin>>inputkey;
            tree->find(inputkey);
            break;
        case 4://printkeys
            tree->printkeys();
            break;
        case 5://printvalues
            break;
    }
    return 0;
}
*/


/*
int main(int argc, const char * argv[]) {
    // insert code here...
   
    int maxkeys;
    cout << "Hello, please enter the number of keys per node: \n";
    cin>>maxkeys;
    
    Bptree tree;
    int choice = 100;
    
    while (choice>0){
        cout<<"What would you like to do? \n";
        cout<<"Enter 1 for insertion \n";
        cout<<"Enter 2 for deletion \n";
        cout<<"Enter 3 to find a key \n";
        cout<<"Enter 4 to print all keys \n";
        cout<<"Enter 5 to print all values \n";
        cout<<"And Enter 0 to exit \n";
        
        cin>>choice;
        printf("you chose %d \n", choice);
        selection(choice, &tree);
        
        string test = "ads";
        cout << "the string is: " << test << endl;

        return 0;
    
}
*/

#endif
//might need an insert node into tree function?
// test commit
