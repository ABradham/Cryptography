# Key Management / Memory Exploitation
This code showcases an attack in which a public key file is read and the private key `d` in exfiltrated by taking advantage of uninitialized pointers scope of the `evil` funciton. Specifically, we select local variable in the `evil` function whose stack pointer once referenced the `d` local variable in the `good` function. Since we leave all pointers in `evil` uninitialized, we know that we can access the same memory that `good` stored in `evil` because no pointed data is overwritten. Thus, even though we never know the value of `d` (or even the name of the `key.pem` file) in the `evil` function, we are able to exfiltrate it because the writer of `good` did not properly purge / obfuscate `d`. 

## How to run
To build this example, type `make` in terminal while in this `Key\ Management` directory.

To run this example once built, type `./good` in terminal and, when prompted, type the name of the key file (`key.pem` in this case).