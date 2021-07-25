/*__________________________________________________________________________________________

            (c) Hash(BEGIN(Satoshi[2010]), END(Sunny[2012])) == Videlicet[2014] ++

            (c) Copyright The Nexus Developers 2014 - 2019

            Distributed under the MIT software license, see the accompanying
            file COPYING or http://www.opensource.org/licenses/mit-license.php.

            "ad vocem populi" - To the Voice of the People

____________________________________________________________________________________________*/

#include <LLD/include/global.h>

#include <TAO/API/include/build.h>
#include <TAO/API/include/check.h>
#include <TAO/API/types/commands.h>

#include <TAO/API/users/types/users.h>
#include <TAO/API/types/commands/assets.h>
#include <TAO/API/names/types/names.h>

#include <TAO/Operation/include/enum.h>
#include <TAO/Operation/include/execute.h>

#include <TAO/Register/include/enum.h>
#include <TAO/Register/types/object.h>

#include <TAO/Ledger/include/create.h>
#include <TAO/Ledger/types/mempool.h>
#include <TAO/Ledger/types/sigchain.h>

#include <Util/templates/datastream.h>

#include <Util/include/convert.h>
#include <Util/include/base64.h>

/* Global TAO namespace. */
namespace TAO
{

    /* API Layer namespace. */
    namespace API
    {

        /* Update the data in an asset */
        encoding::json Assets::Update(const encoding::json& jParams, const bool fHelp)
        {
            encoding::json ret;

            /* Authenticate the users credentials */
            if(!Commands::Get<Users>()->Authenticate(jParams))
                throw Exception(-139, "Invalid credentials");

            /* Get the PIN to be used for this API call */
            SecureString strPIN = Commands::Get<Users>()->GetPin(jParams, TAO::Ledger::PinUnlock::TRANSACTIONS);

            /* Get the session to be used for this API call */
            Session& session = Commands::Get<Users>()->GetSession(jParams);

            /* Lock the signature chain. */
            LOCK(session.CREATE_MUTEX);

            /* Create the transaction. */
            TAO::Ledger::Transaction tx;
            if(!Users::CreateTransaction(session.GetAccount(), strPIN, tx))
                throw Exception(-17, "Failed to create transaction");


            /* Get the Register ID. */
            TAO::Register::Address hashRegister ;

            /* Check whether the caller has provided the asset name parameter. */
            if(jParams.find("name") != jParams.end() && !jParams["name"].get<std::string>().empty())
            {
                /* If name is provided then use this to deduce the register address */
                hashRegister = Names::ResolveAddress(jParams, jParams["name"].get<std::string>());
            }

            /* Otherwise try to find the raw base58 encoded address. */
            else if(jParams.find("address") != jParams.end())
                hashRegister.SetBase58(jParams["address"].get<std::string>());

            /* Fail if no required parameters supplied. */
            else
                throw Exception(-33, "Missing name / address");


            /* Check for format parameter. */
            std::string strFormat = "JSON"; // default to json format if no foramt is specified
            if(jParams.find("format") != jParams.end())
                strFormat = jParams["format"].get<std::string>();

            /* Get the asset from the register DB.  We can read it as an Object.
               If this fails then we try to read it as a base State type and assume it was
               created as a raw format asset */
            TAO::Register::Object asset;
            if(!LLD::Register->ReadObject(hashRegister, asset, TAO::Ledger::FLAGS::MEMPOOL))
                throw Exception(-34, "Asset not found");

            /* Check that this is an updatable object, i.e. not a raw / append obejct */
            if(asset.nType != TAO::Register::REGISTER::OBJECT) //NOTE: this is incorrect, only readonly registers cannot be updated
                throw Exception(-155, "Raw assets can not be updated");

            /* Now lets check our expected types match. */
            if(!CheckStandard(jParams, asset))
                throw Exception(-49, "Unsupported type for name/address");

            /* Declare operation stream to serialize all of the field updates*/
            TAO::Operation::Stream ssOperationStream;

            if(strFormat == "JSON")
            {
                /* Iterate through each field definition */
                for(auto it = jParams.begin(); it != jParams.end(); ++it)
                {
                    /* Skip any incoming parameters that are keywords used by this API method*/
                    if(it.key() == "pin"
                    || it.key() == "PIN"
                    || it.key() == "session"
                    || it.key() == "name"
                    || it.key() == "address"
                    || it.key() == "format")
                    {
                        continue;
                    }

                    if(it->is_string())
                    {
                        /* Get the key / value from the JSON*/
                        std::string strDataField = it.key();
                        std::string strValue = it->get<std::string>();

                        /* Check that the data field exists in the asset */
                        uint8_t nType = TAO::Register::TYPES::UNSUPPORTED;
                        if(!asset.Type(strDataField, nType))
                            throw Exception(-156, debug::safe_printstr("Field not found in asset ", strDataField));

                        if(!asset.Check(strDataField, nType, true))
                            throw Exception(-157, debug::safe_printstr("Field not mutable in asset ", strDataField));

                        /* Convert the incoming value to the correct type and write it into the asset object */
                        if(nType == TAO::Register::TYPES::UINT8_T)
                            ssOperationStream << strDataField << uint8_t(TAO::Operation::OP::TYPES::UINT8_T) << uint8_t(stol(strValue));
                        else if(nType == TAO::Register::TYPES::UINT16_T)
                            ssOperationStream << strDataField << uint8_t(TAO::Operation::OP::TYPES::UINT16_T) << uint16_t(stol(strValue));
                        else if(nType == TAO::Register::TYPES::UINT32_T)
                            ssOperationStream << strDataField << uint8_t(TAO::Operation::OP::TYPES::UINT32_T) << uint32_t(stol(strValue));
                        else if(nType == TAO::Register::TYPES::UINT64_T)
                            ssOperationStream << strDataField << uint8_t(TAO::Operation::OP::TYPES::UINT64_T) << uint64_t(stol(strValue));
                        else if(nType == TAO::Register::TYPES::UINT256_T)
                            ssOperationStream << strDataField << uint8_t(TAO::Operation::OP::TYPES::UINT256_T) << uint256_t(strValue);
                        else if(nType == TAO::Register::TYPES::UINT512_T)
                            ssOperationStream << strDataField << uint8_t(TAO::Operation::OP::TYPES::UINT512_T) << uint512_t(strValue);
                        else if(nType == TAO::Register::TYPES::UINT1024_T)
                            ssOperationStream << strDataField << uint8_t(TAO::Operation::OP::TYPES::UINT1024_T) << uint1024_t(strValue);
                        else if(nType == TAO::Register::TYPES::STRING)
                        {
                            /* Check that the incoming value is not longer than the current value */
                            size_t nMaxLength = asset.Size(strDataField);
                            if(strValue.length() > nMaxLength)
                                throw Exception(-158, debug::safe_printstr("Value longer than maximum length ", strDataField));

                            /* Ensure that the serialized value is padded out to the max length */
                            strValue.resize(nMaxLength);

                            ssOperationStream << strDataField << uint8_t(TAO::Operation::OP::TYPES::STRING) << strValue;
                        }
                        else if(nType == TAO::Register::TYPES::BYTES)
                        {
                            bool fInvalid = false;
                            std::vector<uint8_t> vchBytes = encoding::DecodeBase64(strValue.c_str(), &fInvalid);


                            if(fInvalid)
                                throw Exception(-5, "Malformed base64 encoding");

                            /* Check that the incoming value is not longer than the current value */
                            size_t nMaxLength = asset.Size(strDataField);
                            if(vchBytes.size() > nMaxLength)
                                throw Exception(-158, debug::safe_printstr("Value longer than maximum length ", strDataField));

                            /* Ensure that the serialized value is padded out to the max length */
                            vchBytes.resize(nMaxLength);

                            ssOperationStream << strDataField << uint8_t(TAO::Operation::OP::TYPES::BYTES) << vchBytes;
                        }
                    }
                    else
                    {
                        throw Exception(-159, "Values must be passed in as strings");
                    }
                }
            }

            /* Create the transaction object script. */
            tx[0] << uint8_t(TAO::Operation::OP::WRITE) << hashRegister << ssOperationStream.Bytes();

            /* Add the fee */
            AddFee(tx);

            /* Execute the operations layer. */
            if(!tx.Build())
                throw Exception(-30, "Operations failed to execute");

            /* Sign the transaction. */
            if(!tx.Sign(session.GetAccount()->Generate(tx.nSequence, strPIN)))
                throw Exception(-31, "Ledger failed to sign transaction");

            /* Execute the operations layer. */
            if(!TAO::Ledger::mempool.Accept(tx))
                throw Exception(-32, "Failed to accept");

            /* Build a JSON response object. */
            ret["txid"]  = tx.GetHash().ToString();
            ret["address"] = hashRegister.ToString();


            return ret;
        }
    }
}