#include <stdint.h>
#include <stdio.h>
#include <string.h> /* memset */
#include "sha2.h"
#include "tests.h"
const char *const filename="/tmp/128-1288991-302901023-174658.txt";

unsigned char ascii_hex_char(char ascii) {
  unsigned char ret=0;
  if ( ascii >= 'A' && ascii <= 'F' ) {
    ascii -= 'A';
  }
  if ( ascii >= 'a' && ascii <= 'f' ) {
    ret = (unsigned char)(10+ascii-'a');
  } else if ( ascii >= '0' && ascii <= '9' ) {
    ret = (unsigned char)(ascii - '0');
  }
  return ret;
}
void
convert_hex(const char *const ascii_hex,
    unsigned char *const data,
    const size_t data_len) {
  size_t i=0;
  unsigned char c=0;
  for(i=0;i<data_len;i++) {
    c=ascii_hex_char(ascii_hex[2*i]) << 4;
    c |=ascii_hex_char(ascii_hex[2*i+1]);
    data[i]=c;
  }
}

bool
execute_sha256sum(const char *const fn, unsigned char *const digest) {
  FILE *f=NULL;
  char command[80];
  char ch_di[SHA256_DIGEST_SIZE*2+1];
  memset(command,0,sizeof(command));
  memset(ch_di,0,sizeof(ch_di));
  f=NULL;
  snprintf(command,sizeof(command),"sha256sum %s",fn);
  f=popen(command,"r");
  if ( f == NULL ) {
    fprintf(stderr,"can not execute command.'%s'\n",command);
    return false;
  }
  size_t num = fread(ch_di,sizeof(char),sizeof(ch_di)-1,f);
  fclose(f);
  if ( num != sizeof(ch_di) - 1 ) {
    fprintf(stderr,"output '%s' of '%s' is not what expected\n",ch_di,command);
    return false;
  }
  convert_hex(ch_di,digest,SHA256_DIGEST_SIZE);
  printf("'%s' is\n '%s'=\n '",command,ch_di);
  for(num=0;num < SHA256_DIGEST_SIZE;num++) {
    printf("%02x",digest[num]);
  }
  printf("'\n");
  return true;
}
bool compute_sha256sum(const char *const fn,unsigned char *const digest) {
  FILE *f = fopen(fn,"r");
  sha256_ctx ctx;
  const unsigned char buffer[256];
  if ( f == NULL ) {
    fprintf(stderr,"can not open file '%s'\n",fn);
    return false;
  }
  size_t sz = 0;
  sha256_init(&ctx);
  while( (sz = fread((void*)buffer,sizeof(char),sizeof(buffer),f) ) > 0 ) {
    sha256_update(&ctx,buffer,sz);
  }
  sha256_final(&ctx,digest);
  bool ret = (bool)feof(f);
  fclose(f);
  return ret;
}

unsigned char sha256sum[SHA256_DIGEST_SIZE+1];
unsigned char my_sha256sum[SHA256_DIGEST_SIZE+1];

extern bool prepare_test() {
  memset(sha256sum,0,SHA256_DIGEST_SIZE+1);
  memset(my_sha256sum,0,SHA256_DIGEST_SIZE+1);
  FILE *f = fopen(filename,"w");
  if ( f == NULL ) {
    fprintf(stderr,"can not open file for writing '%s'\n",filename);
    return false;
  }
  fprintf(f,"This is a file to test sha256sum lib.\n filename is '%s'. it should be removed automaticalty.",filename);
  fclose(f);
  return execute_sha256sum(filename,sha256sum);
}

extern bool do_test() {
  bool ret = compute_sha256sum(filename,my_sha256sum);
  printf(" '");
  for(size_t num=0;num < SHA256_DIGEST_SIZE;num++) {
    printf("%02x",my_sha256sum[num]);
    if ( my_sha256sum[num] != sha256sum[num] ) {
      ret = false;
    }
  }
  printf("'\n");
  return ret;
}

extern void tear_down_test() {
  remove(filename);
}

