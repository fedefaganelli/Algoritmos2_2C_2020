//============================================================================
// Name        : TrabajoPractico0.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Block.h"


using namespace std;

int main() {

	cout << "Declaraciones.. " << endl;


	Header h1("str1","str2",1,5);
	Body body1(45);

	Block block1;

	block1.body = body1;
	block1.header = h1;


	cout << "Valor de body.Txn_count " << block1.body.Txn_count << endl;
	cout << "Valor de header.Prev_block " << block1.header.Prev_block << endl;






	return 0;
}