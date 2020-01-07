// Boneh-Lynn-Shacham short signatures demo.
//
// See the PBC_sig library for a practical implementation.
//
// Ben Lynn
#include <pbc.h>
#include <pbc_test.h>
#include <string.h>

int main(int argc, char **argv) {

	if (argc <= 4)
	{
		printf("Give parameters file input: pbc-0.5.14/param/a1.param, Hash, Path to artifacts, key_id (1 for private_key1)\n");
		return 0;
	}

	printf("Hash: %s\n", argv[2]);
	printf("Path to artifacts: %s\n", argv[3]);
	printf("key id: %s\n", argv[4]);

	char private_key_path[500], public_key_path[500], g_path[500], signature_path[500];
	char *hashstring;
	int hashlength;

	hashstring = argv[2];
	hashlength = strlen(hashstring);
	sprintf(private_key_path, "%s/private_key%s", argv[3], argv[4]);
	sprintf(public_key_path, "%s/public_key%s", argv[3], argv[4]);
	sprintf(g_path, "%s/g", argv[3]);
	sprintf(signature_path, "%s.signature", argv[2]);

	printf("Path to g: %s\n", g_path);
	printf("Path to public_key: %s\n", public_key_path);
	printf("Path to private_key: %s\n", private_key_path);
	printf("Hash length: %d\n", hashlength);

	pairing_t pairing;
	element_t g, hash;
	element_t public_key1, sig1;
	element_t secret_key1;

	element_t temp1, temp2;


	pbc_demo_pairing_init(pairing, argc, argv);

	element_init_G2(g, pairing);
	element_init_G1(hash, pairing);

	element_init_G2(public_key1, pairing);
	element_init_Zr(secret_key1, pairing);

	element_init_G1(sig1, pairing);

	element_init_GT(temp1, pairing);
	element_init_GT(temp2, pairing);

	// Read g ==============================
	printf("Reading g =========================\n");
	{
		int n = element_length_in_bytes(g);
		unsigned char *data = malloc(n);

		FILE *fp;
		fp = fopen(g_path, "r");
		fread(data, n, 1, fp);
		fclose(fp);

		element_from_bytes(g, data);
		element_printf("decompressed g %B\n\n", g);
		free(data);
	}

	// Read public_key1 ==============================
	printf("Reading public_key =========================\n");

	{

		int n = element_length_in_bytes(public_key1);
		unsigned char *data = malloc(n);

		FILE *fp;
		fp = fopen(public_key_path, "r");
		fread(data, n, 1, fp);
		fclose(fp);

		element_from_bytes(public_key1, data);
		element_printf("decompressed public_key1 %B\n\n", public_key1);
	}

	// Read secret_key1 ==============================
	printf("Reading private_key =========================\n");

	{

		int n = element_length_in_bytes(secret_key1);
		unsigned char *data = malloc(n);

		FILE *fp;
		fp = fopen(private_key_path, "r");
		fread(data, n, 1, fp);
		fclose(fp);

		element_from_bytes(secret_key1, data);
		element_printf("decompressed secret_key1 %B\n\n", secret_key1);
	}


	//generate element from a hash
	//for toy pairings, should check that pairing(g, h) != 1
	element_from_hash(hash, hashstring, hashlength);
	element_printf("message hash = %B\n\n", hash);

	//h^secret_key is the signature
	//in real life: only output the first coordinate
	element_pow_zn(sig1, hash, secret_key1);
	element_printf("signature1 = %B\n\n", sig1);



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

	// SAVE signature ====================================
	printf("Saving signature ==================================\n");


	{
	    element_printf("original  sig1 = %B\n\n", sig1);

	    int n = pairing_length_in_bytes_compressed_G1(pairing);
	    //int n = element_length_in_bytes_compressed(sig);
	    int i;
	    unsigned char *data = pbc_malloc(n);

	    element_to_bytes_compressed(data, sig1);
	    printf("compressed sig1 = ");
	    for (i = 0; i < n; i++) {
	      printf("%02X", data[i]);
	    }
	    printf("\n\n");

	    element_from_bytes_compressed(sig1, data);
	    element_printf("decompressed sig1 = %B\n\n", sig1);

	    FILE *fp;
		fp = fopen(signature_path, "w");
		fwrite(data, n, 1, fp);
		fclose(fp);

	    pbc_free(data);
	}

	element_clear(sig1);
	element_clear(public_key1);
	element_clear(secret_key1);
	element_clear(g);
	element_clear(hash);
	element_clear(temp1);
	element_clear(temp2);
	pairing_clear(pairing);
	return 0;
}
