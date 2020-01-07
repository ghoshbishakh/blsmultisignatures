// Boneh-Lynn-Shacham short signatures demo.
//
// See the PBC_sig library for a practical implementation.
//
// Ben Lynn
#include <pbc.h>
#include <pbc_test.h>
#include <string.h>

int main(int argc, char **argv) {

	if (argc <= 5)
	{
		printf("Give parameters file input: pbc-0.5.14/param/a1.param, Hash, signature_file_path, Path to artifacts, key_id1 (1 for private_key1), key_id2, ..\n");
		return 0;
	}

	printf("Signature file: %s\n", argv[3]);
	printf("Path to artifacts: %s\n", argv[4]);
	printf("key id: %s\n", argv[5]);

	char public_key_path[500], g_path[500];
	char *signature_path, *hashinput;
	int hashlength;
	int numberofkeys = argc-5;
	int keys_included = 0;

	hashinput = argv[2];
	hashlength = strlen(hashinput);
	printf("Hash: %s\n", hashinput);
	printf("Hash Length: %d\n", hashlength);


	signature_path = argv[3];
	sprintf(public_key_path, "%s/public_key%s", argv[4], argv[5]);
	sprintf(g_path, "%s/g", argv[4]);
	
	printf("Path to g: %s\n", g_path);
	printf("Path to public_key: %s\n", public_key_path);
	printf("Path to signature: %s\n", signature_path);

	pairing_t pairing;
	element_t g, hash;
	element_t public_key1, public_key_combined, sig1;

	element_t temp1, temp2;


	pbc_demo_pairing_init(pairing, argc, argv);

	element_init_G2(g, pairing);
	element_init_G1(hash, pairing);

	element_init_G2(public_key1, pairing);
	element_init_G2(public_key_combined, pairing);

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

	printf("Reading signature =========================\n");

	{

		int n = pairing_length_in_bytes_compressed_G1(pairing);
	    unsigned char *data = pbc_malloc(n);

		FILE *fp;
		fp = fopen(signature_path, "r");
		fread(data, n, 1, fp);
		fclose(fp);

		element_from_bytes_compressed(sig1, data);
		element_printf("decompressed signature %B\n\n", sig1);
	}

	// Read public_key1 ==============================
	printf("Reading %s =========================\n", public_key_path);

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
	element_set(public_key_combined, public_key1);
	keys_included++;

	while(keys_included < numberofkeys){
		printf("Including key_id: %s\n", argv[5+keys_included]);
		sprintf(public_key_path, "%s/public_key%s", argv[4], argv[5+keys_included]);

		printf("Including key: %s\n", public_key_path);
		
		printf("Reading %s=========================\n", public_key_path);

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

		element_mul(public_key_combined, public_key1, public_key_combined);

		keys_included++;
	}

	//generate element from a hash
	//for toy pairings, should check that pairing(g, h) != 1
	element_from_hash(hash, hashinput, hashlength);
	element_printf("message hash = %B\n\n", hash);



	//verification part 1
	element_pairing(temp1, sig1, g);
	element_printf("f(sig1, g) = %B\n\n", temp1);

	//verification part 2
	//should match above
	element_pairing(temp2, hash, public_key_combined);
	element_printf("f(message hash, public_key) = %B\n\n", temp2);

	if (!element_cmp(temp1, temp2)) {
		printf("\n\n ==========> signature1 verifies\n\n");
	} else {
		printf("\n\n ==========> *BUG* signature does not verify *BUG*\n\n");
	}


	element_clear(sig1);
	element_clear(public_key1);
	element_clear(g);
	element_clear(hash);
	element_clear(temp1);
	element_clear(temp2);
	pairing_clear(pairing);
	return 0;
}
