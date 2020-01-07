// Boneh-Lynn-Shacham short signatures demo.
//
// See the PBC_sig library for a practical implementation.
//
// Ben Lynn
#include <pbc.h>
#include <pbc_test.h>
#include <stdlib.h>

#include<stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char **argv) {

	if (argc <= 2)
	{
		printf("Give parameters file input: pbc-0.5.14/param/a1.param, Count: number of keys required\n");
		return 0;
	}

	int N;
	struct stat st = {0};

	N = atoi(argv[2]);
	printf("Generating %d signatures..........\n\n", N);


	pairing_t pairing;
	element_t g, hash;
	element_t public_key1, sig1;
	element_t secret_key1;


	pbc_demo_pairing_init(pairing, argc, argv);

	element_init_G2(g, pairing);
	element_init_G1(hash, pairing);

	element_init_G2(public_key1, pairing);
	element_init_Zr(secret_key1, pairing);


	//generate system parameters
	element_random(g);
	element_printf("system parameter g = %B\n\n", g);

	printf("Creating artifacts directory ........\n");
	if (stat("artifacts", &st) == -1) {
	    mkdir("artifacts", 0700);
	}
	// SAVING g
	printf("Savig g ............\n");
	{
		element_printf("original g = %B\n\n", g);

		int n = element_length_in_bytes(g);
		//int n = element_length_in_bytes_compressed(sig);
		int i;
		unsigned char *data = malloc(n);
		printf("Converting to bytes..\n");
		element_to_bytes(data, g);
		printf("compressed g = ");
		for (i = 0; i < n; i++) {
			printf("%02X", data[i]);
		}
		printf("\n\n");

		element_from_bytes(g, data);
		element_printf("decompressed g %B\n\n", g);

		FILE *fp;
		fp = fopen("artifacts/g", "w");
		fwrite(data, n, 1, fp);
		fclose(fp);
		free(data);
	}



	char public_key_name[50], private_key_name[50];

	for (int i = 1; i <= N; ++i)
	{

		sprintf(public_key_name, "artifacts/public_key%d", i);
		sprintf(private_key_name, "artifacts/private_key%d", i);

		//generate private key
		element_random(secret_key1);
		element_printf("private key1 = %B\n\n", secret_key1);

		//compute corresponding public key
		element_pow_zn(public_key1, g, secret_key1);
		element_printf("public key1 = %B\n\n", public_key1);




		// SAVING publickey
		printf("Savig public_key%d  ............\n", i);

		{
			element_printf("original public_key1 = %B\n\n", public_key1);

			int n = element_length_in_bytes(public_key1);
		//int n = element_length_in_bytes_compressed(sig);
			int j;
			unsigned char *data = malloc(n);
			printf("Converting to bytes..\n");
			element_to_bytes(data, public_key1);
			printf("compressed public_key1 = ");
			for (j = 0; j < n; j++) {
				printf("%02X", data[j]);
			}
			printf("\n\n");

			element_from_bytes(public_key1, data);
			element_printf("decompressed public_key1 %B\n\n", public_key1);

			FILE *fp;
			fp = fopen(public_key_name, "w");
			fwrite(data, n, 1, fp);
			fclose(fp);
			free(data);
		}

		// SAVING secretkey
		printf("Savig private_key%d  ............\n", i);

		{
			element_printf("original secret_key1 = %B\n\n", secret_key1);

			int n = element_length_in_bytes(secret_key1);
		//int n = element_length_in_bytes_compressed(sig);
			int j;
			unsigned char *data = malloc(n);
			printf("Converting to bytes..\n");
			element_to_bytes(data, secret_key1);
			printf("compressed secret_key1 = ");
			for (j = 0; j < n; j++) {
				printf("%02X", data[j]);
			}
			printf("\n\n");

			element_from_bytes(secret_key1, data);
			element_printf("decompressed secret_key1 %B\n\n", secret_key1);

			FILE *fp;
			fp = fopen(private_key_name, "w");
			fwrite(data, n, 1, fp);
			fclose(fp);
			free(data);
		}
	}







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

	element_clear(g);
	element_clear(public_key1);
	element_clear(secret_key1);
	pairing_clear(pairing);
	return 0;
}
