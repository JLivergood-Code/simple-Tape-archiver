#include "header.h"


int mian(int argc, char *argv[]){

  /* if c, then for every file/file directory, add it to the tarfile */

  /* if x, for every file/file directory in tarfile, create coresponding file
 *  in current location */

  /* if t, for every file/file directory in tarfile, display information about
 * that file */

  /* if f, specifies the tar filepath in the next argument */

  /* if S, must follow exact specifications to read/create:
 * version in header must be 00
 * ustar must be ustar\0
 */

  /* if v, displays everything that is happening */

  int c_flag;
  int v_flag;
  int x_flag;
  int t_flag;
  int S_flag;

 
  /* HEADER TESTING */

  char *header = makeHeader("./header.c"); 
  fwrite(header, 512, 1, stdout);

  return 0;
}
