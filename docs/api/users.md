# USERS

## Users API

***

The Users API provides methods for creating and managing users. A user is synonymous with a signature chain.

### `Named Shortcuts`

For each API method we support an alternative endpoint that includes the username at the end of the the URI. This shortcut removes the need to include the username or address as an additional parameter.

For example `/users/login/user/myusername` is a shortcut to `users/login/user?username=myusername`.

Similarly `/users/list/accounts/myusername` is a shortcut to `users/list/accounts?username=myusername`.

### `Methods`

The following methods are currently supported by this API

[`create/user`](users.md#createuser)\
[`login/user`](users.md#loginuser)\
[`logout/user`](users.md#logoutuser)\
[`unlock/user`](users.md#unlockuser)\
[`lock/user`](users.md#lockuser)\
[`update/user`](users.md#updateuser)\
[`get/status`](users.md#getstatus)\
[`list/accounts`](users.md#listaccounts)\
[`list/assets`](users.md#listassets)\
[`list/invoices`](users.md#listinvoices)\
[`list/items`](users.md#listitems)\
[`list/names`](users.md#listnames)\
[`list/namespaces`](users.md#listnamespaces)\
[`list/notifications`](users.md#listnotifications)\
[`process/notifications`](users.md#processnotifications)\
[`list/tokens`](users.md#listtokens)\
[`list/transactions`](users.md#listtransactions) [`save/session`](users.md#savesession) [`load/session`](users.md#loadsession) [`has/session`](users.md#hassession)

***

***

## `create/user`

This will create a new user account (signature chain) for use on the network. The user account is secured by a combination of username, password, and PIN.

**NOTE** : username must be a minimum of 2 characters\
**NOTE** : password must be a minimum of 8 characters\
**NOTE** : pin must be a minimum of 4 characters

#### Endpoint:

`/users/create/user`

#### Parameters:

`username` : The username to be associated with this user. The signature chain genesis (used to uniquely identify user accounts) is a hash of this username, therefore the username must be unique on the blockchain.

`password` : The password to be associated with this user.

`pin` : The PIN can be a combination of letters/numbers/symbols or could be tied into an external digital fingerprint. The PIN is required for all API calls that modify a user account (such as sending or claiming transactions).

#### Return value JSON object:

```
{
    "genesis": "c8749c4e748f3a9a3ca2b3199ed34d9501ef8a5df07d9dadcbacbef74202deb9",
    "hash": "9652d37fcd0ebd9cea0b4ee282aacdb0751c8bc37c10e866f8a8e57f39f397680752ec5295b23fea0252234f494066ab2ded541709c8224f9ec30438b47c8691",
    "nexthash": "257a3811555410db58922e206d2fb9e727c00e63074966effa5acc4247879bcd",
    "prevhash": "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    "pubkey": "031cdb48d689ac689e4d3cc7b3ff863d6cfb969623226584d7231b8842e514a357c5e1ed9784e96dc5496e04a39ac17ebe6b28cfb98416175980b34bb535ad5304",
    "sequence": 0,
    "signature": "30818402404117fdb407d26996f035e670e12f7ca820b073a7d172f6caf93ed8c7b20d5c99d1ffebdbc61ef348400b503c6305217da96fa01ed51da6279f4022293a74a6c102406a19edbb56427a58a2560b6048c2dfcfd120df2a0509f31420f9b6f9cb8864112a7dee458ad0505020fa81f293c31d2469cc7e3ebd3967b575df299f0b610993",
    "timestamp": 1545445970,
    "version": 1
}
```

#### Return values:

`genesis` : The signature chain genesis hash. This is a hash of the username used to create the user.

`hash` : The hash of the transaction that created the user.

`nexthash` : The hash of the next transaction in the sequence.

`prevhash` : The hash of the previous transaction in the sequence.

`pubkey` : The public key.

`signature` : The signature hash.

`sequence` : The sequence number of this transaction within the signature chain.

`timestamp` : The Unix timestamp of when the transaction was created.

`version` : The serialization version of the transaction.

***

## `login/user`

This will start a session for your user account with this specific API instance. Username, password, and pin fields are mandatory for login.

#### Endpoint:

`/users/login/user`

#### Parameters:

`username` : The username associated with this signature chain.

`password` : The password to be associated with this signature chain.

`pin` : The PIN for this signature chain.

#### Return value JSON object:

```
{
    "genesis": "27ef3f31499b6f55482088ba38b7ec7cb02bd4383645d3fd43745ef7fa3db3d1",
    "session": "5e9d8aa625a1838f60f30e12058089169e32c968389f365428f7b0c878bb47f8"
}
```

#### Return values:

`genesis` : The signature chain genesis hash. This is a hash of the username used to create the user.

`session` : When using multi-user API mode, an additional session value is returned and must be supplied in subsequent API calls, to allow the managing of multiple login sessions.

***

## `logout/user`

This will log you out of this specific API, and delete your credentials stored in encrypted memory.

#### Endpoint:

`/users/logout/user`

#### Parameters:

`session` : For multi-user API mode (configured with multiuser=1), the session is required to identify which session you are logging out of. For single-user API mode the session should not be supplied.

#### Return value JSON object:

```
{
    "success": true
}
```

#### Return values:

`success` : Flag indicating if the call was successful

***

## `unlock/user`

This will unlock your signature chain and cache the PIN in encrypted memory to be used for all subsequent API calls. This method is only available when using single-user API mode (multiuser=0).

The `pin` field is mandatory for unlock. The mining/staking/transactions/notifications parameters are optional and, if supplied, allow the signature chain to be used for either mining,staking, creating transactions, or processing notifications (or all three ) when in single-user API mode.

**NOTE** : If the mining/staking/transactions/notifications parameters are not supplied then it is assumed that the signature chain can be used for all. **NOTE** : This method can be called multiple times to unlock the options not previously unlocked.

#### Endpoint:

`/users/unlock/user`

#### Parameters:

`pin` : The PIN for this signature chain.

`mining` : This boolean value determines whether the logged in users signature chain can be used for mining.

`notifications` : This boolean value determines whether the logged in users signature chain can be used for processing notifications.

`staking` : This boolean value determines whether the logged in users signature chain can be used for staking.

`transactions` : This boolean value determines whether the logged in users signature chain can be used for creating or claiming transactions.

#### Return value JSON object:

```
{
    "unlocked": {
        "mining": true,
        "notifications": true,
        "staking": false,
        "transactions": false
    }
}
```

#### Return values:

`unlocked` : This will contain child elements describing which functions the session is currently unlocked for

`mining` : Boolean flag indicating whether the users sig chain is unlocked for mining.

`notifications` : Boolean flag indicating whether the users sig chain is unlocked for processing notifications.

`staking` : Boolean flag indicating whether the users sig chain is unlocked for staking.

`transactions` : Boolean flag indicating whether the users sig chain is unlocked for creating any transactions (except those automatically created through mining/processing notifications if those are unlocked).

***

## `lock/user`

This will lock your signature chain, making it unavailable for use unless it is either unlocked or the PIN is passed in to all API requests. Only available in single-user API mode (multiuser=0).

The mining/staking/transactions/notifications parameters are optional and, if supplied, allow the signature chain to be locked for either mining, staking, creating transactions, or processing notifications. This allows, for example, a signature chain to be continually unlocked for mining, but only temporarily unlocked for sending a transaction.

**NOTE** : This method can be called multiple times to lock the options not previously locked.

#### Endpoint:

`/users/lock/user`

#### Parameters:

`mining` : If set this will lock the users sig chain for mining.

`notifications` : If set this will lock the users sig chain from processing notifications.

`staking` : If set this will lock the users sig chain for staking.

`transactions` : If set this will lock the users sig chain from creating any transactions, except those required for mining or processing notifications which are controlled separately.

#### Return value JSON object:

```
{
    "unlocked": {
        "mining": true,
        "notifications": true,
        "staking": false,
        "transactions": false
    }
}
```

#### Return values:

`unlocked` : This will contain child elements describing which functions the session is currently unlocked for

`mining` : Boolean flag indicating whether the users sig chain is unlocked for mining.

`notifications` : Boolean flag indicating whether the users sig chain is unlocked for processing notifications.

`staking` : Boolean flag indicating whether the users sig chain is unlocked for staking.

`transactions` : Boolean flag indicating whether the users sig chain is unlocked for creating any transactions (except those automatically created through mining/processing notifications if those are unlocked).

***

## `update/user`

This method provides the user with the ability to change the password, pin, or recovery seed for this signature chain.

Updating the credentials will also result in each of the keys in the sig chain's Crypto object being regenerated based on the new password / pin.

If setting a recovery....

This method requires the user to already be logged in.

#### Endpoint:

`/users/update/user`

#### Parameters:

`session` : When using multi-user API mode the session parameter must be supplied to identify which user to update.

`password` : The existing password for this signature chain.

`pin` : The existing pin for this signature chain.

`recovery` : The existing recovery seed for this signature chain. This is only required if an existing recovery seed is being updated via `new_recovery`

`new_password` : The new password to set for for this signature chain. This is optional if new\_pin is provided

`new_pin` : The new pin to set for this signature chain. This is optional if new\_password is provided.

`new_recovery` : The new recovery seed to set on this sig chain. This is optional if new\_pin or new\_password is provided. The recovery seed must be a minimum of 40 characters. **NOTE**: the recovery seed is case sensitive.

#### Example 1:

The following example changes the existing password from `password1` to `password1234` and existing pin from `1234` to `12345`

```
{
    "password": "password1",
    "pin": "1234",
    "new_password": "password12345",
    "new_pin": "12345"
}
```

#### Example 2:

The following example sets the initial recovery seed on the sig chain

```
{
    "password": "password1",
    "pin": "1234",
    "new_recovery": "this is the recovery seed that I wish to use"
}
```

#### Return value JSON object:

```
{
    "txid": "f9dcd28bce2563ab288fab76cf3ee5149ea938c735894ce4833b55e474e08e8a519e8005e09e2fc19623577a8839a280ca72b6430ee0bdf13b3d9f785bc7397d"
}
```

#### Return values:

`txid` : The ID (hash) of the transaction that includes the update to the signature chain credentials.

***

## `get/status`

Return status information for the currently logged in user

#### Endpoint:

`/users/get/status`

#### Parameters:

`session` : When using multi-user API mode the session parameter must be supplied to identify which user to return the status for.

`pin` : The pin for the signature chain. This should only be supplied in multi-user mode and only if the caller requires the `username` to be included in the response. In single user mode the username is always returned

#### Return value JSON object:

```
{
    "username": "bob",
    "genesis": "a2e51edcd41a8152bfedb24e3c22ee5a65d6d7d524146b399145bced269ae000",
    "confirmed": true,
    "recovery": true,
    "transactions": 272,
    "notifications": 10,
    "unlocked": {
        "mining": false,
        "notifications": false,
        "staking": false,
        "transactions": false
    }
}
```

#### Return values:

`username` : The username of the currently logged in user. This is always included in single-user API mode, but is only included in multi-user mode if the caller provides the `pin` parameter.

`genesis` : The signature chain genesis hash for the currently logged in user.

`confirmed` : Boolean flag indicating whether the genesis transaction for this signature chain has at least one confirmation.

`recovery` : Flag indicating whether the recovery seed has been set for this user.

`transactions` : The total transaction count in this sig chain

`notifications` : The number of outstanding notifications for this user

`unlocked` : This will contain child elements describing which functions the session is currently unlocked for

`mining` : Boolean flag indicating whether the users sig chain is unlocked for mining.

`notifications` : Boolean flag indicating whether the users sig chain is unlocked for processing notifications.

`staking` : Boolean flag indicating whether the users sig chain is unlocked for staking.

`transactions` : Boolean flag indicating whether the users sig chain is unlocked for creating any transactions (except those automatically created through mining/processing notifications if those are unlocked).

***

## `list/assets`

This will list off all of the assets owned by a signature chain.

**NOTE** : If you use the username parameter, it will take slightly longer to calculate the username genesis with our brute-force protected hashing algorithm. For higher performance, use the genesis parameter.

#### Endpoint:

`/users/list/assets`

#### Parameters:

`genesis` : The genesis hash identifying the signature chain (optional if username is supplied).

`username` : The username identifying the signature chain (optional if genesis is supplied).

`limit` : The number of records to return for the current page. The default is 100.

`page` : Allows the results to be returned by page (zero based). E.g. passing in page=1 will return the second set of (limit) records. The default value is 0 if not supplied.

`offset` : An alternative to `page`, offset can be used to return a set of (limit) results from a particular index.

`where` : An array of clauses to filter the JSON results. More information on filtering the results from /list/xxx API methods can be found here [Filtering Results](../API/filtering-results.md)

#### Return value JSON object:

```
[
    {
        "name": "myasset1",
        "address": "88dFaTWNYpoGuXtgkWrbQVfsJJ3Sfth5AP9yWqioxCLhHmTu6xo",
        "created": "1553227256",
        "modified": "1553227256",
        "owner": "bf501d4f3d81c31f62038984e923ad01546ff678e305a7cc11b1931742524ce1",
        "serial_number": "123456",
        "description": "This is the description of my asset",
        "shelf_location": "A9S2"
    },
    {
        "name": "myasset2",
        "address": "8B7SMKmECgYU1ydBQbzp5FCSe4AnkU2EwLE59D7eQDBpixmLZ2c",
        "created": "1553227128",
        "modified": "1553227128",
        "owner": "bf501d4f3d81c31f62038984e923ad01546ff678e305a7cc11b1931742524ce1",
        "serial_number": "78901234",
        "description": "This is the description of another asset",
        "shelf_location": "A3S6"
    }
]
```

#### Return values:

`created` : The UNIX timestamp when the asset was created.

`modified` : The UNIX timestamp when the asset was last modified.

`name` : The name identifying the asset. For privacy purposes, this is only included in the response if the caller is the owner of the asset

`address` : The register address of the asset.

`owner` : The genesis hash of the signature chain that owns this asset.

`ownership` : Only included for tokenized assets, this is the percentage of the asset owned by the caller, based on the number of tokens owned

`<fieldname>=<value>` : The key-value pair for each piece of data stored in the asset.

***

## `list/accounts`

This will list off all of the token accounts belonging to a particular signature chain.

**NOTE** : If you use the username parameter, it will take slightly longer to calculate the username genesis with our brute-force protected hashing algorithm. For higher performance, use the genesis parameter.

#### Endpoint:

`/users/list/accounts`

#### Parameters:

`genesis` : The genesis hash identifying the signature chain (optional if username is supplied).

`username` : The username identifying the signature chain (optional if genesis is supplied).

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

`address` : The register address for the account

`token_name` : The name of the token that this account can be used with. For privacy purposes, this is only included in the response if the caller is the owner of the token

`token` : The register address of the token that this account can be used for.

`balance` : The available balance of this account. This is the last confirmed balance less any new debits that you have made since the last block

`pending` : This is the sum of all confirmed debit transactions that have been made to this account, that have not yet been credited. To move coins from pending into the available balance you must create a corresponding credit transaction. NOTE: if configured to run, the events processor does this for you.

`unconfirmed` : This is the sum of all unconfirmed debit transactions that have been made to this account PLUS the sum of all unconfirmed credits that you have for confirmed debit transactions. When someone makes a debit to your account it will immediately appear in the unconfirmed balance until that transaction is included in a block, at which point it moves into `pending`. When you (or the events processor) creates the corresponding credit transaction for that debit, the amount will move from `pending` back into `unconfirmed` until the credit transaction is included in a block, at which point the amount moves to `balance`.

`count` : Only returned if the caller requested `count` : true. This is the number of transactions made to/from the account.

***

## `list/invoices`

This will list all invoices issued or received by the signature chain.

**NOTE** : If you use the username parameter, it will take slightly longer to calculate the username genesis with our brute-force protected hashing algorithm. For higher performance, use the genesis parameter.

#### Endpoint:

`/users/list/invoices`

#### Parameters:

`genesis` : The genesis hash identifying the signature chain (optional if username is supplied).

`username` : The username identifying the signature chain (optional if genesis is supplied).

`status` : Optional filter by invoice status. Values can be `OUTSTANDING` (the invoice has been issued but not paid), `PAID` (the invoice has been paid by the recipient), or `CANCELLED` (the invoice was cancelled by the issuer before payment)

`limit` : The number of records to return for the current page. The default is 100.

`page` : Allows the results to be returned by page (zero based). E.g. passing in page=1 will return the second set of (limit) records. The default value is 0 if not supplied.

`offset` : An alternative to `page`, offset can be used to return a set of (limit) results from a particular index.

`where` : An array of clauses to filter the JSON results. More information on filtering the results from /list/xxx API methods can be found here [Filtering Results](../API/filtering-results.md)

#### Return value JSON object:

```
[
    {
        "address": "822G7ZSsHh1ncTj4AJt6FnZ6MTWG3Q9NNTZ9KvG3CWA1SP3aq97",
        "created": 1581389015,
        "modified": 1581389107,
        "owner": "a2056d518d6e6d65c6c2e05af7fe2d3182a93def20e960fcfa0d35777a082440",
        "account": "8Bx6ZmCev3DsGjoWuhfQSNmycdZT4cyKKJNc36NWTMik6Zkqh7N",
        "recipient": "a2056d518d6e6d65c6c2e05af7fe2d3182a93def20e960fcfa0d35777a082440",
        "number": "0004",
        "PO": "Purch1234",
        "contact": "accounts@mycompany.com",
        "sender_detail": "My Company, 32 Some Street, Some Place",
        "recipient_detail": "Some recipient details such as address or email",
        "items": [
            {
                "description": "First item description",
                "base_price": 1.0,
                "tax": 0.1,
                "unit_amount": "1.1",
                "units": 3
            },
            {
                "description": "Second item description",
                "base_price": 5.0,
                "tax": 0.5,
                "unit_amount": "5.5",
                "units": 1
            }
        ],
        "amount": 8.8,
        "token": "0",
        "status": "PAID"
    }
]
```

#### Return values:

`owner` : The genesis hash of the signature chain that owns this invoice.

`created` : The UNIX timestamp when the invoice was created.

`modified` : The UNIX timestamp when the invoice was last modified.

`name` : The name identifying the invoice. For privacy purposes, this is only included in the response if the caller is the owner of the invoice

`address` : The register address of the invoice.

`recipient` : The genesis hash of the signature chain to issue the invoice to.

`account` : The register address of the account the invoice should be paid to.

`items` : Array of line items that make up this invoice.\
{\
`unit_amount` : The unit amount to be invoiced for this line item.

&#x20;  `units` : The number of units to be invoiced at the unit amount.\
}

`amount` : The total invoice amount. This is the sum of all line item total amounts (unit\_amount x units).

`token` : The register address of the token that this invoice should be paid in. Set to 0 for NXS transactions

`status` : The current status of this invoice. Values can be `OUTSTANDING` (the invoice has been issued but not paid), `PAID` (the invoice has been paid by the recipient), or `CANCELLED` (the invoice was cancelled by the issuer before payment).

***

## `list/items`

This will list off all of the supply chain items (append registers) owned by a signature chain.

**NOTE** : If you use the username parameter, it will take slightly longer to calculate the username genesis with our brute-force protected hashing algorithm. For higher performance, use the genesis parameter.

#### Endpoint:

`/users/list/items`

#### Parameters:

`genesis` : The genesis hash identifying the signature chain (optional if username is supplied).

`username` : The username identifying the signature chain (optional if genesis is supplied).

`limit` : The number of records to return for the current page. The default is 100.

`page` : Allows the results to be returned by page (zero based). E.g. passing in page=1 will return the second set of (limit) records. The default value is 0 if not supplied.

`offset` : An alternative to `page`, offset can be used to return a set of (limit) results from a particular index.

`where` : An array of clauses to filter the JSON results. More information on filtering the results from /list/xxx API methods can be found here [Filtering Results](../API/filtering-results.md)

#### Return value JSON object:

```
[
    {
        "created": 1560982537,
        "modified": 1560982615,
        "name": "myitem",
        "address": "8FJxzexVDUN5YiQYK4QjvfRNrAUym8FNu4B8yvYGXgKFJL8nBse",
        "owner": "2be51edcd41a8152bfedb24e3c22ee5a65d6d7d524146b399145bced269aeff0",
        "data": "xxxxxx"
    }
]
```

#### Return values:

`created` : The UNIX timestamp when the item was created.

`modified` : The UNIX timestamp when the item was last modified.

`name` : The name identifying the item. For privacy purposes, this is only included in the response if the caller is the owner of the item

`address` : The register address of the item.

`owner` : The genesis hash of the owner of the item.

`data` : The data stored in the item.

***

## `list/names`

This will list off all of the names owned by the signature chain. For privacy reasons Names are only returned for the currently logged in user (multiuser=0) or for the logged in session (multiuser=1)

**NOTE** : If you use the username parameter, it will take slightly longer to calculate the username genesis with our brute-force protected hashing algorithm. For higher performance, use the genesis parameter.

#### Endpoint:

`/users/list/names`

#### Parameters:

`session` : For multi-user API mode (configured with multiuser=1), the session is required to identify which signature chain should be used to return the Names for. For single-user API mode the session should not be supplied an the currently logged in users signature chain is used.

`limit` : The number of records to return for the current page. The default is 100.

`page` : Allows the results to be returned by page (zero based). E.g. passing in page=1 will return the second set of (limit) records. The default value is 0 if not supplied.

`offset` : An alternative to `page`, offset can be used to return a set of (limit) results from a particular index.

`where` : An array of clauses to filter the JSON results. More information on filtering the results from /list/xxx API methods can be found here [Filtering Results](../API/filtering-results.md)

#### Return value JSON object:

```
[

    {
        "name": "default",
        "address": "8CvLySLAWEKDB9SJSUDdRgzAG6ALVcXLzPQREN9Nbf7AzuJkg5P",
        "register_address": "8FJxzexVDUN5YiQYK4QjvfRNrAUym8FNu4B8yvYGXgKFJL8nBse"
    }


]
```

#### Return values:

`created` : The UNIX timestamp when the Name was created.

`modified` : The UNIX timestamp when the Name was last modified.

`name` : The name identifying the object register.

`address` : The register address of the Name.

`register_address` : The register address of the the object that this Name points to.

***

## `list/namespaces`

This will list off all of the namespaces owned by the signature chain.

**NOTE** : If you use the username parameter, it will take slightly longer to calculate the username genesis with our brute-force protected hashing algorithm. For higher performance, use the genesis parameter.

#### Endpoint:

`/users/list/namespaces`

#### Parameters:

`genesis` : The genesis hash identifying the signature chain (optional if username is supplied).

`username` : The username identifying the signature chain (optional if genesis is supplied).

`limit` : The number of records to return for the current page. The default is 100.

`page` : Allows the results to be returned by page (zero based). E.g. passing in page=1 will return the second set of (limit) records. The default value is 0 if not supplied.

`offset` : An alternative to `page`, offset can be used to return a set of (limit) results from a particular index.

`where` : An array of clauses to filter the JSON results. More information on filtering the results from /list/xxx API methods can be found here [Filtering Results](../API/filtering-results.md)

#### Return value JSON object:

```
[
    {
        "name": "mynamespace1",
        "address": "8CvLySLAWEKDB9SJSUDdRgzAG6ALVcXLzPQREN9Nbf7AzuJkg5P"
    },
    {
        "name": "mynamespace2",
        "address": "8FJxzexVDUN5YiQYK4QjvfRNrAUym8FNu4B8yvYGXgKFJL8nBse"
    }


]
```

#### Return values:

`created` : The UNIX timestamp when the namespace was created.

`modified` : The UNIX timestamp when the namespace was last modified.

`name` : The name identifying the namespace.

`address` : The register address of the namespace.

***

## `list/notifications`

This will list off all of the transactions sent to a particular genesis or username. It is useful for identifying transactions that you need to accept such as credits.

**NOTE** : If you use the username parameter, it will take slightly longer to calculate the username genesis with our brute-force protected hashing algorithm. For higher performance, use the genesis parameter.

#### Endpoint:

`/users/list/notifications`

#### Parameters:

`genesis` : The genesis hash identifying the signature chain (optional if username is supplied).

`username` : The username identifying the signature chain (optional if genesis is supplied).

`suppressed`: Optional boolean flag indicating that suppressed notifications should be included in the list.

`limit` : The number of records to return for the current page. The default is 100.

`page` : Allows the results to be returned by page (zero based). E.g. passing in page=1 will return the second set of (limit) records. The default value is 0 if not supplied.

`offset` : An alternative to `page`, offset can be used to return a set of (limit) results from a particular index.

`where` : An array of clauses to filter the JSON results. More information on filtering the results from /list/xxx API methods can be found here [Filtering Results](../API/filtering-results.md)

#### Return value JSON object:

```
[
    {
        "OP": "DEBIT",
        "from": "8FJxzexVDUN5YiQYK4QjvfRNrAUym8FNu4B8yvYGXgKFJL8nBse",
        "from_name": "abc",
        "to": "8CvLySLAWEKDB9SJSUDdRgzAG6ALVcXLzPQREN9Nbf7AzuJkg5P",
        "to_name": "myasset",
        "amount": 10.0,
        "reference": 0,
        "token": "8GHxzexVDUN5YiQYK4QjvfRNrAUym8FNu4B8yvYGXgKFJL8nAbD",
        "token_name": "abc",
        "txid": "01fa33c49901f3a2622b724d33e6bae238a8a0c3615facb4a00842b1b3a8545e275aff1015f1f2d04bffb3f47f54f64aa917702e3a8904c1be874ce5e969adb4",
        "time": 1566479032,
        "proof": "8FJxzexVDUN5YiQYK4QjvfRNrAUym8FNu4B8yvYGXgKFJL8nBse",
        "dividend_payment": true
    },
    {
        "OP": "TRANSFER",
        "address": "8HJxzexVDUN5YiQYK4QjvfRNrAUym8FNu4B8yvYGXgKFJL8n8Dc",
        "destination": "a2056d518d6e6d65c6c2e05af7fe2d3182a93def20e960fcfa0d35777a082440",
        "force": false,
        "txid": "0123200b85d720d85211d245fa4a38671622accd544f049262506eff242241733c5acfb396499e88a9156534b7d7943d554b59cbb38070fa323b16d52e439995",
        "time": 1566479610
    }

]

```

#### Return values:

An array of contracts deifned as:

`OP` : The contract operation. Can be `COINBASE`, `DEBIT`, `FEE`, `TRANSFER`.

`txid` : The transaction that was credited / claimed.

`suppressed`: If the caller included the optional `suppressed` flag, then the response will include this field to indicate whether this notification is currently being suppressed or not.

`from` : For `DEBIT` transactions, the register address of the account that the debit is being made from.

`from_name` : For `DEBIT` transactions, the name of the account that the debit is being made from. Only included if the name can be resolved.

`to` : For `DEBIT` transactions, the register address of the account that the debit is being made to.

`to_name` : For `DEBIT` transactions, the name of the account that the debit is being made to. Only included if the name can be resolved.

`amount` : the token amount of the transaction.

`token` : the register address of the token that the transaction relates to. Set to 0 for NXS transactions

`token_name` : The name of the token that the transaction relates to.

`reference` : For `DEBIT` transactions this is the user supplied reference used by the recipient to relate the transaction to an order or invoice number.

`proof` : The register address of the token account proving the the split dividend debit.

`dividend_payment` : Flag indicating that this debit is a split dividend payment to a tokenized asset

***

## `process/notifications`

Process all outstanding notifications for a logged in signature chain. This API method can be used instead of running the

#### Endpoint:

`/users/process/notifications`

#### Parameters:

`pin` : The PIN for this signature chain. This is required unless the signature chain is already unlocked for notifications.

`session` : For multi-user API mode, (configured with multiuser=1) the session is required to identify which session (sig-chain) the notifications should be processed for. For single-user API mode the session should not be supplied.

`suppressed`: Optional boolean flag indicating that suppressed notifications should be included when processing.

#### Return value JSON object:

```
{
    "processed": 7,
    "transactions": [
        {
            "txid": "01c4cf7b356d95bb783c31cd22119ff6dfcb5efc91dd1cf6c80f0cc4fd612dd7d03ed62955b0641f1b8985e38c2d7cd8fe7a07a98a46701b55c77559bd3c3152"
        }
    ]
}

```

#### Return values:

`processed` : The number of notifications (contracts) processed.

`transactions` : Array of transaction ID's (`txid`) for each transaction that was created to process the notifications. If there are more outstanding notifications than can fit in a single transaction, then additional transactions will be created until all notifications are processed.

**NOTE** : Some outstanding notifications may be skipped by this API call without resulting in an error message. For example this can happen if the individual notifications conditions cannot be satisfied.

***

## `list/tokens`

This will list off all of the tokens that were created by a particular signature chain.

**NOTE** : If you use the username parameter, it will take slightly longer to calculate the username genesis with our brute-force protected hashing algorithm. For higher performance, use the genesis parameter.

#### Endpoint:

`/users/list/tokens`

#### Parameters:

`genesis` : The genesis hash identifying the signature chain (optional if username is supplied).

`username` : The username identifying the signature chain (optional if genesis is supplied).

`count` : Optional boolean field that determines whether the response includes the transaction `count` field. This defaults to false, as including the transaction count can slow the response time of the method considerably.

`limit` : The number of records to return for the current page. The default is 100.

`page` : Allows the results to be returned by page (zero based). E.g. passing in page=1 will return the second set of (limit) records. The default value is 0 if not supplied.

`offset` : An alternative to `page`, offset can be used to return a set of (limit) results from a particular index.

`where` : An array of clauses to filter the JSON results. More information on filtering the results from /list/xxx API methods can be found here [Filtering Results](../API/filtering-results.md)

#### Return value JSON object:

```
[
    {
        "name": "mytoken1",
        "address": "8CvLySLAWEKDB9SJSUDdRgzAG6ALVcXLzPQREN9Nbf7AzuJkg5P",
        "balance": 0,
        "maxsupply": 26738388,
        "currentsupply": 26738388,
        "decimals": 2
    },
    {
        "name": "mytoken2",
        "address": "8FJxzexVDUN5YiQYK4QjvfRNrAUym8FNu4B8yvYGXgKFJL8nBse",
        "balance": 990,
        "maxsupply": 1000,
        "currentsupply": 10,
        "decimals": 8
    }


]
```

#### Return values:

`created` : The UNIX timestamp when the token was created.

`modified` : The UNIX timestamp when the token was last modified.

`name` : The name identifying the token. For privacy purposes, this is only included in the response if the caller is the owner of the token

`address` : The register address for the account

`balance` : The available balance of this token. Initially the balance of the token is equal to the max supply, until the tokens have been distributed to accounts. This is the last confirmed balance less any new debits that you have made since the last block.

`pending` : This is the sum of all confirmed debit transactions that have been made to this token, that have not yet been credited. To move tokens from pending into the available balance you must create a corresponding credit transaction. NOTE: if configured to run, the events processor does this for you.

`unconfirmed` : This is the sum of all unconfirmed debit transactions that have been made to this token PLUS the sum of all unconfirmed credits that you have for confirmed debit transactions. When someone makes a debit to the token it will immediately appear in the unconfirmed balance until that transaction is included in a block, at which point it moves into `pending`. When you (or the events processor) creates the corresponding credit transaction for that debit, the amount will move from `pending` back into `unconfirmed` until the credit transaction is included in a block, at which point the amount moves to `balance`.

`maxsupply` : The maximum available supply for this token.

`currentsupply` : The current number of tokens that have been distributed from this token register.

`decimals` : The maximum number of decimal places that can be applied to token amounts. For example decimals=2 will allow a token amount to be given to 2 decimal places.

`count` : Only returned if the caller requested `count` : true. This is the number of transactions made to/from the token.

***

## `list/transactions`

This will list off all of the transactions for the requested signature chain genesis or username (either can be used). You DO NOT need to be logged in to use this command. If you are using single-user API mode and are logged in, then neither username or genesis are required. It will return transactions for the currently logged in user.

**NOTE** : If you use the username parameter, it will take slightly longer to calculate the username genesis with our brute-force protected hashing algorithm. For higher performance, use the genesis parameter.

#### Endpoint:

`/users/list/transactions`

#### Parameters:

`genesis` : The genesis hash identifying the signature chain (optional if username is supplied).

`username` : The username identifying the signature chain (optional if genesis is supplied).

`verbose` : Optional, determines how much transaction data to include in the response. Supported values are :

* `default` : hash
* `summary` : type, version, sequence, timestamp, operation, and confirmations.
* `detail` : genesis, nexthash, prevhash, pubkey and signature.

`order` : The transaction order, based on signature chain sequence. 'asc' for oldest first, 'desc' for most recent first. The default is 'desc'.

`limit` : The number of records to return for the current page. The default is 100.

`page` : Allows the results to be returned by page (zero based). E.g. passing in page=1 will return the second set of (limit) records. The default value is 0 if not supplied.

`offset` : An alternative to `page`, offset can be used to return a set of (limit) results from a particular index.

`where` : An array of clauses to filter the JSON results. More information on filtering the results from /list/xxx API methods can be found here [Filtering Results](../API/filtering-results.md)

#### Return value JSON object:

```
{
    "txid": "222fafd56cf1fa5dc8d274d83dc49d1fcd4ecee958c62338fa865c7d43e5ed9f0abb843fdbf5b161dd1d1b9e2e64a9a1cb3a32657b8559b03ef60210257f8206",
    "contracts": [
        {
            "OP": "COINBASE",
            "genesis": "1ff463e036cbde3595fbe2de9dff15721a89e99ef3e2e9bfa7ce48ed825e9ec2",
            "nonce": 3,
            "amount": 76499504
        }
    ],
    "type": "tritium base",
    "version": 1,
    "sequence": 10,
    "timestamp": 1560196174,
    "confirmations": 6,
    "genesis": "1ff463e036cbde3595fbe2de9dff15721a89e99ef3e2e9bfa7ce48ed825e9ec2",
    "nexthash": "117ac949f88a65f6466e2b332e88abb1b7548cc7a09fdf6512ef4d96b44c1b2e",
    "prevhash": "cdfd0721fd860f52046a4752c6e67a30b11541662f89815091800f54c994455ecf20994651c38d32c0d8a3c3affc32d88977987c25af30c4c907b9666802c229",
    "pubkey": "095afa24f8dcdf15ac8c7701d7a0429ca866b2881ad5a2e59af77d8c6ebeb04a150b5ecc23d4c60593d583022cefea32cdca51f8dd476653208c72a14150ae85567e90f90e649a67fd1a32e501ea135ee88859e2606aa8ec22f524e784c8e9e9722401512028878fa1a56606e4481641b58295d6879e665f1e180777d011e709d120b1214f647bf68713be98c0b968d86815c22a1d9a7128e780e910c8ad82c595b20504772b8c842507d4a26a9345500829a1d4b28665507e1789a526eb6be60662fb4da11e0cd50bd78e698ba0f4220b1798bc64492910dc84876a6c95ace3563e7a23da96627d7b04a1284606d029567bb26a0a412a4a8a82a09dc7062968894b11c22410d8711b90e33f4d20a5385a969d69b05118d2d17585c999e1ef35a97444c8b2da5b54c1ce556da90477c2020bb780e402daa0f4bdee92b5745433f71e4a30a42816f518e2a0da4d38fa49ce136e18aadfcef9463ad641e05b4220f54139605c5328c27fd0d5cf1329f8928094aeebbd70f116e8271e7bb280e7f4aac58115bb0064920a36fcb028f50b0b376553a6af00334899abe5be0cad14b435b377c6514680a58072691947181fa0a85b70ad15e585265e7f8bc6c22954a5bf95065815cc18771eb1a486ad6810b80c4a38f92716500220804324ff01728e3b4e87b9a34984a1456f7ab14459e4859e62961eedbb788e820b4580d12fe0266aec3a82da88c190b94d715a01793c69bf16d94bb89abc5e116f03792aa2e8306604ce559415f74948ebb3e63b8371d0e58a0d5b2798aadeb4a5dafbd44a4ca27e0a523e57a49600c826208fe4932848eb82a7c96b32bb19bb5a2e1c64ffa31756d484eddd521eae75bcf633227783d7ad0150c6198c2754c2e166e34ac00cc488573d40b82ad650e25221e414b419c66450ec266516605759e985ec1cd3f7ebc89525cf67aa2401a3cc2469e686a97f00e47484499554fa2917616e9d9c5d6f42f9d32438910fd1c00bc10a2a9234856ec6021aa69b76da10299e73540ce3175328ccb68c0b1d1878e584bd40b60af0c323c49aad7efef783d5049dae18a53794a5432f30b6c4ec6e0a43dc11f5d869f04dbe5eb963240e716ce220c75a165e6a8b5b33cb318c985e50e5d26aa9c25c2b746b4f3edcc318cae0b060a8dcc628878070b60c24f9e35a9596a0c4e1278d1a9d5b33c8e76984340cf7ec991e767c071b47d47e2ba89cb99145bcae716f402fcdb5416ab7d3b0d50b1ec4399a076faa",
    "signature": "29381098b52dd52a0f4d2f82ec4fcab2c5044ad4f9bb299c713be43345b4c215c8125936ab3445a6bde280e5f22bbf35d588458ed5169f25f0711efc2299bc6d7885c96c4cd978149dd5a1bc3b554e9bc3b8c5cbaf1d3185202ca3778c42d1d8161eeab49c05b3733e90959e7d626e96b75adc5ba18e514f36cac68ad111e8f1a8e9a2710cd5252cc54121d0b39459ec49662531a250e64e1d2571c863c8697dc91f04551dfb3517555e91014f4a36a2763452ac1698d32cadd727b1dca320c6c0efc0a6cc63f8d8e850ec5592b0d6358b25fa0246f50a7ba89161cab487f4d6925f43a3978367300c59f1f64bac24cdb1ea6ce3d7457620c9b2c508a6619f29a91a19f816393b57503ff7059954dac507fd5ab756ee1c5b861352b9e5086fb6fe388c317e59778c79a7a1ccd2152543bf56ed58c8cfefb788ecef56c9ae84ab26f71545926d1c26b91a6bc20896b99087c3eb36720c9998cb7018c2247d321c7eadf4c6d1def93b451c4ddb94123ef8691fa83f81eddc426a5cc1afbeb1f615331a97eda571e57b8cf2b46eb25cdd19691d3b8723786e9b4476afdb7e99adc56e5d237d33311f09162c74fc24c5386458a4290a7d1b8b870fdeae68e689675b5290c9704d7f1323d1657bdc2c6be2914a7de447d7c475f8ce74487b6225958831cdbbc1887af2c8016141568bd51e039c648c6541578067b2c626fe40455791ae345d0d5702356567fcda5c24365ed6b039cd9b0286dffad2c982044ae2f0b38e76deac4b19e5bac5773e45eaa78125b13df9db726cffe8c6e266cb68520ce3febf4a66fd66225f639e095ea6bdb833529a5f2bb1c9b1c544c17dee717256b471ab703d11541d55f0f78bd60d7e671f9a22a2328b70fb034b8f6459f155a17c21ad2750ce7282257980ba571c6296"
},
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

&#x20;  `contract` : The ID of the contract within the transaction that was credited / claimed.

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

## `save/session`

This will save the users session to the local database, allowing the session to be resumed at a later time without the need to login or unlock. The users PIN is required as this is used (in conjunction with the genesis) to encrypt the session data before persisting it to the local database.

#### Endpoint:

`/users/save/session`

#### Parameters:

`pin` : The PIN for this signature chain.

#### Return value JSON object:

```
{
    "success": true
}
```

#### Return values:

`success` : Boolean flag indicating that the session was saved successfully .

***

## `load/session`

Loads a previously saved session from the local database. In addition to restoring the logged in status of the user, the session resumes the unlocked status that was active at the time it was saved. The users PIN is required as this is used (in conjunction with the genesis) to decrypt the session data loaded from the local database.

#### Endpoint:

`/users/load/session`

#### Parameters:

`genesis` : The genesis hash identifying the signature chain to load the session for (optional if username is supplied).

`username` : The username identifying the signature chain to load the session for (optional if genesis is supplied).

`pin` : The PIN for this signature chain.

#### Return value JSON object:

```
{
    "genesis": "a2e51edcd41a8152bfedb24e3c22ee5a65d6d7d524146b399145bced269aeff0",
    "session": "0000000000000000000000000000000000000000000000000000000000000000"
}

```

#### Return values:

`genesis` : The signature chain genesis hash is echoed back to confirm the session has been loaded for the correct user.

`session` : The session ID for the resumed session. NOTE: in multi-user mode this will be a NEW session ID, generated at the point the session is loaded. The previous session ID is not persisted and restored. When not in multi-user mode this will always be 0

***

## `has/session`

Determines whether a previously saved session exists in the local database for the specified user.

#### Endpoint:

`/users/has/session`

#### Parameters:

`genesis` : The genesis hash identifying the signature chain to check the session for (optional if username is supplied).

`username` : The username identifying the signature chain to check the session for (optional if genesis is supplied).

#### Return value JSON object:

```
{
    "genesis": "a2e51edcd41a8152bfedb24e3c22ee5a65d6d7d524146b399145bced269aeff0",
    "has": true
}

```

#### Return values:

`genesis` : The signature chain genesis hash is echoed back to confirm the session has been checked for the correct user.

`has` : Boolean flag indicating whether the session exists in the local database or not

***
