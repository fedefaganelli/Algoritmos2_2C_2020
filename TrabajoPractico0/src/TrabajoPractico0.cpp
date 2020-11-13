// Tp0Cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "sha256.h"
#include "cmdline.h"
#include "Block.h"
#include <list>
#include <vector>
#include<ctime>

#define MSG_ERR_OPEN_FILE "Error al abrir el archivo "
#define MSG_ERR_DIFFICULTY_MISSING "Error: No se encontró el parámetro --difficulty (-d)"
#define NUMBER_INPUT_ARGUMENTS 3
#define NUMBER_OUTPUT_ARGUMENTS 2
#define PRE_BLOCK_INIT "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
#define MAX_RAND 100000

using namespace std;

static void opt_input(string const& arg);
static void opt_output(string const& arg);
static void opt_difficulty(string const& arg);
bool is_numeric(const std::string& s);
bool is_float(string str);
bool hasEnding(std::string const& fullString, std::string const& ending);
string* parse_line(string line, string* splitLine, int length);
int get_inputs(int, Transaction&);
int get_outputs(int, Transaction&);
static void print_to_stdout(Transaction&);
static void print_to_stdout(Block& b);
int print_to_file(Transaction&, const string);
int print_to_file(Block& b, const string fileName);
lista<Transaction> read_transactions_stdin(int totalTransactions);
lista<Transaction> read_transactions_file(const string fileName);
string print_transaction(Transaction& t);
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
	//parseo
	cmdline cmdl(options);

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

	//
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
	if(transactions.Tamano() == 0)
	{
		cout << "Error: " << "No se pudieron leer las transacciones" << endl;
		return 1;
	}
	body.Transactions = transactions;
	body.Txn_count = transactions.Tamano();

	//creo el header
	Header header;
	header.Prev_block = PRE_BLOCK_INIT;
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

	cout << "Dificultad satisfecha!" << endl << "Header Hash: " << endl << hHash << endl << endl;

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

bool is_numeric(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

bool is_float(string str) {
	istringstream iss(str);
	float f;
	iss >> noskipws >> f;
	return iss.eof() && !iss.fail();
}

bool hasEnding(std::string const& fullString, std::string const& ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

string* parse_line(string line, string* splitLine, int length)
{

	int j = 0;
	stringstream ssin(line);
	while (ssin.good() && j < length) {
		ssin >> splitLine[j];
		++j;
	}

	return splitLine;
}

int get_inputs(int total, Transaction& t)
{

		Input in;
		string line;
		for (int i = 0; i < total; i++)
		{
			if (i == 0)
			{
				cin.ignore();
			}
			getline(cin, line);

			//parseo
			string temp[NUMBER_INPUT_ARGUMENTS];

			string* splitLine = parse_line(line, temp, NUMBER_INPUT_ARGUMENTS);

			in.Tx_id = splitLine[0];
			if (!is_numeric(splitLine[1]))
			{
				cout << "Error: " << "El campo idx ingresado no es numerico: " << splitLine[1] << endl;
				return 1;
			}
			int idx = stoi(splitLine[1]);
			in.Idx = idx;
			in.Addr = splitLine[2];

			t.Inputs.enqueue(in);

		}

	return 0;
}

int get_outputs(int total, Transaction& t)
{
	for (int i = 0; i < total; i++)
	{
		Output out;
		string line;

		if (i == 0)
		{
			cin.ignore();
		}
		getline(cin, line);

		//parseo
		string temp[NUMBER_OUTPUT_ARGUMENTS];

		string* splitLine = parse_line(line, temp, NUMBER_OUTPUT_ARGUMENTS);

		if (!is_float(splitLine[0]))
		{
			cout << "Error: " << "El campo value ingresado no es numerico: " << splitLine[0] << endl;
			return 1;
		}
		float value = atof(splitLine[0].c_str());

		out.Value = value;
		out.Addr = splitLine[1];

		t.Outputs.enqueue(out);

	}

	return 0;
}


static void print_to_stdout(Transaction& t)
{
	cout << t.N_tx_in << endl;

	lista<Input>::iterador itIn;
	for (itIn = t.Inputs.primero(); itIn != t.Inputs.ultimo(); itIn.avanzar()) {
		cout << itIn.dato().Tx_id << " " << itIn.dato().Idx << " " << itIn.dato().Addr << endl;
	}
	cout << itIn.dato().Tx_id << " " << itIn.dato().Idx << " " << itIn.dato().Addr << endl;

	cout << t.N_tx_out << endl;

	lista<Output>::iterador itOut;
	for (itOut = t.Outputs.primero(); itOut != t.Outputs.ultimo(); itOut.avanzar()) {
		cout << itOut.dato().Value << " " << itOut.dato().Addr << endl;
	}
	cout << itOut.dato().Value << " " << itOut.dato().Addr << endl;
}

int print_to_file(Transaction& t, const string fileName)
{
	ofstream file;
	file.open(fileName, ios_base::app);

	file << t.N_tx_in << endl;

	lista<Input>::iterador itIn;
	for (itIn = t.Inputs.primero(); itIn != t.Inputs.ultimo(); itIn.avanzar()) {
		file << itIn.dato().Tx_id << " " << itIn.dato().Idx << " " << itIn.dato().Addr << endl;
	}
	file << itIn.dato().Tx_id << " " << itIn.dato().Idx << " " << itIn.dato().Addr << endl;

	file << t.N_tx_out << endl;

	lista<Output>::iterador itOut;
	for (itOut = t.Outputs.primero(); itOut != t.Outputs.ultimo(); itOut.avanzar()) {
		file << itOut.dato().Value << " " << itOut.dato().Addr << endl;
	}
	file << itOut.dato().Value << " " << itOut.dato().Addr << endl;

	file.close();
	return 0;
}


lista<Transaction> read_transactions_stdin(int totalTransactions)
{
	string line;
	lista<Transaction> transactions;

	for(int i = 0; i < totalTransactions; i++)
	{
		cout << "Ingrese cantidad de inputs para transaccion " << i+1 << ": "<<flush;
		cin >> line;
		if (!is_numeric(line))
		{
			cout << "Error: " << "Cantidad total de inputs no es un numero: " << line << endl;
			return lista<Transaction>();
		}
		int totalInputs = stoi(line);

		Transaction t;
		t.N_tx_in = totalInputs;

		int status = get_inputs(totalInputs, t);
		if (status != 0)
			return lista<Transaction>();

		cout << "Ingrese cantidad de outputs para transaccion " << i+1 << ": "<<flush;
		cin >> line;
		if (!is_numeric(line))
		{
			cout << "Error: " << "Cantidad total de outputs no es un numero: " << line << endl;
			return lista<Transaction>();
		}
		int totalOutputs = stoi(line);

		t.N_tx_out = totalOutputs;

		status = get_outputs(totalOutputs, t);
		if (status != 0)
			return lista<Transaction>();

		//agregar la transaction al body
		transactions.enqueue(t);

	}

	return transactions;
}



lista<Transaction> read_transactions_file(const string fileName)
{
	ifstream inFile(fileName);
	string line;
	lista<Transaction> transactions;

	while(true)
	{
		getline(inFile, line);
		if (!is_numeric(line))
		{
			cout << "Error: " << "Cantidad total de inputs no es un numero: " << line << endl;
			return lista<Transaction>();
		}
		int totalInputs = stoi(line);

		Transaction t;
		t.N_tx_in = totalInputs;

		for (int j = 0; j < totalInputs; j++)
		{
			/////
			Input in;

			getline(inFile, line);

			//parseo
			string temp[NUMBER_INPUT_ARGUMENTS];

			string* splitLine = parse_line(line, temp, NUMBER_INPUT_ARGUMENTS);

			in.Tx_id = splitLine[0];
			if (!is_numeric(splitLine[1]))
			{
				cout << "Error: " << "El campo idx no es numerico: " << splitLine[1] << endl;
				return lista<Transaction>();
			}
			int idx = stoi(splitLine[1]);
			in.Idx = idx;
			in.Addr = splitLine[2];

			t.Inputs.enqueue(in);
			/////
			/*int status = get_inputs(totalInputs, t);
			if (status != 0)
				return 1;*/
			//
		}

		// leo la cant de outputs
		getline(inFile, line);
		if (!is_numeric(line))
		{
			cout << "Error: " << "Cantidad total de outputs no es un numero: " << line << endl;
			return lista<Transaction>();
		}
		int totalOutputs = stoi(line);

		t.N_tx_out = totalOutputs;

		for (int j = 0; j < totalOutputs; j++)
		{
			////
			Output out;

			getline(inFile, line);

			//parseo
			string temp[NUMBER_OUTPUT_ARGUMENTS];

			string* splitLine = parse_line(line, temp, NUMBER_OUTPUT_ARGUMENTS);

			if (!is_float(splitLine[0]))
			{
				cout << "Error: " << "El campo value no es numerico: " << splitLine[0] << endl;
				return lista<Transaction>();
			}
			float value = atof(splitLine[0].c_str());

			out.Value = value;
			out.Addr = splitLine[1];

			t.Outputs.enqueue(out);
			////
			/*int status = get_outputs(totalOutputs, t);
			if (status != 0)
				return 1;*/
		}

		//agregar la transaction al body
		transactions.enqueue(t);

		if (inFile.eof()) break;

	}

	inFile.close();
	return transactions;
}

string print_transaction(Transaction& t)
{
	string txn = to_string(t.N_tx_in) + '\n';

	lista<Input>::iterador itIn;
	for (itIn = t.Inputs.primero(); itIn != t.Inputs.ultimo(); itIn.avanzar()) {
		txn = txn + itIn.dato().Tx_id + " " + to_string(itIn.dato().Idx) + " " + itIn.dato().Addr + '\n';
	}
	txn = txn + itIn.dato().Tx_id + " " + to_string(itIn.dato().Idx) + " " + itIn.dato().Addr + '\n';

	txn = txn + to_string(t.N_tx_out) + '\n';

	lista<Output>::iterador itOut;
	for (itOut = t.Outputs.primero(); itOut != t.Outputs.ultimo(); itOut.avanzar()) {
		txn = txn + to_string(itOut.dato().Value) + " " + itOut.dato().Addr + '\n';
	}
	txn = txn + to_string(itOut.dato().Value) + " " + itOut.dato().Addr + '\n';

	return txn;
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


static void print_to_stdout(Block& b)
{
	cout << b.header.Prev_block << endl << b.header.Txns_hash << endl << b.header.Bits << endl << b.header.Nonce << endl;
	cout << b.body.Txn_count << endl;

	lista<Transaction>::iterador it;
	for (it = b.body.Transactions.primero(); it != b.body.Transactions.ultimo(); it.avanzar()) {
		print_to_stdout(it.dato());
	}
	print_to_stdout(it.dato());

}

int print_to_file(Block& b, const string fileName)
{
	ofstream file;
	file.open(fileName, ios_base::app);

	file << b.header.Prev_block << endl << b.header.Txns_hash << endl << b.header.Bits << endl << b.header.Nonce << endl;
	file << b.body.Txn_count << endl;

	file.close();
	lista<Transaction>::iterador it;
	for (it = b.body.Transactions.primero(); it != b.body.Transactions.ultimo(); it.avanzar()) {
		print_to_file(it.dato(), fileName);
	}
	print_to_file(it.dato(),fileName);


	return 0;
}

string get_header_hash(Header& h)
{
	string header;
	string bitsStr = to_string(h.Bits);
	string nonceStr = to_string(h.Nonce);
	header = h.Prev_block + '\n' + h.Txns_hash + '\n' + bitsStr + '\n' + nonceStr + '\n';
	return sha256(sha256(header));
}
