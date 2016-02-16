/*
 ============================================================================
 Name        : TMI_test.c
 Author      : dannyno
 Description : Test program for TMI.c
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "TMI.h"

/* definitions */
#define TEST_V2 1

/* Local prototypes */
int dummyAction(void);

/* Test table */
enum TableIndices
{
    TOP_MENU,
    SUB_MENU_1,
    SUB_MENU_1_1,
    SUB_MENU_1_1_1,
    SUB_MENU_1_1_2,
    SUB_MENU_1_1_3,

    SUB_MENU_1_2,
    
    SUB_MENU_2,
    SUB_MENU_2_1,
    SUB_MENU_2_2,
    
    TABLE_LENGTH
};

static const TMI_item test_cli_struct[] = {
    /*{INDEX, DESCRIPTION_TEXT, PARENT, BELOW, THROUGH, action}*/
    {TOP_MENU, "Top menu, where do you want to go?", NO_PARENT, NO_BELOW, SUB_MENU_1, NULL},
        {SUB_MENU_1, "Sub menu 1", TOP_MENU, SUB_MENU_2, SUB_MENU_1_1, NULL},
            {SUB_MENU_1_1, "Sub menu 1-1", SUB_MENU_1, SUB_MENU_1_2, SUB_MENU_1_1_1, NULL},
                {SUB_MENU_1_1_1, "Sub menu 1-1-1", SUB_MENU_1_1, SUB_MENU_1_1_2, NO_ELDER, &dummyAction},
                {SUB_MENU_1_1_2, "Sub menu 1-1-2", SUB_MENU_1_1, SUB_MENU_1_1_3, NO_ELDER, &dummyAction},
                {SUB_MENU_1_1_3, "Sub menu 1-1-3", SUB_MENU_1_1, NO_BELOW, NO_ELDER, &dummyAction},
            {SUB_MENU_1_2, "Sub menu 1-2", SUB_MENU_1, NO_BELOW, NO_ELDER, &dummyAction},
        {SUB_MENU_2, "Sub menu 2", TOP_MENU, NO_BELOW, SUB_MENU_2_1, NULL},
            {SUB_MENU_2_1, "Sub menu 2-1", SUB_MENU_2, SUB_MENU_2_2, NO_ELDER, &dummyAction},
            {SUB_MENU_2_2, "Sub menu 2-2", SUB_MENU_2, NO_BELOW, NO_ELDER, &dummyAction},
    {TABLE_LENGTH, "", NO_PARENT, NO_BELOW, NO_ELDER, NULL}
};

// Function to print the menu from current item:
void printMenu(const TMI_item * pTable, unsigned int index)
{
    unsigned int currentIndex = index;
    unsigned int num = 1;
    puts("0: go up -1: exit");
    puts(pTable[currentIndex].text);
    currentIndex = pTable[currentIndex].elder;
    if (currentIndex != 0)
    {
	    printf("\t%d.", num);
	    num++;
	    puts(pTable[currentIndex].text);
	    while (pTable[currentIndex].below != NO_BELOW)
	    {
	        currentIndex = pTable[currentIndex].below;
	        printf("\t%d.", num);
	        num++;
	        puts(pTable[currentIndex].text);
	    }
    }
}

// Function returning an integer read from standard input
int getIntFromStdIn(void)
{
	int myInt = 0;
	while (scanf("%d", &myInt) != 1)
	{
		while (getchar() != '\n');	
	}
	return myInt;
}

/* Not super good but that works for test */
int myPrintFunction(const char * toPrint)
{
	printf(toPrint);
	return 0;
}

/* dummy action function */
int dummyAction(void)
{
	puts("Dummy action");
	return 0;
}

int main(void) {
	
#if TEST_V2
	int myInt = 0;
	unsigned int currentIndex = 0;
	unsigned int errCode = TMI_init(&test_cli_struct[0], TABLE_LENGTH);
	printf("Error Code: %i\n", errCode);
	puts("Whole table:");
    TMI_display_table(&myPrintFunction);
    while (myInt != -1)
	{
		printMenu(&test_cli_struct[0], currentIndex);
		myInt = getIntFromStdIn();
		currentIndex = TMI_item_process(myInt);
	}
#else
    unsigned int errCode = 0;
    unsigned int currentIndex = 0;
    int myInt = 0;
    const TMI_item* table = &test_cli_struct[0];

    errCode = TMI_table_ok(table, TABLE_LENGTH);
    printf("Error Code: %i\n", errCode);

    puts("Whole table:");
    TMI_display_table(table, TABLE_LENGTH, &myPrintFunction);

	while (myInt != -1)
	{
		printMenu(table, currentIndex);
		myInt = getIntFromStdIn();
		currentIndex = TMI_item_process(table, currentIndex, myInt);
	}
#endif
    return EXIT_SUCCESS;
}


