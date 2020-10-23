/*__________________________________________________________________________________________

            (c) Hash(BEGIN(Satoshi[2010]), END(Sunny[2012])) == Videlicet[2014] ++

            (c) Copyright The Nexus Developers 2014 - 2019

            Distributed under the MIT software license, see the accompanying
            file COPYING or http://www.opensource.org/licenses/mit-license.php.

            "ad vocem populi" - To the Voice of the People

____________________________________________________________________________________________*/

#pragma once
#ifndef NEXUS_LLD_CONFIG_HASHMAP_H
#define NEXUS_LLD_CONFIG_HASHMAP_H

#include <cstdint>
#include <mutex>

#include <LLD/hash/xxhash.h>
#include <LLD/config/base.h>

namespace LLD::Config
{
    /** Structure to contain the configuration variables for a BinaryHashMap Keychain. **/
    class Hashmap : public Base
    {
    public:

        /** The maximum number of hashmap files per shard. **/
        uint32_t MAX_HASHMAP_FILES;


        /** The maximum number of linear probes per key. **/
        uint32_t MIN_LINEAR_PROBES;


        /** The maximum number of cycles allowed to expand in probing. **/
        uint32_t MAX_LINEAR_PROBES;


        /** Maximum size a file can be in the keychain. **/
        uint64_t MAX_HASHMAP_FILE_SIZE;


        /** Maximum number of open file streams. **/
        uint16_t MAX_HASHMAP_FILE_STREAMS;


        /** The number of k-hashes for the primary bloom filter. **/
        uint32_t PRIMARY_BLOOM_HASHES;


        /** The number of bits in the primary bloom filter. **/
        uint32_t PRIMARY_BLOOM_BITS;


        /** The number of k-hashes for the secondary bloom filter. **/
        uint32_t SECONDARY_BLOOM_HASHES;


        /** The number of bits per secondary bloom filter. **/
        uint32_t SECONDARY_BLOOM_BITS;


        /** The Maximum buckets allowed in the hashmap. **/
        uint32_t HASHMAP_TOTAL_BUCKETS;


        /** The total space that a key consumes. **/
        const uint16_t HASHMAP_KEY_ALLOCATION;


        /** Flag to determine if initialization should be fast or slow. **/
        bool QUICK_INIT;


        /** No empty constructor. **/
        Hashmap() = delete;


        /** Default constructor uses optimum values for bloom filters. **/
        Hashmap(const Base& base)
        : Base                     (base)
        , MAX_HASHMAP_FILES        (256)
        , MIN_LINEAR_PROBES        (3) //default of 3 linear probes before moving to next hashmap file
        , MAX_LINEAR_PROBES        (3) //default of 3 fibanacci probing cycles before exhausting bucket
        , MAX_HASHMAP_FILE_SIZE    (1024 * 1024 * 512) //512 MB filesize by default
        , MAX_HASHMAP_FILE_STREAMS (MAX_HASHMAP_FILES) //default of maximum hashmap files, otherwise you will degrade performance
        , PRIMARY_BLOOM_HASHES     (7)
        , PRIMARY_BLOOM_BITS       (1.44 * MAX_HASHMAP_FILES * PRIMARY_BLOOM_HASHES)
        , SECONDARY_BLOOM_HASHES   (7)
        , SECONDARY_BLOOM_BITS     (13)
        , HASHMAP_TOTAL_BUCKETS    (77773)
        , HASHMAP_KEY_ALLOCATION   (16 + 13) //16 bytes for key checksum, 13 bytes for ckey class
        , QUICK_INIT               (true)   //this only really gives us total keys output and makes startup a little slower
        , KEYCHAIN_LOCKS           (1024)
        , FILESYSTEM_LOCKS         (MAX_HASHMAP_FILE_STREAMS)
        {
        }


        /** Copy Constructor. **/
        Hashmap(const Hashmap& map)
        : Base                     (map)
        , MAX_HASHMAP_FILES        (map.MAX_HASHMAP_FILES)
        , MIN_LINEAR_PROBES        (map.MIN_LINEAR_PROBES)
        , MAX_LINEAR_PROBES        (map.MAX_LINEAR_PROBES)
        , MAX_HASHMAP_FILE_SIZE    (map.MAX_HASHMAP_FILE_SIZE)
        , MAX_HASHMAP_FILE_STREAMS (map.MAX_HASHMAP_FILE_STREAMS)
        , PRIMARY_BLOOM_HASHES     (map.PRIMARY_BLOOM_HASHES)
        , PRIMARY_BLOOM_BITS       (map.PRIMARY_BLOOM_BITS)
        , SECONDARY_BLOOM_HASHES   (map.SECONDARY_BLOOM_HASHES)
        , SECONDARY_BLOOM_BITS     (map.SECONDARY_BLOOM_BITS)
        , HASHMAP_TOTAL_BUCKETS    (map.HASHMAP_TOTAL_BUCKETS)
        , HASHMAP_KEY_ALLOCATION   (map.HASHMAP_KEY_ALLOCATION)
        , QUICK_INIT               (map.QUICK_INIT)
        , KEYCHAIN_LOCKS           (map.KEYCHAIN_LOCKS.size())
        , FILESYSTEM_LOCKS         (map.FILESYSTEM_LOCKS.size())
        {
        }


        /** Move Constructor. **/
        Hashmap(Hashmap&& map)
        : Base                     (std::move(map))
        , MAX_HASHMAP_FILES        (std::move(map.MAX_HASHMAP_FILES))
        , MIN_LINEAR_PROBES        (std::move(map.MIN_LINEAR_PROBES))
        , MAX_LINEAR_PROBES        (std::move(map.MAX_LINEAR_PROBES))
        , MAX_HASHMAP_FILE_SIZE    (std::move(map.MAX_HASHMAP_FILE_SIZE))
        , MAX_HASHMAP_FILE_STREAMS (std::move(map.MAX_HASHMAP_FILE_STREAMS))
        , PRIMARY_BLOOM_HASHES     (std::move(map.PRIMARY_BLOOM_HASHES))
        , PRIMARY_BLOOM_BITS       (std::move(map.PRIMARY_BLOOM_BITS))
        , SECONDARY_BLOOM_HASHES   (std::move(map.SECONDARY_BLOOM_HASHES))
        , SECONDARY_BLOOM_BITS     (std::move(map.SECONDARY_BLOOM_BITS))
        , HASHMAP_TOTAL_BUCKETS    (std::move(map.HASHMAP_TOTAL_BUCKETS))
        , HASHMAP_KEY_ALLOCATION   (std::move(map.HASHMAP_KEY_ALLOCATION))
        , QUICK_INIT               (std::move(map.QUICK_INIT))
        , KEYCHAIN_LOCKS           (map.KEYCHAIN_LOCKS.size())
        , FILESYSTEM_LOCKS         (map.FILESYSTEM_LOCKS.size())
        {
        }


        /** Copy Assignment **/
        Hashmap& operator=(const Hashmap& map)
        {
            /* Database configuration. */
            DIRECTORY                = map.DIRECTORY;
            NAME                     = map.NAME;
            FLAGS                    = map.FLAGS;

            /* Hashmap configuration. */
            MAX_HASHMAP_FILES        = map.MAX_HASHMAP_FILES;
            MIN_LINEAR_PROBES        = map.MIN_LINEAR_PROBES;
            MAX_LINEAR_PROBES        = map.MAX_LINEAR_PROBES;
            MAX_HASHMAP_FILE_SIZE    = map.MAX_HASHMAP_FILE_SIZE;
            MAX_HASHMAP_FILE_STREAMS = map.MAX_HASHMAP_FILE_STREAMS;
            PRIMARY_BLOOM_HASHES     = map.PRIMARY_BLOOM_HASHES;
            PRIMARY_BLOOM_BITS       = map.PRIMARY_BLOOM_BITS;
            SECONDARY_BLOOM_HASHES   = map.SECONDARY_BLOOM_HASHES;
            SECONDARY_BLOOM_BITS     = map.SECONDARY_BLOOM_BITS;
            HASHMAP_TOTAL_BUCKETS    = map.HASHMAP_TOTAL_BUCKETS;

            return *this;
        }


        /** Move Assignment **/
        Hashmap& operator=(Hashmap&& map)
        {
            /* Database configuration. */
            DIRECTORY                = std::move(map.DIRECTORY);
            NAME                     = std::move(map.NAME);
            FLAGS                    = std::move(map.FLAGS);

            /* Hashmap configuration. */
            MAX_HASHMAP_FILES        = std::move(map.MAX_HASHMAP_FILES);
            MIN_LINEAR_PROBES        = std::move(map.MIN_LINEAR_PROBES);
            MAX_LINEAR_PROBES        = std::move(map.MAX_LINEAR_PROBES);
            MAX_HASHMAP_FILE_SIZE    = std::move(map.MAX_HASHMAP_FILE_SIZE);
            MAX_HASHMAP_FILE_STREAMS = std::move(map.MAX_HASHMAP_FILE_STREAMS);
            PRIMARY_BLOOM_HASHES     = std::move(map.PRIMARY_BLOOM_HASHES);
            PRIMARY_BLOOM_BITS       = std::move(map.PRIMARY_BLOOM_BITS);
            SECONDARY_BLOOM_HASHES   = std::move(map.SECONDARY_BLOOM_HASHES);
            SECONDARY_BLOOM_BITS     = std::move(map.SECONDARY_BLOOM_BITS);
            HASHMAP_TOTAL_BUCKETS    = std::move(map.HASHMAP_TOTAL_BUCKETS);

            return *this;
        }


        /** Default Destructor. **/
        ~Hashmap()
        {
        }


        /** KeychainLock
         *
         *  Grabs a lock from the set of keychain locks by key data.
         *
         *  @param[in] vKey The binary data of key to lock for.
         *
         *  @return a reference of the lock object.
         *
         **/
        std::mutex& KEYCHAIN(const std::vector<uint8_t>& vKey) const
        {
            /* Calculate the lock that will be obtained by the given key. */
            uint64_t nLock = XXH3_64bits((uint8_t*)&vKey[0], vKey.size()) % KEYCHAIN_LOCKS.size();
            return KEYCHAIN_LOCKS[nLock];
        }


        /** File
         *
         *  Grabs a lock from the set of keychain locks by file handle.
         *
         *  @param[in] nFile The binary data of key to lock for.
         *
         *  @return a reference of the lock object.
         *
         **/
        std::mutex& FILE(const uint32_t nFile) const
        {
            /* Calculate the lock that will be obtained by the given key. */
            uint64_t nLock = XXH3_64bits((uint8_t*)&nFile, 4) % FILESYSTEM_LOCKS.size();
            return FILESYSTEM_LOCKS[nLock];
        }

    private:


        /** The keychain level locking hashmap. **/
        mutable std::vector<std::mutex> KEYCHAIN_LOCKS;


        /** The keychain level locking hashmap. **/
        mutable std::vector<std::mutex> FILESYSTEM_LOCKS;

    };
}

#endif
