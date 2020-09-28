# One-Time-Pad Program
*Programmer: Glen Gougeon*
#### School:   Oregon State University
#### Last MOD: 7-30-20

## Description: OTP Client-Server Project Documentation

* Descriptions of SCRIPTS, PROGRAMS, MAKEFILE
	
## INSTRUCTIONS:
	
### Scripts:
#### 1. compileall
* Sets up program for testing(using testscript) or operation			

* TO RUN: [chmod +x compileall] (if not executable)

* Commands
	* compileall
        * unzips 'plaintext' files for use with testscript
        * compiles all 5 sub-programs for operation [keygen, otp_enc_d, otp_enc, otp_dec_d, otp_dec]
        * makes testscript executable for testing	
        * USAGE: **compileall**
    * clean
        * removes unpacked zip files
        * removes executables from 'compileall' command
        * USAGE:	**compileall clean**
					
#### testscript
* TBD
					
### PROGRAMS:
#### 1. keygen	
* Key generation program in the set [A-Z, SPACE].
* Outputs to stdout, or may redirect output to a file (recommended)   
* USAGE: **keygen [size]**
* (if saving to a file):* **keygen [size] > file**
					
#### 2. otp_enc_d
* OTP style encryption server program. Performs actual encryption
according to specification once client encryption program (otp_enc)
is verified. SHOULD run as background process [&].
* USAGE: **otp_enc_d [port] &**
					
#### 3. otp_enc
* OTP style encryption client program designed to encrypt a file
by connections to the encryption server program otp_enc_d.	 
* USAGE: **otp_enc [plaintext] [key] [port]**
			
#### 4. otp_dec_d
* Analagous to otp_enc_d except that it performs decryption instead.
* USAGE: **otp_dec_d [port] &**
			
#### 5. otp_dec
* Analagous to otp_enc client program except used for decryption.
* USAGE: **otp_dec [ciphertext] [key] [port]**
	
### Makefile AVAILABLE:
* Included makefile included as alternative to 'compileall' script
* Performs more specific compilations for each sub-program listed above.
    * EXAMPLE: make opt_enc_d
    * EXAMPLE: make keygen
		
* Additional commands available:
    * **make chmod**
        * used to make the p4gradingscript executable
				
    * **make unzips**
        * used to unzip 5 files used as plaintexts in p4testscript
				
    * **make compileall**
        * same as 'compileall' script version. 
        * makes testscript executable
        * unzips plaintext files
        * compiles all 5 programs
				
    * **make clean**
        * same as 'compileall' script version
        * removes the 5 main executables
        * removes mytestresults file
        * removes execution from testscript

#### NOTES:
* [port1] CANNOT be the same as [port2]
* *Linux/Unix will hold onto ports sometimes, so must change in between 
different testing runs*

