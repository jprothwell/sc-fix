
Parthus Technologies Bluetooth Host Stack



API example Programs 
======================

	ex1.c	- Retrieving Device Information
	ex2.c	- L2CAP Server Application 
	ex3.c	- L2CAP Client Application
	ex4.c	- Performing an Inquiry Procedure
	ex5.c	- Adding an SDP record
	ex6.c	- Performing an SDP search
	ex7.c	- Using HCI Functions 
	ex8.c	- Security-Aware L2CAP Server Application 
	ex9.c	- Security-Aware L2CAP Client Application 


Refer to Host Stack Programming Manual for individual discussion.

Performing an SDP search for an RFCOMM server

Building The Examples 
=======================

The address of the remote device to connect to is hardcoded in the
program source files ex3.c, ex9.c and ex6.c
This address must be changes as appropriate for the Host Controller
devices in use.

In ex3.c and ex9.c change the define TARGET_ADDRESS to that of the server device.
In ex6.c change the address passed to MGR_ParseHexBDAddress() to that of the server device.

Furthermore the TARGET_ADDRESS in ex8.c must be changed to that
of the client which will connect.

In order to be portable, the examples are designed to be stand-alone
and use only stdio functionality. 


MS Windows -

A makefile is provided for Win32. This assumes that dllstack.dll and 
dllstack.lib are placed in the current directory, and that bt.h and 
associated header files are placed in the parent directory. To build
the examples, run "NMAKE /F Makefile.Win". If the Microsoft development
tools are not available in the system PATH, the VCVARS32.BAT file which
is provided with Microsoft Visual C++ should first be run.

Since An RFCOMM port entity is not currently provided for Windows, 
examples 10 and 11 are not supported on this platform.


The makefile Makefile_TCI.win shoudl be used if you want the example
to link the TCI Upper.In this way the examples run on the PC and 
connect to a combined stack over the TCI.
To build the examples, run "NMAKE /F Makefile_TCI.Win".
Examples 5 and 6 are not supported by the TCI - SDP Database access.

03 April 2003
