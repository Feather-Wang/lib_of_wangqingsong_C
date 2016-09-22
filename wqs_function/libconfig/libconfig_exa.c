#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include <string.h>

/*获取bool类型*/
#include <stdbool.h>

//读取配置文件
int read_config(config_t* conf)
{
    int ret = -1;
    const char *pstr = NULL;
    int value_int = 0;
    double value_double = 0;
    bool value_bool = -1;
    do{
        //从test.conf文件中读取数据
        config_read_file(conf,"test.conf");

        printf("error_file --> %s\n", config_error_file(conf));
        printf("error_line --> %d\n", config_error_line(conf));
        printf("error_text --> %s\n", config_error_text(conf));

        /*读取根节点下的user属性，user属性为字符串*/
        config_lookup_string(conf,"user",&pstr);
        printf("user = %s\n", pstr);

        /************************** 读取群组的属性 ****************************/
        int count_group = config_setting_length( config_lookup(conf, "group") );
        printf("count_group = %d\n", count_group);
        int index_group = config_setting_index( config_lookup(conf, "group") );
        printf("index_group = %d\n", count_group);
        int type_group = config_setting_get_format( config_lookup(conf, "group") );
        printf("type_group = %d\n", type_group);

#if 0
        /* 读取群组的第一种方式，这三种方式适用于群组、列表、数组 */

        /*读取group1组中，group10组中的score属性，score属性为整型*/
        config_lookup_int(conf, "group.group_1.score", &value_int);
        printf("group.group_1.score = %d\n", value_int);
        /*读取group2组中的avg属性，avg属性为浮点型*/
        config_lookup_float(conf, "group.group_2.avg", &value_double);
        printf("group.group_2.avg = %lf\n", value_double);
#elif 0
        /* 读取群组的第二种方式 */
        config_setting_t *group = config_lookup(conf, "group");

        config_setting_t *group_1 = config_setting_get_elem(group, 0);
        config_setting_lookup_int(group_1, "score", &value_int);
        printf("group.group_1.score = %d\n", value_int);

        config_setting_t *group_2 = config_setting_get_elem(group, 1);
        config_setting_lookup_float(group_2, "avg", &value_double);
        printf("group.group_2.avg = %lf\n", value_double);
       
#else 
        /* 读取群组的第三种方式 */
        config_setting_t *group = config_lookup(conf, "group");

        config_setting_t *group_1 = config_setting_get_elem(group, 0);
        value_int = config_setting_get_int_elem(group_1, 0);
        printf("group.group_1.score = %d\n", value_int);
#endif
        printf("\n\n\n");

#if 0
        /************************** 读取列表的属性 ****************************/

        config_setting_t* info1 = config_lookup(conf,"info.[1]");
        /*读取info列表中的is_man*/
        //config_lookup_bool(info1, "is_man", &value_bool);
        config_setting_lookup_bool(info1, "is_man", &value_bool);
        printf("info[0].is_man = %d\n", value_bool);

        /*读取info列表中的is_girl*/
        config_lookup_bool(conf, "info.[1].is_girl", &value_bool);
        printf("info[1].is_girl = %d\n", value_bool);
#endif
        /************************** 读取数组的属性 ****************************/

        int count_array = config_setting_length( config_lookup(conf, "array") );
        printf("count_group = %d\n", count_group);
        config_lookup_string(conf, "array.[0]", &pstr);
        printf("array --> array.[0] = %s\n", pstr);
        config_lookup_string(conf, "array.[1]", &pstr);
        printf("array --> array.[1] = %s\n", pstr);
        config_lookup_string(conf, "array.[2]", &pstr);
        printf("array --> array.[2] = %s\n", pstr);

        
        ret = 0;

    }while(0);

    return ret;
}


//生成配置文件
int write_config(config_t* conf)
{
    int ret = -1;
    do{
        /*创建根节点*/
        config_setting_t* root = config_root_setting(conf);
        if( NULL == root )
            break;

        /*添加user属性，user为字符串，返回节点指针*/
        config_setting_t* user = config_setting_add(root,"User",CONFIG_TYPE_STRING);
        /*设置属性的值*/
        config_setting_set_string(user,"zhangwc");

        /***********************************   群组操作  *******************************/

        /*添加group1组，返回节点指针*/
        config_setting_t* group = config_setting_add(root,"group",CONFIG_TYPE_GROUP);
        /*在group1组中，嵌入group10组*/
        config_setting_t* group_1 = config_setting_add(group,"group_1",CONFIG_TYPE_GROUP);
        /*在group10组中添加score属性，score属性为int型，返回节点指针*/
        config_setting_t* score = config_setting_add(group_1,"score",CONFIG_TYPE_INT);
        /*设置score的值*/
        config_setting_set_int(score, 60);

        /*在group1组中，添加group2组，与group1组并列关系*/
        config_setting_t* group_2 = config_setting_add(group,"group_2",CONFIG_TYPE_GROUP);
        /*在group2组中添加avg属性，avg属性为double型，返回节点指针*/
        config_setting_t *avg = config_setting_add(group_2, "avg", CONFIG_TYPE_FLOAT);
        /*设置avg的值*/
        config_setting_set_float(avg, 3.123456788789111);

        /*******************************  列表操作   *************************************/

        /*在跟节点上，添加info列表，在列表中属性名将不会显示，只会显示属性值*/
        config_setting_t* list = config_setting_add(root,"list", CONFIG_TYPE_LIST);
        /*在info列表中嵌入info1组*/
        config_setting_t* list_1 = config_setting_add(list,"1",CONFIG_TYPE_GROUP);
        /*在info1组中添加is_man属性，is_man属性为bool类型*/
        config_setting_t* is_man = config_setting_add(list_1,"is_man",CONFIG_TYPE_BOOL);
        /*设置is_man为true*/
        config_setting_set_bool(is_man, 1);

        /*在info列表中添加is_girl属性，is_girl属性为bool类型*/
        config_setting_t* is_girl = config_setting_add(list,"is_girl",CONFIG_TYPE_BOOL);
        /*设置is_girl为false*/
        config_setting_set_bool(is_girl, 0);

        /**********************************  数组操作   *********************************/

        /* 在根节点，添加array数组 */
        config_setting_t *array = config_setting_add(root, "array", CONFIG_TYPE_ARRAY);
        /* 在array数组中加入三个数据 */
        config_setting_t *array_1 = config_setting_add(array, "var1", CONFIG_TYPE_STRING);
        config_setting_t *array_2 = config_setting_add(array, "var2", CONFIG_TYPE_STRING);
        config_setting_t *array_3 = config_setting_add(array, "var3", CONFIG_TYPE_STRING);
        /* 设置数据的属性值 */
        config_setting_set_string(array_1, "wang");
        config_setting_set_string(array_2, "qing");
        config_setting_set_string(array_3, "song");

        /****************************** 群组属性移除操作 *************************************/
        config_setting_remove(group, "group_2");
        /****************************** 群组、列表、数组属性移除操作 *************************************/
        //config_setting_remove_elem(group, 1);

        /*将设置的内容写入到test.conf文件中，如果文件中有内容，将被覆盖*/
        config_write_file(conf,"test.conf");

        ret = 0;

    }while(0);

    return ret;
}

int main(int argc , char* argv[])
{
    int ret = -1;
    config_t conf;

    do{
        /*初始化句柄*/
        config_init(&conf);

#if 1
        if( -1 == write_config(&conf) )
            break;
#else
        if( -1 == read_config(&conf) )
            break;
#endif
        ret = 0;

    }while(0);

    config_destroy(&conf);

    return 0;
}
