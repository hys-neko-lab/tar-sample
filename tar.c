#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tar.h"

struct header {
    char name[TAR_FIELD_LEN_NAME];
    char mode[TAR_FIELD_LEN_MODE];
    char uid[TAR_FIELD_LEN_UID];
    char gid[TAR_FIELD_LEN_GID];
    char size[TAR_FIELD_LEN_SIZE];
    char mtime[TAR_FIELD_LEN_MTIME];
    char chksum[TAR_FIELD_LEN_CHKSUM];
    char typeflag[TAR_FIELD_LEN_TYPEFLAG];
    char linkname[TAR_FIELD_LEN_LINKNAME];
    char magic[TAR_FIELD_LEN_MAGIC];
    char version[TAR_FIELD_LEN_VERSION];
    char uname[TAR_FIELD_LEN_UNAME];
    char gnmae[TAR_FIELD_LEN_GNAME];
    char devmajor[TAR_FIELD_LEN_DEVMAJOR];
    char devminor[TAR_FIELD_LEN_DEVMINOR];
    char prefix[TAR_FIELD_LEN_PREFIX];
    char padding[12]; // 0x00が12個
    char content[1]; // データ本体(512バイトアラインメント)
};

/**
 * @brief アーカイブに含まれるファイル数(非レギュラーファイル含む)の取得
 * 
 * @param bin I:tarのバイナリ
 * @param num O:ファイル数
 * @return int エラー情報
 */
int tarGetNum(char *bin, int *num)
{
    char *block;
    struct header *head;
    int i = 0;
    int align = 0;
    int size;

    block = bin;
    head = (struct header *)block;

    /* nameフィールドがNULL文字でなくなるまでブロックを数える */
    while(head->name[0] != '\0'){
        size = atoi(head->size);
        if (size % TAR_DATA_ALIGN != 0)
            align = 1;
        /* データは512バイトでアラインメントされるので、余りがあるなら+512バイト */
        block += (TAR_HEADER_LEN + TAR_DATA_ALIGN * (size / TAR_DATA_ALIGN + align));
        head = (struct header *)block;
        i++;
        align = 0;
    }

    *num = i;
    return 0;
}

/**
 * @brief 各ファイルのオフセット取得
 * 
 * @param bin I:tarのバイナリ
 * @param offset O:オフセットの配列
 * @return int エラー情報
 */
int tarGetOffset(char *bin, char **offset)
{
    char *block;
    struct header *head;
    int i = 0;
    int align = 0;
    int size;

    block = bin;
    head = (struct header *)block;

    /* nameフィールドがNULL文字でなくなるまでブロックを数える */
    while(head->name[0] != '\0'){
        offset[i] = block;

        size = atoi(head->size);
        if (size % TAR_DATA_ALIGN != 0)
            align = 1;
        /* データは512バイトでアラインメントされるので、余りがあるなら+512バイト */
        block += (TAR_HEADER_LEN + TAR_DATA_ALIGN * (size / TAR_DATA_ALIGN + align));
        head = (struct header *)block;
        i++;
        align = 0;
    }

    return 0;
}

/**
 * @brief ファイルのサイズ取得
 * 
 * @param offset I:ファイルのオフセット
 * @param size O:ファイルのサイズ
 * @return int エラー情報
 */
int tarGetSize(char *offset, int *size)
{
    struct header *head;
    head = (struct header *)offset;

    *size = strtol(head->size, NULL, 8);

    return 0;
}

/**
 * @brief ファイルの名前取得
 * 
 * @param offset I:ファイルのオフセット
 * @param name O:ファイルの名前
 * @return int 
 */
int tarGetName(char *offset, char *name)
{
    struct header *head;
    head = (struct header *)offset;

    strncpy(name, head->name, strlen(head->name));

    return 0;
}

/**
 * @brief ファイルのコンテンツ(データ本体)取得
 * 
 * @param offset I:ファイルのオフセット
 * @param content O:ファイルのコンテンツ
 * @param n I:コピーするバイト数
 * @return int エラー情報
 */
int tarGetContent(char *offset, char *content, int n)
{
    struct header *head;
    head = (struct header *)offset;

    strncpy(content, head->content, n);

    return 0;
}

/**
 * @brief ファイルのチェックサム情報取得および計算
 * 
 * @param offset I:ファイルのオフセット
 * @param checksum O:チェックサム文字列(8進数のASCII表現)
 * @return int チェックサム結果(0:一致, 1:不一致)
 */
int tarChecksum(char *offset, char *checksum)
{
    int i;
    unsigned int chk;
    unsigned int calc_check = 0;
    struct header *head;
    head = (struct header *)offset;

    /* checksumフィールドは8進数をASCII表現 */
    strncpy(checksum, head->chksum, TAR_FIELD_LEN_CHKSUM);
    chk = strtol(head->chksum, NULL, 8);

    /* チェックサム計算 */
    for(i = 0; i < TAR_HEADER_LEN; i++){
        if(i < TAR_FIELD_OFFSET_CHKSUM || 
            TAR_FIELD_OFFSET_CHKSUM + TAR_FIELD_LEN_CHKSUM <= i)
            calc_check += (unsigned int)(*(offset + i));
    }
    if(calc_check != chk) // FIXME: calc_checkが256足りず必ず不一致になる
        return 1;
    return 0;
}

/**
 * @brief ファイルタイプフラグの取得
 * 
 * @param offset I:ファイルのオフセット
 * @param typeflag O:ファイルタイプフラグ
 * @return int エラー情報
 */
int tarGetType(char *offset, char *typeflag)
{
    struct header *head;
    head = (struct header *)offset;

    *typeflag = head->typeflag[0];
}
