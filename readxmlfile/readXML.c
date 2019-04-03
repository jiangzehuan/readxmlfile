#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#define DEFAULT_XML_FILE "Address_books.xml"
#define PHONE_BOOK_FILE   "new_books.xml"
#define ID_STR_LEN        16
#define NAME_STR_LEN      16
#define GEN_STR_LEN       16
#define BIR_STR_LEN       64
#define SCH_STR_LEN       128
#define PRO_STR_LEN       128

typedef struct phone_t {
    int id;                      
    unsigned char name[NAME_STR_LEN];     
    unsigned char gender[GEN_STR_LEN];    
    unsigned char birthday[BIR_STR_LEN];  
    unsigned char school[SCH_STR_LEN];    
    unsigned char profession[PRO_STR_LEN];
}phone;

phone person[6]= {0};
int n = 0;
int count = 0;
static int parse_phone(xmlDocPtr doc, xmlNodePtr cur)
{
    assert(doc || cur);
    xmlChar *key = NULL;
    cur = cur->xmlChildrenNode;
    while (cur != NULL) 
	{
		//获取name
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"name"))) 
		{
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			strcpy(person[n].name, key);
			printf("name: %s\t", key);
			xmlFree(key);
		}
		//获取gender
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"gender"))) 
		{
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			strcpy(person[n].gender, key);
			printf("gender %s\t", key);
			xmlFree(key);
		}

		//获取birthday
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"birthday"))) 
		{
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			strcpy(person[n].birthday, key);
			printf("birthday: %s\t", key);
			xmlFree(key);
		}

		//获取school
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"school"))) 
		{
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			strcpy(person[n].school, key);
			printf("school: %s\t", key);
			xmlFree(key);
		}
		//获取profession
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"profession"))) 
		{
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			strcpy(person[n].profession, key);
			printf("profession: %s\n", key);
			xmlFree(key);
		}
		cur = cur->next;
		}
    return 0;
}

static int parse_phone_book(const char *file_name)
{
    assert(file_name);
    xmlDocPtr doc;   //xml整个文档的树形结构
    xmlNodePtr cur;  //xml节点
    xmlChar *id;     //phone id

    //获取树形结构
    doc = xmlParseFile(file_name);
    if (doc == NULL) 
	{
		fprintf(stderr, "Failed to parse xml file:%s\n", file_name);
		goto FAILED;
    }

    //获取根节点
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) 
	{
		fprintf(stderr, "Root is empty.\n");
		goto FAILED;
    }

    if ((xmlStrcmp(cur->name, (const xmlChar *)"phone_books"))) 
	{
		fprintf(stderr, "The root is not Address_books.\n");
		goto FAILED;
    }

    //遍历处理根节点的每一个子节点
    cur = cur->xmlChildrenNode;
    while (cur != NULL) 
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"phone"))) 
		{
			id = xmlGetProp(cur, "id");
			printf("id:%s\t",id);
			person[n].id = atoi(id);
			parse_phone(doc, cur);
			n++;

		}
		cur = cur->next;
    }
    //count = n;
    xmlFreeDoc(doc);
    return 0;
FAILED:
    if (doc) 
	{
		xmlFreeDoc(doc);
    }
    return -1;
}

void sort(int num)  //按照生日排序
{
	int i, j;
	phone *tmp = NULL;
	tmp = (phone *)malloc(sizeof(phone));
	for(i = 0; i < num-1; i++)
	{
		for(j = 0; j < num-i-1; j++)
		{
			if( strcmp(person[j].birthday, person[j+1].birthday) > 0) //j>j+1 返回>0
			{
				tmp[j].id = person[j].id;
				strcpy(tmp[j].name, person[j].name);
				strcpy(tmp[j].gender, person[j].gender);
				strcpy(tmp[j].birthday, person[j].birthday);
				strcpy(tmp[j].school, person[j].school);
				strcpy(tmp[j].profession, person[j].profession);

				person[j].id = person[j+1].id;
				strcpy(person[j].name, person[j+1].name);
				strcpy(person[j].gender, person[j+1].gender);
				strcpy(person[j].birthday, person[j+1].birthday);
				strcpy(person[j].school, person[j+1].school);
				strcpy(person[j].profession, person[j+1].profession);

				person[j+1].id = tmp[j].id;
				strcpy(person[j+1].name, tmp[j].name);
				strcpy(person[j+1].gender, tmp[j].gender);
				strcpy(person[j+1].birthday, tmp[j].birthday);
				strcpy(person[j+1].school, tmp[j].school);
				strcpy(person[j+1].profession, tmp[j].profession);
				
			}
		}
	}
	free(tmp);
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

//初始化
static void set_phone_item(phone *phone_item)
{
    assert(phone_item);
    phone_item->id = person[count].id;
    snprintf(phone_item->name, NAME_STR_LEN, "%s", person[count].name);
    snprintf(phone_item->gender, GEN_STR_LEN, "%s", person[count].gender);
    snprintf(phone_item->birthday, BIR_STR_LEN, "%s", person[count].birthday);
    snprintf(phone_item->school, SCH_STR_LEN, "%s", person[count].school);
    snprintf(phone_item->profession, PRO_STR_LEN, "%s", person[count].profession);
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
    count = 1;
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


int main(int argc, char*argv[])
{
    char *xml_file = DEFAULT_XML_FILE;
    if (argc == 2) 
	{
		xml_file = argv[1];
    }
    if (parse_phone_book(xml_file) != 0) 
	{
		fprintf(stderr, "Failed to parse phone book.\n");
		return -1;
    }
    sort(n);
    char *phone_book_file = PHONE_BOOK_FILE;
    while(count < n)
    {
        if (access(phone_book_file, F_OK) == 0) 
        {
            //文件存在，添加一个新的phone节点
            add_phone_node(phone_book_file);
            count++;  //将从xml文件读取到的内容全部重新写入新的xml文件中
        }
        else 
        {
            //文件不存在，创建一个信息的phone book
            create_phone_books(phone_book_file);
        
        }
    }
    return 0;
}