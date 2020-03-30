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
		printf("Give parameters file input: pbc-0.5.14/param/a1.param, Path to artifacts,\n Path to signature 1, Path to signature 2, Path to signature combined\n");
		return 0;
	}

	printf("Path to artifacts: %s\n", argv[2]);
	printf("key id: %s\n", argv[6]);

	char g_path[500], signature1_path[500], signature2_path[500], signature_combined_path[500];


	sprintf(g_path, "%s/g", argv[2]);
	sprintf(signature1_path, "%s", argv[3]);
	sprintf(signature2_path, "%s", argv[4]);
	sprintf(signature_combined_path, "%s", argv[5]);

	printf("Path to g: %s\n", g_path);

	pairing_t pairing;
	element_t g, hash;
	element_t sig1, sig2, sig_combined;

	element_t temp1, temp2;


	pbc_demo_pairing_init(pairing, argc, argv);

	element_init_G2(g, pairing);
	element_init_G1(hash, pairing);


	element_init_G1(sig1, pairing);
	element_init_G1(sig2, pairing);
	element_init_G1(sig_combined, pairing);

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



	printf("Reading signature 1==================================\n");

	{

		int n = pairing_length_in_bytes_compressed_G1(pairing);
		unsigned char *data = pbc_malloc(n);

		FILE *fp;
		fp = fopen(signature1_path, "r");
		fread(data, n, 1, fp);
		fclose(fp);

		element_from_bytes_compressed(sig1, data);
		element_printf("decompressed signature2 %B\n\n", sig1);
	}

	printf("Reading signature 2==================================\n");

	{

		int n = pairing_length_in_bytes_compressed_G1(pairing);
		unsigned char *data = pbc_malloc(n);

		FILE *fp;
		fp = fopen(signature2_path, "r");
		fread(data, n, 1, fp);
		fclose(fp);

		element_from_bytes_compressed(sig2, data);
		element_printf("decompressed signature2 %B\n\n", sig2);
	}

	printf("Combining signatures =============================\n");
	element_mul(sig_combined, sig1, sig2);
	element_printf("signature_combined = %B\n\n", sig_combined);



	// SAVE signature ====================================
	printf("Saving signature ==================================\n");


	{
		element_printf("original  sig_combined = %B\n\n", sig_combined);

		int n = pairing_length_in_bytes_compressed_G1(pairing);
		//int n = element_length_in_bytes_compressed(sig);
		int i;
		unsigned char *data = pbc_malloc(n);

		element_to_bytes_compressed(data, sig_combined);
		printf("compressed sig_combined = ");

		for (i = 0; i < n; i++) {
			printf("%02X", data[i]);
		}
		printf("\n\n");

		element_from_bytes_compressed(sig_combined, data);
		element_printf("decompressed sig_combined = %B\n\n", sig_combined);

		FILE *fp;
		fp = fopen(signature_combined_path, "w");
		fwrite(data, n, 1, fp);
		fclose(fp);

		pbc_free(data);
	}

	element_clear(sig1);
	element_clear(sig2);
	element_clear(sig_combined);
	element_clear(g);
	element_clear(hash);
	element_clear(temp1);
	element_clear(temp2);
	pairing_clear(pairing);
	return 0;
}
