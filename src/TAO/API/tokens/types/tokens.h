/*__________________________________________________________________________________________

            (c) Hash(BEGIN(Satoshi[2010]), END(Sunny[2012])) == Videlicet[2014] ++

            (c) Copyright The Nexus Developers 2014 - 2019

            Distributed under the MIT software license, see the accompanying
            file COPYING or http://www.opensource.org/licenses/mit-license.php.

            "ad vocem populi" - To the Voice of the People

____________________________________________________________________________________________*/

#pragma once
#ifndef NEXUS_TAO_API_INCLUDE_TOKENS_H
#define NEXUS_TAO_API_INCLUDE_TOKENS_H

#include <TAO/API/types/base.h>

/* Global TAO namespace. */
namespace TAO
{

    /* API Layer namespace. */
    namespace API
    {

        /** Assets
         *
         *  Assets API Class.
         *  Manages the function pointers for all Asset commands.
         *
         **/
        class Tokens : public Base
        {
        public:

            /** Default Constructor. **/
            Tokens()
            : Base()
            {
                Initialize();
            }


            /** Initialize.
             *
             *  Sets the function pointers for this API.
             *
             **/
            void Initialize() final;


            /** RewriteURL
             *
             *  Allows derived API's to handle custom/dynamic URL's where the strMethod does not
             *  map directly to a function in the target API.  Insted this method can be overriden to
             *  parse the incoming URL and route to a different/generic method handler,
             *
             *  @param[in] strMethod The name of the method being invoked.
             *  @param[out] jParams The json array of parameters to be modified and passed back.
             *
             *  @return the modified API method URL as a string
             *
             **/
            std::string RewriteURL(const std::string& strMethod, json::json &jParams) final;


            /** GetName
             *
             *  Returns the name of this API.
             *
             **/
            std::string GetName() const final
            {
                return "Tokens";
            }


            /** Get
             *
             *  Get an account or token object register
             *
             *  @param[in] jParams The parameters from the API call.
             *  @param[in] fHelp Trigger for help data.
             *
             *  @return The return object in JSON.
             *
             **/
            json::json Get(const json::json& jParams, bool fHelp);


            /** Debit
             *
             *  Debit tokens from an account or token object register
             *
             *  @param[in] jParams The parameters from the API call.
             *  @param[in] fHelp Trigger for help data.
             *
             *  @return The return object in JSON.
             *
             **/
            json::json Debit(const json::json& jParams, bool fHelp);



            /** ListAccounts
             *
             *  List all token accounts
             *
             *  @param[in] jParams The parameters from the API call.
             *  @param[in] fHelp Trigger for help data.
             *
             *  @return The return object in JSON.
             *
             **/
            json::json ListAccounts(const json::json& jParams, bool fHelp);


            /** ListTransactions
             *
             *  Lists all transactions for a given token or account
             *
             *  @param[in] jParams The parameters from the API call.
             *  @param[in] fHelp Trigger for help data.
             *
             *  @return The return object in JSON.
             *
             **/
            json::json ListTransactions(const json::json& jParams, bool fHelp);


            /** ListTokenAccounts
             *
             *  Lists all accounts that have been created for a particular token
             *
             *  @param[in] jParams The parameters from the API call.
             *  @param[in] fHelp Trigger for help data.
             *
             *  @return The return object in JSON.
             *
             **/
            json::json ListTokenAccounts(const json::json& jParams, bool fHelp);

        };
    }
}

#endif
