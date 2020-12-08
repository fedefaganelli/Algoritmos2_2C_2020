#include "Transaction.h"

Transaction::Transaction()
{
	N_tx_in = 0;
	lista<Input> Inputs;
	N_tx_out = 0;
	lista<Output> Outputs;

}


///*imprimo las transacciones*/
//string& Transaction::tnx()
//{
//	string tnx;
//	for (size_t i = 0; i < N_tx_in; i++)
//		tnx << td_ix << " " << idx << " " << addr << '\n';
//
//	for (size_t i = 0, i < n_tx_out, i++)
//		tnx << value << " " << value << '\n';
//
//	return tnx;
//}