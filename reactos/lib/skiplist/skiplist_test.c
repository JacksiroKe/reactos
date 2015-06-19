/*
 * PROJECT:     Skiplist implementation for the ReactOS Project
 * LICENSE:     GNU LGPL v2.1 or any later version as published by the Free Software Foundation
 * PURPOSE:     A simple program for testing the Skiplist implementation
 * COPYRIGHT:   Copyright 2015 Colin Finck <colin@reactos.org>
 */

#include <windows.h>
#include <stdio.h>
#include "skiplist.h"

void
DumpSkiplist(PSKIPLIST Skiplist)
{
    CHAR i;
    DWORD j;
    PSKIPLIST_NODE pNode;

    printf("======= DUMPING SKIPLIST =======\n");

    for (i = Skiplist->MaximumLevel + 1; --i >= 0;)
    {
        pNode = &Skiplist->Head;
        printf("H");

        while (pNode->Next[i])
        {
            printf("-");

            // By using the Distance array for painting the lines, we verify both the links and the distances for correctness.
            for (j = 1; j < pNode->Distance[i]; j++)
                printf("---");

            printf("%02lu", (DWORD)pNode->Next[i]->Element);

            pNode = pNode->Next[i];
        }

        printf("\n");
    }

    printf("================================\n\n");
}

PVOID WINAPI
MyAlloc(DWORD Size)
{
    return HeapAlloc(GetProcessHeap(), 0, Size);
}

int WINAPI
MyCompare(PVOID A, PVOID B)
{
    return (DWORD)A - (DWORD)B;
}

void WINAPI
MyFree(PVOID Ptr)
{
    HeapFree(GetProcessHeap(), 0, Ptr);
}

int
main()
{
    DWORD Element;
    DWORD ElementIndex;
    DWORD i;
    SKIPLIST Skiplist;

    system("mode con cols=300");
    InitializeSkiplist(&Skiplist, MyAlloc, MyCompare, MyFree);

    // Insert some random elements with random numbers.
    for (i = 0; i < 40; i++)
        InsertElementSkiplist(&Skiplist, (PVOID)(rand() % 100));

    // Delete all with index 0 to 29.
    for (i = 0; i < 30; i++)
        DeleteElementSkiplist(&Skiplist, (PVOID)i);

    // Insert some more random elements.
    for (i = 0; i < 40; i++)
        InsertElementSkiplist(&Skiplist, (PVOID)(rand() % 100));

    // Check if an element with number 44 is in the list and output its index.
    Element = (DWORD)LookupElementSkiplist(&Skiplist, (PVOID)44, &ElementIndex);
    printf("Element = %lu, ElementIndex = %lu\n\n", Element, ElementIndex);

    DumpSkiplist(&Skiplist);

    return 0;
}
