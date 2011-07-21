// XMD operation.
#ifndef _ZYX_XMD_H
#define _ZYX_XMD_H

#include "ZyxTypes.h"  
#include "ZyxFile.h"

#define MAX_PARSE_LEVEL	10		// 最大解析层次
#define MAX_XMD_NAME_SIZE	32

#define XMD_PARSE_USE_MEM	// lijb 2010

typedef enum {
    XMD_FAIL	= -1,
    XMD_OK		= 0
} TXmdRet;

typedef long HNodeXmd;	// 节点句柄，文件来说就是偏移，内存则是地址。

#define OFF_XmdName(pNode)  (pNode->hNode+sizeof(pNode->info))
#define OFF_XmdAttr(pNode)  (OFF_XmdName(pNode)+pNode->info.len_name)
#define OFF_XmdValue(pNode) (OFF_XmdAttr(pNode)+pNode->info.len_attr)

// 节点信息
typedef struct tagXmdNodeInfo {
    HNodeXmd sibling;
    HNodeXmd child;
    unsigned short len_name;
    unsigned short len_attr;
    unsigned short len_value;
    unsigned short reserved;
} TXmdNodeInfo;


struct tagXmdParse;
// xmd节点。
typedef struct tagXmdNode{
    struct tagXmdParse *parse;			// lijb 2010
    //	JFS_HANDLE hFile;
    HNodeXmd hNode;
    TXmdNodeInfo info;
    char name[MAX_XMD_NAME_SIZE];
    // 	char *attr;
    // 	char *value;
} TXmdNode;


// XMD解析器
typedef struct tagXmdParse {
    ZFILE hFile;
    TXmdNode root;
    unsigned int iFileLen;		// lijb 2010 xmdfile len;
    char *pFileBuf;				// lijb 2010 xmdfile buf;
    unsigned int iTempBufLen;	// lijb 2010 
    char *pTempBuf;				// lijb 2010 tempstr;
} TXmdParse;

//////////////////////////////////////////////////////////////////////////
// public:
// 创建XMD解析器。
extern TXmdParse *new_XmdParse(char *xmlName);
// 释放XMD解析器
extern void del_XmdParse(TXmdParse *xmdParse);

// 获取节点值
extern char *get_XmdNode_Value(TXmdNode *pNode);
extern TXmdRet get_XmdNode_Value_To_Buf(TXmdNode *pNode, char *str, int size);

// 获取第index个子节点.
extern TXmdRet get_XmdChildNode_Index(TXmdNode *root, char *name, int index, TXmdNode *pNode);

// 获取第0个子节点.
extern TXmdRet get_XmdChildNode(TXmdNode *root, char *name, TXmdNode *pNode);
// 获取下个兄弟节点.
extern TXmdRet get_XmdSiblingNode(TXmdNode *root, TXmdNode *pNode);

// 获取子节点的值，返回分配好的字符串指针
// if ok, return alloced buffer. note: you  need free it yourself!
extern char *get_XmdChildNode_VarStr(TXmdNode *root, char *name);

// 返回子节点的ucs2值，返回分配好的字符串指针
extern void *get_XmdChildNode_Ucs2(TXmdNode *root, char *name);

// 获取子节点的值，并把值存到str中。
// if ok, return XMD_OK. str is your's buffer.
extern TXmdRet get_XmdChildNode_Str(TXmdNode *root, char *name, char *str, int size);

// 获取子节点的值，并把值以long形返回。
// if ok, return long value.
extern long get_XmdChildNode_Long(TXmdNode *root, char *name, int default_val);

extern int get_XmdNode_AttrByName(TXmdNode *pNode,char *attrName , char *attrVal);

#endif
