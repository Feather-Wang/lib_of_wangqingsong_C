#include <stdio.h>

#include <avro.h>

/*指定模式*/
#define JSON_STRING \
   "{\"type\":\"array\", \"items\": {\"type\": \"array\", \"items\": \"long\"}}"

int main(int argc, const char *argv[])
{
    int value = 12;
    int get_value = 0;
    avro_schema_t wqs_schema;
    avro_value_iface_t *wqs_array_class;
    avro_value_t wqs_array;
    avro_value_t wqs_value;
    avro_schema_error_t wqs_error;
    size_t wqs_array_size;
    int ret = -1;

    /*加载模式*/
    fprintf(stderr, "111\n");
    ret = avro_schema_from_json(JSON_STRING, sizeof(JSON_STRING), &wqs_schema, &wqs_error);
    if( 0 != ret )
    {
        fprintf(stderr, "avro_schema_from_json error, ret = [%d]\n", ret);
        exit(0);
    }

    /*模式对象化*/
    fprintf(stderr, "222\n");
    wqs_array_class = avro_generic_class_from_schema(wqs_schema);

    /*获取对象的值*/
    fprintf(stderr, "333\n");
    ret = avro_generic_value_new(wqs_array_class, &wqs_array);
    if( 0 != ret )
    {
        fprintf(stderr, "avro_generic_value_new error, ret = [%d]\n", ret);
        exit(0);
    }

    fprintf(stderr, "444\n");
    /*
    ret = avro_value_set_int(&wqs_value, value);
    if( 0 != ret )
    {
        fprintf(stderr, "avro_value_set_int error, ret = [%d]\n", ret);
        exit(0);
    }
    */

    fprintf(stderr, "555\n");

    /*判断值的类型*/
    avro_type_t wqs_type;
    wqs_type = avro_value_get_type(&wqs_array);
    fprintf(stderr, "wqs_type of wqs_array=[%d]\n", wqs_type);

    /*如果是数组*/
    if( AVRO_ARRAY == wqs_type )
    {
        /*获取数组的大小*/
        ret = avro_value_get_size(&wqs_array, &wqs_array_size);
        if( 0 != ret )
        {
            fprintf(stderr, "avro_value_get_size() error, ret=[%d], error msg=[%s]\n", ret, avro_strerror());
            exit(0);
        }
        fprintf(stderr, "wqs_array size=[%ld]\n", wqs_array_size);

        /*获取第一个元素的值*/
        ret = avro_value_get_by_index(&wqs_array, 0, &wqs_value, NULL);
        if( 0 != ret )
        {
            fprintf(stderr, "avro_value_get_by_index() error, ret=[%d], error msg=[%s]\n", ret, avro_strerror());
            exit(0);
        }

        /*判断第一个元素的值的类型*/
        wqs_type = avro_value_get_type(&wqs_value);
        fprintf(stderr, "wqs_type of wqs_value=[%d]\n", wqs_type);

        /*如果是string类型，则用该函数取数据*/
        char buf[1024] = {0};
        ret = avro_value_get_string(&wqs_value, &buf, &get_value);
        if( 0 != ret )
        {
            fprintf(stderr, "avro_value_get_int error, ret = [%d]\n", ret);
            exit(0);
        }
        fprintf(stderr, "get integer from avro_value_t, value=[%d]\n", get_value);
    }

    return 0;
}
