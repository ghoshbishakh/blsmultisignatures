# BLS Multisignatures

Based on the PBC Library: [https://crypto.stanford.edu/pbc/](https://crypto.stanford.edu/pbc/)

## Dependency

1. [PBC Library](https://crypto.stanford.edu/pbc/)

which depends on:

2. [GMP library](https://gmplib.org/)


## How to use

#### Compiling:

Change the `Makefile` by updating the path to your installed PBC Library.

```
make
```

#### Generate Keys

```
./generate_keys <parameters file input> <number of keys to be generated>
```

Generates keys and g in the `artifacts` directory. Each key is named as public_keyX, private_keyX, where X is the key id.  (Don't change key names).

#### Signing

```
./sign <parameters file input> <hash string to be signed> <path to artifacts> <keyid>
```

Produces a signature file with name `hash_string.signature`.
File name is important for appending signatures.

#### Add signature

```
./appendsignature <parameters file input> <hash string to be signed> <path to artifacts> <keyid>
```

#### Verify signatures

```
./verify <parameters file input> <hash string to be verified> <signature_file_path> <path to artifacts> <key_id1> <key_id2> ....

```

## Example

```
./generate_keys pbc-0.5.14/param/a1.param 5

./sign pbc-0.5.14/param/a1.param ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad artifacts 1

./appendsignature pbc-0.5.14/param/a1.param ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad artifacts 3

./appendsignature pbc-0.5.14/param/a1.param ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad artifacts 4

./verify pbc-0.5.14/param/a1.param ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad.signature artifacts 1 3 4

