
all: main.o MyThreadDataStructure.o MyMutexDataStructure.o RoundRobinScheduler.o SortScheduler.o RealTimeScheduler.o Animation.o PrincipalMenu.o XMLParser.o TransmitterMode.o ReceiverMode.o libmythread.a 
	gcc -Wall `xml2-config --cflags --libs` -l ncurses -std=gnu99 main.o MyThreadDataStructure.o MyMutexDataStructure.o RoundRobinScheduler.o SortScheduler.o RealTimeScheduler.o Animation.o PrincipalMenu.o XMLParser.o TransmitterMode.o ReceiverMode.o libmythread.a -o MultiDisplayAnimation

libmythread.a: MyThread.o
	ar cr $@ MyThread.o
	ranlib $@

MyThread.o: MyThread.h MyThreadErrors.h MyMutexDataStructure.h MyThread.c
	gcc -c -Wall -std=gnu99 MyThread.c -o $@

MyThreadDataStructure.o: MyThreadDataStructure.h MyThreadErrors.h MyThreadDataStructure.c 
	gcc -c -Wall -std=gnu99 MyThreadDataStructure.c -o $@

MyMutexDataStructure.o: MyMutexDataStructure.h MyThreadErrors.h MyMutexDataStructure.c 
	gcc -c -Wall -std=gnu99 MyMutexDataStructure.c -o $@

RealTimeScheduler.o: RealTimeScheduler.h RoundRobinScheduler.h SortScheduler.h MyThread.h RealTimeScheduler.c
	gcc -c -Wall -std=gnu99 RealTimeScheduler.c -o $@

RoundRobinScheduler.o: RoundRobinScheduler.h MyThread.h RoundRobinScheduler.c
	gcc -c -Wall -std=gnu99 RoundRobinScheduler.c -o $@

SortScheduler.o: SortScheduler.h MyThread.h SortScheduler.c
	gcc -c -Wall -std=gnu99 SortScheduler.c -o $@

Animation.o: Animation.h MyThread.h Animation.c MyThread.c
	gcc -c -Wall `xml2-config --cflags --libs` -l ncurses -std=gnu99 Animation.c -o $@

PrincipalMenu.o: PrincipalMenu.h Animation.h XMLParser.h PrincipalMenu.c Animation.c XMLParser.c
	gcc -c -Wall `xml2-config --cflags --libs` -l ncurses -std=gnu99 PrincipalMenu.c -o $@

TransmitterMode.o: TransmitterMode.h XMLParser.h TransmitterMode.c XMLParser.c
	gcc -c -Wall `xml2-config --cflags --libs` -l ncurses -std=gnu99 TransmitterMode.c -o $@

ReceiverMode.o: ReceiverMode.h ReceiverMode.c
	gcc -c -Wall -l ncurses -std=gnu99 ReceiverMode.c -o $@

XMLParser.o: XMLParser.h Animation.h XMLParser.c Animation.c
	gcc -c -Wall `xml2-config --cflags --libs` -std=gnu99 XMLParser.c -o $@

main.o: Animation.h PrincipalMenu.h main.c
	gcc -c -Wall -l ncurses -std=gnu99 main.c -o $@