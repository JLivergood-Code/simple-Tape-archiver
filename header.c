#include "header.h"
#define HEADER_SIZE 512

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


char *makeHeader(char *filePath){
  int pathSize = strlen(filePath);
  struct stat *statVal;
  struct passwd *pwd;
  struct group *grpwd;
  char *header;

  int size;
  int gid;
  int uid;
  int mtime;
  char typeflag;
  mode_t mode;
  dev_t dev;

  unsigned int majorNum;
  unsigned int minorNum;

  char *uname;
  char *grname;
  char *linkname;

  char *modeStr;
  char *outStr;

/* Mallocs everything that is needed */
  if(NULL == (header = malloc(HEADER_SIZE))){
    perror("malloc");
    return NULL;
  }
  
  if(NULL == (pwd = malloc(sizeof(struct passwd)))){
    perror("malloc");
    return NULL;
  }
  
  if(NULL == (grpwd = malloc(sizeof(struct group)))){
    perror("malloc");
    return NULL;
  }

  if(NULL == (statVal = malloc(sizeof(struct stat)))){
    perror("malloc");
    return NULL;
  }  
  if(NULL == (linkname = malloc(sizeof(char) * 100))){
    perror("malloc");
    return NULL;
  }


  /* Truncates Path */
  if(pathSize < 100){
    header = insertHeader(header, filePath, 0, pathSize, 100);   
  } else {
    /* WHAT IF PATH IS > 100 */
    /* insert first half of path size */
    header = insertHeader(header, filePath, 0, 100, 100);
    /* Insert rest of path size*/
    header = insertHeader(header, (filePath+100), 345, pathSize-100, 155);
  }

  /* read in values form stat */
  if(stat(filePath, statVal)){
    perror("stat");
    return NULL;
  }
  
  size = statVal->st_size;
  uid = statVal->st_uid;
  gid = statVal->st_gid; 
  mtime = statVal->st_mtime;
  dev = statVal->st_dev;

  mode = statVal->st_mode;
  modeStr = convertToStr(mode, 8, 8);
  
  /*out put valeus form stat */
  /*printf("size: %o\nuid: %o\ngid: %o\nmtime: %d\n", size, uid, gid, mtime);*/
  /*printf("mode: %o\nstrMode: %s\n", mode, modeStr);*/

  /* format and insert into header */
  header = insertHeader(header, modeStr, 100, sizeof(modeStr), 8);
  
  outStr = convertToStr(uid, 8, 8);
  header = insertHeader(header, outStr, 108, sizeof(outStr), 8);
  free(outStr);  

  outStr = convertToStr(gid, 8, 8);
  header = insertHeader(header, outStr, 116, sizeof(outStr), 8);
  free(outStr);  
  
  outStr = convertToStr(mtime, 12, 8);
  header = insertHeader(header, outStr, 136, sizeof(outStr), 12);
  free(outStr);  

  outStr = convertToStr(size, 12, 8);
  header = insertHeader(header, outStr, 124, sizeof(outStr), 12);
  free(outStr);
  
/* tree fo typeflag indicator */
  if(S_ISREG(mode)){
    typeflag = '0';
    strcpy(linkname, "");
  } else if(S_ISDIR(mode)){
    typeflag = '5';
    strcpy(linkname, "");
  } else if(S_ISLNK(mode)){
    typeflag = '2';
    strcpy(linkname, filePath); 
  } else {
    typeflag = '\0';
    strcpy(linkname, "");
  }
  /*printf("type: %c\n", typeflag);*/

  header = insertHeader(header, &typeflag, 156, 1, 1); 
 
/* gets pw name and gr name */
  pwd = getpwuid(uid);

  if(pwd->pw_uid == uid){
    uname = pwd->pw_name;
  } else {
    perror("Get uname");
    return NULL;
  }

  grpwd = getgrgid(gid);

  if(grpwd->gr_gid == gid){
    grname = grpwd->gr_name;
  }else {
    perror("Get Group");
    return NULL;
  }
  
  /* inserts uname, gname, magic and version */
  header = insertHeader(header, uname, 265, sizeof(uname), 32);
  header = insertHeader(header, grname, 297, sizeof(grname), 32);
 
  header = insertHeader(header, "ustar", 257, sizeof("ustar"), 6);
  header = insertHeader(header, "00", 263, sizeof("00"), 2);

  majorNum = major(dev);
  minorNum = minor(dev);
  

  outStr = convertToStr(majorNum, 8, 8);
  header = insertHeader(header, outStr, 329, sizeof(outStr), 8);
  free(outStr);  
  
  outStr = convertToStr(minorNum, 8, 8);
  header = insertHeader(header, outStr, 337, sizeof(outStr), 8);
  free(outStr);  

  /* frees */
  free(statVal); 
  free(linkname); 
  /*free(pwd);
  free(grpwd); */
 
  return header;

}

/* reads in the number to conver, the maxStr size and the base to convert to*/
char *convertToStr(int inNum, int maxStr, int base){
  char *numStr;
  int i = 0;
  int outNum;

  int N = inNum;

  if(NULL == (numStr = malloc(maxStr*sizeof(char)))){
    perror("malloc");
    return NULL;
  }

  while(N > 0){
    outNum = N % base;
    outNum += 48;    
   
    /* shifts number over by 1 and inserts char at start of string*/ 
    memmove(numStr+1, numStr, strlen(numStr)+1);
    numStr[0] = outNum;

    N = N / base;
    i++;
  }

  return numStr;

}

char *insertHeader(char *header,char *val,int offset,int numBytes,int totBytes)
{

  int i;

  /* for every value in valure string that needs to be added */
  for(i = 0; i < totBytes; i++){

    if(i < numBytes){
    /* places value in string detirmined by the offset */
      header[offset + i] = val[i]; 
    } else if( i == totBytes-1){
      header[offset + i] = '\0';
    } else {
      /*used for debugging purposes to add spaces/newlines */
      header[offset + i] = '\0';
    }
  }
 
  return header;

}

int main(int argc, char *argv[]){
  char *header;

  header = makeHeader("./header.c");
  fwrite(header, 512, 1, stdout); 

  free(header);

  return 0;
}
