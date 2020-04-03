/*
 *  Ruoyun Sun
 *  March 30th, updated CrawlFilesToMap
 *
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
#include <sys/stat.h>
#include <dirent.h>

#include "FileCrawler.h"
#include "DocIdMap.h"
#include "LinkedList.h"


void CrawlFilesToMap(const char *dir, DocIdMap map) {
  // STEP 3: Implement the file crawler.
  // Use namelist (declared below) to find files and put in map.
  // NOTE: There may be nested folders.
  // Be sure to lookup how scandir works. Don't forget about memory use.
  struct stat s;
  struct dirent **namelist;
  int n;
  n = scandir(dir, &namelist, 0, alphasort);
  printf("crawling dir: ");
  printf("%s\n", dir);
  if (n == -1) {
    return;
  }
  int i;
  i = n-1;
  while (i >= 0) {
    if (strcmp(namelist[i]->d_name, ".") == 0) {
      i--;
      continue;
    }
    if (strcmp(namelist[i]->d_name, "..") == 0) {
      i--;
      continue;
    }
    char cur[1024];
    strcpy(cur, dir);
    strcat(cur, namelist[i]->d_name);
    if (namelist[i]->d_type == DT_DIR) {
      strcat(cur, "/");
      CrawlFilesToMap((const char *)cur, map);
    } else {
      char* new = (char*)malloc(sizeof(cur));
      strcpy(new, cur);
      PutFileInMap(new, map);
    }
    i -= 1;
  }
  for (int i = 0; i < n; i++) {
    free(namelist[i]);
  }
  free(namelist);
}
