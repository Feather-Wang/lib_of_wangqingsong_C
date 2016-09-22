/*
 * 对字符串的处理:
 * 从json中获取字符串的函数有两个json_object_get_string()和json_object_to_json_string()
 * 1、如果字符串的值为NULL（例如在字符串数组中，如果设置完第三个值，跳过第四个值，直接设置第五个值，第四个值就会
 * 默认为null），json_object_get_string()就会段错误，但是json_object_to_json_string()可以读null，显示也是null
 * 2、json_object_to_json_string()打印的字符串带双引号，而json_object_get_string()没有，而json_object_to_json_string()则去不掉
 *
 * */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json/json.h>

//json_tokener.h，该头文件中的函数主要是用于将json形式的字符串，转换成json，下面是主要函数的使用
int test_json_tokener_h()
{
    puts("test_json_tokener_h ...............................................................................");

    char buf[] = "{\"retCode\": 501001, \"retMsg\":\"111\"}";

    //使用json_tokener_parse将字符串格式的json数据转换成json
    struct json_object* root_obj = json_tokener_parse(buf);
    
    //is_error是一个宏，用来判断json结构是否正确，正确返回0，错误返回1
    if( is_error(root_obj) )
    {
        /*如果通过root_obj==NULL去判断释放json资源，这里需要设置root_obj为NULL，root_obj实际是转换失败的，并没有分配内存，如果这时候用json_object_put去释放，会产生signal 11的错误*/
        root_obj = NULL;
        puts("aaaa");
        return -1;
    }
    /* 判断json是否为object结构，除了json_type_object外，还有:
     * typedef enum json_type {
     *   json_type_null,
     *   json_type_boolean,
     *   json_type_double,
     *   json_type_int,
     *   json_type_object,
     *   json_type_array,
     *   json_type_string
     *   } json_type;
     * 除了json_object_is_type()外，还可以使用json_object_get_type(root_obj)获取类型，返回值为json_type枚举
     *
     * */
    if( !json_object_is_type(root_obj, json_type_object) )
    {
        printf("json is not object\n");
        return -1;
    }
    struct json_object* retCode = json_object_object_get(root_obj, "retCode");
    if( NULL == retCode )
    {
        puts("can't find retCode");
        json_object_put(root_obj);
        return -1;
    }
    struct json_object* retMsg = json_object_object_get(root_obj, "retMsg");
    if( NULL == retMsg )
    {
        puts("can't find retMsg");
        json_object_put(root_obj);
        return -1;
    }
    printf("retMsg --> %s\n", json_object_get_string(retMsg));
    printf("retMsg len --> %d\n", strlen(json_object_get_string(retMsg)));

    // 使用json_object_get()函数，可以使该节点不随根节点的释放而释放，而是单独释放
    json_object_get(retCode);

    json_object_put(root_obj);

    printf("retCode 1 --> [%d]\n", json_object_get_int(retCode));
    json_object_put(retCode);
    printf("retCode 2 --> [%d]\n", json_object_get_int(retCode));
    return 0;
}

//json_object.h该头文件中的函数，主要是用于创建json，下面是主要函数的使用
int test_json_object_h()
{
    puts("test_json_object_h ...............................................................................");
    //创建第一个节点
    struct json_object *user_obj = NULL;
    user_obj = json_object_new_object();

    //向节点添加数据,第三个参数需要使用json_object_new_string()将字符串转换成struct
    //json_object*类型
    json_object_object_add(user_obj, "username", json_object_new_string("wangqingsong"));
    json_object_object_add(user_obj, "password", json_object_new_string("123456789"));

    //将json转换成字符串并打印
    printf("user_obj --> %s\n", json_object_to_json_string(user_obj));

    //获取json中的数据,获取节点中的节点中的数据，方法类似，只不过要先取得相应的节点指针
    char userName[16] = {0};
    struct json_object* user_name = json_object_object_get(user_obj, "username");
    sprintf(userName, "%s", json_object_to_json_string(user_name));
    printf("get from json --> %s\n", userName);

    //创建第二个节点
    struct json_object *info_obj = NULL;
    info_obj = json_object_new_object();

    //向节点添加数据,第三个参数需要使用json_object_new_string()将字符串转换成struct
    //json_object*类型
    json_object_object_add(info_obj, "age", json_object_new_string("20"));
    json_object_object_add(info_obj, "phone", json_object_new_string("18811228623"));

    //将json转换成字符串并打印
    printf("info_obj --> %s\n", json_object_to_json_string(info_obj));

    //将user_obj和info_obj添加到一个节点
    struct json_object *second_obj = NULL;
    second_obj = json_object_new_object();

    json_object_object_add(second_obj, "user", user_obj);
    json_object_object_add(second_obj, "info", info_obj);
    
    printf("second_obj --> %s\n", json_object_to_json_string(second_obj));

    //再添加一个first节点
    struct json_object *first_obj = NULL;
    first_obj = json_object_new_object();

    json_object_object_add(first_obj, "first_num", json_object_new_string("12345"));
    json_object_object_add(first_obj, "first_char", json_object_new_string("aaaaa"));
    
    printf("first_obj --> %s\n", json_object_to_json_string(first_obj));
  
    //创建根节点
    struct json_object *root_obj = NULL;
    root_obj = json_object_new_object();

    json_object_object_add(root_obj, "first", first_obj);
    json_object_object_add(root_obj, "second", second_obj);

    // 在根节点添加布尔值
    json_object_object_add(root_obj, "isMan", json_object_new_boolean(1));
    
    printf("root_obj --> %s\n", json_object_to_json_string(root_obj));
    
    //删除第一个节点
    json_object_object_del(root_obj, "first");
    printf("root_obj after del first --> %s\n", json_object_to_json_string(root_obj));

    // 遍历根节点中的参数，只能打印第一层的，而且json_object_object_foreach()需要c99支持，编译时需要添加-std=c99 or -std=gnu99选项
    json_object_object_foreach(root_obj, key, value)
    {
        printf("key:%s, value:%s\n", key, json_object_to_json_string(value));
    }


    //释放json，配合json_object_new_object()使用,参数必须是最外层的节点
    json_object_put(user_obj);

    return 0;
}

static int sort_fn(const void *j1, const void *j2)
{
    json_object * const *jso1, * const *jso2;
    int i1, i2;

    jso1 = (json_object* const*)j1;
    jso2 = (json_object* const*)j2;
    if (!*jso1 && !*jso2)
        return 0;
    if (!*jso1)
        return -1;
    if (!*jso2)
        return 1;

    i1 = json_object_get_int(*jso1);
    i2 = json_object_get_int(*jso2);

    return i1 - i2;
}
int test_array_of_json()
{
    puts("test_array_of_json ...............................................................................");
    struct json_object *obj = NULL;
    
    obj = json_object_new_object();

    json_object_object_add(obj, "first_obj", json_object_new_string("wangqingsong"));


    struct json_object *obj_array = NULL;
    obj_array = json_object_new_array();
    json_object_array_add(obj_array, json_object_new_string("first array"));
    json_object_array_add(obj_array, json_object_new_string("second array"));
    json_object_array_add(obj_array, json_object_new_string("third array"));
    json_object_array_put_idx(obj_array, 4, json_object_new_string("third array"));

    json_object_object_add(obj, "obj_array", obj_array);

    int i = 0;
    // first show array
    for(i = 0; i < json_object_array_length(obj_array); ++i)
    {
        json_object *tmp = json_object_array_get_idx(obj_array, i);
        printf(" index [%d] --> [%s]\n", i, json_object_to_json_string(tmp));
    }
    printf("obj_array --> [%s]\n", json_object_to_json_string(obj_array));
    
    //排序，不一定所有的libjson版本都支持
    //json_object_array_sort(obj_array, sort_fn);

    // second show array
    for(i = 0; i < json_object_array_length(obj_array); ++i)
    {
        json_object *tmp = json_object_array_get_idx(obj_array, i);
        printf(" index [%d] --> [%s]\n", i, json_object_to_json_string(tmp));
    }
    printf("obj_array --> [%s]\n", json_object_to_json_string(obj_array));
 
    printf("obj --> [%s]\n", json_object_to_json_string(obj));

    json_object_put(obj);

    return 0;
}

int main(int argc, const char *argv[])
{
    //test_json_object_h();

    printf("test_json_tokener_h -------------------> %d\n", test_json_tokener_h());

    //test_array_of_json();
    
    return 0;
}
