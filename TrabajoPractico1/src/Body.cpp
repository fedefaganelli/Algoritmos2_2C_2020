#include "Body.h"

Body::Body()
{
	Txn_count = 0;
	lista<Transaction> Transactions;
}

Body::Body(int txn_count)
{
	Txn_count = txn_count;
}