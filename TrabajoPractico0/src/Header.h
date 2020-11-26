#pragma once
#ifndef HEADER_H_
#define HEADER_H_

#include <string>
#include <iostream>

class Header
{
public:
	Header();
	Header(std::string, std::string, int, int);
	//std::string& getHeader();
	std::string getHeader();
	

	std::string Prev_block;
	std::string Txns_hash;
	int Bits;
	int Nonce;
};

#endif /* HEADER_H_ */

