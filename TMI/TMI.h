/*
 * TMI.h
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

#ifdef __cplusplus
extern "C" {
#endif

#include "TMI_types.h"

#ifndef TMI_H_
#define TMI_H_

#define NO_PARENT  0
#define NO_BELOW   0
#define NO_ELDER 0

/*
 * action function typedef
 */
typedef int (*pfAction)(void);
/*
 * Main structure
 */
typedef struct TMI_item_s{
    UINT index;
    STRING text;
    UINT parent;
    UINT below;
    UINT elder;
    pfAction action;
}TMI_item;

/* 
 * returns the index of the item to become current after making the choice
 * rules for choice:
 *  0: go up (to parent)
 *  1: go to elder
 *  2: go to elder's below
 *  3: etc.
 *
 *  Other choices: remain on current item
 */
UINT TMI_item_process(INT choice);

/* 
 * returns 0 if everything went fine, otherwise returns index
 * where error was found, without further details on the reason
 * for check fail.
 */
UINT TMI_table_ok(const TMI_item* pTable, UINT tableLength);

/*
 * displays the whole table
 */
void TMI_display_table(int (*printFunction)(const STRING));

/* Init function */
UINT TMI_init(const TMI_item* pTable, UINT length);

#endif /* TMI_H_ */

#ifdef __cplusplus
}
#endif

