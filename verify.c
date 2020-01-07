// Boneh-Lynn-Shacham short signatures demo.
//
// See the PBC_sig library for a practical implementation.
//
// Ben Lynn
#include <pbc.h>
#include <pbc_test.h>

int main(int argc, char **argv) {

	if (argc <= 5)
	{
		printf("Give parameters file input: pbc-0.5.14/param/a1.param, Hash, Hash Length, signature_file_path, Path to artifacts, key_id1 (1 for private_key1), key_id2, ..\n");
		return 0;
	}

	printf("Hash: %s\n", argv[2]);
	printf("Hash Length: %s\n", argv[3]);
	printf("Signature file: %s\n", argv[4]);
	printf("Path to artifacts: %s\n", argv[5]);
	printf("key id: %s\n", argv[6]);

	char private_key_path[500], public_key_path[500], g_path[500];
	char *signature_path, *hashinput;
	int hashlength;

	hashinput = argv[2];
	hashlength = atoi(argv[3]);
	signature_path = argv[4];
	sprintf(private_key_path, "%s/private_key%s", argv[5], argv[6]);
	sprintf(public_key_path, "%s/public_key%s", argv[5], argv[6]);
	sprintf(g_path, "%s/g", argv[5]);
	
	printf("Path to g: %s\n", g_path);
	printf("Path to public_key: %s\n", public_key_path);
	printf("Path to private_key: %s\n", private_key_path);

	pairing_t pairing;
	element_t g, hash;
	element_t public_key1, sig1;

	element_t temp1, temp2;


	pbc_demo_pairing_init(pairing, argc, argv);

	element_init_G2(g, pairing);
	element_init_G1(hash, pairing);

	element_init_G2(public_key1, pairing);

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

	printf("Reading signature =========================\n");

	{

		int n = pairing_length_in_bytes_compressed_G1(pairing);
	    unsigned char *data = pbc_malloc(n);

		FILE *fp;
		fp = fopen(signature_path, "r");
		fread(data, n, 1, fp);
		fclose(fp);

		element_from_bytes_compressed(sig1, data);
		element_printf("decompressed public_key1 %B\n\n", sig1);
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
	element_pairing(temp2, hash, public_key1);
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
