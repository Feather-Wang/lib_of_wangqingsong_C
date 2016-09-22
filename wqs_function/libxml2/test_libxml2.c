#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <string.h>

/* 在xml解析前，使用该函数对xml进行去除多余字符，最后一个字符应该是'>'，如果不是，就设置成'\0' */
static int xml_clear(char *databuff)
{
    int buf_size = strlen(databuff);
    int i = 1;
    while(databuff[buf_size-i] != '>'){
        databuff[buf_size-i] = '\0';
        ++i;
    }

    return 0;
}

void test_1()
{
    puts("test_1 .............................");
    char buf[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?><UploadApplyResponse Status=\"200\"><UploadCode>924e976bb082784ad5d154a0376314ed</UploadCode><Message>Success</Message></UploadApplyResponse>";
    puts(buf);
    xmlDocPtr doc = NULL;
    xmlChar *szKey;          /*临时字符串变量 */

    doc = xmlParseMemory(buf, strlen(buf));
    xmlNodePtr curNode = xmlDocGetRootElement(doc);
    if (xmlStrcmp(curNode->name, BAD_CAST "UploadApplyResponse")) 
    {
        fprintf(stderr,"document of the wrong type, root node != root\n"); 
        xmlFreeDoc(doc); 
        return ; 
    } 

    if (xmlHasProp(curNode,BAD_CAST "Status"))
    {
        xmlAttrPtr attrPtr = curNode->properties;
        //while (attrPtr != NULL)
        {
            if (!xmlStrcmp(attrPtr->name, BAD_CAST "Status"))
            {
                xmlChar* szAttr = xmlGetProp(curNode,BAD_CAST "Status");
                printf("[%d] %s : %s\n", __LINE__, attrPtr->name, szAttr);
                xmlFree(szAttr);
            }
            //attrPtr = attrPtr->next;
        }
    }

    curNode = curNode->xmlChildrenNode;
    while(curNode != NULL) 
    {
        szKey = xmlNodeGetContent(curNode);
        printf("[%d] %s : %s\n", __LINE__, curNode->name, szKey);
        xmlFree(szKey); 
        if (!xmlHasProp(curNode,BAD_CAST "Status"))
        {
            xmlAttrPtr attrPtr = curNode->properties;
            while (attrPtr != NULL)
            {
                if (!xmlStrcmp(attrPtr->name, BAD_CAST "Status"))
                {
                    xmlChar* szAttr = xmlGetProp(curNode,BAD_CAST "Status");
                    printf("[%d] %s : %s\n", __LINE__, attrPtr->name, szAttr);
                    xmlFree(szAttr);
                }
                attrPtr = attrPtr->next;
            }
        }
        curNode = curNode->next; 
    } 
    xmlFreeDoc(doc);
}

void test_2()
{
    puts("test_2 .............................");
    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
    xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"root");

    xmlDocSetRootElement(doc,root_node);

    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode1", BAD_CAST "newNode1 content");
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode2", BAD_CAST "newNode2 content");
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode3", BAD_CAST "newNode3 content");

    xmlNodePtr node = xmlNewNode(NULL,BAD_CAST"node2");
    xmlNodePtr content = xmlNewText(BAD_CAST"NODE CONTENT");
    xmlAddChild(root_node,node);
    xmlAddChild(node,content);
    xmlNewProp(node,BAD_CAST"attribute",BAD_CAST "yes");

    node = xmlNewNode(NULL, BAD_CAST "son");
    xmlAddChild(root_node,node);
    xmlNodePtr grandson = xmlNewNode(NULL, BAD_CAST "grandson");
    xmlAddChild(node,grandson);
    xmlAddChild(grandson, xmlNewText(BAD_CAST "This is a grandson node"));

    int nRel = xmlSaveFile("CreatedXml.xml",doc);

    xmlFreeDoc(doc);
}

void test_3()
{
    puts("test_3 .............................");
    xmlDocPtr doc;           /*定义解析文件指针*/
    xmlNodePtr curNode;       /*定义结点指针(你需要他为了在各个结点间移动) */
    xmlChar *szKey;          /*临时字符串变量 */
    char *szDocName;
    szDocName = "CreatedXml.xml";
    doc = xmlReadFile(szDocName,"GB2312",XML_PARSE_RECOVER); /*解析文件*/
    if (NULL == doc) 
    {  
        fprintf(stderr,"Document not parsed successfully\n");     
        return ; 
    } 
    curNode = xmlDocGetRootElement(doc); //确定文件根元素
    /*检查确认当前文件中包含内容*/ 
    if (NULL == curNode)
    { 
        fprintf(stderr,"empty document\n"); 
        xmlFreeDoc(doc); 
        return ; 
    } 
    /*在这个例子中，我们需要确认文件是正确的类型。“root”是在这个示例中使用文件的根类型。*/
    if (xmlStrcmp(curNode->name, BAD_CAST "root")) 
    {
        fprintf(stderr,"document of the wrong type, root node != root"); 
        xmlFreeDoc(doc); 
        return ; 
    } 
    curNode = curNode->xmlChildrenNode;
    xmlNodePtr propNodePtr = curNode;
    while(curNode != NULL) 
    {
        //if ((!xmlStrcmp(curNode->name, (const xmlChar *)"newNode1"))) 
        {
            szKey = xmlNodeGetContent(curNode);
        } 


        if (xmlHasProp(curNode,BAD_CAST "attribute"))
        {
            propNodePtr = curNode;
            printf("propNodePtr : %s\n", propNodePtr->name); 
        }

        printf("[%d] %s : %s\n", __LINE__, curNode->name, szKey);
            xmlFree(szKey); 
        curNode = curNode->next; 
    } 

    xmlAttrPtr attrPtr = propNodePtr->properties;
    while (attrPtr != NULL)
    {
        if (!xmlStrcmp(attrPtr->name, BAD_CAST "attribute"))
        {
            xmlChar* szAttr = xmlGetProp(propNodePtr,BAD_CAST "attribute");
            printf("[%d] %s : %s\n", __LINE__, attrPtr->name, szAttr);
            xmlFree(szAttr);
        }
        attrPtr = attrPtr->next;
    }
    xmlFreeDoc(doc);
}

int main(int argc, const char *argv[])
{
    test_1();

    test_2();

    test_3();

    return 0;
}
