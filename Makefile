########################################################################
# Makefile for quick sort in serial, dac and peer
########################################################################

CC         =  gcc
CCFLAGS    =  -O3

quick_sort: 		quick_sort.c
	$(CC) $(CCFLAGS) -o quick_sort quick_sort.c

DAC_quick_sort: 		DAC_quick_sort.c
	$(CC) $(CCFLAGS) -o DAC_quick_sort DAC_quick_sort.c	

BUCKET_quick_sort: 		BUCKET_quick_sort.c
	$(CC) $(CCFLAGS) -o BUCKET_quick_sort BUCKET_quick_sort.c	
