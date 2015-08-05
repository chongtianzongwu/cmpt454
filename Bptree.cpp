//
//  Bptree.cpp
//  Bptree
//
//	Authors: Tyler Wong(301207988), Colin Qiao(301191776)
//  Created by Tyler Wong on 2015-07-24.
//  Copyright (c) 2015 Tyler Wong. All rights reserved.
//

#include <iostream>
//#include "Node.h"
#include "Bptree.h"
using namespace std;

#ifdef _Bptree_
/*
record::record(){
    //default
    key=0;
    stringvalue = "nothing";
}

record::record(int key, string stringvalue){
    //proper construct
    this->key = key;
    this->stringvalue =stringvalue;
}


string record::getstring(){
    return stringvalue;
}

int record::getkey(){
    
    return key;
}
*/

/*
Node::Node()//default node constructor
{
    holder = 700;
    //implementation here
}

Node::Node(int maxkeyvalue, int y)
{
    keyarray = new int[maxkeyvalue]; // this is an array that is going to hold the key values.
    
    
    
}

int Node::someFunction(int parameters)
{
    //holder = parameters;
    printf("holder value is %d \n", holder);
    //implemenation here
    keyarray[1]=0;
    return 0;
}

*/

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
        
        /*
        bool isFound = false;
        for (int i=0; i<nd->currentSize; i++) {
            if (nd->keyArray[i] == key) {
                isFound = true;
                break;
            }
        }
        if (isFound) {
            return nd;
        } else {
            return NULL;
        }
         */
        return nd;
       
    } else {
        for(int i=0; i<nd->currentSize; i++) {
        //for (int i=nd->currentSize;i>=0; i--){
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
					leaf->shuffleUp(insertIndex);
					break;
				}
			}
			leaf->keyArray[insertIndex] = key;
			leaf->valuePointers[insertIndex] = value;
			leaf->currentSize++;
		} else {
			cout << "this node is full!" << endl;
			split(key, value, leaf, NULL);
		}
}

void Bptree::split(int key, string value, Node* nd, Node* child) {
	Node* sibling = new Node(keySize);
	Node* keyNd = makeSortedKeysNd(key, value, nd, NULL);
	
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
	} else {
		int splitPointerSz = 0;
		int splitKeySz = 0;
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
		for (int i=splitPointerSz; i<keyNd->currentSize; i++) {
			sibling->nodePointers[i] = keyNd->nodePointers[i];
		}
		
		// distribute keys between nd and its sibling
		for (int i=0; i<splitKeySz; i++) {
			nd->keyArray[i] = keyNd->keyArray[i];
		}
		nd->currentSize = splitKeySz;
		for (int i=0; i<keySize/2; i++) {
			sibling->keyArray[i] = keyNd->keyArray[i+splitKeySz+1]; // '+1' is because we need to leave the middle element out
		}
		sibling->currentSize = keySize/2;
	}
	delete keyNd;
	
	nd->next = sibling;
    sibling->previous = nd;
	if (nd->parent == NULL) {
		root = new Node(keySize);
		root->nodePointers = new Node*[keySize+1];
		root->nodePointers[0] = nd;
		root->nodePointers[1] = sibling;
		root->keyArray[0] = key;
		root->currentSize++;
		nd->parent = root;
		sibling->parent = root;
	} else {
		cout << "parent is full!" << endl;
		sibling->parent = nd->parent;
        //nd->parent->currentSize++;
		insertToInterior(key, nd->parent, sibling);
	}
}

void Bptree::insertToInterior(int key, Node* nd, Node* child) {
	if (nd->nodePointers == NULL) {
		nd->nodePointers = new Node*[keySize+1];
	}
	if (!nd->isFull()) {
		int i = 0;
        
        
        
		for (; i<nd->currentSize+1; i++) {
			if (key < nd->keyArray[i]) {
				nd->shuffleUp(i);
				break;
			}
		}
		nd->keyArray[i] = key;
		nd->nodePointers[i] = child;
		nd->currentSize++;
	} else {
		split(key, NULL, nd, child);
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
		for (int i=0; i<keySize; i++) {
			if (keyNd->keyArray[i] == key) {
				keyNd->nodePointers[i] = child;
			} else {
				keyNd->nodePointers[i] = nd->nodePointers[oldValuesInserted];
				oldValuesInserted++;
			}
		}
		if (oldValuesInserted < keySize) {
			keyNd->nodePointers[keySize] = nd->nodePointers[keySize-1];
			keyNd->nodePointers[keySize+1] = nd->nodePointers[keySize];
		} else {
			keyNd->nodePointers[keySize] = nd->nodePointers[keySize];
			keyNd->nodePointers[keySize] = child;
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
	
	for (int i=0; i<keySize+1; i++) {
		cout << "arr " << i << " is: " << newArr[i] << endl;
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
        
        node = findnode(key, node->nodePointers[nextNdIndex]);

    }
    return node;
    
}


string Bptree::find(int key){
    
    string returnvalue;
    //int nextNdIndex =0;
    if(root == NULL) {
        cout<<"Tree is empty";
    }
    
    else{
        Node * node=findnode(key,root);
        /*
        if (root->nodePointers==NULL){
            node = root;
        }
        
        else {
            for(int i=0; i<node->currentSize; i++) {
                //for (int i=nd->currentSize;i>=0; i--){
                if(key < node->keyArray[i]) {
                    break;
                }
                nextNdIndex++;
            }
            node = findnode(key, node->nodePointers[nextNdIndex]);
            
        }
*/
        
        
        for (int i=0; i<node->currentSize; i++){
            if (node->keyArray[i]==key){
                printf("Found Node, Key is : %d", key);
                printf("And the String value is : %s", node->valuePointers[i].c_str());
                returnvalue = node->valuePointers[i];
                
            }
        }
    }
    return returnvalue;
}


void Bptree::remove(int key){
    int indexpointer;

    if(root == NULL) {
        cout<<"Tree is empty";
    }
    else{
        Node* node = findnode(key, root);
        for (int i=0; i<node->currentSize; i++){
            if (node->keyArray[i]==key){
                printf("Found Node, Key is : %d, now deleting", key);
                indexpointer = i;
                node->shuffleDown(i);
                node->currentSize--;
            }
        }
        //chech if node has enough entries. if not, need to merge.
        if ((node->currentSize<leafReq)){
            //check for a sibling, not a cousin
            if(node->next->parent == node->parent ){
                //then next is a sibling, and we can try to coalese
                //check currentsize of sibling node, if that node has too little, we cannot coalsee
                if(node->next->currentSize<leafReq){
                    node->valuePointers[node->currentSize] = node ->next->valuePointers[0];
                    node->keyArray[node->currentSize] = node ->next->keyArray[0];
                    node->next->shuffleDown(0);
                    node->next->currentSize--;
                    node->currentSize++;
                }
            }
            else if (node->previous->parent == node->parent){
                //then previous is a sibling and we need to coalese

                node->shuffleUp(0);
                node->valuePointers[0]=node->previous->valuePointers[node->previous->currentSize-1];
                node->keyArray[0] = node->previous->keyArray[node->previous->currentSize-1];
                node->previous->currentSize--;
                node->currentSize++;
            }
            
            else{
                //the tree must collapse a floor, and shrink in height
                
                
                
                
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
