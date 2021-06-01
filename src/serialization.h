#pragma once
#include <map>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

/**
 * Deserialize a `SignedKey` flatbuffers object
 */
void deserializeSignedKey(
    const uint8_t*  fb_signed_key,
    const uint8_t** sym_key,
    size_t*         sym_key_size,
    const uint8_t** sig,
    size_t*         sig_size
);

/**
 * Serialize a symmetric key and a corresponding signature
 * into a `SignedKey` flatbuffer object
 */
void serializeSignedKey(
    const uint8_t* key,
    size_t         key_size,
    uint8_t*       signature,
    size_t         signature_size,
    std::unique_ptr<uint8_t>& serialized_signed_key,
    size_t*        serialized_signed_key_size
);
