target:
	gcc -o generate_keys generate_keys.c -I pbc-0.5.14/include/ -L /user/local/lib -l pbc -l gmp -static
	gcc -o test_signatures test_signatures.c -I pbc-0.5.14/include/ -L /user/local/lib -l pbc -l gmp -static
	gcc -o sign sign.c -I pbc-0.5.14/include/ -L /user/local/lib -l pbc -l gmp -static
	gcc -o verify verify.c -I pbc-0.5.14/include/ -L /user/local/lib -l pbc -l gmp -static
	gcc -o appendsignature appendsignature.c -I pbc-0.5.14/include/ -L /user/local/lib -l pbc -l gmp -static
	gcc -o appendsignature_to_signature appendsignature_to_signature.c -I pbc-0.5.14/include/ -L /user/local/lib -l pbc -l gmp -static
	gcc -o sign_to_file sign_to_file.c -I pbc-0.5.14/include/ -L /user/local/lib -l pbc -l gmp -static

