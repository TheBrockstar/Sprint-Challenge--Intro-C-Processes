#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

/**
 * Main
 */
int filestructure(char *path, int depth) {
  struct dirent *name;
  struct stat buffer;
  char *filepath;
  DIR *directory;

  // Open directory and Check if directory could not be opened:
  if ((directory = opendir(path)) == NULL) {
    printf("Directory could not be opened: %s\n", path);
    exit(1);
  }

  // Repeatly read and print entries:
  while((name = readdir(directory)) != NULL) {
    // +2 for the Null Pointer and a "/" 
    filepath = malloc(strlen(path) + strlen(name -> d_name) + 2);

    // Build filepath for stat() and Check if the item name is "." or "..":
    if (strcmp(name -> d_name, ".") != 0 && strcmp(name -> d_name, "..") != 0) {
      sprintf(filepath, "%s/%s", path, name -> d_name);
    } else {
      sprintf(filepath, "%s", name -> d_name);
    }

    // Get stats of file and Check if unable to get stats:
    if (stat(filepath, &buffer) != -1) {
      // If item is a folder:
      if(buffer.st_mode == 16895) {


          if (depth > 1) {
            printf("\n     <DIR> %s\n", name -> d_name);
            filestructure(filepath, depth - 1);
          } else {
            printf("          <DIR> %s\n", name -> d_name);
          }

      } else {
        if (depth == 1) {
          printf("%15ld %s\n", buffer.st_size, name -> d_name);
        } else {
          printf("%10ld %s\n", buffer.st_size, name -> d_name);
        }
      }     
    }
    
    free(filepath);
  }

// Close directory
closedir(directory);
return 0;
}

int main(int argc, char **argv)
{
  char *path;

  // Parse command line and Check for empty command line:
  if (argc > 1) {
    path = argv[1];
  } else {
    path = ".";
  }

  filestructure(path, 2);

  return 0;
}