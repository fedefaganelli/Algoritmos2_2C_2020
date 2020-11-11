#pragma once
#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include "Input.h"
#include "Output.h"
#include <list>

class Transaction
{
public:
	Transaction(); //default constructor
	//Transaction(int); //custom constructor

	int N_tx_in;
	std::list<Input> Inputs;
	int N_tx_out;
	std::list<Output> Outputs;
};

#endif // !TRANSACTION_H_



