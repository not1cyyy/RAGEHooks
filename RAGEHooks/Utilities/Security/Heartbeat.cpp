#include <pch.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/err.h>
// TODO
bool VerifyResponse(std::string response)
{
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
	return true;
}