/*__________________________________________________________________________________________

        (c) Hash(BEGIN(Satoshi[2010]), END(Sunny[2012])) == Videlicet[2014] ++

        (c) Copyright The Nexus Developers 2014 - 2021

        Distributed under the MIT software license, see the accompanying
        file COPYING or http://www.opensource.org/licenses/mit-license.php.

        "ad vocem populi" - To the Voice of the People

____________________________________________________________________________________________*/

#include <LLC/hash/SK.h>

#include <LLD/include/global.h>

#include <LLP/include/global.h>

#include <TAO/Register/include/names.h>
#include <TAO/Register/types/address.h>

#include <Util/include/debug.h>

#include <vector>


/* Global TAO namespace. */
namespace TAO
{

    /* Register Layer namespace. */
    namespace Register
    {
        /* Retrieve the name register for a namespace/name combination. */
        bool GetNameRegister(const uint256_t& hashNamespace, const std::string& strName, Object& rName)
        {
            /* Get the register address for the Name object */
            Address hashAddress = Address(strName, hashNamespace, Address::NAME);

            /* Read the Name Object */
            if(!LLD::Register->ReadObject(hashAddress, rName, TAO::Ledger::FLAGS::LOOKUP))
                return false; /* Don't log an error if it is not in the DB as the caller might have provided an invalid name */

            /* Check that the name object is proper type. */
            if(rName.nType != TAO::Register::REGISTER::OBJECT)
                return debug::error(FUNCTION, "Name register not an object: ", strName);

            /* Check that this is a Name register */
            if(rName.Standard() != TAO::Register::OBJECTS::NAME)
                return debug::error(FUNCTION, "Register is not a name register: ", strName);

            return true;
        }


        /* Retrieve the namespace register by namespace name. */
        bool GetNamespaceRegister(const std::string& strNamespace, Object &rNamespace)
        {
            /* Namespace hash is a SK256 hash of the namespace name */
            const uint256_t hashAddress = Address(strNamespace, Address::NAMESPACE);

            /* Read the Name Object */
            if(!LLD::Register->ReadState(hashAddress, rNamespace, TAO::Ledger::FLAGS::LOOKUP))
                return debug::error(FUNCTION, "Namespace register not found: ", strNamespace);

            /* Check that the name object is proper type. */
            if(rNamespace.nType != TAO::Register::REGISTER::OBJECT)
                return debug::error(FUNCTION, "Namespace register not an object: ", strNamespace);

            /* Parse the object. */
            if(!rNamespace.Parse())
                return debug::error(FUNCTION, "Unable to parse namespace register: ", strNamespace);

            /* Check that this is a Name register */
            if(rNamespace.Standard() != TAO::Register::OBJECTS::NAMESPACE)
                return debug::error(FUNCTION, "Register is not a namespace register: ", strNamespace);

            return true;
        }
    }
}
