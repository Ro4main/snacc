// file: .../c++examples/simple/genber.C---builds an PersonnelRecord value and writes BER form of the value to a file called "pr.ber"
//
// MS 92
//
// $Header: /usr/app/odstb/CVS/snacc/c++-examples/simple/genber.C,v 1.5 1995/07/24 15:40:32 rj Exp $
// $Log: genber.C,v $
// Revision 1.5  1995/07/24 15:40:32  rj
// changed `_' to `-' in file names.
//
// Revision 1.4  1994/12/11  15:36:14  rj
// const for a constant value [DEC]
//
// Revision 1.3  1994/10/08  01:27:03  rj
// several \size_t'
//
// Revision 1.2  1994/08/31  08:56:33  rj
// first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
//


#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <fstream>

#include "asn-incl.h"
#include "p-rec.h"


main (int argc, char *argv[])
{
    std::ofstream outputFile;
    AsnBuf outputBuf;
    size_t encodedLen;
    const size_t dataSize = 1024;
    char data[dataSize];
    ChildInformation *ciPtr;
    PersonnelRecord pr;

    // build internal value of a PersonnelRecord
    pr.name = new Name;
    pr.name->givenName = "John";  // this calls pr.name->givenName.Set ("John");
    pr.name->initial = "E";
    pr.name->familyName = "Smith";

    pr.title.Set ("The Big Cheese");
    pr.employeeNumber = 99999;
    pr.dateOfHire.Set ("19820104");

    pr.nameOfSpouse = new Name;
    pr.nameOfSpouse->givenName.Set ("Mary");
    pr.nameOfSpouse->initial.Set ("L");
    pr.nameOfSpouse->familyName.Set ("Smith");

    pr.children = new PersonnelRecordSeqOf;

    ciPtr = pr.children->Append();
    ciPtr->name = new Name;
    ciPtr->name->givenName.Set ("James");
    ciPtr->name->initial.Set ("R");
    ciPtr->name->familyName.Set ("Smith");
    ciPtr->dateOfBirth.Set ("19570310");

    ciPtr = pr.children->Append();
    ciPtr->name = new Name;
    ciPtr->name->givenName.Set ("Lisa");
    ciPtr->name->initial.Set ("M");
    ciPtr->name->familyName.Set ("Smith");
    ciPtr->dateOfBirth.Set ("19610621");


    // set up buffer for writing to
    outputBuf.Init (data, dataSize);
    outputBuf.ResetInWriteRvsMode();

    // encode the internal value we just build into the buffer
    if (!pr.BEncPdu (outputBuf, encodedLen))
        std::cout << "failed encoding AnyTestType value" << std::endl;

    // open file to hold the BER value
    outputFile.open ("pr.ber");
    if (!outputFile)
    {
        perror ("ofstream::open");
        exit (1);
    }

    // copy the BER value from the buffer to the file
    outputBuf.ResetInReadMode();
    for (; encodedLen > 0; encodedLen--)
        outputFile.put (outputBuf.GetByte());


    std::cout << "Wrote the following BER PersonnelRecord value to pr.ber." << std::endl;
    std::cout << "Test it with \"def\" and \"indef\"." << std::endl;
    std::cout << pr << std::endl;

    return 0;
}
