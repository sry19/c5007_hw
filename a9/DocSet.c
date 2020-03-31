/*
 *  Created by Adrienne Slaughter
 *  CS 5007 Summer 2019
 *  Northeastern University, Seattle
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  See <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DocSet.h"
#include "Hashtable.h"
#include "Util.h"

int AddDocInfoToSet(DocumentSet set,  uint64_t docId, int rowId) {
  // STEP 4: Implement AddDocInfoToSet.
  // Make sure there are no duplicate rows or docIds.
  int *row_ptr = rowId;
  HTKeyValue kvp;
  kvp.key = docId;
  HTKeyValue old_kvp;
  HTKeyValue old_ll;
  int result = LookupInHashtable(set->doc_index, docId, &old_ll);
  if (result == 0) {
    LinkedList ll = old_ll.value;
    LLIter iter = CreateLLIter(ll);
    int *payload;
    while (LLIterHasNext(iter) != 0) {
      LLIterGetPayload(iter, (void**)&payload);
      if ((int*)payload == row_ptr) {
        DestroyLLIter(iter);
        return -1;
      } else {
        LLIterNext(iter);
      }
    }
    LLIterGetPayload(iter, (void**)&payload);
    if ((int*)payload == row_ptr) {
        DestroyLLIter(iter);
        return -1;
    } else {
      int* rowid = (int*)malloc(sizeof(rowId));
      InsertLinkedList(ll, (void*)rowid);
      DestroyLLIter(iter);
      return 0;
    }
  }
  kvp.value = CreateLinkedList();
  int* rowid = (int*)malloc(sizeof(rowId));
  InsertLinkedList(kvp.value, (void*)rowid);
  PutInHashtable(set->doc_index, kvp, &old_kvp);
  return 0;
}

int DocumentSetContainsDoc(DocumentSet set, uint64_t docId) {
  // STEP 5: Implement DocumentSetContainsDoc
  HTKeyValue old_ll;
  if (LookupInHashtable(set->doc_index, docId, &old_ll) == 0) {
    return 0;
  }
  return -1;
}

void PrintOffsetList(LinkedList list) {
  printf("Printing offset list\n");
  LLIter iter = CreateLLIter(list);
  int* payload;
  while (LLIterHasNext(iter) != 0) {
    LLIterGetPayload(iter, (void**)&payload);
    printf("%d\t", *((int*)payload));
    LLIterNext(iter);
  }
}


DocumentSet CreateDocumentSet(char *desc) {
  DocumentSet set = (DocumentSet)malloc(sizeof(struct docSet));
  if (set == NULL) {
    // Out of memory
    printf("Couldn't malloc for movieSet %s\n", desc);
    return NULL;
  }
  int len = strlen(desc);
  set->desc = (char*)malloc((len + 1) *  sizeof(char));
  if (set->desc == NULL) {
    printf("Couldn't malloc for movieSet->desc");
    return NULL;
  }
  snprintf(set->desc, len + 1, "%s", desc);
  set->doc_index = CreateHashtable(16);
  return set;
}


void DestroyOffsetList(void *val) {
  LinkedList list = (LinkedList)val;
  DestroyLinkedList(list, &SimpleFree);
}

void DestroyDocumentSet(DocumentSet set) {
  // Free desc
  free(set->desc);
  // Free doc_index
  DestroyHashtable(set->doc_index, &DestroyOffsetList);
  // Free set
  free(set);
}
