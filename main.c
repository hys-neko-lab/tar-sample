#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "tar.h"

int main(int argc, char *argv[])
{
    /* バイナリでのファイルオープン */
    FILE* fp = fopen(argv[1], "rb");
    if (fp == NULL){
        printf("File open failure.\n");
        exit(EXIT_FAILURE);
    }

    /* ファイルサイズの取得 */
    struct stat statBuf;
    size_t size;
    stat(argv[1], &statBuf);
    size = statBuf.st_size;

    /* バッファに読込み */
    char *bin;
    bin = (char *)malloc(size);
    fread(bin, 1, size, fp);

    /* 表示 */
    int num, dsize, ret_chk;
    char name[TAR_FIELD_LEN_NAME];
    char chk[TAR_FIELD_LEN_CHKSUM];
    char **offset;
    char *content;
    char typeflag;
    /* ファイル数の取得 */
    tarGetNum(bin, &num);
    printf("File num:%d\n", num);
    /* ファイル数分、オフセット情報取得 */
    offset = (char **)malloc(num);
    tarGetOffset(bin, offset);
    /* 各情報の表示 */
    for(int i = 0; i < num; i++){
        tarGetSize(offset[i], &dsize);
        tarGetName(offset[i], name);
        tarGetType(offset[i], &typeflag);
        ret_chk = tarChecksum(offset[i], chk);
        printf(
            "Name:%s, Size:%d, Type:%c, Check:%s\n",
            name, dsize, typeflag, chk
        );
        /* 通常ファイルなら中身をASCII表示 */
        if(typeflag=='0') {
            content = (char *)malloc(dsize);
            tarGetContent(offset[i], content, dsize);
            printf("%s\n", content);
        }
        /* バッファのクリア */
        memset(name, 0, TAR_FIELD_LEN_NAME);
        memset(chk, 0, TAR_FIELD_LEN_CHKSUM);
    }

    free(offset);
    free(content);
    exit(EXIT_SUCCESS);
}