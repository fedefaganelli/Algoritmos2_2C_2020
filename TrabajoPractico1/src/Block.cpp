#include "Block.h"
#include "sha256.h"

Block::Block()
{
	header = Header();
	body = Body();
}

bool Block::verify_dificulty(string hash, size_t dificulty/*campo bits*/)
{
	size_t i, d = 0;

		if(dificulty == 0)
		    return true;

		for(i = 0; d < dificulty; i++)
		{
		    if(hash[i] != '0')
		        return false;

		    d = d + 4;
		}

		return true;
}

//std::string& Header::txns_hash(std::string& txns, int txn_count) /*  generar campo hash*/
//{
//	for (size_t i = 0; i < txn_count; i++)
//		txns << tnx(txns[i]);
//
//		return sha256(sha256(txns));
//}
