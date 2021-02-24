/*__________________________________________________________________________________________

            (c) Hash(BEGIN(Satoshi[2010]), END(Sunny[2012])) == Videlicet[2014] ++

            (c) Copyright The Nexus Developers 2014 - 2019

            Distributed under the MIT software license, see the accompanying
            file COPYING or http://www.opensource.org/licenses/mit-license.php.

            "ad vocem populi" - To the Voice of the People

____________________________________________________________________________________________*/

#include <TAO/API/types/rpc.h>

#include <Legacy/include/money.h>
#include <Legacy/types/secret.h>
#include <Legacy/wallet/wallet.h>
#include <Legacy/wallet/walletdb.h>

#include <LLC/include/eckey.h>

#include <TAO/Ledger/include/chainstate.h>

#include <Util/encoding/include/json.h>
#include <Util/include/runtime.h>

/* Global TAO namespace. */
namespace TAO
{

    /* API Layer namespace. */
    namespace API
    {

        /* backupwallet <destination>
        *  Safely copies wallet.dat to destination, which can be a directory or a path with filename */
        json::json RPC::BackupWallet(const json::json& params, bool fHelp)
        {
            if(fHelp || params.size() != 1)
            {
                return std::string(
                    "backupwallet <destination>"
                    " - Safely copies wallet.dat to destination, which can be a directory or a path with filename.");
            }

            std::string strDest = params[0].get<std::string>();
            std::string result;
            if(Legacy::WalletDB::BackupWallet(Legacy::Wallet::GetInstance(), strDest))
                result = "Wallet backup completed";
            else
                result = "Wallet backup unsuccessful";

            return result;
        }


        /*  keypoolrefill
        *   Fills the keypool, requires wallet passphrase to be set */
        json::json RPC::KeypoolRefill(const json::json& params, bool fHelp)
        {
            if((fHelp || params.size() > 0))
                return std::string(
                    "keypoolrefill"
                    " - Fills the keypool, requires wallet passphrase to be set.");

            if(Legacy::Wallet::GetInstance().IsLocked())
                throw APIException(-13, "Error: Please enter the wallet passphrase with walletpassphrase first.");

            Legacy::Wallet::GetInstance().GetKeyPool().TopUpKeyPool(true);

            if(Legacy::Wallet::GetInstance().GetKeyPool().GetKeyPoolSize() < config::GetArg("-keypool", 100))
                throw APIException(-4, "Error refreshing keypool.");

            return "";
        }


        /* Set the default transaction fee. */
        json::json RPC::SetTxFee(const json::json& params, bool fHelp)
        {
            if (fHelp || params.size() < 1 || params.size() > 1 || Legacy::AmountToSatoshis(params[0]) < Legacy::MIN_TX_FEE)
                return std::string(
                    "settxfee <amount>\n"
                    "<amount> is a real and is rounded to 0.01 (cent)\n"
                    "Minimum and default transaction fee per KB is 1 cent");

            Legacy::TRANSACTION_FEE = Legacy::AmountToSatoshis(params[0]);
            Legacy::TRANSACTION_FEE = (Legacy::TRANSACTION_FEE / Legacy::CENT) * Legacy::CENT;  // round to cent

            return true;
        }


        /* walletpassphrase <passphrase> [timeout] [mintonly]
        *  Stores the wallet decryption key in memory for [timeout] seconds.
        *  mintonly is optional true/false allowing only block minting. timeout is ignored if mintonly is true / 1*/
        json::json RPC::WalletPassphrase(const json::json& params, bool fHelp)
        {
            if(fHelp || (Legacy::Wallet::GetInstance().IsCrypted() && (params.size() < 1 || params.size() > 3)))
                return std::string(
                    "walletpassphrase <passphrase> [timeout] [mintonly]"
                    "\n - Stores the wallet decryption key in memory for [timeout] seconds."
                    "\n - [mintonly] is optional true/false allowing only block minting."
                    "\n - [timeout] is ignored if mintonly is true / 1");

            if(!Legacy::Wallet::GetInstance().IsCrypted())
                throw APIException(-15, "Error: running with an unencrypted wallet, but walletpassphrase was called.");

            if(!Legacy::Wallet::GetInstance().IsLocked())
                throw APIException(-17, "Error: Wallet is already unlocked, use walletlock first if need to change unlock settings.");

            // Note that the walletpassphrase is stored in params[0]
            SecureString strWalletPass;
            strWalletPass.reserve(100);
            strWalletPass = params[0].get<std::string>().c_str();

            if(strWalletPass.length() > 0)
            {
                uint32_t nLockSeconds = 0;
                if(params.size() > 1)
                    nLockSeconds = (uint32_t)params[1];

                // Nexus: if user OS account compromised prevent trivial sendmoney commands
                if(params.size() > 2)
                    Legacy::fWalletUnlockMintOnly = (bool)params[2];
                else
                    Legacy::fWalletUnlockMintOnly = false;

                if(!Legacy::Wallet::GetInstance().Unlock(strWalletPass, nLockSeconds))
                    throw APIException(-14, "Error: The wallet passphrase entered was incorrect.");

                // asynchronously top up the keypool while we have the wallet unlocked.
                std::thread([]()
                {
                    Legacy::Wallet::GetInstance().GetKeyPool().TopUpKeyPool();

                }).detach();
            }
            else
                return std::string(
                    "walletpassphrase <passphrase> [timeout] [mintonly]"
                    "\n - Stores the wallet decryption key in memory for [timeout] seconds."
                    "\n - [mintonly] is optional true/false allowing only block minting."
                    "\n - [timeout] is ignored if mintonly is true / 1");


            return "";
        }

        /* walletpassphrasechange <oldpassphrase> <newpassphrase>
        *  Changes the wallet passphrase from <oldpassphrase> to <newpassphrase>*/
        json::json RPC::WalletPassphraseChange(const json::json& params, bool fHelp)
        {
            if(fHelp || (Legacy::Wallet::GetInstance().IsCrypted() && params.size() != 2))
                return std::string(
                    "walletpassphrasechange <oldpassphrase> <newpassphrase>"
                    " - Changes the wallet passphrase from <oldpassphrase> to <newpassphrase>.");

            if(!Legacy::Wallet::GetInstance().IsCrypted())
                throw APIException(-15, "Error: running with an unencrypted wallet, but walletpassphrasechange was called.");

            SecureString strOldWalletPass;
            strOldWalletPass.reserve(100);
            strOldWalletPass = params[0].get<std::string>().c_str();

            SecureString strNewWalletPass;
            strNewWalletPass.reserve(100);
            strNewWalletPass = params[1].get<std::string>().c_str();

            if(strOldWalletPass.length() < 1 || strNewWalletPass.length() < 1)
                return std::string(
                    "walletpassphrasechange <oldpassphrase> <newpassphrase>"
                    " - Changes the wallet passphrase from <oldpassphrase> to <newpassphrase>.");

            if(!Legacy::Wallet::GetInstance().ChangeWalletPassphrase(strOldWalletPass, strNewWalletPass))
                throw APIException(-14, "Error: The wallet passphrase entered was incorrect.");

            return "";
        }


        /* walletlock"
        *  Removes the wallet encryption key from memory, locking the wallet.
        *  After calling this method, you will need to call walletpassphrase again
        *  before being able to call any methods which require the wallet to be unlocked */
        json::json RPC::WalletLock(const json::json& params, bool fHelp)
        {
            if(fHelp || (Legacy::Wallet::GetInstance().IsCrypted() && params.size() != 0))
                return std::string(
                    "walletlock"
                    " - Removes the wallet encryption key from memory, locking the wallet."
                    " After calling this method, you will need to call walletpassphrase again"
                    " before being able to call any methods which require the wallet to be unlocked.");

            if(!Legacy::Wallet::GetInstance().IsCrypted())
                throw APIException(-15, "Error: running with an unencrypted wallet, but walletlock was called.");

            {
                Legacy::Wallet::GetInstance().Lock();
            }

            return "";
        }

        /* encryptwallet <passphrase>
        *  Encrypts the wallet with <passphrase>. */
        json::json RPC::EncryptWallet(const json::json& params, bool fHelp)
        {
            if(fHelp || (!Legacy::Wallet::GetInstance().IsCrypted() && params.size() != 1))
                return std::string(
                    "encryptwallet <passphrase>"
                    " - Encrypts the wallet with <passphrase>.");

            if(Legacy::Wallet::GetInstance().IsCrypted())
                throw APIException(-15, "Error: running with an encrypted wallet, but encryptwallet was called.");

            SecureString strWalletPass;
            strWalletPass.reserve(100);
            strWalletPass = params[0].get<std::string>().c_str();

            if(strWalletPass.length() < 1)
                return std::string(
                    "encryptwallet <passphrase>"
                    " - Encrypts the wallet with <passphrase>.");

            if(!Legacy::Wallet::GetInstance().EncryptWallet(strWalletPass))
                throw APIException(-16, "Error: Failed to encrypt the wallet.");

            return "wallet encrypted";
        }

        /* checkwallet
        *  Check wallet for integrity */
        json::json RPC::CheckWallet(const json::json& params, bool fHelp)
        {
            if(fHelp || params.size() > 0)
                return std::string("checkwallet - Check wallet for integrity.");

            uint32_t nMismatchSpent;
            int64_t nBalanceInQuestion;
            Legacy::Wallet::GetInstance().FixSpentCoins(nMismatchSpent, nBalanceInQuestion, true);
            json::json result;
            if(nMismatchSpent == 0)
                result["wallet check passed"] = true;
            else
            {
                result["mismatched spent coins"] =  nMismatchSpent;
                result["amount in question"] =  Legacy::SatoshisToAmount(nBalanceInQuestion);
            }
            return result;

        }

        /* listtrustkeys
        *  List all the Trust Keys this Node owns */
        json::json RPC::ListTrustKeys(const json::json& params, bool fHelp)
        {
            if(fHelp || params.size() > 0)
                return std::string("listtrustkeys - List all the Trust Keys this Node owns.");

        //     Object result;
        //     LLD::CTrustDB trustdb("cr");
        //     LLD::CIndexDB indexdb("cr");

        //     Core::CTrustKey trustKey;
        //     if(trustdb.ReadMyKey(trustKey))
        //     {

        //         /* Set the wallet address. */
        //         Legacy::NexusAddress address;
        //         address.SetPubKey(trustKey.vchPubKey);
        //         result.push_back(Pair(address.ToString(), Core::dInterestRate));
            // }


        //     return result;
            json::json ret;
            return ret;
        }

        /* repairwallet
        *  Repair wallet if checkwallet reports any problem */
        json::json RPC::RepairWallet(const json::json& params, bool fHelp)
        {
            if(fHelp || params.size() > 0)
                return std::string("repairwallet - Repair wallet if checkwallet reports any problem.");

            uint32_t nMismatchSpent;
            int64_t nBalanceInQuestion;
            Legacy::Wallet::GetInstance().FixSpentCoins(nMismatchSpent, nBalanceInQuestion);
            json::json result;
            if(nMismatchSpent == 0)
                result["wallet check passed"] = true;
            else
            {
                result["mismatched spent coins"] =  nMismatchSpent;
                result["amount in question"] =  Legacy::SatoshisToAmount(nBalanceInQuestion);
            }
            return result;
        }


        /* rescan
        *  Rescans the database for relevant wallet transactions */
        json::json RPC::Rescan(const json::json& params, bool fHelp)
        {
            if(fHelp || params.size() != 0)
                return std::string("rescan - Rescans the database for relevant wallet transactions.");

            Legacy::Wallet::GetInstance().ScanForWalletTransactions(TAO::Ledger::ChainState::stateGenesis, true);

            return "success";

        }

        /* importprivkey <PrivateKey> [label]
        *  Adds a private key (as returned by dumpprivkey) to your wallet */
        json::json RPC::ImportPrivKey(const json::json& params, bool fHelp)
        {
            if(fHelp || params.size() < 1 || params.size() > 2)
                return std::string(
                    "importprivkey <PrivateKey> [label]"
                    " - Adds a private key (as returned by dumpprivkey) to your wallet.");

            std::string strSecret = params[0];
            std::string strLabel = "";

            if(params.size() > 1)
                strLabel = params[1];

            Legacy::NexusSecret vchSecret;
            bool fGood = vchSecret.SetString(strSecret);

            if(!fGood)
                throw APIException(-5,"Invalid private key");

            if(Legacy::Wallet::GetInstance().IsLocked())
                throw APIException(-13, "Error: Please enter the wallet passphrase with walletpassphrase first.");

            if(Legacy::fWalletUnlockMintOnly)
                throw APIException(-102, "Wallet is unlocked for minting only.");

            LLC::ECKey key;
            bool fCompressed;
            LLC::CSecret secret = vchSecret.GetSecret(fCompressed);
            key.SetSecret(secret, fCompressed);
            Legacy::NexusAddress vchAddress = Legacy::NexusAddress(key.GetPubKey());

            {
                Legacy::Wallet::GetInstance().MarkDirty();
                Legacy::Wallet::GetInstance().GetAddressBook().SetAddressBookName(vchAddress, strLabel);

                if(!Legacy::Wallet::GetInstance().AddKey(key))
                    throw APIException(-4,"Error adding key to wallet");
            }

            return "";
        }

        /* dumpprivkey <NexusAddress>
        *  Reveals the private key corresponding to <NexusAddress> */
        json::json RPC::DumpPrivKey(const json::json& params, bool fHelp)
        {
            if(fHelp || params.size() != 1)
                return std::string(
                    "dumpprivkey <NexusAddress>"
                    " - Reveals the private key corresponding to <NexusAddress>.");

            std::string strAddress = params[0];
            Legacy::NexusAddress address;
            if(!address.SetString(strAddress))
                throw APIException(-5, "Invalid Nexus address");

            if(Legacy::Wallet::GetInstance().IsLocked())
                throw APIException(-13, "Error: Please unlock the wallet with walletpassphrase first.");

            if(Legacy::fWalletUnlockMintOnly) // Nexus: no dumpprivkey in mint-only mode
                throw APIException(-102, "Wallet is unlocked for minting only.");

            LLC::CSecret vchSecret;
            bool fCompressed;

            if(!Legacy::Wallet::GetInstance().GetSecret(address, vchSecret, fCompressed))
                throw APIException(-4,"Private key for address " + strAddress + " is not known");

            return Legacy::NexusSecret(vchSecret, fCompressed).ToString();
        }

        /* importkeys
        *  Import List of Private Keys and return if they import properly or fail with their own code in the output sequence"
        *  You need to list the imported keys in a JSON array of {[account],[privatekey]} */
        json::json RPC::ImportKeys(const json::json& params, bool fHelp)
        {
            if(fHelp || params.size() < 1)
            {
                return std::string(
                    "importkeys"
                    " - Import List of Private Keys and return if they import properly or fail with their own code in the output sequence"
                    " You need to list the imported keys in a JSON array of {[account],[privatekey]}");
            }
            /** Make sure the Wallet is Unlocked fully before proceeding. **/
            if(Legacy::Wallet::GetInstance().IsLocked())
                throw APIException(-13, "Error: Please enter the wallet passphrase with walletpassphrase first.");

            if(Legacy::fWalletUnlockMintOnly)
                throw APIException(-102, "Wallet is unlocked for minting only.");

            if(!params[0].is_object())
                throw APIException(-8, "Invalid import list format.");

            /** Establish the JSON Object from the Parameters. **/
            json::json jsonImportArray = params[0];


            /** Establish a return value JSON object for Error Reporting. **/
            json::json response;

            /** Import the Keys one by One. **/
            for(json::json::iterator it = jsonImportArray.begin(); it != jsonImportArray.end(); ++it)
            {
                Legacy::NexusSecret vchSecret;
                if(!vchSecret.SetString(it.value()))
                {
                    response[it.key()] =  "Code -5 Invalid Key";
                    continue;
                }

                LLC::ECKey key;
                bool fCompressed;
                LLC::CSecret secret = vchSecret.GetSecret(fCompressed);
                key.SetSecret(secret, fCompressed);
                Legacy::NexusAddress vchAddress = Legacy::NexusAddress(key.GetPubKey());

                {
                    Legacy::Wallet::GetInstance().MarkDirty();
                    Legacy::Wallet::GetInstance().GetAddressBook().SetAddressBookName(vchAddress, it.key());

                    if(!Legacy::Wallet::GetInstance().AddKey(key))
                    {
                        response[it.key()] = "Code -4 Error Adding to Wallet";
                        continue;
                    }
                }

                response[it.key()] =  "Successfully Imported";
            }

            return response;
        }

        /* exportkeys
        *  Export the private keys of the current UTXO values.
        *  This will allow the importing and exporting of private keys much easier. */
        json::json RPC::ExportKeys(const json::json& params, bool fHelp)
        {
            if(fHelp || params.size() != 0)
                return std::string(
                    "exportkeys"
                    " - Export the private keys of the current UTXO values. "
                    " This will allow the importing and exporting of private keys much easier");

            /** Disallow the exporting of private keys if the encryption key is not available in the memory. **/
            if(Legacy::Wallet::GetInstance().IsLocked())
                throw APIException(-13, "Error: Please enter the wallet passphrase with walletpassphrase first.");

            if(Legacy::fWalletUnlockMintOnly) // Nexus: no dumpprivkey in mint-only mode
                throw APIException(-102, "Wallet is unlocked for minting only.");

            /** Compile the list of available Nexus Addresses and their according Balances. **/
            std::map<Legacy::NexusAddress, int64_t> mapAddresses;
            if(!Legacy::Wallet::GetInstance().GetAddressBook().AvailableAddresses((unsigned int)runtime::unifiedtimestamp(), mapAddresses))
                throw APIException(-3, "Error Extracting the Addresses from Wallet File. Please Try Again.");

            /** Loop all entries of the memory map to compile the list of account names and their addresses.
                JSON object format is a reflection of the import and export options. **/
            json::json ret = json::json::array();
            json::json entry;
            for(std::map<Legacy::NexusAddress, int64_t>::iterator it = mapAddresses.begin(); it != mapAddresses.end(); ++it)
            {
                /** Extract the Secret key from the Wallet. **/
                LLC::CSecret vchSecret;
                bool fCompressed;
                if(!Legacy::Wallet::GetInstance().GetSecret(it->first, vchSecret, fCompressed))
                    throw APIException(-4,"Private key for address " + it->first.ToString() + " is not known");

                /** Extract the account name from the address book. **/
                std::string strAccount;
                if(!Legacy::Wallet::GetInstance().GetAddressBook().HasAddress(it->first))
                    strAccount = "Default";
                else
                    strAccount = Legacy::Wallet::GetInstance().GetAddressBook().GetAddressBookName(it->first);

                /** Compile the Secret Key and account information into a listed pair. **/
                std::string strSecret = Legacy::NexusSecret(vchSecret, fCompressed).ToString();
                entry["strAccount"] = strSecret;

                ret.push_back(entry);
            }

            return ret;

        }
    }
}
