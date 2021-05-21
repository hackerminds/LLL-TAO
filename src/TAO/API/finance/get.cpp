/*__________________________________________________________________________________________

            (c) Hash(BEGIN(Satoshi[2010]), END(Sunny[2012])) == Videlicet[2014] ++

            (c) Copyright The Nexus Developers 2014 - 2019

            Distributed under the MIT software license, see the accompanying
            file COPYING or http://www.opensource.org/licenses/mit-license.php.

            "ad vocem populi" - To the Voice of the People

____________________________________________________________________________________________*/

#include <LLD/include/global.h>

#include <TAO/API/finance/types/finance.h>

#include <TAO/API/include/build.h>
#include <TAO/API/include/check.h>
#include <TAO/API/include/extract.h>
#include <TAO/API/include/json.h>

/* Global TAO namespace. */
namespace TAO::API
{
    /* Get the data from a digital asset */
    json::json Finance::Get(const json::json& jParams, bool fHelp)
    {
        /* Get the Register address. */
        const TAO::Register::Address hashRegister = ExtractAddress(jParams);

        /* Get the token / account object. */
        TAO::Register::Object objThis;
        if(!LLD::Register->ReadObject(hashRegister, objThis, TAO::Ledger::FLAGS::LOOKUP))
            throw APIException(-13, "Object not found");

        /* Check the object standard. */
        if(objThis.Base() != TAO::Register::OBJECTS::ACCOUNT)
            throw APIException(-15, "Object is not an account or token");

        /* Now lets check our expected types match. */
        CheckType(jParams, objThis);

        /* Build our response object. */
        json::json jRet  = ObjectToJSON(jParams, objThis, hashRegister);
        jRet["owner"]    = TAO::Register::Address(objThis.hashOwner).ToString();
        jRet["created"]  = objThis.nCreated;
        jRet["modified"] = objThis.nModified;

        /* Filter out our expected fieldnames if specified. */
        FilterResponse(jParams, jRet);

        return jRet;
    }
}
