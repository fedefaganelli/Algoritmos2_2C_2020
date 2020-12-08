// Tp0Cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include<ctime>

#include "cmdline.h"
#include "sha256.h"
#include "lista.h"
#include "Block.h"

#define MSG_ERR_OPEN_FILE "Error al abrir el archivo "
#define MSG_ERR_DIFFICULTY_MISSING "Error: No se encontro el parametro --difficulty (-d)"
#define PRE_BLOCK_INIT "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
#define HASH_NULL "0000000000000000000000000000000000000000000000000000000000000000"
#define MAX_RAND 100000
#define MAX_SPLIT_NUMBER 20

using namespace std;

static void opt_input(string const& arg);
static void opt_output(string const& arg);

bool is_numeric(const std::string&);
bool is_float(string);
string* parse_line(string, string*, int);
int mapCommand(string);
bool validateCommand(int,string*);
lista<Block> process_user_input(void);
string print_transaction(Transaction&);
void print_to_stdout(Block&);
void print_to_stdout(Transaction&);
string get_transactions_hash(lista<Transaction>);
string get_header_hash(Header&);
string get_body_hash(Body&);

static string input;
static string output;
static option_t options[] = {
	{1, "i", "input", "-", opt_input, OPT_DEFAULT},
	{1, "o", "output", "-", opt_output, OPT_DEFAULT},
	{0, }
};


int main(int argc, char* argv[])
{
	//recibo el CLA
	cmdline cmdl(options);

	//parseo
	cmdl.parse(argc, argv);


	if (input.empty())
	{
		lista<Block> bloques = process_user_input();
	}
	else
	{
		//si viene, checkeo que sea un archivo .txt (va a venir la transaccion ya hecha)
		cout << "Invocacion con archivo no implementada" << endl;
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

lista<Block> process_user_input(void)
{
	string line = "";
	lista<Block> bloques;

	srand(time(0));
	while(true)
	{
		getline(cin, line);

		if(line == "quit")
			break;
		//parseo
		string temp[MAX_SPLIT_NUMBER];

		string* splitLine = parse_line(line, temp, MAX_SPLIT_NUMBER);

		int command = mapCommand(splitLine[0]);

		switch(command)
		{
		case 0:
		{
			if(!validateCommand(0,splitLine))
				break;

			float initValue = atof(splitLine[2].c_str());
			int bits = stoi(splitLine[3]);

			Input initInput;
			Output initOutput;

			initInput.Addr = sha256(sha256(splitLine[1]));
			//outpoint
			initInput.Idx = 0;
			initInput.Tx_id = "";

			initOutput.Addr = initInput.Addr;
			initOutput.Value = initValue;

			Transaction initTransaction;

			initTransaction.N_tx_in = 1;
			initTransaction.Inputs.enqueue(initInput);
			initTransaction.N_tx_out = 1;
			initTransaction.Outputs.enqueue(initOutput);

			Body initBody;

			initBody.Txn_count = 1;
			initBody.Transactions.enqueue(initTransaction);

			Header initHeader;

			initHeader.Prev_block = HASH_NULL;
			initHeader.Bits = bits;
			initHeader.Txns_hash = get_transactions_hash(initBody.Transactions);

			Block initBlock;
			srand(time(0));
			bool satisfiedDif = false;
			string hHash;

			do
			{
				initHeader.Nonce = rand();
				hHash = get_header_hash(initHeader);
				satisfiedDif = initBlock.verify_dificulty(hHash, bits);
			}while(!satisfiedDif);

			string bdHash = get_body_hash(initBody);

			initBlock.header = initHeader;
			initBlock.body = initBody;

			string bHash = sha256(sha256(hHash + bdHash));
			cout << bHash << endl;

		}
			break;

		default:
			cout << "Comando <" << splitLine[0] << "> no reconocido" << endl;

			break;
		}

	}

	return lista<Block>();
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

int mapCommand(string command)
{
	if(command == "init")
		return 0;

	/*if(command == "transfer")
			return 1;*/

	return -1;
}

bool validateCommand(int command,string* arguments)
{
	switch(command)
	{
	case 0:
		//buscar usuario (para saber el hash de la direccion)
		//en primer instancia, los usuarios se van creando en el moento, y su direccion es doble sha256(userName)
		/*if(arguments[1] != "pepe")
		{
			cout << "arguments[1] =  "<< arguments[1] << endl;
			return false;
		}*/
		//validar float <value>
		if(!is_float(arguments[2]))
		{
			cout << "value no es float: "<< arguments[2] << endl;
			return false;
		}
		//validar int > 0
		if(!is_numeric(arguments[3]))
		{
			cout << "bits no es int positivo: "<< arguments[3] << endl;
			return false;
		}
		break;

	default:
		cout << "Ocurrio un error validando comando " << command << endl;
		return false;
	}

	return true;

}

bool is_numeric(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

bool is_float(string str)
{
	istringstream iss(str);
	float f;
	iss >> noskipws >> f;
	return iss.eof() && !iss.fail();
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

void print_to_stdout(Block& b)
{
	cout << b.header.Prev_block << endl << b.header.Txns_hash << endl << b.header.Bits << endl << b.header.Nonce << endl;
	cout << b.body.Txn_count << endl;

	if(b.body.Transactions.Tamano() == 0)
		return;
	lista<Transaction>::iterador it;
	for (it = b.body.Transactions.primero(); it != b.body.Transactions.ultimo(); it.avanzar()) {
		print_to_stdout(it.dato());
	}
	print_to_stdout(it.dato());

}

void print_to_stdout(Transaction& t)
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

string get_header_hash(Header& h)
{
	string header;
	string bitsStr = to_string(h.Bits);
	string nonceStr = to_string(h.Nonce);
	header = h.Prev_block + '\n' + h.Txns_hash + '\n' + bitsStr + '\n' + nonceStr + '\n';
	return sha256(sha256(header));
}

string get_body_hash(Body& b)
{
	string body;
	string tTotal = to_string(b.Txn_count);
	string tHash = get_transactions_hash(b.Transactions);
	body = tTotal + '\n' + tHash + '\n';
	return sha256(sha256(body));
}

