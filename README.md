# Password-Schemes

###1) Password Scheme Mimic of LMHash and NTLMHash v1###

###Files:###

**hashtable.txt** 
- is where OLHash program will store the user id and hashed passwords.

**OLHash.c**
- is the OLHash program source code.

**passwords.txt**
- contains user ids and passwords to  use the OLHash program

###Setup:###

- OLHash program strictly accepts only 4 character user ids.
- OLHash program strictly accepts only 12 character passwords.
- OLHash program will create a temporary txt file called clone.txt. 
- Open passwords.txt and choose a user id and password to enter into the OLHash program.
- Compile OLHash.c to run program or run OLHash.exe

------------------------------------------------------------------------------------------------------

###2) Password Cracker####

###Files:###

**OPasswordCracker.c**
- is the OPassword Cracker program source code.

###Setup:###

- OPasswordCracker program will create a txt file called hashedpws.txt which contains table of hashes of all possible 4-character long passwords. 
- Open hashedpws.txt in Microsoft Words with encoding "MS-DOS", then copy one of the hash and close the file.
- Compile OPasswordCracker.c to run program and enter the copied hash.
