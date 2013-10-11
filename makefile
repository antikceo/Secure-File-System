EXECUTABLES =objgetacl objsetacl objtestacl objput objget objlist
SOURCES = ${EXECUTABLES:=.cpp}

all: ${EXECUTABLES}
${EXECUTABLES}: ${SOURCES}
	g++ $@.cpp -o $@
	chmod u+s $@

clean:
	rm -rf *o objgetacl objsetacl objtestacl objput objget objlist *~ *#

test:	
	./objput file1
	@echo "------------"
	./objget tca2111+file1	 
	@echo "------------"
	./objget file1
	@echo "------------"
	./objlist   
	@echo "------------"
	./objsetacl file1 
	@echo "------------"
	./objgetacl file1
	@echo "------------"
	./objtestacl -a access file1
	@echo "------------"
	./objtestacl -a access tca2111+file1
	@echo "------------"

exec:
	./sample $(ARG)

rmtxt:
	rm *.txt