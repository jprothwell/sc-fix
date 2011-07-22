//
// CUnit.h
//

#ifndef CUNIT_H
#define CUNIT_H

void cuStart(void(*pf)(void));

// Your test is registered with:
//  its name, number of reports, function to call
void cuRegTest(const char* ,int ,void (*pf)(void));

// Make your reports with this function:
// Name of Test just run, result (true=1, false=0)
void cuReport(const char*,int );
// Implementation specific Memory Pool setup functions (malloc, free ...)
void cuSetupMemPool8(void);
void cuSetupMemPool4(void);

//*************************************
//
// User implements main()
//
// User implements USER()
//
// void USER(void);
//
// Implement main() like this:
/*
int main(void){
// cuSetupMemPool8();    // Special for Keil C
// SetupSerial();        // Special for Keil C
cuStart(USER);
// for(;;);   // Hang like this at the end?
return 0;
}
*/
// Implement USER like this:
/*
void USER(void){
  // Register each of your test functions
  cuRegTest("first test",1,p);
  cuRegTest("second test",2,p);
  //
}
}
// Write your test routines like this:
void p(void){
cuReport("Test should work",(addint(4,3)==7));
cuReport("Test should fail",!(addint(2,7)==9));
}

// Example of one of your real routines:
int addint(int i,int j){
return i+j;
}
*/

//*************************************


#endif  // CUNIT_H

