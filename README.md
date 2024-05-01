# Password Generator

This project reflects a brute-force approach for cracking passwords.


It is done in C, using threads for speeding up the process.

## Installing and running

### Compilation

By using `gcc` or `clang`:

`$> gcc generate_passwords.c -o generate_passwords -lm`


### Running

```
./a.out -l <> - c <>

-l <args>       The length of the password to be generated
-c <args>       The characters of the password.
                a: for chars in a-z
                A: for chars in A-Z
                0: for chars in 0-9
```

**Examples**:


Generate passwords of length 8 that contain chars in range **a-z**:
`./generate_passwords -l 8 -c a`


Generate passwords of length 5 that contain chars in range **a-z**, **A-Z**, **0-9**:
`./generate_passwords -l 5 -c aA0`


