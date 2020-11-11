#include "Transaction.h"

Transaction::Transaction()
{
	N_tx_in = 0;
	std::list<Input> Inputs;
	N_tx_out = 0;
	std::list<Output> Outputs;

}

//Transaction::Transaction(int txn_count)
//{
//	Txn_count = txn_count;
//}