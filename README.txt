Brian Grant




to compile run "make"

to run all combinations required run "make testall"
	-this will run the heuristics, pruned or not pruned
		on all datasets, in the order given in the
		provided sample result.txt

to run any single specified execution run make test<number(1-8)>
	- where the number 1 through 8 executes whatever is clearly specified 
	 	in the makefile

to run any single specified execution that prints the tree
	run make test<number(1-8)>p
	- where the number 1 through 8 executes whatever is clearly specified 
	 	in the makefile followed by the letter p to indicate print

to run any custom execution follow the specified format
	./program <training>.csv <validation>.csv <test>.csv 
			<print?>{yes, no} <prune?>{yes, no} <Heuristic>{H1, H2}

run "make clean" to delete executables and object files



This design makes the following decision
	-if we are in a node of mixed classes, and dont have any
		more attributes to split on...if there are more 1 class 
		instances mark as a decision node of class 1
		otherwise mark as a decision node of class 0
	-if there are attributes left to split on but none provide any
		information gain, mark 1 if majority are class 1 otherwise
		mark as 0. Same as described above	
	-if pruning a branch results in the same accuracy on the validation
		set it is still pruned
	
