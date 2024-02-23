#include <stdio.h>
#include <sys/types.h>
#include <sys/sysmacros.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>


/*typedef struct header {

  char *name;       comes from filePath
  char *mode;       comes from struct
  char *uid;        comes from stat
  char *gid;        comes from stat
  char *size;       comes from stat
  char *mtime;      coems from stat
  char *chksum;     
  char typeflag;    use macros on stat.st_mode
  char *linkname;   only if it is a link
  char *magic;      always "ustat\0"
  uint16_t version; always "00";
  char *uname;      comes from getpwuid
  char *gname;      coems from getgrgid
  char *devmajor;   ?
  char devminor;    ?
  char *prefix;     later 155 bytes of name
  

} header;*/

#ifndef HEADER_FILE
#define HEADER_FILE

char *insertHeader(char*, char*, int, int, int);

char *convertToStr(int, int, int);

char *makeHeader(char *filePath);

#endif
