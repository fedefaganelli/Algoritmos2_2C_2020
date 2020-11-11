//============================================================================
// Name        : TrabajoPractico0.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "sha256.h"
#include "cmdline.h"
#include "Block.h"

#define MSG_ERR_OPEN_FILE "Error al abrir el archivo "

using namespace std;

int main(int argc, char* argv[])
{
	/*for (int i = 0; i < argc; i++)
	{
		std::cout << "Argumento " << i+1 << ": " << argv[i] << endl;
	}*/

	/*for (auto const& v : block.body.Transactions)
	{
		std::cout << "n_tx_in: " << v.n_tx_in << "\n";
		std::cout << "n_tx_out: " << v.n_tx_out << "\n";
	}*/

	string input = "Sarasa.";
	string output = sha256(input);
	cout << "input: " << input << endl;
	cout << "output: " << output << endl;

	return 0;

}

