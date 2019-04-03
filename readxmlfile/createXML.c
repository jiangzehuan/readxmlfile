#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>

#define PHONE_BOOK_FILE   "Address_books.xml"
#define ID_STR_LEN        16
#define NAME_STR_LEN      16
#define GEN_STR_LEN       16
#define BIR_STR_LEN       64
#define SCH_STR_LEN       128
#define PRO_STR_LEN       128
int n = 0;
//通讯录结构体
typedef struct phone_t {
    int id;
    unsigned char name[NAME_STR_LEN];
    char gender[GEN_STR_LEN];
    char birthday[BIR_STR_LEN];
    char school[SCH_STR_LEN];
    char profession[PRO_STR_LEN];
}phone;

phone person[6]= {
    {1, "Anker", "male", "02.09", "schguangzhou", "elc"},
    {2, "Bnker", "male", "01.07", "schguangzhou", "elc"},
    {3, "Cnker", "male", "05.03", "schguangzhou", "elc"},
    {4, "Dnker", "male", "07.23", "schguangzhou", "elc"},
    {5, "Enker", "male", "09.01", "schguangzhou", "elc"},
    {6, "Fnker", "male", "06.03", "schguangzhou", "elc"},
};

//初始化
static void set_phone_item(phone *phone_item)
{
    assert(phone_item);
    phone_item->id = person[n].id;
    snprintf(phone_item->name, NAME_STR_LEN, "%s", person[n].name);
    snprintf(phone_item->gender, GEN_STR_LEN, "%s", person[n].gender);
    snprintf(phone_item->birthday, BIR_STR_LEN, "%s", person[n].birthday);
    snprintf(phone_item->school, SCH_STR_LEN, "%s", person[n].school);
    snprintf(phone_item->profession, PRO_STR_LEN, "%s", person[n].profession);
}

//创建phone节点——xmlNodePtr
static xmlNodePtr create_phone_node(const phone *phone_item)
{
    assert(phone_item);
    char id[ID_STR_LEN] = {0};
    xmlNodePtr phone_node = NULL;
    phone_node = xmlNewNode(NULL, BAD_CAST"phone");
    if (phone_node == NULL) 
    {
        fprintf(stderr, "Failed to create new node.\n");
        return NULL;
    }
    //设置属性
    snprintf(id, ID_STR_LEN, "%d", phone_item->id);
    xmlNewProp(phone_node, BAD_CAST"id", (xmlChar*)id);  
    //为子节点添加内容
    xmlNewChild(phone_node, NULL, BAD_CAST"name", (xmlChar *)phone_item->name);
    xmlNewChild(phone_node, NULL, BAD_CAST"gender", (xmlChar *)phone_item->gender);
    xmlNewChild(phone_node, NULL, BAD_CAST"birthday", (xmlChar *)phone_item->birthday);
    xmlNewChild(phone_node, NULL, BAD_CAST"school", (xmlChar *)phone_item->school);
    xmlNewChild(phone_node, NULL, BAD_CAST"profession", (xmlChar *)phone_item->profession);
    return phone_node;
}

//向根节点中添加一个phone节点
static int add_phone_node_to_root(xmlNodePtr root_node)
{
    xmlNodePtr phone_node = NULL;
    phone *phone_item = NULL;
    //创建一个新的phone
    phone_item = (phone *)malloc(sizeof(phone));
    if (phone_item == NULL) 
    {
        fprintf(stderr, "Failed to malloc memory.\n");
        return -1;
    }
    set_phone_item(phone_item);
    //创建一个phone节点
    phone_node = create_phone_node(phone_item); 
    if (phone_node == NULL) 
    {
        fprintf(stderr, "Failed to create phone node.\n");
        goto FAILED;
    }
    //根节点添加一个子节点
    xmlAddChild(root_node, phone_node);

    free(phone_item);
    return 0;
FAILED:
    if (phone_item)
    {
        free(phone_item);
    }
    return -1;
}

//创建phone_books
static int create_phone_books(const char *phone_book_file)
{
    assert(phone_book_file);
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;
    doc = xmlNewDoc(BAD_CAST"1.0");  //创建一个xml 文档
    if (doc == NULL) 
    {
        fprintf(stderr, "Failed to new doc.\n");
        return -1;
    }
    root_node = xmlNewNode(NULL, BAD_CAST"phone_books");  //创建根节点
    if (root_node == NULL) 
    {
        fprintf(stderr, "Failed to new root node.\n");
        goto FAILED;
    }
    xmlDocSetRootElement(doc, root_node);   //将根节点添加到文档中
    if (add_phone_node_to_root(root_node) != 0) 
    {
        fprintf(stderr, "Failed to add a new phone node.\n");
        goto FAILED;
    }
    //将文档保存到文件中，按照utf-8编码格式保存
    xmlSaveFormatFileEnc(phone_book_file, doc, "UTF-8", 1);
    xmlFreeDoc(doc);
    n = 1;
    return 0; 
FAILED:
    if (doc) 
    {
        xmlFreeDoc(doc);
    }
    return -1;
}

static int add_phone_node(const char *phone_book_file)
{
    assert(phone_book_file);

    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;
    xmlNodePtr phone_node = NULL;
    phone *phone_item = NULL;
    
    doc = xmlParseFile(phone_book_file);
    if (doc == NULL) 
    {
        fprintf(stderr, "Failed to parser xml file:%s\n", phone_book_file);
        return -1;
    }
    root_node = xmlDocGetRootElement(doc);
    if (root_node == NULL) 
    {
        fprintf(stderr, "Failed to get root node.\n");
        goto FAILED;
    }
    
    if (add_phone_node_to_root(root_node) != 0) 
    {
        fprintf(stderr, "Failed to add a new phone node.\n");
        goto FAILED;
    }
    //将文档保存到文件中，按照utf-8编码格式保存
    xmlSaveFormatFileEnc(phone_book_file, doc, "UTF-8", 1);
    xmlFreeDoc(doc);

    return 0;
FAILED:
    if (doc) 
    {
        xmlFreeDoc(doc);
    }

    return -1;
}

int main(int argc, char *argv[])
{
    char *phone_book_file = PHONE_BOOK_FILE;

    if (argc == 2) 
    {
        phone_book_file = argv[1];
    }
    while(n < 6)
    {
        if (access(phone_book_file, F_OK) == 0) 
        {
            //文件存在，添加一个新的phone节点
            add_phone_node(phone_book_file);
            n++;  //将结构体数组的内容全部添加进去
        }
        else 
        {
            //文件不存在，创建一个信息的phone book
            create_phone_books(phone_book_file);
        
        }
    }

    return 0;
}