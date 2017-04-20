P2.out : Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o 
	g++ -g -o P2.out Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o 

Project2.o : Project2.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project2.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.save
	cp LexicalAnalyzer.save LexicalAnalyzer.o

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp


clean : 
	rm *.o P2.out *.gch *.lst *.dbg *.p2

submit : Project2.cpp LexicalAnalyzer.h LexicalAnalyzer.save SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp RuleMonitor.cpp RuleMonitor.h makefile README.txt
	rm -rf TeamNP2
	mkdir TeamNP2
	cp Project2.cpp TeamNP2
	cp LexicalAnalyzer.h TeamNP2
	cp LexicalAnalyzer.save TeamNP2
	cp SyntacticalAnalyzer.h TeamNP2
	cp SyntacticalAnalyzer.cpp TeamNP2
	cp makefile TeamNP2
	cp README.txt TeamNP2
	tar cfvz TeamNP2.tgz TeamNP2
	cp TeamNP2.tgz ~tiawatts/cs460drop
