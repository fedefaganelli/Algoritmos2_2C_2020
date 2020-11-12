#pragma once
#ifndef BLOCK_H_
#define BLOCK_H_

#include "Header.h"
#include "Body.h"

class Block
{

public:

	Block(void); //default constructor
	bool verify_dificulty(string hash, size_t dificulty/*campo bits*/);
	//std::string& txns_hash(std::string& txns, int txn_count);
	//Block(); //Block constructor with Header and Body details

	Header header;
	Body body;
};

#endif /* BLOCK_H_ */

