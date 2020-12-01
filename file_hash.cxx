#include <experimental/map>
#include <iostream>
#include <fstream>
#include <crypto++/filters.h>
#include <crypto++/hex.h>
#include <crypto++/crc.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>
#include <crypto++/sha.h>
#include "file_hash.h"

using std::cerr, std::cout, std::endl, std::begin, std::cbegin, std::end, std::cend, std::string;

HashDiggest calculateFileHash(const Path& path, HashFunctionPtr hash_ptr) noexcept {
    HashDiggest digest;
    if( hash_ptr ) {
        hash_ptr->Restart();
        
        digest.resize( hash_ptr->DigestSize() );
        CryptoPP::HashFilter filter(hash_ptr->Ref(), new CryptoPP::ArraySink(digest.data(), digest.size()));//ArraySink and HashFilter constructors are noexcept
        
        std::ifstream input(path, std::ios::binary);
        for( uint8_t buffer[8192]; input.good(); input.read((char*)buffer, sizeof(buffer)) )
            if( input.gcount() )
                filter.Put(buffer, input.gcount());
        
        if( input.eof() )
            filter.MessageEnd();
        else
            digest.clear();
    }
    return digest;
}

string hashToString(const HashDiggest& digest) noexcept {
    string output;
    if(digest.size()) {
        CryptoPP::HexEncoder encoder( new CryptoPP::StringSink( output ) );
        encoder.Put( digest.data(), digest.size() );
        encoder.MessageEnd();
    }
    return output;
}

struct HashCreatorBase {
    virtual CryptoPP::HashFunction* create() const = 0;
};
template <class T>
struct HashCreator: public HashCreatorBase { 
    CryptoPP::HashFunction* create() const {return static_cast<CryptoPP::HashFunction*>(new T);}
};
typedef std::shared_ptr<HashCreatorBase> HashCreatorPtr;

HashFunctionPtr createHashFunction(const string& name) {
    static std::map< string, HashCreatorPtr > creators {
        { "CRC32" , HashCreatorPtr(new HashCreator<CryptoPP::CRC32>)     },
        { "CRC32C", HashCreatorPtr(new HashCreator<CryptoPP::CRC32C>)    },
        { "MD5"   , HashCreatorPtr(new HashCreator<CryptoPP::Weak::MD5>) },
        { "SHA1"  , HashCreatorPtr(new HashCreator<CryptoPP::SHA1>)      },
        { "SHA256", HashCreatorPtr(new HashCreator<CryptoPP::SHA256>)    },
        { "SHA224", HashCreatorPtr(new HashCreator<CryptoPP::SHA224>)    },
        { "SHA512", HashCreatorPtr(new HashCreator<CryptoPP::SHA512>)    },
        { "SHA384", HashCreatorPtr(new HashCreator<CryptoPP::SHA384>)    },
    };
    auto it = creators.find( utils::to_upper(name) );
    return HashFunctionPtr( it != end(creators) ? it->second->create() : nullptr );
}

/*
vi:set tabstop=8 softtabstop=4 shiftwidth=4 noexpandtab nowrap:
*/
