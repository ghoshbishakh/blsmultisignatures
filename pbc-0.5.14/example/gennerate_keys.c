// Boneh-Lynn-Shacham short signatures demo.
//
// See the PBC_sig library for a practical implementation.
//
// Ben Lynn
#include <pbc.h>
#include <pbc_test.h>

int main(int argc, char **argv) {
  pairing_t pairing;
  element_t g, hash;
  element_t public_key1, sig1;
  element_t public_key2, sig2;
  element_t secret_key1, secret_key2;

  element_t public_key_combined;
  element_t sig_combined;

  element_t temp1, temp2;


  pbc_demo_pairing_init(pairing, argc, argv);

  element_init_G2(g, pairing);
  element_init_G1(hash, pairing);

  element_init_G2(public_key1, pairing);
  element_init_G2(public_key2, pairing);
  element_init_G2(public_key_combined, pairing);

  element_init_Zr(secret_key1, pairing);
  element_init_Zr(secret_key2, pairing);

  element_init_G1(sig1, pairing);
  element_init_G1(sig2, pairing);
  element_init_G1(sig_combined, pairing);

  element_init_GT(temp1, pairing);
  element_init_GT(temp2, pairing);

  printf("Short signature test\n\n");

  //generate system parameters
  element_random(g);
  element_printf("system parameter g = %B\n\n", g);

  //generate private key
  element_random(secret_key1);
  element_printf("private key1 = %B\n\n", secret_key1);
  element_random(secret_key2);
  element_printf("private key2 = %B\n\n", secret_key2);

  //compute corresponding public key
  element_pow_zn(public_key1, g, secret_key1);
  element_pow_zn(public_key2, g, secret_key2);
  element_printf("public key1 = %B\n\n", public_key1);
  element_printf("public key2 = %B\n\n", public_key2);

  //generate element from a hash
  //for toy pairings, should check that pairing(g, h) != 1
  element_from_hash(hash, "THISISHASH", 10);
  element_printf("message hash = %B\n\n", hash);

  //h^secret_key is the signature
  //in real life: only output the first coordinate
  element_pow_zn(sig1, hash, secret_key1);
  element_printf("signature1 = %B\n\n", sig1);

  element_pow_zn(sig2, hash, secret_key2);
  element_printf("signature2 = %B\n\n", sig2);

  // SIGNATURE COMBINED
  element_mul(sig_combined, sig1, sig2);
  element_printf("signature_combined = %B\n\n", sig_combined);


  //verification part 1
  element_pairing(temp1, sig1, g);
  element_printf("f(sig1, g) = %B\n\n", temp1);

  //verification part 2
  //should match above
  element_pairing(temp2, hash, public_key1);
  element_printf("f(message hash, public_key) = %B\n\n", temp2);

  if (!element_cmp(temp1, temp2)) {
    printf("\n\n ==========> signature1 verifies\n\n");
  } else {
    printf("\n\n ==========> *BUG* signature does not verify *BUG*\n\n");
  }

  // Verification signature 2

  element_pairing(temp1, sig2, g);
  element_printf("f(sig1, g) = %B\n\n", temp1);

  //verification part 2
  //should match above
  element_pairing(temp2, hash, public_key2);
  element_printf("f(message hash, public_key) = %B\n\n", temp2);

  if (!element_cmp(temp1, temp2)) {
    printf("\n\n ==========> signature2 verifies\n\n");
  } else {
    printf("\n\n ==========> *BUG* signature does not verify *BUG*\n\n");
  }

  // Verification signature combined
  element_mul(public_key_combined, public_key1, public_key2);

  element_pairing(temp1, sig_combined, g);
  element_printf("f(sig1, g) = %B\n\n", temp1);

  element_pairing(temp2, hash, public_key_combined);
  element_printf("f(message hash, public_key) = %B\n\n", temp2);

  if (!element_cmp(temp1, temp2)) {
    printf("\n\n ==========> signature_combined verifies!\n\n");
  } else {
    printf("\n\n ==========> *BUG* signature does not verify *BUG* ===================\n\n");
  }



  // ================== SAVING SIGNATURE AND PARING ==================================
  // {
  //   element_printf("original  sig_combined = %B\n\n", sig_combined);

  //   int n = pairing_length_in_bytes_compressed_G1(pairing);
  //   //int n = element_length_in_bytes_compressed(sig);
  //   int i;
  //   unsigned char *data = pbc_malloc(n);

  //   element_to_bytes_compressed(data, sig_combined);
  //   printf("compressed sig_combined = ");
  //   for (i = 0; i < n; i++) {
  //     printf("%02X", data[i]);
  //   }
  //   printf("\n\n");

  //   element_from_bytes_compressed(sig_combined, data);
  //   element_printf("decompressed sig_combined = %B\n\n", sig_combined);

  //   pbc_free(data);
  // }

printf("==========================================================\n");

  // SAVING KEYS
  // {
    element_printf("original  secret_key1 = %B\n\n", secret_key1);

    int n = element_length_in_bytes(secret_key1);
  //   //int n = element_length_in_bytes_compressed(sig);
    int i;
    unsigned char *data = malloc(n);
    printf("Converting to bytes..\n");
    element_to_bytes(data, secret_key1);
    printf("compressed secret_key1 = ");
    for (i = 0; i < n; i++) {
      printf("%02X", data[i]);
    }
    printf("\n\n");

    element_from_bytes(secret_key1, data);
    element_printf("decompressed secret_key1 = %B\n\n", secret_key1);

    free(data);
  // }


  // {
  //   int n = pairing_length_in_bytes_x_only_G1(pairing);
  //   //int n = element_length_in_bytes_x_only(sig);
  //   int i;
  //   unsigned char *data = pbc_malloc(n);

  //   element_to_bytes_x_only(data, sig1);
  //   printf("x-coord = ");
  //   for (i = 0; i < n; i++) {
  //     printf("%02X", data[i]);
  //   }
  //   printf("\n");

  //   element_from_bytes_x_only(sig1, data);
  //   element_printf("de-x-ed = %B\n", sig1);

  //   element_pairing(temp1, sig1, g);
  //   if (!element_cmp(temp1, temp2)) {
  //     printf(" ===============> signature verifies on first guess\n");
  //   } else {
  //     element_invert(temp1, temp1);
  //     if (!element_cmp(temp1, temp2)) {
  //       printf(" =============> signature verifies on second guess\n");
  //     } else {
  //       printf(" =============> *BUG* signature does not verify *BUG*\n");
  //     }
  //   }

  //   pbc_free(data);
  // }

  //a random signature shouldn't verify
  // element_random(sig1);
  // element_pairing(temp1, sig1, g);
  // if (element_cmp(temp1, temp2)) {
  //   printf("random signature doesn't verify\n");
  // } else {
  //   printf("*BUG* random signature verifies *BUG*\n");
  // }

  element_clear(sig1);
  element_clear(sig2);
  element_clear(public_key1);
  element_clear(public_key2);
  element_clear(secret_key1);
  element_clear(secret_key2);
  element_clear(g);
  element_clear(hash);
  element_clear(temp1);
  element_clear(temp2);
  pairing_clear(pairing);
  return 0;
}
