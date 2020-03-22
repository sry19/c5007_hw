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
#include <stdint.h>
#include <assert.h>


#include "gtest/gtest.h"

extern "C" {
  #include "./MovieIndex.h"
  #include "htll/LinkedList.h"
  #include "MovieIndex.h"
  #include "Movie.h"
  #include "MovieReport.h"
  #include "FileParser.h"
  #include <string.h>
}

const char* movie_row_A = "9.3|The Shawshank Redemption|R|Crime|142|Tim Robbins,Morgan Freeman,Bob Gunton";
const char* movie_row_B = "7.4|Back to the Future Part III|PG|Adventure|118|Michael J. Fox,Christopher Lloyd,Mary Steenburgen";
const char* movie_row_C = "9.6|ABC|R|Crime|1|Bob·Gunton";

void DestroyLLMovie(void *payload) {
  DestroyMovie((Movie*)payload);
}

void DoNothing(void *payload) {
}

TEST(MovieSet, CreateDestroy) {
  MovieSet set = CreateMovieSet("My test set");
  ASSERT_NE(set, nullptr);
  DestroyMovieSet(set);
}

TEST(MovieSet, AddOneMovie) {
  MovieSet set = CreateMovieSet("My test set");
  ASSERT_NE(set, nullptr);
  ASSERT_EQ(NumElementsInLinkedList(set->movies), 0);

  char row[1000];
  strcpy(row, movie_row_A);
  MoviePtr movie = CreateMovieFromRow(row);
  int res = AddMovieToSet(set, movie);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(NumElementsInLinkedList(set->movies), 1);

  DestroyMovieSet(set);
}

TEST(MovieIndex, CreateDestroy) {
  Index index = CreateIndex();
  ASSERT_NE(index, nullptr);
  DestroyIndex(index);
}

TEST(MovieIndex, AddMovieToIndex) {
  // Copying the string from a const to an array
  // (to be more similar to the actual use case)
  char row[1000];
  strcpy(row, movie_row_A);
  // Take a movie
  MoviePtr m1 = CreateMovieFromRow(row);
  strcpy(row, movie_row_B);
  MoviePtr m2 = CreateMovieFromRow(row);

  // Create Index
  Index index = CreateIndex();

  // Add movie to index
  AddMovieToIndex(index, m1, ContentRating);

  // Check that movie is in index

  // Check size/num elements (should be num of types, not of movies)
  ASSERT_EQ(NumElemsInHashtable(index), 1);

  // Try to insert movie again
  AddMovieToIndex(index, m1, ContentRating);
  // DONE(Ruoyun): test if we cannot add the same movie to the index
  ASSERT_EQ(NumElemsInHashtable(index), 1);
  // Add another movie to the index (same IndexType)
  AddMovieToIndex(index, m2, ContentRating);
  ASSERT_EQ(NumElemsInHashtable(index), 2);

  // DONE(Ruoyun):check if movies have similar contentrating are in
  // the same movieset
  strcpy(row, movie_row_C);
  MoviePtr m3 = CreateMovieFromRow(row);
  AddMovieToIndex(index, m3, ContentRating);
  ASSERT_EQ(NumElemsInHashtable(index), 2);

  // Destroy movie index
  DestroyIndex(index);
}


TEST(MovieIndex, BuildMovieIndexFromFile) {
  // DONE(Ruoyun): build an empty index
  LinkedList empty_list = CreateLinkedList();
  Index index1 = BuildMovieIndex(empty_list, Genre);
  ASSERT_EQ(true, NumElemsInHashtable(index1) == 0);
  DestroyIndex(index1);
  free(empty_list);

  LinkedList movie_list  = ReadFile(const_cast<char *>("data/test"));

  ASSERT_EQ(5u, NumElementsInLinkedList(movie_list));

  Index index = BuildMovieIndex(movie_list, Genre);

  // Do a few spot checks.
  // E.g., Pull out a MovieSet from the Hashtable;
  // Check to see if the set has expected number of movies,
  // a particular movie, etc.
  // DONE(Ruoyun): check if similar movies are in the same set
  ASSERT_EQ(true, NumElemsInHashtable(index) <= 3);
  DestroyLinkedList(movie_list, &DoNothing);
  DestroyIndex(index);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
