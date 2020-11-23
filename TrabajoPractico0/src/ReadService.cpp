#include "ReadService.h"
#include <fstream>
#include <sstream>

#define NUMBER_INPUT_ARGUMENTS 3
#define NUMBER_OUTPUT_ARGUMENTS 2

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

lista<Transaction> read_transactions_file(string& fileName)
{

	ifstream inFile;
	inFile.open(fileName.c_str());
	if(!inFile.is_open()) {
		string errorMessage = "Error abriendo archivo " + fileName;
		perror(errorMessage.c_str());
		exit(EXIT_FAILURE);
	}
	string line;
	lista<Transaction> transactions;

	while(true)
	{
		getline(inFile, line);
		if (inFile.eof()) break;

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

	}

	inFile.close();
	return transactions;
}
