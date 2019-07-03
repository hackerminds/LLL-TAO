/*__________________________________________________________________________________________

            (c) Hash(BEGIN(Satoshi[2010]), END(Sunny[2012])) == Videlicet[2014] ++

            (c) Copyright The Nexus Developers 2014 - 2019

            Distributed under the MIT software license, see the accompanying
            file COPYING or http://www.opensource.org/licenses/mit-license.php.

            "ad vocem populi" - To the Voice of the People

____________________________________________________________________________________________*/
#pragma once

#include <Util/include/json.h>

#include <TAO/Register/types/object.h>

/* Global TAO namespace. */
namespace TAO
{

    /* API Layer namespace. */
    namespace API
    {

        /** IsRegisterAddress
         *
         *  Determins whether a string value is a register address.
         *  This only checks to see if the value is 64 characters in length and all hex characters (i.e. can be converted to a uint256).
         *  It does not check to see whether the register address exists in the database
         *
         *  @param[in] strValueToCheck The value to check
         *
         *  @return True if the value is 64 characters in length and all hex characters (i.e. can be converted to a uint256).
         **/
        bool IsRegisterAddress(const std::string& strValueToCheck);


        /** GetDigits
         *
         *  Retrieves the number of digits that applies to amounts for this token or account object.
         *  If the object register passed in is a token account then we need to look at the token definition
         *  in order to get the digits.  The token is obtained by looking at the token_address field,
         *  which contains the register address of the issuing token
         *
         *  @param[in] object The Object Register to determine the digits for
         *
         *  @return the number of digits that apply to amounts for this token or account
         *
         **/
        uint64_t GetDigits(const TAO::Register::Object& object);


        /** GetRegistersOwnedBySigChain
         *
         *  Scans a signature chain to work out all registers that it owns
         *
         *  @param[in] hashGenesis The genesis hash of the signature chain to scan
         *  @param[out] vRegisters The list of register addresses from sigchain.
         *
         *  @return A vector of register addresses owned by the sig chain
         *
         **/
        bool ListRegisters(const uint256_t& hashGenesis, std::vector<uint256_t>& vRegisters);


        


    }
}