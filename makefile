

DATA1= data/dataset1
DATA2= data/dataset2
DATA3= data/dataset3
all:	program

program:	Driver.o DecTree.o Node.o
	g++ -g -Wall -std=c++11 Driver.o DecTree.o Node.o -o program

Driver.o:	Driver.cpp
	g++ -g -Wall -std=c++11 -c Driver.cpp

DecTree.o:	DecTree.cpp
	g++ -g -Wall -std=c++11 -c DecTree.cpp

Node.o:	Node.cpp
	g++ -g -Wall -std=c++11 -c Node.cpp

test1: program
#h1 Set1 noPrune	
	./program $(DATA1)/training_set.csv $(DATA1)/validation_set.csv \
	$(DATA1)/test_set.csv no no H1

test2: program
#h1 Set1 yesPrune	
	./program $(DATA1)/training_set.csv $(DATA1)/validation_set.csv \
	$(DATA1)/test_set.csv no yes H1

test3: program
#h2 Set1 noPrune
	./program $(DATA1)/training_set.csv $(DATA1)/validation_set.csv \
	$(DATA1)/test_set.csv no no H2

test4: program
#h2 Set1 yesPrune
	./program $(DATA1)/training_set.csv $(DATA1)/validation_set.csv \
	$(DATA1)/test_set.csv no yes H2

test5: program
#h1 Set2 noPrune	
	./program $(DATA2)/training_set.csv $(DATA2)/validation_set.csv \
	$(DATA2)/test_set.csv no no H1

test6: program
#h1 Set2 yesPrune	
	./program $(DATA2)/training_set.csv $(DATA2)/validation_set.csv \
	$(DATA2)/test_set.csv no yes H1

test7: program
#h2 Set2 noPrune
	./program $(DATA2)/training_set.csv $(DATA2)/validation_set.csv \
	$(DATA2)/test_set.csv no no H2

test8: program
#h2 Set2 yesPrune
	./program $(DATA2)/training_set.csv $(DATA2)/validation_set.csv \
	$(DATA2)/test_set.csv no yes H2

#printing executions


test1p: program
#h1 Set1 noPrune	
	./program $(DATA1)/training_set.csv $(DATA1)/validation_set.csv \
	$(DATA1)/test_set.csv yes no H1

test2p: program
#h1 Set1 yesPrune	
	./program $(DATA1)/training_set.csv $(DATA1)/validation_set.csv \
	$(DATA1)/test_set.csv yes yes H1

test3p: program
#h2 Set1 noPrune
	./program $(DATA1)/training_set.csv $(DATA1)/validation_set.csv \
	$(DATA1)/test_set.csv yes no H2

test4p: program
#h2 Set1 yesPrune
	./program $(DATA1)/training_set.csv $(DATA1)/validation_set.csv \
	$(DATA1)/test_set.csv yes yes H2

test5p: program
#h1 Set2 noPrune	
	./program $(DATA2)/training_set.csv $(DATA2)/validation_set.csv \
	$(DATA2)/test_set.csv yes no H1

test6p: program
#h1 Set2 yesPrune	
	./program $(DATA2)/training_set.csv $(DATA2)/validation_set.csv \
	$(DATA2)/test_set.csv yes yes H1

test7p: program
#h2 Set2 noPrune
	./program $(DATA2)/training_set.csv $(DATA2)/validation_set.csv \
	$(DATA2)/test_set.csv yes no H2

test8p: program
#h2 Set2 yesPrune
	./program $(DATA2)/training_set.csv $(DATA2)/validation_set.csv \
	$(DATA2)/test_set.csv yes yes H2




testall: program
#1. h1 Set1 noPrune	
	./program $(DATA1)/training_set.csv $(DATA1)/validation_set.csv \
	$(DATA1)/test_set.csv no no H1
#2. h1 Set1 yesPrune	
	./program $(DATA1)/training_set.csv $(DATA1)/validation_set.csv \
	$(DATA1)/test_set.csv no yes H1
#3. h2 Set1 noPrune
	./program $(DATA1)/training_set.csv $(DATA1)/validation_set.csv \
	$(DATA1)/test_set.csv no no H2
#4. h2 Set1 yesPrune
	./program $(DATA1)/training_set.csv $(DATA1)/validation_set.csv \
	$(DATA1)/test_set.csv no yes H2
#5. h1 Set2 noPrune	
	./program $(DATA2)/training_set.csv $(DATA2)/validation_set.csv \
	$(DATA2)/test_set.csv no no H1
#6. h1 Set2 yesPrune	
	./program $(DATA2)/training_set.csv $(DATA2)/validation_set.csv \
	$(DATA2)/test_set.csv no yes H1
#7. h2 Set2 noPrune
	./program $(DATA2)/training_set.csv $(DATA2)/validation_set.csv \
	$(DATA2)/test_set.csv no no H2
#8. h2 Set2 yesPrune
	./program $(DATA2)/training_set.csv $(DATA2)/validation_set.csv \
	$(DATA2)/test_set.csv no yes H2




gdb:	program
	gdb --args ./program $(DATA2)/training_set.csv $(DATA2)/validation_set.csv \
	$(DATA2)/test_set.csv yes no H2

clean:
	rm -f *.o program
