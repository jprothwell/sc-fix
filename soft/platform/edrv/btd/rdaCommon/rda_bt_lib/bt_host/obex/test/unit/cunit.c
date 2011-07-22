//

// CUnit

//

#include <stdio.h>

#include <stdlib.h>

#include "cunit.h"



// typedef struct UTest UTest;

typedef struct UTest{

const char*name;

int num;

void (*pf)(void);

struct UTest*I;

}UTest;



UTest* cuMakeTest(const char* ,int ,void (*pf)(void));

void cuRegTest_i(UTest*i);

void ShowTitle(void);

void PrintStars(void);

void PrintAsterisks(int n);



static UTest*base;     // Always points to last entered test

static UTest*run;      // Points to currently running test

static int fail;       // Flag to indicate one or more test failed

static int title;      // Flag to indicate that the currently running test has had its title printed

static int totaltest;  // Total number of tests reported by the current test

static int totalrun;   // Total number of tests reported altogether

static int total;      // Total number of tests registered

static int tast;	   // Total number of asterisks printed so far

static int astop;

const int maxast=20;



void cuStart(void(*pf)(void)){

UTest*tmp;

base=0;

total=0;

// Call the User Implemented Function

pf();

run=base;

fail=0;

totalrun=0;

tast=0;

astop=0;

printf("****** OBEX TESTS *******\n");

printf("%d tests to run\n",total);

do{

  run=run->I;

  title=0;

  totaltest=0;

  printf("\n%s\n",run->name);
  run->pf();

  if(totaltest!=run->num) {

     ShowTitle();

	  printf("Wrong number of reports: %d expected, %d actually received!\n",run->num,totaltest);

	  astop=1;

  }
/*
  PrintAsterisks(totalrun);
*/
	
} while (run!=base);



if(!fail){

	printf("\n100%% tests worked!");

	PrintStars();

}

// Now release all the memory I have allocated

run=base;

do{

  tmp=run->I;

  free(run);

  run=tmp;

}while(run!=base);



}





// Register test by apending i to end of singly linked list

// Accrue total number of tests

void cuRegTest(const char*name,int num,void (*pf)(void)){

 cuRegTest_i(cuMakeTest(name,num,pf));

}



void cuRegTest_i(UTest*i){

if(base==0) {

	 base=i;

	 i->I=i;

}else{

	 i->I=base->I;

	 base->I=i;

	 base=i;

}

total+=base->num;

}





// Take in report

// If it is a report of failure, show title and report name

// Accrue totals

void cuReport(const char*c,int i){

if(!i) {

  /* ShowTitle();
*/
	 fail=1;

	 printf(" %s\n",c);

}
 else {
  printf(".");
}
++totalrun;

++totaltest;

}



// Factory routine to allocate memory and initialise an UTest

UTest* cuMakeTest(const char*name,int num,void (*pf)(void)){

UTest*I;

I=malloc(sizeof(UTest));

I->name=name;

I->num=num;

I->pf=pf;

I->I=0;

return I;

}



void ShowTitle(void){

if(!fail) printf("\n");

if(!title) printf("%s\n",run->name);

title=1;

}

void PrintStars(void){

printf("\n********************\n");

}



// Make asterisks flow across the screen!

// Global is the total number of tests

// Global is the total number of asterisks printed

// Parameter is the number of tests run so far



void PrintAsterisks(int n){

int t;

long x;

if(!fail && !astop){

  if(n<=total){

	 for(t=tast;t<maxast;++t){

		 x=total;

		x*=t;

		x/=maxast;

		if(x<=n) {

		  printf("*");

		 ++tast;

		} else break;

	 }

  }

}

}



