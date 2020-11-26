#include "Header.h"

Header::Header()
{
	Prev_block = "";
	Txns_hash = "";
	Bits = 0;
	Nonce = 0;
}

Header::Header(std::string prev_block, std::string txns_hash, int bits, int nonce)
{
	Prev_block = prev_block;
	Txns_hash = txns_hash;
	Bits = bits;
	if (nonce < 0)
	{
		Nonce = 0;
	}
	else
	{
		Nonce = nonce;
	}
}

std::string Header::getHeader()
{
	std::string bitsStr = std::to_string(Bits);
	std::string nonceStr = std::to_string(Nonce);
	return Prev_block + '\n' + Txns_hash + '\n' + bitsStr + '\n' + nonceStr + '\n';
}

