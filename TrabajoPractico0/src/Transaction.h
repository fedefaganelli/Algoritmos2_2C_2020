#pragma once
#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include "Input.h"
#include "Output.h"
#include "lista.h"

class Transaction
{
public:
	Transaction(); //default constructor
	//Transaction(int); //custom constructor

	int N_tx_in;
	lista<Input> Inputs;
	int N_tx_out;
	lista<Output> Outputs;
};

#endif // !TRANSACTION_H_



