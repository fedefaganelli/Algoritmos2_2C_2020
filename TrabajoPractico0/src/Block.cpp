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
	for (i = 0; hash[i] == '0'; i++)
		d = d + 4;

	if (hash[i] == '1')
		d = d + 3;

	if (hash[i] == '2' || hash[i] == '3')
		d = d + 2;

	if (hash[i] == '4' || hash[i] == '5' || hash[i] == '6' || hash[i] == '7')
		d = d + 1;

	/*otro caso d = d + 0*/
	return d >= dificulty ? true : false;
}

//std::string& Header::txns_hash(std::string& txns, int txn_count) /*  generar campo hash*/
//{
//	for (size_t i = 0; i < txn_count; i++)
//		txns << tnx(txns[i]);
//
//		return sha256(sha256(txns));
//}
