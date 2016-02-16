/*
 * TMI.c
 *
 *  Created on: Feb 27, 2015
 *      Author: dannyno
 *
 *  Copyright 2015 dannyno (at) dannyno (dot) 33mail (dot) com
 *
 *  This file is part of TMI.
 *
 *  TMI is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TMI is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TMI.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Own header */
#include "TMI.h"

/* Macros */
#define JUMP_BELOW(pTable, pItem) pItem = &pTable[pItem->below]
#define JUMP_ELDER(pTable, pItem) pItem = &pTable[pItem->elder]
#define JUMP_PARENT(pTable, pItem) pItem = &pTable[pItem->parent]

/* Static data */
const TMI_item* table = 0;
UINT tableLength = 0;
UINT currentIndex = 0;

/* Functions */

/* see header file */
UINT TMI_item_process(INT choice){
	if (table == NULL) return 0; // Figure out what's smartest here...
    UINT index = table[currentIndex].elder;
    UINT count = 1;
    UINT indexFound = 0;
    
    // return parent when choice is 0
    if (choice == 0)
    {
    	currentIndex = table[currentIndex].parent;
    	return currentIndex;
    }
    
    // stay there if there is no through (we should never reach there)
    if (index == NO_ELDER)
    {
    	return currentIndex;
    }
    
    // TODO: insert security to avoid infinite loop
    while (table[index].below != NO_BELOW)
    {
        if (count == choice)
        {
        	indexFound = index;
        }
        index = table[index].below;
        count ++;
    }
    if (count == choice)
    {
        indexFound = index;
    }
    if (choice > count)
    {
        return currentIndex;
    }
    
    if (table[indexFound].elder == NO_ELDER)
    {
    	if (table[indexFound].action != NULL)
    	{
    		table[indexFound].action();
    	}
    	return currentIndex;
    }
    currentIndex = indexFound;
	return currentIndex;
}

/* see header file */
UINT TMI_table_ok(const TMI_item* pTable, UINT tableLength)
{
    /* A table is OK if:
     * 1) each item is correctly indexed (pTable[i].index == i)
     * 2) There is no infinite link loop in the table (should be guaranteed by rules below)
     * 3) Each item has a child (through) or an action function
     * 4) An item's elder's parent is the item
     TODO: make 4) an option
     * 5) An item's below has the same parent as the item
     TODO: make 5) an option
     */
    UINT i, count, parentIndex, childIndex, siblingIndex;
    const TMI_item* currentItem;
    for (i=0; i < tableLength; i++)
    {
        count = 0;
        currentItem = &pTable[i];
        /* 1) Test correct indexing */
        if (currentItem->index != i) return i+1;
        /* 3) Test through or Action function rule */
        if ((currentItem->elder == NO_ELDER) && (currentItem->action == NULL))
        {
        	return i+1;
        }
        /* 4) Test correct parenting */
        if (currentItem->elder != NO_ELDER)
        {
            childIndex = currentItem->elder;
            parentIndex = pTable[childIndex].parent;
            if (currentItem->index != pTable[parentIndex].index) return i+1;
        }
        /* 5) Test siblings have same parent */
        if (currentItem->below != NO_BELOW)
        {
            siblingIndex = currentItem->below;
            parentIndex = currentItem->parent;
            if (parentIndex != pTable[siblingIndex].parent) return i+1;
        }
        /* 2) Test no infinite loop in the table */
        while (currentItem->below !=0)
        {
            count++;
            if (count > tableLength) return i+1;
            JUMP_BELOW(pTable, currentItem);
        }
    }
    return 0;
}

/* see header file */
void TMI_display_table(int (*printFunction)(const STRING))
{
    const TMI_item *currentItem = &table[0];
    UINT depth = 1;
    UINT num = 1;
    UINT i;
    const STRING TAB = "  ";
    const STRING NL = "\n";

    if (printFunction == NULL) return;

    while((depth > 0) && (num < tableLength))
    {
    	for (i=0;i<depth;i++) printFunction(TAB);
        printFunction(currentItem->text);
        printFunction(NL);
        if (currentItem->elder != NO_ELDER)
        {
            num++;
            depth++;
            JUMP_ELDER(table, currentItem);
        }
        else if(currentItem->below != NO_BELOW)
        {
            num++;
            JUMP_BELOW(table, currentItem);
        }
        else
        {
            JUMP_PARENT(table, currentItem);
            depth--;
            while ((currentItem->below == NULL) && (depth > 0))
            {
                JUMP_PARENT(table, currentItem);
                depth--;
            }
            if (currentItem->below != NULL) JUMP_BELOW(table, currentItem);
        }
    }
    
    return;
}

/* see header file */
UINT TMI_init(const TMI_item* pTable, UINT length)
{
	UINT err = TMI_table_ok(pTable, length);
	if (err == 0)
	{
		tableLength = length;
		table = pTable;
		return 0;
	}
	else
	{
		return err;
	}
	
}
