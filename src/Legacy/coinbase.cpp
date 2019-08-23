/*__________________________________________________________________________________________

			(c) Hash(BEGIN(Satoshi[2010]), END(Sunny[2012])) == Videlicet[2014] ++

			(c) Copyright The Nexus Developers 2014 - 2019

			Distributed under the MIT software license, see the accompanying
			file COPYING or http://www.opensource.org/licenses/mit-license.php.

			"ad vocem populi" - To the Voice of the People

____________________________________________________________________________________________*/

#include <Util/include/debug.h>
#include <Legacy/types/coinbase.h>

namespace Legacy
{

    /* Default constructor */
    Coinbase::Coinbase()
    : vOutputs()
	, nMaxValue(0)
	, nPoolFee(0)
	{
	}


    /* Constructor. */
    Coinbase::Coinbase(const std::map<std::string, uint64_t>& vTxOutputs, uint64_t nValue, uint64_t nLocalFee)
    : vOutputs(vTxOutputs)
	, nMaxValue(nValue)
	, nPoolFee(nLocalFee)
	{
	}


    /* Copy constructor. */
    Coinbase::Coinbase(const Coinbase& rhs)
    : vOutputs(rhs.vOutputs)
    , nMaxValue(rhs.nMaxValue)
    , nPoolFee(rhs.nPoolFee)
	{
	}


    /* Assignment operator. */
    Coinbase &Coinbase::operator=(const Coinbase& rhs)
	{
        vOutputs = rhs.vOutputs;
        nMaxValue = rhs.nMaxValue;
        nPoolFee = rhs.nPoolFee;

        return *this;
	}


	/* Destructor */
	Coinbase::~Coinbase()
	{
	}


    /** SetNull
     *
     *  Set the coinbase to null state.
     *
     **/
    void Coinbase::SetNull()
    {
        vOutputs.clear();
        nMaxValue = 0;
        nPoolFee = 0;
    }


    /** IsNull
	 *
	 *	Checks the objects null state.
	 *
	 **/
	bool Coinbase::IsNull() const
	{
		return vOutputs.empty() && nPoolFee == 0;
	}


    /** IsValid
     *
     *  Determines if the coinbase tx has been built successfully.
     *
     *  @return true if the coinbase tx is valid, otherwise false.
     *
     **/
    bool Coinbase::IsValid() const
    {
        uint64_t nCurrentValue = nPoolFee;
        for(const auto& entry : vOutputs)
            nCurrentValue += entry.second;

        return nCurrentValue == nMaxValue;
    }


    /** Print
     *
     *  Writes the coinbase tx data to the log.
     *
     **/
    void Coinbase::Print() const
    {
        debug::log(0, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        uint64_t nTotal = 0;
        for(const auto& entry : vOutputs)
        {
            debug::log(0, entry.first, ":", static_cast<double>(entry.second) / 1000000.0);
            nTotal += entry.second;
        }

        debug::log(0, "Total Value of Coinbase = ", static_cast<double>(nTotal) / 1000000.0);
        debug::log(0, "Set Value of Coinbase = ", static_cast<double>(nMaxValue) / 1000000.0);
        debug::log(0, "PoolFee in Coinbase ", static_cast<double>(nPoolFee) / 1000000.0);
        debug::log(0, "Is Complete: ", IsValid() ? "TRUE" : "FALSE");
        debug::log(0, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    }
}