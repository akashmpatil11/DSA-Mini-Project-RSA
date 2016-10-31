project: rsa.c
	cc rsa.c -o project -lgmp
clean:
	$(RM) project
