#include "PrintService.h"
#include <fstream>

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

int print_to_file(Block& b, const string fileName)
{
	ofstream file;
	file.open(fileName, ios_base::app);

	file << b.header.Prev_block << endl << b.header.Txns_hash << endl << b.header.Bits << endl << b.header.Nonce << endl;
	file << b.body.Txn_count << endl;

	file.close();

	if(b.body.Transactions.Tamano() == 0)
		return 0;

	lista<Transaction>::iterador it;
	for (it = b.body.Transactions.primero(); it != b.body.Transactions.ultimo(); it.avanzar()) {
		print_to_file(it.dato(), fileName);
	}
	print_to_file(it.dato(),fileName);


	return 0;
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
