//
//  main.cpp
//  bptree
//
//  Created by Tyler Wong on 2015-07-24.
//  Copyright (c) 2015 Tyler Wong. All rights reserved.
//

#include <iostream>
#include <string>
//#include "bptree.cpp"
#include "bptree.h"
//#include "Node.h"
using namespace std; 

int main(int argc, const char * argv[]) {
	int valueSize = 4;

	Bptree* tree = new Bptree(valueSize);
	
	tree->printKeys();
    tree->remove(1);
	
	tree->insert(1, "hello");
	tree->insert(3, "world");
	tree->insert(10, "!");
	tree->printKeys();

	tree->insert(2, "pc");
	tree->printKeys();


	tree->insert(4, "welcome");
	tree->printKeys();
	tree->printValues();
	
	tree->insert(5, "yup");
	tree->insert(6, "nope");
	tree->printKeys();
	tree->printValues();
    

  	tree->insert(7, "a");
    tree->insert(8, "b");
    tree->insert(9, "c");
    tree->printKeys();
    tree->insert(11, "d");
    tree->printKeys();
    
    tree->insert(13, "f");
    tree->insert(12, "e");
    tree->printKeys();

    tree->insert(14, "g");
 	tree->insert(20, "v");
  	tree->insert(15, "h");
    tree->printKeys();

    tree->insert(16, "i");
    tree->printKeys();
    //tree->insert(9, "c");
   // tree->printKeys();
	//tree->printValues();
    /*
    tree->remove(4);
    
    tree->printKeys();
    
    
    tree->remove(5);
    
    tree->printKeys();
    
    
    tree->remove(6);
    
    tree->printKeys();
    
    tree->remove(1);
    
    tree->printKeys();
    
    string outputstring = tree->find(4);
    
    cout<<"\n" + outputstring;
    
    
	tree->insert(7, "a");
	tree->printKeys();
	tree->printValues();
    
   
   
	tree->insert(8, "b");
	tree->insert(9, "c");
    tree->insert(11, "d");
    tree->insert(12, "e");
	tree->printKeys();
	tree->printValues();
	
	
	
    
    return 0;
}
