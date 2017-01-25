#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

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

void test_read_xml_string()
{
    puts("test_read_xml_string .............................");
    char buf[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?><UploadApplyResponse Status=\"200\"><UploadCode>924e976bb082784ad5d154a0376314ed</UploadCode><Message>Success</Message></UploadApplyResponse>";

    xml_clear(buf);

    puts(buf);

    xmlDocPtr doc = NULL;
    xmlChar *szKey;          /*临时字符串变量 */
    xmlNodePtr root = NULL; /*根结点*/
    xmlNodePtr child_node = NULL; /*子结点*/

    /*将字符串格式的xml，转化成xml结构体*/
    doc = xmlParseMemory( buf, strlen(buf) );
    /*获取根结点，并且对根结点进行确认*/
    root = xmlDocGetRootElement( doc );
    if( xmlStrcmp( root->name, BAD_CAST "UploadApplyResponse" ) ) 
    {
        fprintf(stderr,"document of the wrong type, root node != root\n"); 
        xmlFreeDoc(doc); 
        return ; 
    } 

    /*判断根结点是否有Status属性*/
    if( xmlHasProp(root,BAD_CAST "Status") )
    {
        /*获取根结点的属性列表*/
        xmlAttrPtr attrPtr = root->properties;
        while( attrPtr != NULL )
        {
            if( !xmlStrcmp(attrPtr->name, BAD_CAST "Status") )
            {
                /*获取属性的值*/
                xmlChar* szAttr = xmlGetProp(root, BAD_CAST "Status");
                printf("[%d] %s : %s\n", __LINE__, attrPtr->name, szAttr);
                xmlFree(szAttr);
            }
            /*下一个属性*/
            attrPtr = attrPtr->next;
        }
    }

    /*获取根结点的子结点列表*/
    child_node = root->xmlChildrenNode;
    while( NULL != child_node ) 
    {
        /*获取结点的值*/
        szKey = xmlNodeGetContent(child_node);
        printf("[%d] %s : %s\n", __LINE__, child_node->name, szKey);
        xmlFree(szKey); 

        /*下一个子结点*/
        child_node = child_node->next; 
    } 
    xmlFreeDoc(doc);
}

void test_write_xml_file()
{
    puts("test_write_xml_file .............................");
    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
    /*创建根结点*/
    xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"root");

    /*将根结点添加到xml对象指针中*/
    xmlDocSetRootElement(doc,root_node);

    /**/
    xmlNewDocProp( doc, BAD_CAST "charset", BAD_CAST "utf-8");

    /*第一种方式添加子结点*/
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode1", BAD_CAST "newNode1 content");
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode2", BAD_CAST "newNode2 content");
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode3", BAD_CAST "newNode3 content");

    /*第二种方式添加子结点：先分别创建结点和值，然后将结点添加到根结点，将内容添加到结点中*/
    xmlNodePtr node01 = xmlNewNode( NULL, BAD_CAST"node01" );
    xmlNodePtr node01_content = xmlNewText( BAD_CAST "this is node01" );
    xmlAddChild( root_node, node01);
    xmlAddChild( node01, node01_content);
    /*为结点添加属性*/
    xmlNewProp( node01, BAD_CAST "attribute", BAD_CAST "yes");

    /*还是第二种方式，只不过在将创建值和将值添加到节点中，合并成了一步，在上面是分开做的*/
    xmlNodePtr node02 = xmlNewNode(NULL, BAD_CAST "node02");
    xmlAddChild( root_node, node02 );
    xmlNodePtr node02_child01 = xmlNewNode(NULL, BAD_CAST "node02_child01");
    xmlAddChild(node02, node02_child01);
    xmlAddChild(node02_child01, xmlNewText(BAD_CAST "This is node02_child01"));

    xmlNewTextChild(node02, NULL, BAD_CAST "newNode1", BAD_CAST "newNode1 content");

    /*第三种 在前两种方式中，都是使用xmlAddChild添加成目标结点的子结点，而这种，是添加成目标结点的兄弟结点*/
    /*将node02_sibling_prev添加为node02的前一个结点*/
    xmlNodePtr node02_sibling_prev = xmlNewNode( NULL, BAD_CAST "node02_sibling_prev" );
    xmlAddChild( node02_sibling_prev, xmlNewText( BAD_CAST "this is node02_sibling_prev" ) );
    xmlAddPrevSibling( node02, node02_sibling_prev );
    /*将node02_sibling_next添加为node02的下一个结点*/
    xmlNodePtr node02_sibling_next = xmlNewNode( NULL, BAD_CAST "node02_sibling_next" );
    xmlAddChild( node02_sibling_next, xmlNewText( BAD_CAST "this is node02_sibling_next" ) );
    xmlAddNextSibling( node02, node02_sibling_next );
    /*将node02_sibling_next添加为node02的一个兄弟结点，该结点会添加到node02所在结点列表的最后面*/
    xmlNodePtr node02_sibling = xmlNewNode( NULL, BAD_CAST "node02_sibling" );
    xmlAddChild( node02_sibling, xmlNewText( BAD_CAST "this is node02_sibling" ) );
    xmlAddSibling( node02, node02_sibling );

    /*修改某一节点的值*/
    xmlNodeSetContent( node02_child01, BAD_CAST "modified node02_child01");
    //xmlNodeSetName( node02_child01, BAD_CAST "modifyname_node02_child01");
    /*修改属性值*/
    xmlSetProp( node01, BAD_CAST "attribute", BAD_CAST "no");
    /*获取父结点下有多少个子结点*/
    long child_node_count = xmlChildElementCount( root_node );
    printf("child_node_count = %ld\n", child_node_count);
    /*获取父结点下的第一个子结点和最后一个子结点*/
    xmlNodePtr first_node = xmlFirstElementChild( root_node );
    printf("root first node name = %s\n", first_node->name);
    xmlNodePtr last_node = xmlLastElementChild( root_node );
    printf("root last node name = %s\n", last_node->name);
    /*如果某一个结点存在父结点，且该父结点有多个子结点，则获取该子结点的下一个子结点，或者上一个子结点*/
    xmlNodePtr next_node = xmlNextElementSibling( first_node );
    printf("root first node, next node name = %s\n", next_node->name);
    xmlNodePtr pre_node = xmlPreviousElementSibling( last_node );
    printf("root last node, previous node name = %s\n", pre_node->name);
    /*删除结点，先将结点node从xmlDocPtr中移除，再释放其内存*/
    xmlUnlinkNode( node01 );
    xmlFreeNode( node01 );


    /*保存成文件*/
    //xmlSaveFile("CreatedXml.xml",doc);
    xmlSaveFormatFile("CreatedXml.xml", doc, 1);

    xmlFreeDoc(doc);
}

void test_read_xml_file()
{
    puts("test_read_xml_file .............................");
    xmlDocPtr doc;           /*定义解析文件指针*/
    xmlNodePtr root;       /*指向根结点指针*/
    xmlNodePtr child_node;       /*指向子结点指针*/
    xmlChar *szKey;          /*临时字符串变量 */
    char *szDocName;
    szDocName = "CreatedXml.xml";

    /*读取xml文件，制定文件编码格式为utf-8，第三个参数表名移除空结点*/
    doc = xmlReadFile(szDocName,"utf-8",XML_PARSE_NOBLANKS);
    if (NULL == doc) 
    {  
        fprintf(stderr,"Document not parsed successfully\n");     
        return ; 
    } 

    root = xmlDocGetRootElement(doc); //确定文件根元素
    /*检查确认当前文件中包含内容*/ 
    if (NULL == root)
    { 
        fprintf(stderr,"empty document\n"); 
        xmlFreeDoc(doc); 
        return ; 
    } 
    /*在这个例子中，我们需要确认文件是正确的类型。“root”是在这个示例中使用文件的根类型。*/
    if (xmlStrcmp(root->name, BAD_CAST "root")) 
    {
        fprintf(stderr,"document of the wrong type, root node != root"); 
        xmlFreeDoc(doc); 
        return ; 
    } 

    child_node = root->xmlChildrenNode;
    int i = 0;
    while( child_node != NULL) 
    {
        szKey = xmlNodeGetContent(child_node);
        printf("[%d:%d:node] name = %s, value = %s\n", __LINE__, i++, child_node->name, szKey);
        xmlFree(szKey); 

        /*如果有属性的话，把属性都打印出来*/
        xmlAttrPtr attrPtr = child_node->properties;
        while (attrPtr != NULL)
        {
            /*这里是判断属性名是否为attribute,并根据属性名获取属性值*/
            if (!xmlStrcmp(attrPtr->name, BAD_CAST "attribute"))
            {
                xmlChar* szAttr = xmlGetProp(child_node, BAD_CAST "attribute");
                printf("[%d] %s : %s\n", __LINE__, attrPtr->name, szAttr);
                xmlFree(szAttr);
            }
            attrPtr = attrPtr->next;
        }

        child_node = child_node->next; 
    } 

    xmlFreeDoc(doc);
}

void test_get_specify_element_by_xpath()
{
    printf("test_get_specify_element_by_xpath...\n");
    char *xmlFile = "CreatedXml.xml";
    /*
     * 使用规则目前发现有两种：
     * 1、node02是root的子结点，而node02_child01又是node02的子结点，依次排开，用‘/'隔开，例如："/root/node02/node02_child01"
     * 2、"//newNode1"，搜索所有结点名为newNode1的结点，结点名如果不是在同一个父结点下，是可以重复的
     * */
    //char *pattern = "/root/node02/node02_child01";
    char *pattern = "//newNode1";

    xmlDocPtr doc = NULL;
    xmlXPathContextPtr context = NULL; /*获取到的xml的上下文*/
    xmlXPathObjectPtr result = NULL; /*存放查询后的结果集*/
    xmlNodeSetPtr      node = NULL; /*用于获取结果集中的单个结点*/

    do{
        doc = xmlReadFile( xmlFile, "utf-8", XML_PARSE_NOBLANKS );
        if( NULL == doc )
        {
            fprintf(stderr,"Document not parsed successfully\n");     
            break;
        }

        /*获取xml的上下文*/
        context = xmlXPathNewContext( doc );
        if( NULL == context ) 
        {
            fprintf(stderr,"Xml Context get failed\n");     
            break;
        }

        /*通过过滤表达式获取结果集*/
        result = xmlXPathEvalExpression( BAD_CAST pattern, context );

        /*判断是否失败了*/
        if( NULL == result ) 
        {
            fprintf(stderr, "get result failed by pattern\n");
            break;
        }

        /*判断结果集释放为空*/
        if( xmlXPathNodeSetIsEmpty(result->nodesetval) ) 
        {
            printf("no result\n");
            break;
        }

        node = result->nodesetval;
        int i = 0;
        for( i = 0; i < node->nodeNr; i++ ) 
        {
            xmlChar *value = xmlNodeListGetString( doc, node->nodeTab[i]->xmlChildrenNode, 1);
            printf("value = %s\n", value);
            xmlFree(value);
        }

        /*清楚库内存*/
        xmlCleanupParser();

    }while(0);

    if( NULL == result )
    {
        xmlXPathFreeObject(result);
        result = NULL;
    }

    if( NULL == context ) 
    {
        xmlXPathFreeContext(context);
        context = NULL;
    }

    return;
}

int main(int argc, const char *argv[])
{
    test_read_xml_string();

    test_write_xml_file();

    test_read_xml_file();

    test_get_specify_element_by_xpath();

    return 0;
}
