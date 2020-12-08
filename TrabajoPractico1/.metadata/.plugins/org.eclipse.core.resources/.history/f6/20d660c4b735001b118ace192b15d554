// Tp0Cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <cstdlib>
#include<ctime>
#include "sha256.h"
#include "cmdline.h"
#include "Block.h"
#include "ReadService.h"
#include "PrintService.h"

#define MSG_ERR_OPEN_FILE "Error al abrir el archivo "
#define MSG_ERR_DIFFICULTY_MISSING "Error: No se encontro el parametro --difficulty (-d)"
/*#define NUMBER_INPUT_ARGUMENTS 3
#define NUMBER_OUTPUT_ARGUMENTS 2*/
#define PRE_BLOCK_INIT "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
#define MAX_RAND 100000

using namespace std;

static void opt_input(string const& arg);
static void opt_output(string const& arg);
static void opt_difficulty(string const& arg);
string get_transactions_hash(lista<Transaction>);
string get_header_hash(Header&);

static string input;
static string output;
static string difficulty;
static option_t options[] = {
	{1, "i", "input", "-", opt_input, OPT_DEFAULT},
	{1, "o", "output", "-", opt_output, OPT_DEFAULT},
	{1, "d", "difficulty", "-", opt_difficulty, OPT_DEFAULT},
	{0, }
};


int main(int argc, char* argv[])
{
	//recibo el CLA
	cmdline cmdl(options);

	//parseo
	cmdl.parse(argc, argv);

	if (difficulty.empty())
	{
		cout << "Error: " << "Se debe ingresar el valor de Difficulty" << endl;
		return 1;
	}
	if (!is_numeric(difficulty))
	{
		cout << "Error: " << "Difficulty no es un numero: " << difficulty << endl;
		return 1;
	}
	int dif = stoi(difficulty);

	Body body;
	lista<Transaction> transactions;
	if (input.empty())
	{
		cout << "Ingrese cantidad de transacciones: " << flush;
		cin >> input;

		if (!is_numeric(input))
		{
				cout << "Error: " << "La cantidad de transacciones ingresada no es un numero: " << input << endl;
				return 1;
		}
		int totalTransacciones = stoi(input);

		transactions = read_transactions_stdin(totalTransacciones);

	}
	else
	{
		//si viene, checkeo que sea un archivo .txt (va a venir la transaccion ya hecha)
		if (!hasEnding(input, ".txt"))
		{
			cout << "Error: " << "El archivo especificado para la entrada tiene formato incorrecto: " << input << endl;
			return 1;
		}
		//crear transtaction a partir de archivo de entrada .txt

		transactions = read_transactions_file(input);

	}

	/*if(transactions.Tamano() == 0)
	{
		cout << "Error: " << "No se pudieron leer las transacciones" << endl;
		return 1;
	}*/

	body.Transactions = transactions;
	body.Txn_count = transactions.Tamano();

	//creo el header
	Header header;
	header.Prev_block = PRE_BLOCK_INIT;

	if(transactions.Tamano() == 0)
		header.Txns_hash = sha256(sha256(""));
	else
		header.Txns_hash = get_transactions_hash(body.Transactions);

	header.Bits = dif;
	srand(time(0));
	header.Nonce = rand();

	string hHash = get_header_hash(header);

	//creo el block
	Block block;
	block.header = header;
	block.body = body;

	bool satisfiedDif = false;
	do
	{
		header.Nonce = rand();
		hHash = get_header_hash(header);
		satisfiedDif = block.verify_dificulty(hHash, dif);
	}while(!satisfiedDif);

	//cout << "Dificultad satisfecha!" << endl << "Header Hash: " << endl << hHash << endl << endl;
	cout << "Header Hash: " << endl << hHash << endl << endl;
	//imprimo el block
	if (output.empty())
	{
		print_to_stdout(block);
	}
	else
	{
		if (!hasEnding(output, ".txt"))
		{
			cout << "Error: " << "El archivo especificado para la salida tiene formato incorrecto: " << output << endl;
			return 1;
		}
		print_to_file(block, output);
	}

	return 0;
}



static void opt_input(string const& arg)
{
	if (arg == "-") {
		input.clear();
	}
	else {
		std::stringstream in(arg);
		if (!in.good()) {
			cerr << MSG_ERR_OPEN_FILE << arg << endl;
			exit(1);
		}
		in >> input;
	}
}

static void opt_output(string const& arg)
{
	if (arg == "-") {
		output.clear();
	}
	else {
		std::stringstream in(arg);
		if (!in.good()) {
			cerr << MSG_ERR_OPEN_FILE << arg << endl;
			exit(1);
		}
		in >> output;
	}
}

static void opt_difficulty(string const& arg)
{
	if (arg == "-") {
		//cin >> difficulty;
		cerr << MSG_ERR_DIFFICULTY_MISSING << endl;
		exit(1);
	}
	else {
		std::stringstream in(arg);
		if (!in.good()) {
			cerr << MSG_ERR_OPEN_FILE << arg << endl;
			exit(1);
		}
		in >> difficulty;
	}
}


string get_transactions_hash(lista<Transaction> transactions)
{
	string txns = "";
	lista<Transaction>::iterador it;
	for (it = transactions.primero(); it != transactions.ultimo(); it.avanzar()) {
		txns = txns + print_transaction(it.dato());
	}
	txns = txns + print_transaction(it.dato());

	return sha256(sha256(txns));
}

string get_header_hash(Header& h)
{
	string header;
	string bitsStr = to_string(h.Bits);
	string nonceStr = to_string(h.Nonce);
	header = h.Prev_block + '\n' + h.Txns_hash + '\n' + bitsStr + '\n' + nonceStr + '\n';
	return sha256(sha256(header));
}
