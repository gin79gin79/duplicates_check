#ifndef __FILE_HASH_H__
#define __FILE_HASH_H__
#include <crypto++/cryptlib.h>
#include "common_definitions.h"

using CryptoPP::HashFunction;
typedef std::shared_ptr<HashFunction> HashFunctionPtr;

HashDiggest calculateFileHash(const Path&, HashFunctionPtr) noexcept;
std::string hashToString(const HashDiggest&) noexcept;

HashFunctionPtr createHashFunction(const std::string&);

#endif//__FILE_HASH_H__

/*
vi:set tabstop=8 softtabstop=4 shiftwidth=4 noexpandtab nowrap:
*/
