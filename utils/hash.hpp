#ifndef hash_hpp
#define hash_hpp

#include <openssl/md5.h>
#include <iostream>
#include <string>
#include <iomanip> 

using namespace std;



string md5Hash(string input){
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), digest);

    stringstream ss;
    for(int i =0; i<MD5_DIGEST_LENGTH; i++){
        ss << hex << setw(2) << setfill('0') << static_cast<int>(digest[i]);
    }

    return ss.str();
}

#endif hash_hpp