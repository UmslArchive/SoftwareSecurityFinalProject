# CS3780-Software-Security
Final project.

Dependencies: OpenSSL cryptographic library

This was a three part assignment. We worked in pairs to build this out.

The first part of the program, the user navigates a menu which allows them to either create an account or login. User credentials are stored in a local file based on the users selection between plaintext, hashed, or hashed and salted.

The second part skips user interaction and just generates programmer-specified-length passwords randomly and stores them in plaintext, hashed, and hashed + salted formatted files. 

The last part of the program cracks the passwords and outputs their plain text forms into a file. It times the execution in order to investigate which method of password storage is most secure from brute force attacks.

Had a lot of fun with this one.
