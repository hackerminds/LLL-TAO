# SYSTEM

## System API

***

The System API provides public access to information about this node. This includes data such as the version of software the node is running, ledger and mempool state, node IP address, and connected peers.

### `Methods`

The following methods are currently supported by this API

[`stop`](system.md#stop)\
[`get/info`](system.md#getinfo)\
[`get/metrics`](system.md#getmetrics)\
[`list/peers`](system.md#listpeers)\
[`list/lisp-eids`](system.md#listlisp-eids)\
[`validate/address`](system.md#validateaddress)

***

***

## `stop`

Initiate node shutdown sequence.

#### Endpoint:

`/system/stop`

#### Parameters:

```
NONE
```

#### Return value JSON object:

```
Nexus server stopping
```

***

## `get/info`

Returns a summary of information about this node

#### Endpoint:

`/system/get/info`

#### Parameters:

```
NONE
```

#### Return value JSON object:

```
{
    "version": "0.3.0.0 Tritium CLI [LLD][ECC][x64] Beta",
    "protocolversion": 20000,
    "walletversion": 10000,
    "hostname": "myhost",
    "ipaddress": "121.155.32.70",
    "testnet": 100,
    "private": false,
    "multiuser": true,
    "sessions": 1,
    "clientmode": true,
    "legacywallet": true,
    "blocks": 380,
    "synchronizing": true,
    "synccomplete": 31,
    "syncprogress": 3,
    "txtotal": 0,
    "connections": 1,
    "eids": [
        "240.139.76.244",
        "fe::139:76:244"
    ]
}
```

#### Return values:

`version` : The daemon software version.

`protocolversion` : The LLP version.

`walletversion` : Legacy wallet version.

`timestamp` : Current unified time as reported by this node.

`hostname` : The local machine host name.

`ipaddress` : The public IP address of the local machine, as seen by peers that you are connected to. This will be blank until you have at least one peer connection.

`testnet` : If this node is running on the testnet then this shows the testnet number.

`private` : Whether this node is running in private mode.

`multiuser` : Whether this node is running in multiuser mode.

`sessions` : Only returned in multiuser mode. This shows the number of user sessions currently logged in to the node.

`clientmode` : Flag indicating whether this node is running in client mode.

`legacy_unsupported` : Flag indicating whether this node has been compiled with support for the legacy UTxO wallet. If it has not then all of the legacy wallet functions (available via the legacy RPC) are not available. This flag is currently omitted from the response if it has been compiled with legacy support, hence it will only be included if it is true.

`blocks` : The current block height of this node.

`synchronizing` : Flag indicating whether this node is currently syncrhonizing. Not included if private=true

`synccomplete` : The percentage of the total blocks downloaded

`syncprogress` : The percentage of the current sync completed relative to when the node was started.

`txtotal` : Number of transactions in the node's mempool.

`connections` : Number of peer connections.

`eids` : Array of LISP Endpoint Idendifiers (EID's) for this node. Not included unless this node is using LISP.

***

## `get/metrics`

Returns metrics and statistics for the ledger, registers, etc

#### Endpoint:

`/system/get/metrics`

#### Parameters:

```
NONE
```

#### Return value JSON object:

```
{
    "registers": {
        "total": 54732,
        "account": 14190,
        "append": 0,
        "crypto": 13375,
        "name": 26847,
        "name_global": 2,
        "name_namespaced": 1,
        "namespace": 5,
        "object": 3,
        "object_tokenized": 1,
        "raw": 0,
        "readonly": 0,
        "token": 32
    },
    "sig_chains": 13375,
    "trust": {
        "total": 280,
        "stake": 21939510.0,
        "trust": 3004859583
    },
    "supply": {
        "total": 65177607.726929,
        "target": 64175336.948024,
        "inflationrate": 1.5617694063947738,
        "minute": 6.97545,
        "hour": 418.519742,
        "day": 10040.397562,
        "week": 70104.681744,
        "month": 277954.775646
    },
    "reserves": {
        "ambassador": 671.554894,
        "developer": 510.261963,
        "fee": 10946.79,
        "hash": 156.374712,
        "prime": 100.545744
    }
}

```

#### Return values:

`registers` : Statistics on the register database.\
{\
`total` : Total number of registers in the register database.

&#x20;  `account` : Number of account registers. This includes both NXS and other token accounts.

&#x20;  `append` : Number of append registers.

&#x20;  `crypto` : Number of crypto registers.

&#x20;  `name` : Total number of name registers, including user local, global, and namespaced.

&#x20;  `name_global` : Number of names that have been created in the global namespace.

&#x20;  `name_namespaced` : Number of names that have been created in a namespace.

&#x20;  `object` : Number of non-standard object registers. These include assets, items, and any other derived object register

&#x20;  `object_tokenized` : Number of object registers that have been tokenized (where the object owner is a token rather than a signature chain).

&#x20;  `raw` : Number of raw registers.

&#x20;  `readonly` : Number of read only registers.

&#x20;  `token` : Number of token registers.\
}

`sig_chains` : Total number of signature chains in the ledger.

`trust` : Statistics on active trust accounts.\
{\
`total` : Total number trust accounts that are active (have a non-zero stake balance).

&#x20;  `stake` : Total amount of NXS currently being staked network-wide).

&#x20;  `trust` : Total trust score of all trust accounts being staked. }

`supply` : Metrics on NXS supply rates.\
{\
`total` : Total amount of NXS in existence.

&#x20;  `target` : The target supply rate for this point in time.

&#x20;  `inflationrate` : The current inflation rate percentage based on the last year of data.

&#x20;  `minute` : The increase in supply rate within the last minute.

&#x20;  `hour` : The increase in supply rate within the last hour.

&#x20;  `day` : The increase in supply rate within the last day.

&#x20;  `week` : The increase in supply rate within the last week.

&#x20;  `month` : The increase in supply rate within the last 30 days.\
}

`reserves` : Information on the amount of NXS currently in the various reserves.\
{\
`ambassador` : Amount of NXS in the ambassador reserves, waiting to be paid to the ambassador sig chains.

&#x20;  `developer` : Amount of NXS in the developer reserves, waiting to be paid to the developer sig chains.

&#x20;  `fee` : Amount of NXS in the fee reserves.

&#x20;  `hash` : Amount of NXS in the hash channel reserves.

&#x20;  `ambassador` : Amount of NXS in the prime channel reserves.\
}

***

## `list/peers`

Returns a summary of information about the peers currently connected to this node. The return array is sorted by the peer `score` value.

#### Endpoint:

`/system/list/peers`

#### Parameters:

```
NONE
```

#### Return value JSON object:

```
[
    {
        "address": "144.76.165.123:8898",
        "version": "IPv4",
        "height": 498,
        "latency": "877 ms",
        "lastseen": 1557884710351,
        "connects": 2,
        "drops": 0,
        "fails": 0,
        "score": 200.0
    },
    {
        "address": "132.45.165.111:8898",
        "version": "IPv4",
        "height": 498,
        "latency": "125 ms",
        "lastseen": 1557884710352,
        "connects": 3,
        "drops": 1,
        "fails": 0,
        "score": 100.0
    }
]

```

#### Return values:

`address` : The IP address and port of the peer. This could be a LISP EID using the LISP overlay or an IP address using the native underlay.

`type` : The version string of the connected peer.

`version` : The protocol version being used to communicate.

`session` : Session ID for the current connection

`outgoing` : Flag indicating whether this was an outgoing connection or incoming

`height` : The current height of the peer.

`best` : Block hash of the peer's best chain

`latency` : The calculated network latency between this node and the peer.

`lastseen` : Unix timestamp of the last time this node had any communications with the peer.

`connects` : The number of connections successfully established with this peer since this node started.

`drops` : The number of connections dropped with this peer since this node started.

`fails` : The number of failed connection attempts to this peer since this node started.

`score` : The score value assigned to this peer based on latency and other connection statistics.

***

## `list\lisp-eids`

This will return the LISP Endpoint Indentifers (EID's) currently configured for this node. If the lispers.net API is not running / available then this will return an empty array.

#### Endpoint:

`/system/list/lisp-eids`

#### Parameters:

```
NONE
```

#### Return value JSON object:

```
[
    {
        "instance-id": "200",
        "eid": "240.139.76.244",
        "rlocs": [
            {
                "interface": "wlp59s0",
                "rloc-name": "mymachine",
                "rloc": "102.162.64.46"
            }
        ]
    },
    {
        "instance-id": "200",
        "eid": "fe::139:76:244",
        "rlocs": [
            {
                "interface": "wlp59s0",
                "rloc-name": "mymachine",
                "rloc": "102.162.64.46"
            }
        ]
    }
]
```

#### Return values:

`instance-id` : The LISP instance group that this EID belongs to.

`eid` : The endpoint identifier. This can be either IPv4 or IPv6 format

`rlocs` : The array of RLOC's (routing locators) associtated with the EID

`interface` : Name of the device for the RLOC

`rloc-name` : Hostname associated with the device

`rloc` : The IP address associated with the device.

***

## `validate/address`

Validates a register / legacy address .

#### Endpoint:

`/system/validate/address`

#### Parameters:

`address` : The base58 encoded register or legacy address to check.

#### Return value JSON object:

```
{
    "address": "8BuhE1y5oard3cEWqCSYK35bC6LdPRvcPR8N7GXYNatTw6De8eK",
    "is_valid": true,
    "type": "OBJECT",
    "object_type": "ACCOUNT"
}   
```

#### Return values:

`address` : The address that was validated.

`is_valid` : Boolean flag indicating if the address is valid or not. For legacy addresses this indicates that the address if simply formatted correctly. For register addresses this flag checks that a register exists in the blockchain at the specified address.

`type` : For register addresses, the type of register at the address. Values can be `APPEND`, `LEGACY`, `OBJECT`, `RAW`, `READONLY`, `RESERVED`, `SYSTEM`, or `UNKNOWN`.

`object_type` : If the `type` is `OBJECT` this field contains the standard object type of the object register. Values can be `ACCOUNT`, `CRYPTO`, `NAME`, `NAMESPACE`, `REGISTER`, `TOKEN`, `TRUST`, or `UNKNOWN`.

`is_mine` : If the `type` is `LEGACY` this boolean flag indicates if the private key for the address is held in the local wallet.

***
