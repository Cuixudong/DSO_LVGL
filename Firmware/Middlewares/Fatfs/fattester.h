#ifndef __FATTESTER_H
#define __FATTESTER_H

#include "main.h"
#include "ff.h"

uint8_t mf_mount(char *path,uint8_t mt);
uint8_t mf_open(char *path,uint8_t mode);
uint8_t mf_close(void);
uint8_t mf_read(uint16_t len);
uint8_t mf_write(uint8_t *dat,uint16_t len);
uint8_t mf_opendir(char *path);
uint8_t mf_closedir(void);
uint8_t mf_readdir(void);
uint8_t mf_scan_files(char * path);
uint32_t mf_showfree(char *drv);
uint8_t mf_lseek(uint32_t offset);
uint32_t mf_tell(void);
uint32_t mf_size(void);
uint8_t mf_mkdir(char *pname);
uint8_t mf_fmkfs(char *path,uint8_t mode,uint16_t au);
uint8_t mf_unlink(char *pname);
uint8_t mf_rename(char *oldname,char* newname);
void mf_getlabel(char *path);
void mf_setlabel(char *path);
void mf_gets(uint16_t size);
uint8_t mf_putc(char c);
uint8_t mf_puts(char *c);

#endif
