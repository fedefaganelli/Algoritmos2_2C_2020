/*
 * Body.cpp
 *
 *  Created on: Nov 10, 2020
 *      Author: fedef
 */
#include "Body.h"

Body::Body()
{
	Txn_count = 0;
}

Body::Body(int txn_count)
{
	Txn_count = txn_count;
}



