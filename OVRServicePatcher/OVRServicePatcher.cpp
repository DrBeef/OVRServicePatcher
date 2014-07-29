// OVRServicePatcher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

typedef std::basic_ofstream<unsigned char, std::char_traits<unsigned char> > uofstream;
typedef std::basic_ifstream<unsigned char, std::char_traits<unsigned char> > uifstream;

unsigned long searchRotationVal = 90;
unsigned long newRotationVal = 270;

int _tmain(int argc, _TCHAR* argv[])
{
	if (argv[1] == NULL)
	{
		cout << endl << endl << "ERROR: input executable filename not provided - Patching aborted" << endl << endl;
		system("pause");

		return -1;
	}

	bool reportOffets = false;
	if (argv[2] != NULL)
	{
		string c(argv[2]);
		if (c == "reportOffsets")
			reportOffets = true;
	}

	string s = argv[1];

	//Check if we're patching the 64 bit binary
	string exeName = s.substr(s.find_last_of('\\')+1);
	bool is64Bit = exeName.find("x64") != string::npos;
	bool is32Bit = exeName.find("x86") != string::npos;
	if (!reportOffets && !is64Bit && !is32Bit)
	{
		cout << endl << endl << "ERROR: input executable filename does not contain 64 or 32: " + exeName + " - Patching aborted" << endl << endl;
		system("pause");

		return -1;
	}

	uifstream inFile(s,  ios::in | ios::binary);
	if (inFile.fail())
	{
		cout << endl << endl << "ERROR: unable to open input executable: " + s + " - Patching aborted" << endl << endl;
		system("pause");
		return -1;
	}

	uofstream outFile(s + ".riftup",  ios::out | ios::binary);
	if (outFile.fail())
	{
		cout << endl << endl << "ERROR: unable to open output file: " + s + ".riftup - Patching aborted" << endl << endl;
		system("pause");
		return -1;
	}

	unsigned long byteIndex = 0;

	if (!reportOffets)
		cout << "Patching";
	else
		cout << "Scanning: " + s << endl;

	//array to check for the values in
	unsigned char bytes[4] = {0};

	while (!inFile.eof())
	{
		if (!reportOffets && byteIndex % 65536 == 0)
			cout << ".";

		//Are we at the right position yet?
		if (!reportOffets && 
			((byteIndex == 0x00019341 && is64Bit) ||
			 (byteIndex == 0x0001812A && is32Bit)))
		{
			unsigned char byte[4];
			inFile.read(byte, 4);

			unsigned long &value = *(reinterpret_cast<unsigned long*>(byte));
			if (value == searchRotationVal)
			{
				//Set the new rotation
				value = newRotationVal;
			}
			else
			{
				cout << endl << endl << "ERROR: input file patching value does not match expected value of 90 in file: " + s + " - Patching aborted" << endl << endl;
				system("pause");
				return -1;
			}
			
			outFile << byte[0] << byte[1] << byte[2] << byte[3];

			byteIndex += 4;
		}
		else
		{
			unsigned char byte;
			inFile.read(&byte, 1);

			if (!inFile.fail() && !reportOffets)
			{
				outFile << byte;
				if (outFile.fail())
				{
					cout << endl << endl << "ERROR: could not write to " + s + ".riftup - Patching aborted" << endl << endl;
					system("pause");
					return -1;
				}
			}

			if (reportOffets)
			{
				bytes[0] = bytes[1];
				bytes[1] = bytes[2];
				bytes[2] = bytes[3];
				bytes[3] = byte;

				unsigned long value = *(reinterpret_cast<unsigned long*>(bytes));
				if (value == searchRotationVal)
				{
					char buffer[32];
					sprintf(buffer, "Value (%i) found at: 0x%.8X", searchRotationVal, (byteIndex-3));
					cout << buffer << endl;					
				}
			}

			++byteIndex;
		}
		
	}

	if (!reportOffets)
	{
		cout << endl << "Successfully patched: " + s << endl << endl;
		cout << endl << "Next Step: ";
		cout << endl << "========== ";
		cout << endl << "1. Rename the original executable " + exeName + " to " + exeName + ".backup";
		cout << endl << "2. Rename the patched executable " + exeName + ".riftup to " + exeName;
		cout << endl << "3. Run " + exeName + " as administrator" << endl << endl;
	}
	system("pause");

	return 0;
}


