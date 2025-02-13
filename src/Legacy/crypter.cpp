/*__________________________________________________________________________________________

			(c) Hash(BEGIN(Satoshi[2010]), END(Sunny[2012])) == Videlicet[2014] ++

			(c) Copyright The Nexus Developers 2014 - 2021

			Distributed under the MIT software license, see the accompanying
			file COPYING or http://www.opensource.org/licenses/mit-license.php.

			"ad vocem populi" - To the Voice of the People

____________________________________________________________________________________________*/

#ifdef WIN32

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600    //targeting minimum Windows Vista version for winsock2, etc.
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1  //prevents windows.h from including winsock.h and messing up winsock2.h definitions we use
#endif

#ifndef NOMINMAX
#define NOMINMAX //prevents windows.h from including min/max and potentially interfering with std::min/std::max
#endif

#include <windows.h>
#endif

#include <Legacy/wallet/crypter.h>

#include <openssl/aes.h>
#include <openssl/evp.h>

#include <algorithm>
#include <string>
#include <vector>


namespace Legacy
{

    /* Default constructor */
    Crypter::Crypter()
    : fKeySet(false)
    {
    }
    

    /* Destructor */
    Crypter::~Crypter()
    {
        CleanKey();
    }


    /* Assign new encryption key context (chKey and chIV). */
    bool Crypter::SetKey(const CKeyingMaterial& chNewKey, const std::vector<uint8_t>& chNewIV)
    {
        if(chNewKey.size() != WALLET_CRYPTO_KEY_SIZE || chNewIV.size() != WALLET_CRYPTO_KEY_SIZE)
            return false;

        /* Try to keep the keydata out of swap
         * Note that this does nothing about suspend-to-disk (which will put all our key data on disk)
         * Note as well that at no point in this program is any attempt made to prevent stealing of keys
         * by reading the memory of the running process.
         */
        mlock(&chKey[0], sizeof(chKey));
        mlock(&chIV[0], sizeof(chIV));

        //memcpy(&chKey[0], &chNewKey[0], sizeof(chKey));
        //memcpy(&chIV[0], &chNewIV[0], sizeof(chIV));

        std::copy((uint8_t *)&chNewKey[0], (uint8_t *)&chNewKey[0] + sizeof(chKey), &chKey[0]);
        std::copy(&chNewIV[0],  &chNewIV[0]  + sizeof(chIV),  &chIV[0]);

        fKeySet = true;
        return true;
    }


    /* Generate new encryption key context (chKey and chIV) from a passphrase. */
    bool Crypter::SetKeyFromPassphrase(const SecureString& strKeyData, const std::vector<uint8_t>& chSalt, const uint32_t nRounds, const uint32_t nDerivationMethod)
    {
        if(nRounds < 1 || chSalt.size() != WALLET_CRYPTO_SALT_SIZE)
            return false;

        /* Try to keep the keydata out of swap
         * Note that this does nothing about suspend-to-disk (which will put all our key data on disk)
         * Note as well that at no point in this program is any attempt made to prevent stealing of keys
         * by reading the memory of the running process.
         */
        mlock(&chKey[0], sizeof chKey);
        mlock(&chIV[0], sizeof chIV);

        int i = 0;
        if(nDerivationMethod == 0)
            i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha512(), &chSalt[0],
                            (uint8_t *)&strKeyData[0], strKeyData.size(), nRounds, chKey, chIV);

        if(i != (int) 32)
        {
            memset(&chKey, 0, sizeof chKey);
            memset(&chIV, 0, sizeof chIV);
            return false;
        }

        fKeySet = true;
        return true;
    }


    /* Clear the current encryption key from memory. */
    void Crypter::CleanKey()
    {
        memset(&chKey, 0, sizeof chKey);
        memset(&chIV, 0, sizeof chIV);

        munlock(&chKey, sizeof chKey);
        munlock(&chIV, sizeof chIV);

        fKeySet = false;
    }


    /* Encrypt a plain text value using the current encryption key settings. */
    bool Crypter::Encrypt(const CKeyingMaterial& vchPlaintext, std::vector<uint8_t>& vchCiphertext)
    {
        if(!IsKeySet())
            return false;

        /* max ciphertext len for a n bytes of plaintext is (n + AES_BLOCK_SIZE - 1) bytes */
        int nLen = vchPlaintext.size();
        int nCLen = nLen + AES_BLOCK_SIZE, nFLen = 0;
        vchCiphertext = std::vector<uint8_t> (nCLen);

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if(nullptr == ctx)
            return false;

        bool fOk = true;

        EVP_CIPHER_CTX_init(ctx);
        if(fOk) fOk = EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, chKey, chIV);
        if(fOk) fOk = EVP_EncryptUpdate(ctx, &vchCiphertext[0], &nCLen, &vchPlaintext[0], nLen);
        if(fOk) fOk = EVP_EncryptFinal_ex(ctx, (&vchCiphertext[0])+nCLen, &nFLen);
        EVP_CIPHER_CTX_free(ctx);

        if(!fOk) return false;

        vchCiphertext.resize(nCLen + nFLen);
        return true;
    }


    /* Decrypt an encrypted text value using the current encryption key settings. */
    bool Crypter::Decrypt(const std::vector<uint8_t>& vchCiphertext, CKeyingMaterial& vchPlaintext)
    {
        if(!IsKeySet())
            return false;

        /* Plain text will always be equal to or lesser than length of ciphertext */
        int nLen = vchCiphertext.size();
        int nPLen = nLen, nFLen = 0;

        vchPlaintext = CKeyingMaterial(nPLen);

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if(nullptr == ctx)
            return false;

        bool fOk = true;

        EVP_CIPHER_CTX_init(ctx);
        if(fOk) fOk = EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, chKey, chIV);
        if(fOk) fOk = EVP_DecryptUpdate(ctx, &vchPlaintext[0], &nPLen, &vchCiphertext[0], nLen);
        if(fOk) fOk = EVP_DecryptFinal_ex(ctx, (&vchPlaintext[0])+nPLen, &nFLen);
        EVP_CIPHER_CTX_free(ctx);

        if(!fOk) return false;

        vchPlaintext.resize(nPLen + nFLen);
        return true;
    }


    /* Function to encrypt a private key using a master key and IV pair. */
    bool EncryptSecret(const CKeyingMaterial& vMasterKey, const LLC::CSecret& vchPlaintext, const uint576_t& nIV, std::vector<uint8_t>& vchCiphertext)
    {
        Crypter cKeyCrypter;

        std::vector<uint8_t> chIV(WALLET_CRYPTO_KEY_SIZE);
        //memcpy(&chIV[0], &nIV, WALLET_CRYPTO_KEY_SIZE);
        std::copy((uint8_t *)&nIV, (uint8_t *)&nIV + WALLET_CRYPTO_KEY_SIZE, &chIV[0]);

        if(!cKeyCrypter.SetKey(vMasterKey, chIV))
            return false;

        /* This works on the assumption that CKeyingMaterial and CSecret are both std::vector<uint8_t, secure_allocator<uint8_t> > */
        /* Can remove this assumption (and the old fashioned C-style cast) by copying CSecret into a CKeyMaterial variable */
        return cKeyCrypter.Encrypt((CKeyingMaterial)vchPlaintext, vchCiphertext);
    }


    /* Function to encrypt a private key using a master key and IV pair. */
    bool DecryptSecret(const CKeyingMaterial& vMasterKey, const std::vector<uint8_t>& vchCiphertext, const uint576_t& nIV, LLC::CSecret& vchPlaintext)
    {
        Crypter cKeyCrypter;

        std::vector<uint8_t> chIV(WALLET_CRYPTO_KEY_SIZE);
        //memcpy(&chIV[0], &nIV, WALLET_CRYPTO_KEY_SIZE);
        std::copy((uint8_t *)&nIV, (uint8_t *)&nIV + WALLET_CRYPTO_KEY_SIZE, &chIV[0]);

        if(!cKeyCrypter.SetKey(vMasterKey, chIV))
            return false;

        /* This works on the assumption that CKeyingMaterial and CSecret are both std::vector<uint8_t, secure_allocator<uint8_t> > */
        /* Can remove this assumption (and the old fashioned C-style cast) by copying CSecret into a CKeyMaterial variable */
        return cKeyCrypter.Decrypt(vchCiphertext, *((CKeyingMaterial*)&vchPlaintext));
    }

}
