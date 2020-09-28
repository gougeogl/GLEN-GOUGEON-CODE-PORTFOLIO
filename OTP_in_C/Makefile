# ***********************************************
# FileName:	Makefile
# Author:	Glen Gougeon
# Class:	CS344 Operating Systems
# Created:	12-3-2019
# Last Mod:	7-30-2020
#
# Description:
# 		Program 4 OTP compilation script
#
# *********************************************** 

CC = gcc
CFLAGS = -Wall
CFLAGS += -pedantic
CFLAGS += -std=c99

keygen: keygen.c
	${CC} ${CFLAGS} -o keygen keygen.c

otp_enc:  helpers.h otp_enc.c  
	${CC} ${CFLAGS} -o otp_enc -g helpers.h otp_enc.c

otp_enc_d: helpers.h otp_enc_d.c 
	${CC} ${CFLAGS} -o otp_enc_d -g helpers.h otp_enc_d.c 

otp_dec:  helpers.h otp_dec.c  
	${CC} ${CFLAGS} -o otp_dec -g helpers.h otp_dec.c

otp_dec_d: helpers.h otp_dec_d.c 
	${CC} ${CFLAGS} -o otp_dec_d -g helpers.h otp_dec_d.c 


chmod: 
	chmod +x p4gradingscript

unzip: ptxts.zip
	unzip ptxts.zip

compileall: otp_enc otp_enc_d otp_dec otp_dec_d keygen ptxts.zip
	unzip ptxts.zip
	chmod +x p4gradingscript

clean:
	rm -f keygen mytestresults 
	rm -f otp_enc otp_enc_d
	rm -f otp_dec otp_dec_d
	rm -f *ciphertext*
	rm -f *plaintext*
	chmod -x p4gradingscript

