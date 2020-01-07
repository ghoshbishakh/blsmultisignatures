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
	{
		int n = element_length_in_bytes(g);
		unsigned char *data = malloc(n);

		FILE *fp;
		fp = fopen("g", "r");
		fread(data, n, 1, fp);
		fclose(fp);

		element_from_bytes(g, data);
		element_printf("decompressed g %B\n\n", g);
		free(data);
	}

	// Read public_key1 ==============================
	{

		int n = element_length_in_bytes(public_key1);
		unsigned char *data = malloc(n);

		FILE *fp;
		fp = fopen("public_key1", "r");
		fread(data, n, 1, fp);
		fclose(fp);

		element_from_bytes(public_key1, data);
		element_printf("decompressed public_key1 %B\n\n", public_key1);
	}

	// Read secret_key1 ==============================
	{

		int n = element_length_in_bytes(secret_key1);
		unsigned char *data = malloc(n);

		FILE *fp;
		fp = fopen("secret_key1", "r");
		fread(data, n, 1, fp);
		fclose(fp);

		element_from_bytes(secret_key1, data);
		element_printf("decompressed secret_key1 %B\n\n", secret_key1);
	}


	//generate element from a hash
	//for toy pairings, should check that pairing(g, h) != 1
	element_from_hash(hash, "THISISHASH", 10);
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
