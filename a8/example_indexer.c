/*
 *  Ruoyun Sun
 *  3/20 updated some functions
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
#include <assert.h>
#include <string.h>

#include <ctype.h>
#include <unistd.h>


#include "htll/LinkedList.h"
#include "MovieIndex.h"
#include "Movie.h"
#include "MovieReport.h"
#include "./FileParser.h"

void DestroyNothing(void* thing) {
  // Helper method to destroy the LinkedList.
}

int main(int argc, char* argv[]) {
  // STEP 8(Student): Check args, do the right thing.
  if (argc <= 1) {
    printf("no argument provided\n");
    return -1;
  }
  if (argc == 2) {
    if (argv[1][0] == '-') {
      printf("no filename provided\n");
    } else {
      printf("no flag provided\n");
    }
    return -1;
  }
  if (argc > 3) {
    printf("too many arguments\n");
    return -1;
  }
  char * filename = NULL;
  enum IndexField field;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-s") == 0) {
        field = StarRating;
      } else if (strcmp(argv[i], "-c") == 0) {
        field = ContentRating;
      } else if (strcmp(argv[i], "-a") == 0) {
        field = Actor;
      } else if (strcmp(argv[i], "-g") == 0) {
        field = Genre;
      } else {
        printf("undefined option");
        return -1;
      }
    } else if (filename == NULL) {
        filename = argv[i];
    } else {
      printf("missing arguments\n");
      return -1;
    }
  }
  if (filename == NULL) {
    printf("no filename provided\n");
    return -1;
  }

  LinkedList movie_list  = ReadFile(filename);
  // STEP 9(Student): Create the index properly.
  Index index = BuildMovieIndex(movie_list, field);
  PrintReport(index);

  DestroyLinkedList(movie_list, &DestroyNothing);
  DestroyIndex(index);
  return 0;
}
