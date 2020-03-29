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
#include <stdlib.h>
#include <stdio.h>
#include "DocIdMap.h"
#include "Hashtable.h"
#include <string.h>

void DestroyString(void *val) {
    free(val);
}

DocIdMap CreateDocIdMap() {
  DocIdMap docs = (DocIdMap)CreateHashtable(64);
  return docs;
}

void DestroyDocIdMap(DocIdMap map) {
  DestroyHashtable(map, &DestroyString);
}

int PutFileInMap(char *filename, DocIdMap map) {
  // STEP 1: Put File in Map
  // Ensure that each file/entry has a unique ID as a key
  // Insert the id/filename into the Hashtable.
  // If PutInHashtable returns 2 (there's a duplicate ID),
  // check if the filenames are the same. If not,
  // create a new ID for the file and insert it.
  HTKeyValue kvp;
  HTKeyValue old_kvp;
  // kvp.key = FNVHash64((unsigned char*)filename, strlen(filename));
  kvp.key = NumElemsInHashtable(map)+1;
  kvp.value = filename;
  int result = PutInHashtable(map, kvp, &old_kvp);
  if (result == 0) {
    return 0;
  }
  if (result == 1) {
    return -1;
  }
  while (PutInHashtable(map, kvp, &old_kvp) == 2) {
    if (old_kvp.value == kvp.value) {
      return 0;
    }
    kvp.key++;
  }
  return 0;
}

DocIdIter CreateDocIdIterator(DocIdMap map) {
  HTIter iter = CreateHashtableIterator(map);
  return iter;
}

void DestroyDocIdIterator(DocIdIter iter) {
  DestroyHashtableIterator(iter);
}

char *GetFileFromId(DocIdMap docs, int docId) {
  // STEP 2: Return the pointer to the filename
  //  that corresponds to the given docid.
  // If there's an issue of some kind, return NULL.
  HTKeyValue result;
  if (LookupInHashtable(docs, (uint64_t)docId, &result) == 0) {
    return result.value;
  }
  return "FAIL";
}
