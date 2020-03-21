/*
 *  Ruoyun Sun
 *  3/20 updated some methods
 *
 *  Adrienne Slaughter
 *  5007 Spr 2020
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

#include <stdio.h>
#include <stdlib.h>

#include "MovieIndex.h"
#include "MovieReport.h"
#include "Movie.h"
#include "MovieSet.h"
#include "htll/LinkedList.h"
#include "htll/Hashtable.h"


void PrintReport(Index index) {
  // Create Iter
  HTIter iter = CreateHashtableIterator(index);

  HTKeyValue movie_set;

  HTIteratorGet(iter, &movie_set);
  OutputMovieSet((MovieSet)movie_set.value);

  while (HTIteratorHasMore(iter)) {
    HTIteratorNext(iter);
    HTIteratorGet(iter, &movie_set);
    OutputMovieSet((MovieSet)movie_set.value);
  }
  // For every movie set, create a LLIter
  DestroyHashtableIterator(iter);
}

void OutputMovieSet(MovieSet movie_set) {
  // STEP 7(Student): Print the MovieSet to the terminal.
  printf("indexType: %s\n", movie_set->desc);
  printf("%d items\n", NumElementsInLinkedList(movie_set->movies));
  if (NumElementsInLinkedList(movie_set->movies) == 0) {
    return;
  }
  if (NumElementsInLinkedList(movie_set->movies) == 1) {
    LLIter iter = CreateLLIter(movie_set->movies);
    Movie* payload = NULL;
    LLIterGetPayload(iter, (void**)&payload);
    printf("    %s\n", payload->title);
    DestroyLLIter(iter);
    return;
  }
  LLIter iter = CreateLLIter(movie_set->movies);
  Movie* payload = NULL;
  LLIterGetPayload(iter, (void**)&payload);
  printf("    %s\n", payload->title);
  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter, (void**)&payload);
    printf("    %s\n", payload->title);
  }
  DestroyLLIter(iter);
  return;
}


