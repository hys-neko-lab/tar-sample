#ifndef __TAR_H__
#define __TAR_H__

#define TAR_HEADER_LEN          (512)
#define TAR_DATA_ALIGN          (512)
#define TAR_FIELD_OFFSET_CHKSUM (148)
#define TAR_FIELD_LEN_NAME      (100)
#define TAR_FIELD_LEN_MODE      (8)
#define TAR_FIELD_LEN_UID       (8)
#define TAR_FIELD_LEN_GID       (8)
#define TAR_FIELD_LEN_SIZE      (12)
#define TAR_FIELD_LEN_MTIME     (12)
#define TAR_FIELD_LEN_CHKSUM    (8)
#define TAR_FIELD_LEN_TYPEFLAG  (1)
#define TAR_FIELD_LEN_LINKNAME  (100)
#define TAR_FIELD_LEN_MAGIC     (6)
#define TAR_FIELD_LEN_VERSION   (2)
#define TAR_FIELD_LEN_UNAME     (32)
#define TAR_FIELD_LEN_GNAME     (32)
#define TAR_FIELD_LEN_DEVMAJOR  (8)
#define TAR_FIELD_LEN_DEVMINOR  (8)
#define TAR_FIELD_LEN_PREFIX    (155)


int tarGetNum(char *bin, int *num);
int tarGetOffset(char *bin, char **offset);
int tarGetSize(char *offset, int *size);
int tarGetName(char *offset, char* name);
int tarGetContent(char *offset, char* content, int n);
int tarChecksum(char *offset, char *checksum);
int tarGetType(char *offset, char *typeflag);

#endif