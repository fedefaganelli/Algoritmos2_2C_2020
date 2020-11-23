#pragma once
#ifndef PRINTSERVICE_H_
#define PRINTSERVICE_H_

#include "Block.h"

void print_to_stdout(Transaction&);
void print_to_stdout(Block& b);
int print_to_file(Transaction&, const string);
int print_to_file(Block& b, const string fileName);
string print_transaction(Transaction& t);

#endif /* PRINTSERVICE_H_ */
