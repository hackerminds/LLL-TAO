# FINANCE

## Finance API

***

The Finance API provides methods for sending and receiving NXS coins or other tokens between users / accounts, creating accounts, and managing staking.

### `Named Shortcuts`

For each API method we support an alternative endpoint that includes the account name or register address at the end of the the URL. This shortcut removes the need to include the account name or address as an additional parameter.

For example `finance/create/account/savings` is a shortcut to `finance/create/account?name=savings`.

Similarly `finance/get/account/5efc8a9437d93e894defd50f8ba73a0b2b5529cc593d5e4a7ea413022a9c35e9` is a shortcut to `finance/get/account?address=5efc8a9437d93e894defd50f8ba73a0b2b5529cc593d5e4a7ea413022a9c35e9`

The logic for resolving the shortcut to either a name or address is that if the data is 64 characters of hexadecimal then it will be assumed to be a register address. Otherwise it will be considered a name.

### `Methods`

The following methods are currently supported by this API

[`create/account`](finance.md#createaccount)\
[`debit/account`](finance.md#debitaccount)\
[`credit/account`](finance.md#creditaccount)\
[`get/account`](finance.md#getaccount)\
[`list/accounts`](finance.md#listaccounts)\
[`list/account/transactions`](finance.md#listaccounttransactions)\
[`get/stakeinfo`](finance.md#getstakeinfo)\
[`set/stake`](finance.md#setstake)\
[`migrate/accounts`](finance.md#migrateaccounts)\
[`get/balances`](finance.md#getbalances)\
[`list/balances`](finance.md#listbalances)\
[`list/trustaccounts`](finance.md#listtrustaccounts)

***

***

## `create/account`

Create a new account for receiving NXS. The API supports an alternative endpoint that can include the account name in the URL. For example `/finance/create/account/savings` will resolve to `finance/create/account?name=savings`.

#### Endpoint:

`/finance/create/account`

#### Parameters:

`pin` : The PIN for this signature chain.

`session` : For multi-user API mode, (configured with multiuser=1) the session is required to identify which session (sig-chain) the account should be created with. For single-user API mode the session should not be supplied.

`token_name` : Optional name of a token to create the account for. `token` can be supplied as an alternative to `token_name`. Defaults to `NXS`.

`token` : Optional token address to create the account for. `token_name` can be supplied as an alternative to `token`. Defaults to `0` (`NXS`)

`name` : An optional name to identify the account. If provided a Name object will also be created in the users local namespace, allowing the account to be accessed/retrieved by name. If no name is provided the account will need to be accessed/retrieved by its 256-bit register address.

`data` : This optional field allows callers to add arbitrary data to an account register.

#### Example:

This example creates a NXS account called "main".

```
{
    "pin": "1234",
    "name": "main"
}
```

#### Return value JSON object:

```
{
    "txid": "f9dcd28bce2563ab288fab76cf3ee5149ea938c735894ce4833b55e474e08e8a519e8005e09e2fc19623577a8839a280ca72b6430ee0bdf13b3d9f785bc7397d",
    "address": "8CvLySLAWEKDB9SJSUDdRgzAG6ALVcXLzPQREN9Nbf7AzuJkg5P"
}
```

#### Return values:

`txid` : The ID (hash) of the transaction that includes the account creation.

`address` : The register address for this account. The address (or name that hashes to this address) is needed when creating crediting or debiting the account.

***

## `debit/account`

Deduct an amount of NXS from an account and send it to another account or legacy UTXO address. The API supports an alternative endpoint that can include the account name in the URL. For example `/finance/debit/account/savings` will resolve to `finance/debit/account?name=savings`.

Any fee that is applicable fo this transaction is deducted from the sending account, NOT the default account.

The method supports the ability to send to multiple recipients in one transaction by supplying a `recipients` array instead of the single `name_to`/`address_to`/`amount`. The method allows up to 99 recipients to be supplied.

#### Endpoint:

`/finance/debit/account`

#### Parameters:

`pin` : The PIN for this signature chain.

`session` : For multi-user API mode, (configured with multiuser=1) the session is required to identify which session (sig-chain) the account owner. For single-user API mode the session should not be supplied.

`name` : The name identifying the account to debit. This is optional if the address is provided. The name should be in the format username:name (for local names) or namespace::name (for names in a namespace). However, if the account was created in the callers namespace (their username), then the username can be omitted from the name if the `session` parameter is provided (as we can deduce the username from the session)

`address` : The register address of the account to debit. This is optional if the name is provided.

`amount` : The amount of NXS to debit.

`name_to` : The name identifying the account to send to. This is optional if address\_to is provided. The name should be in username:name format

`address_to` : The register address of the account to send to. This is optional if name\_to is provided. The address\_to can also contain a legacy UTXO address if sending from a signature chain account to a legacy address.

`reference` : This optional field allows callers to provide a reference, which the recipient can then use to relate the transaction to an order number, invoice number etc. The reference is be a `64-bit unsigned integer` in the range of 0 to 18446744073709551615

`expires` : This optional field allows callers to specify an expiration for the debit transaction. The expires value is the `number of seconds` from the transaction creation time after which the transaction can no longer be credited by the recipient. Conversely, when you apply an expiration to a transaction, you are unable to void the transaction until after the expiration time. If expires is set to 0, the transaction will never expire, making the sender unable to ever void the transaction. If omitted, a default expiration of 7 days (604800 seconds) is applied.

`recipients` : This optional array can be provided as an alternative to the single `name_to`/`address_to`, `amount`, `reference`, and `expires` fields. Each object in the array can have `name_to`/`address_to`, `amount`, `reference`, and `expires` fields, as described above. Up to 99 recipients can be included in the array.

#### Example:

The following example shows the json payload for a debit of 10.5 NXS from an account called "main" (that itself was created by the sig chain logged into session "5e9d8aa625a1838f60f30e12058089169e32c968389f365428f7b0c878bb47f8") being sent to an account identified by the name "bob:savings". In the name\_to field, "bob" refers to the username of the account holder and "savings" is their NXS account. If Bob is concerned about privacy he could have provided the register address of his token account instead

```
{
    "pin": "1234",
    "session": "5e9d8aa625a1838f60f30e12058089169e32c968389f365428f7b0c878bb47f8",
    "name": "main",
    "amount": 10.5,
    "name_to": "bob:savings"
}
```

#### Example Multiple Recipients:

The following example shows the json payload for a debit from an account called "default" made to 3 separate recipients, each receiving 5 NXS, and each with a different reference

```
{
    "pin": "1234",
    "name": "default"
    "recipients" :
    [
        {
            "name_to": "bob:savings",
            "amount": 5,
            "reference": 1234
        },
        {
            "address_to": "8CHjS5Qe7Mghgrqb6NeEaVxjexbdp9p2QVdRTt8W4rzbWhu3fL8",
            "amount": 5,
            "reference": 5678
        },
        {
            "address_to": "4kYv4Xft6wufMVi191nQDNprgkK6kzy6gqWRpmfMxyx9KMfEV1u",
            "amount": 5
        }
    ]
}
```

#### Return value JSON object:

```
{
    "txid": "f9dcd28bce2563ab288fab76cf3ee5149ea938c735894ce4833b55e474e08e8a519e8005e09e2fc19623577a8839a280ca72b6430ee0bdf13b3d9f785bc7397d"
}
```

#### Return values:

`txid` : The ID (hash) of the transaction that includes the debit.

***

## `credit/account`

Increment an amount received from another NXS account to an account owned by your signature chain. The API supports an alternative endpoint that can include the account name to credit in the URL. For example `/finance/credit/account/savings` will resolve to `finance/credit/account?name=savings`.

#### Endpoint:

`/finance/credit/account`

#### Parameters:

`pin` : The PIN for this signature chain.

`session` : For multi-user API mode, (configured with multiuser=1) the session is required to identify which session (sig-chain) owns the account. For single-user API mode the session should not be supplied.

`txid` : The transaction ID (hash) of the corresponding debit transaction for which you are creating this credit for. The transaction can also be a legacy transaction created from sendtoaddress or sendmany where the inputs are UTXO but the output is an account register address.

`name` : The name identifying the account to credit. This is only required when crediting a split payment transaction (where the receiving account is not included in the credit transaction) and is optional if the address is provided. The name should be in the format username:name (for local names) or namespace::name (for names in a namespace). However, if the account was created in the callers namespace (their username), then the username can be omitted from the name if the `session` parameter is provided (as we can deduce the username from the session)

`address` : The register address of the account to credit. This is only required when crediting a split payment transaction (where the receiving account is not included in the credit transaction) and is optional if the name is provided.

`name_proof` : The name identifying the account that proves your ownership of the share of the debit. This is only required for split payments and is optional if the address is provided. The name should be in the format username:name (for local names) or namespace::name (for names in a namespace). However, if the account was created in the callers namespace (their username), then the username can be omitted from the name if the `session` parameter is provided (as we can deduce the username from the session)

`address_proof` : The register address of the account that proves your ownership of the share of the debit. This is only required for spit payments and is optional if the name is provided.

#### Example:

The following example shows the json payload for a credit to an account called "savings" (that itself was created by the sig chain logged into session "5e9d8aa625a1838f60f30e12058089169e32c968389f365428f7b0c878bb47f8") identified by the name "bob:savings". In the name field, "bob" refers to the username of the account holder and "savings" is their NXS account.

```
{
    "pin": "1234",
    "session": "5e9d8aa625a1838f60f30e12058089169e32c968389f365428f7b0c878bb47f8",
    "name": "bob:savings",
    "txid": "f9dcd28bce2563ab288fab76cf3ee5149ea938c735894ce4833b55e474e08e8a519e8005e09e2fc19623577a8839a280ca72b6430ee0bdf13b3d9f785bc7397d"
}
```

#### Return value JSON object:

```
{
    "txid": "318b86d2c208618aaa13946a3b75f14472ebc0cce9e659f2830b17e854984b55606738f689d886800f21ffee68a3e5fd5a29818e88f8c5b13b9f8ae67739903d"
}
```

#### Return values:

`txid` : The ID (hash) of the transaction that includes the credit.

***

## `get/account`

Retrieves information about a NXS account . The API supports an alternative endpoint that can include the account name in the URL. For example `/finance/get/account/savings` will resolve to `finance/get/account?name=savings`.

Additionally the API supports passing a field name in the URL after the account name, which will populate the `fieldname` parameter in the request and filter the response on that field. For example `/finance/get/account/savings/balance` will resolve to `finance/get/account?name=savings&fieldname=balance`

#### Endpoint:

`/finance/get/account`

#### Parameters:

`name` : The name identifying the NXS account to retrieve. This is optional if the address is provided. The name should be in the format username:name (for local names) or namespace::name (for names in a namespace). However, if the object was created in the callers namespace (their username), then the username can be omitted from the name if the `session` parameter is provided (as we can deduce the username from the session)

`session` : For multi-user API mode, (configured with multiuser=1) the session can be provided in conjunction with the name in order to deduce the register address of the object. The `session` parameter is only required when a name parameter is also provided without a namespace in the name string. For single-user API mode the session should not be supplied.

`address` : The register address of the NXS account to retrieve. This is optional if the name is provided.

`count` : Optional boolean field that determines whether the response includes the transaction `count` field. This defaults to false, as including the transaction count can slow the response time of the method considerably.

`fieldname`: This optional field can be used to filter the response to return only a single field from the account.

#### Return value JSON object:

```
{
    "owner": "a2e51edcd41a8152bfedb24e3c22ee5a65d6d7d524146b399145bced269aeff0",
    "created": 1566534164,
    "modified": 1566616211,
    "name": "default",
    "address": "8CbkwEQ9S8owmX74joU6XmiwxJq1aoiqUoXc9fLCKzw15HscM99",
    "token_name": "NXS",
    "token": "0",
    "data": "abcd1234",
    "balance": 2903.239792,
    "pending": 0.0,
    "unconfirmed": 76.492244
}
```

#### Return values:

`owner` : The genesis hash of the signature chain that owns this account.

`created` : The UNIX timestamp when the account was created.

`modified` : The UNIX timestamp when the account was last modified.

`name` : The name identifying the account. For privacy purposes, this is only included in the response if the caller is the owner of the account

`address` : The register address of the account.

`token` : This will always be set to 0 for NXS accounts.

`data` : The arbitrary data included in the account register. If no data was included during the account creation then this field will be omitted.

`balance` : The available balance of this account. This is the last confirmed balance less any new debits that you have made since the last block

`pending` : This is the sum of all confirmed debit transactions that have been made to this account, that have not yet been credited. To move coins from pending into the available balance you must create a corresponding credit transaction. NOTE: if configured to run, the events processor does this for you.

`unconfirmed` : This is the sum of all unconfirmed debit transactions that have been made to this account PLUS the sum of all unconfirmed credits that you have for confirmed debit transactions. When someone makes a debit to your account it will immediately appear in the unconfirmed balance until that transaction is included in a block, at which point it moves into `pending`. When you (or the events processor) creates the corresponding credit transaction for that debit, the amount will move from `pending` back into `unconfirmed` until the credit transaction is included in a block, at which point the amount moves to `balance`.

`stake` : Only returned for the trust account, this is the amount of NXS currently staked in the trust account.

`count` : Only returned if the caller requested `count` : true. This is the number of transactions made to/from the account.

***

## `list/accounts`

This will list off all of the NXS accounts belonging to the currently logged in signature chain.

#### Endpoint:

`/finance/list/accounts`

#### Parameters:

`session` : For multi-user API mode, (configured with multiuser=1) the session is required to identify which session (sig-chain) owns the account. For single-user API mode the session should not be supplied.

`count` : Optional boolean field that determines whether the response includes the transaction `count` field. This defaults to false, as including the transaction count can slow the response time of the method considerably.

`limit` : The number of records to return for the current page. The default is 100.

`page` : Allows the results to be returned by page (zero based). E.g. passing in page=1 will return the second set of (limit) records. The default value is 0 if not supplied.

`offset` : An alternative to `page`, offset can be used to return a set of (limit) results from a particular index.

`where` : An array of clauses to filter the JSON results. More information on filtering the results from /list/xxx API methods can be found here [Filtering Results](../API/filtering-results.md)

#### Return value JSON object:

```
[
    {
        "created": 1568025836,
        "modified": 1568025836,
        "name": "default",
        "address": "8CbkwEQ9S8owmX74joU6XmiwxJq1aoiqUoXc9fLCKzw15HscM99",
        "token_name": "NXS",
        "token": "0",
        "balance": 5000,
        "pending": 0.0,
        "unconfirmed": 76.492244
    },
    {
        "created": 1568025836,
        "modified": 1568025836,
        "name": "savings",
        "address": "8GhrC2TKkU4ra9Uuj8LuiAyxDAtza2u483N1rKDSaVp24dNgUy9",
        "token_name": "mytoken",
        "token": "8GHrC2TKkU4ra9Uuj8LuiAyxDAtza2u483N1rKDSaVp24dNgUx8",
        "balance": 10000.0,
        "pending": 0.0,
        "unconfirmed": 0.0
    }
]
```

#### Return values:

`created` : The UNIX timestamp when the account was created.

`modified` : The UNIX timestamp when the account was last modified.

`name` : The name identifying the account. For privacy purposes, this is only included in the response if the caller is the owner of the account

`address` : The register address of the account.

`token_name` : This will always be set to `NXS`.

`token` : This will always be set to 0 for NXS accounts.

`data` : The arbitrary data included in the account register. If no data was included during the account creation then this field will be omitted.

`balance` : The available balance of this account. This is the last confirmed balance less any new debits that you have made since the last block

`pending` : This is the sum of all confirmed debit transactions that have been made to this account, that have not yet been credited. To move coins from pending into the available balance you must create a corresponding credit transaction. NOTE: if configured to run, the events processor does this for you.

`unconfirmed` : This is the sum of all unconfirmed debit transactions that have been made to this account PLUS the sum of all unconfirmed credits that you have for confirmed debit transactions. When someone makes a debit to your account it will immediately appear in the unconfirmed balance until that transaction is included in a block, at which point it moves into `pending`. When you (or the events processor) creates the corresponding credit transaction for that debit, the amount will move from `pending` back into `unconfirmed` until the credit transaction is included in a block, at which point the amount moves to `balance`.

`stake` : Only returned for the trust account, this is the amount of NXS currently staked in the trust account.

`count` : Only returned if the caller requested `count` : true. This is the number of transactions made to/from the account.

***

## `list/account/transactions`

This will list off all of the transactions related to a given account. You DO NOT need to be logged in to use this command. If you are logged in, then neither username or genesis are required as it will default to the logged in user.

**NOTE** : The returned transaction data will only include contracts that related to the requested account. Any other contracts are omitted from the transaction result.

**NOTE** : If you use the username parameter it will take slightly longer to calculate the username genesis with our brute-force protected hashing algorithm. For higher performance, use the genesis parameter.

#### Endpoint:

`/finance/list/account/transactions`

#### Parameters:

`genesis` : The genesis hash identifying the signature chain to scan for transactions (optional if username is supplied or already logged in).

`username` : The username identifying the signature chain to scan for transactions(optional if genesis is supplied or already logged in).

`session` : For multi-user API mode, (configured with multiuser=1) the session can be provided in conjunction with the account name in order to deduce the register address of the account. The `session` parameter is only required when a name parameter is also provided without a namespace in the name string. For single-user API mode the session should not be supplied.

`name` : The name identifying the NXS account to list transactions for. This is optional if the address is provided. The name should be in the format username:name (for local names) or namespace::name (for names in a namespace). However, if the object was created in the callers namespace (their username), then the username can be omitted from the name if the `session` parameter is provided (as we can deduce the username from the session)

`address` : The register address of the NXS account to list transactions for. This is optional if the name is provided.

`verbose` : Optional, determines how much transaction data to include in the response. Supported values are :

* `default` : hash, contracts
* `summary` : type, version, sequence, timestamp, operation, and confirmations.
* `detail` : genesis, nexthash, prevhash, pubkey and signature.

`limit` : The number of records to return for the current page. The default is 100.

`page` : Allows the results to be returned by page (zero based). E.g. passing in page=1 will return the second set of (limit) records. The default value is 0 if not supplied.

`offset` : An alternative to `page`, offset can be used to return a set of (limit) results from a particular index.

`where` : An array of clauses to filter the JSON results. More information on filtering the results from /list/xxx API methods can be found here [Filtering Results](../API/filtering-results.md)

#### Return value JSON object:

```
[
    {
        "txid": "01034b39cb3635d370f97339e6f87b8751d4c0d62676da7d6ec20416966f298f47dea99603d03a74e638b0d50b31b1e721790e5b103abfe3353a709ccf5d1e7c",
        "contracts": [
            {
                "OP": "CREDIT",
                "txid": "01e73b498dbabbf4629ad674b9ae3824b96cca83199c25a67901db53b271d19acf1411b0c4f9a3d8ded80860ffe2dcf683d2d227a675d453303b31f86f868f9e",
                "contract": 0,
                "proof": "8FJxzexVDUN5YiQYK4QjvfRNrAUym8FNu4B8yvYGXgKFJL8nBse",
                "to": "8CvLySLAWEKDB9SJSUDdRgzAG6ALVcXLzPQREN9Nbf7AzuJkg5P",
                "to_name": "default",
                "amount": 76.450762,
                "token": "0",
                "token_name": "NXS"
            },
            {
                "OP": "FEE",
                "from": "8CvLySLAWEKDB9SJSUDdRgzAG6ALVcXLzPQREN9Nbf7AzuJkg5P",
                "from_name": "default",
                "amount": 0.1
            }
        ]
    }
]
```

#### Return values:

`txid` : The transaction hash.

`type` : The description of the transaction (`legacy` | `tritium base` | `trust` | `genesis` | `user`).

`version` : The serialization version of the transaction.

`sequence` : The sequence number of this transaction within the signature chain.

`timestamp` : The Unix timestamp of when the transaction was created.

`blockhash` : The hash of the block that this transaction is included in. Blank if not yet included in a block.

`confirmations` : The number of confirmations that this transaction obtained by the network.

`genesis` : The signature chain genesis hash.

`nexthash` : The hash of the next transaction in the sequence.

`prevhash` : the hash of the previous transaction in the sequence.

`pubkey` : The public key.

`signature` : The signature hash.

`contracts` : The array of contracts bound to this transaction and their details with opcodes.\
{\
`id` : The sequential ID of this contract within the transaction.

&#x20;  `OP` : The contract operation. Can be `APPEND`, `CLAIM`, `COINBASE`, `CREATE`, `CREDIT`, `DEBIT`, `FEE`, `GENESIS`, `LEGACY`, `TRANSFER`, `TRUST`, `STAKE`, `UNSTAKE`, `WRITE`.

&#x20;  `for` : For `CREDIT` transactions, the contract that this credit was created for . Can be `COINBASE`, `DEBIT`, or`LEGACY`.

&#x20;  `txid` : The transaction that was credited / claimed.

&#x20;  `contract` : The ID of this contract within the transaction that was credited / claimed.

&#x20;  `proof` : The register address proving the credit.

&#x20;  `from` : For `DEBIT`, `CREDIT`, `FEE` transactions, the register address of the account that the debit is being made from.

&#x20;  `from_name` : For `DEBIT`, `CREDIT`, `FEE` transactions, the name of the account that the debit is being made from. Only included if the name can be resolved.

&#x20;  `to` : For `DEBIT` and `CREDIT` transactions, the register address of the recipient account.

&#x20;  `to_name` : For `DEBIT` and `CREDIT` transactions, the name of the recipient account. Only included if the name can be resolved.

&#x20;  `amount` : the token amount of the transaction.

&#x20;  `token` : the register address of the token that the transaction relates to. Set to 0 for NXS transactions

&#x20;  `token_name` : The name of the token that the transaction relates to.

&#x20;  `reference` : For `DEBIT` and `CREDIT` transactions this is the user supplied reference used by the recipient to relate the transaction to an order or invoice number.

}

***

## `get/stakeinfo`

This will retrieve account values and staking metrics for the trust account belonging to the currently logged in signature chain. If called when the stake minter is not running, this method only returns trust account values. Staking metrics will return 0.

#### Endpoint:

`/finance/get/stakeinfo`

#### Parameters:

`session` : For multi-user API mode, (configured with multiuser=1) the session is required to identify which session (sig-chain) owns the trust account. For single-user API mode the session should not be supplied.

`fieldname`: This optional field can be used to filter the response to return only a single field from the account.

#### Return value JSON object:

```
{
    "address": "8FJxzexVDUN5YiQYK4QjvfRNrAUym8FNu4B8yvYGXgKFJL8nBse",
    "balance": 150,
    "stake": 5000,
    "trust": 54322,
    "new": false,
    "staking": true,
    "pooled": false,
    "onhold": false,
    "stakerate": 1.97,
    "trustweight": 58.97,
    "blockweight": 47.62,
    "stakeweight": 54.03,
    "change": false
}
```

#### Return values:

`address` : The register address of the trust account.

`balance` : The current NXS balance of the trust account. This is general account balance that is not staked.

`stake` : The amount of NXS currently staked in the trust account.

`trust` : The current raw trust score of the trust account.

`new` : Indicates whether trust account is new (true, staking Genesis) or established (false, staking Trust).

`staking` : Indicates whether staking is actively running for user account (when false, weight metrics will be 0).

`pooled` : Flag indicating whether pooled staking is enabled or not

`onhold` : When trust account is new, any change to balance requires a minimum wait period before staking begins. During this period, staking is on hold and this field returns true. "staking" field will still be true when on hold.

`holdtime` : Conditional field. Only appears when onhold=true and will contain number of seconds remaining in hold period.

`stakerate` : The current annual reward rate earned for staking as an annual percent.

`trustweight` : The current trust weight applied to staking as a percent of maximum.

`blockweight` : The current block weight applied to staking as a percent of maximum.

`stakeweight` : The current stake weight (trust weight and block weight combined) as a percent of maximum.

`change` : Indicates whether or not there is a pending request to change stake. The remaining fields only appear when this one is true.

`amount` : Amount of stake change. Positive will be added to stake (moved from balance) and negative will unstake (move to balance).

`requested` : Timestamp when the stake change request was created.

`expires`: Timestamp when the stake change request expires, if assigned. Zero indicates does not expire.

***

## `set/stake`

Creates a stake change request for a signature chain's trust account. This request will add or remove stake to set the stake value to the requested amount. If the new value is more than the current stake amount, it adds stake from the account balance. If the new value is less, it removes stake to the account balance (with appropriate trust penalty, if applicable).

Requests are saved locally and take effect with the next stake block found by staking the signature chain's trust account. Because they are saved locally, you must continue to stake on the machine where it was created until the next stake block is found, or the request will not be processed.

Until implemented, you can update the request by calling set/stake again.

To remove a stake change request, you can either set an expiration time, or set the amount equal to the current trust account stake.

#### Endpoint:

`/finance/set/stake`

#### Parameters:

`pin` : The PIN for the signature chain.

`session` : For multi-user API mode, (configured with multiuser=1) the session is required to identify which session (sig-chain) owns the trust account. For single-user API mode the session should not be supplied.

`amount` : The new amount of NXS to stake.

`expires` : Optional field to assign the number of seconds until the stake change request expires. A value of zero indicates it does not expire. Default is zero if not passed.

#### Return value JSON object:

```
{
    "txid": "318b86d2c208618aaa13946a3b75f14472ebc0cce9e659f2830b17e854984b55606738f689d886800f21ffee68a3e5fd5a29818e88f8c5b13b9f8ae67739903d"
}
```

#### Return values:

`txid` : The ID (hash) of the transaction that includes the stake change.

***

## `migrate/accounts`

This method will migrate your legacy accounts to signature chain accounts, sending the balance across in the process. A new account will be created in your signature chain for each legacy account, with a corresponding matching name (unless flagged not to create names). The balance of each legacy account is sent to the newly created signature chain account in individual transactions. As such, each transaction incurs the default legacy fee of 0.01 NXS, which is deducted from the amount being migrated.

The method uses the arbitrary `data` field in the account object register to track which legacy account it was created from. As a result, it is possible to invoke this method multiple times, and each time it will sweep any NXS from legacy accounts to existing signature chain accounts (as well as creating any necessary new accounts).

#### Endpoint:

`/finance/migrate/accounts`

#### Parameters:

`pin` : The PIN for the signature chain.

`session` : For multi-user API mode, (configured with multiuser=1) the session is required to identify which session (sig-chain) owns the trust account. For single-user API mode the session should not be supplied.

`walletpassphrase` : Optional field to provide the wallet passphrase. This value is required if the wallet is not currently unlocked.

`createname` : Optional boolean field indicating whether to create a Name record for the newly created signature chain accounts. If omitted, the default behaviour is to create a Name record, which incurs a fee of 1 NXS per account.

#### Return value JSON object:

```
[
    {
        "account": "default",
        "address": "8BzPhYgcHAP26CYnCvHTQmPiBH7FBC2PAB4rR9YgWxcK7L4c46X",
        "amount": 44.9,
        "txid": "022fed45add52c82110411c703c070519ef1136f3fe401def06a9fe29b0fe378935f17c3846c8824410e197f1caea2aaae5e994530cf6ee3d06207dd46126171"
    },
    {
        "account": "test1",
        "address": "8CEwJUPAkQQuBbAoQtiiswwk38ASuUZ63JQ2yLakV2cEXPbsmsd",
        "amount": 1.0,
        "txid": "02b88a4a9972aafdf09461bd976617162b8f8df853f46a5e0d98d608b987d5d52a3cb67c3861e6b47b65721d83b4b47c851af32b01df3292d1e814242966b658"
    }
]
```

#### Return values:

`account` : The legacy account name (and tritium account name, unless `createname=false` was explicitly set in the request)

`address` : The register address of the signature chain account that the legacy funds have been migrated to.

`amount` : The NXS amount transferred from the legacy account to the signature chain account.

`txid` : If the the legacy send was successful, the ID (hash) of the legacy transaction.

`error` : If the legacy send failed, this field includes the legacy send error message

***

## `get/balances`

This will retrieve a summary of balance information across all accounts belonging to the currently logged in signature chain for a particular token type.

#### Endpoint:

`/finance/get/balances`

#### Parameters:

`session` : For multi-user API mode, (configured with multiuser=1) the session is required to identify which session (sig-chain) to return data for. For single-user API mode the session should not be supplied.

`token_name` : Optional name of a token to return the balances for. `token` can be supplied as an alternative to `token_name`. Defaults to `NXS`.

`token` : Optional token address to return balances for. `token_name` can be supplied as an alternative to `token`. Defaults to `0` (`NXS`)

#### Return value JSON object:

```
{
    "token_name": "NXS",
    "token": "0000000000000000000000000000000000000000000000000000000000000000",
    "available": 1000,
    "pending": 50,
    "unconfirmed": 5,
    "stake": 5000,
    "immature": 100
}
```

#### Return values:

`token_name` : The name of the token that these balances are for, if known.

`token` : The register address of the token that these balances are for.

`available` : The current balance across all accounts that is available to be spent.

`pending` : The sum of all debit and coinbase transactions made to your accounts that are confirmed but have not yet been credited. This does NOT include immature and unconfirmed amounts.

`unconfirmed` : The sum of all debit transactions made to your accounts that are not confirmed, or credits you have made to your accounts that are not yet confirmed (not yet included in a block).

`immature` The sum of all coinbase transactions that have not yet reached maturity. Only included when returning NXS balances.

`stake` : The amount of NXS currently staked in the trust account. Only included when returning NXS balances.

***

## `list/balances`

This will retrieve a summary of balance information across all accounts for each coin/token type owned, belonging to the currently logged in signature chain.

#### Endpoint:

`/finance/list/balances`

#### Parameters:

`session` : For multi-user API mode, (configured with multiuser=1) the session is required to identify which session (sig-chain) to return data for. For single-user API mode the session should not be supplied.

`limit` : The number of records to return for the current page. The default is 100.

`page` : Allows the results to be returned by page (zero based). E.g. passing in page=1 will return the second set of (limit) records. The default value is 0 if not supplied.

`offset` : An alternative to `page`, offset can be used to return a set of (limit) results from a particular index.

`where` : An array of clauses to filter the JSON results. More information on filtering the results from /list/xxx API methods can be found here [Filtering Results](../API/filtering-results.md)

#### Return value JSON object:

```
[
     {
        "token_name": "NXS",
        "token": "0000000000000000000000000000000000000000000000000000000000000000",
        "available": 17002.095753,
        "pending": 1.0,
        "unconfirmed": 0.0,
        "stake": 9000.0,
        "immature": 0.0
    },
    {
        "token_name": "ABC",
        "token": "d7885b8ce210375377ee23d6b54f922ddb64c9c8e70fabd392753337331bf29a",
        "available": 250.0,
        "pending": 0.0,
        "unconfirmed": 0.0
    }
]
```

#### Return values:

`token_name` : The name of the token that these balances are for, if known.

`token` : The register address of the token that these balances are for.

`available` : The current balance across all accounts that is available to be spent.

`pending` : The sum of all debit and coinbase transactions made to your accounts that are confirmed but have not yet been credited. This does NOT include immature and unconfirmed amounts.

`unconfirmed` : The sum of all debit transactions made to your accounts that are not confirmed, or credits you have made to your accounts that are not yet confirmed (not yet included in a block).

`immature` The sum of all coinbase transactions that have not yet reached maturity. Only included when returning NXS balances.

`stake` : The amount of NXS currently staked in the trust account. Only included when returning NXS balances.

***

## `list/trustaccounts`

This will list all known trust accounts

#### Endpoint:

`/finance/list/trustaccounts`

#### Parameters:

`sort` : Determines which field the results should be sorted on. Values can be `balance`, `stake`, or `trust`. Default is `trust`

`order` : Determines the order of the sort. Values can be `desc` for descending (the default) or `asc` for ascending.

`limit` : The number of records to return for the current page. The default is 100.

`page` : Allows the results to be returned by page (zero based). E.g. passing in page=1 will return the second set of (limit) records. The default value is 0 if not supplied.

`offset` : An alternative to `page`, offset can be used to return a set of (limit) results from a particular index.

`where` : An array of clauses to filter the JSON results. More information on filtering the results from /list/xxx API methods can be found here [Filtering Results](../API/filtering-results.md)

#### Return value JSON object:

```
[
    {
        "address": "8FJxzexVDUN5YiQYK4QjvfRNrAUym8FNu4B8yvYGXgKFJL8nBse",
        "owner": "a2cfad9f505f8166203df2685ee7cb8582be9ae017dcafa4ca4530cd5b4f1dca",
        "created": 1569298627,
        "modified": 1569359986,
        "balance": 9.897773,
        "stake": 1000000.0,
        "stakerate": 0.5179354594112684
    },
    {
        "address": "8FBtvmLSLqhVM9PsJ5Zzy1XzcZyP9XaEErjSoGPaoN5fBJaQbp8",
        "owner": "a2e7b433a4dcb54c3b4dc1111d7945394dc3e140c9a400dc623b3f6d53ec758b",
        "created": 1569306341,
        "modified": 1569312960,
        "balance": 0.004081,
        "stake": 5000.0,
        "stakerate": 0.5013274626265375
    }
]
```

#### Return values:

`address` : The register address of the trust account

`owner` : The genesis hash of the trust account owner

`created` : The UNIX timestamp when the account was created.

`modified` : The UNIX timestamp when the account was last modified.

`balance` : The current NXS balance of the trust account. This is general account balance that is not staked.

`stake` : The amount of NXS currently staked in the trust account.

`trust` : The current raw trust score of the trust account.

`stakerate` : The current annual reward rate earned for staking as an annual percent.

***
