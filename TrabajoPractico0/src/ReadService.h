#pragma once
#ifndef READSERVICE_H_
#define READSERVICE_H_

#include "Transaction.h"

bool is_numeric(const std::string& s);
bool is_float(string str);
bool hasEnding(std::string const& fullString, std::string const& ending);
string* parse_line(string line, string* splitLine, int length);

int get_inputs(int, Transaction&);
int get_outputs(int, Transaction&);

lista<Transaction> read_transactions_stdin(int totalTransactions);
lista<Transaction> read_transactions_file(string& fileName);

#endif /* READSERVICE_H_ */
