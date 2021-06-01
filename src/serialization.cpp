#include "serialization.h"

#include "SignedKey_generated.h"

using namespace tuix;


void deserializeSignedKey(
    const uint8_t*  fb_signed_key,
    const uint8_t** sym_key,
    size_t*         sym_key_size,
    const uint8_t** sig,
    size_t*         sig_size
) {
    // Deserialize `SignedKey` table
    auto root = GetSignedKey(fb_signed_key);

    // Extract data
    *sym_key = root->key()->data();
    *sig = root->sig()->data();

    // Extract data sizes
    *sym_key_size = root->key()->size();
    *sig_size = root->sig()->size();
}


void serializeSignedKey(
    const uint8_t* key,
    size_t         key_size,
    uint8_t*       signature,
    size_t         signature_size,
    std::unique_ptr<uint8_t>& serialized_signed_key,
    size_t*        serialized_signed_key_size
) {
    // Create the Flatbuffer Builder
    flatbuffers::FlatBufferBuilder signed_key_builder;

    // Create the SignedKey serialized buffer
    auto fb_key = signed_key_builder.CreateVector(key, key_size);
    auto fb_sig = signed_key_builder.CreateVector(signature, signature_size);
    auto fb_signed_key = CreateSignedKey(signed_key_builder, fb_key, fb_sig);

    signed_key_builder.Finish(fb_signed_key);

    // Retrieve the buffer containing the serialized signed key
    // and return it to calling function
    size_t fb_buffer_size = signed_key_builder.GetSize();
    *serialized_signed_key_size = fb_buffer_size;

    serialized_signed_key = std::unique_ptr<uint8_t>(new uint8_t[fb_buffer_size]);
    memcpy(serialized_signed_key.get(), signed_key_builder.GetBufferPointer(), fb_buffer_size);

    signed_key_builder.Clear();
}
