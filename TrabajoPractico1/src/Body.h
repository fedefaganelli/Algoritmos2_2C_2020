#pragma once
#ifndef BODY_H_
#define BODY_H_

#include "Transaction.h"

class Body
{

public:
	Body();
	Body(int); //revisar

	int Txn_count;
	lista<Transaction> Transactions;
};

#endif /* BODY_H_ */


