#include <stdio.h>

#include <avro.h>

int main(int argc, const char *argv[])
{
    avro_schema_t wqs_schema_array;
    avro_schema_t wqs_schema_long;
    avro_value_iface_t *wqs_array_class;
    avro_schema_error_t wqs_error;
    avro_value_t wqs_array;
    avro_value_t wqs_value;
    int ret = -1;

    /*创建一个long类型模式*/
    fprintf(stderr, "111\n");
    wqs_schema_long = avro_schema_long();

    /*创建一个数组类型模式，并且把long类型模式添加到数组里*/
    fprintf(stderr, "222\n");
    wqs_schema_array = avro_schema_array(wqs_schema_long);

    /*模式对象化*/
    fprintf(stderr, "333\n");
    wqs_array_class = avro_generic_class_from_schema(wqs_schema_array);

    /*获取对象的值*/
    ret = avro_generic_value_new(wqs_array_class, &wqs_array);
    if( 0 != ret )
    {
        fprintf(stderr, "avro_generic_value_new error, ret = [%d]\n", ret);
        exit(0);
    }

    /*判断值的类型*/
    avro_type_t wqs_type;
    wqs_type = avro_value_get_type(&wqs_array);
    fprintf(stderr, "wqs_type of wqs_array=[%d]\n", wqs_type);

    /*向数组中添加4个long值*/
    int index = 0;
    for( index = 0; index < 4; ++index)
    {
        avro_value_t element;
        size_t idx;
        ret = avro_value_append(&wqs_array, &element, &idx);
        if( 0 != ret )
        {
            fprintf(stderr, "avro_value_append() error, ret=[%d], error msg=[%s]\n", ret, avro_strerror());            
            exit(0);
        }
        fprintf(stderr, "index=[%d], idx=[%d]\n", index, idx);

        ret = avro_value_set_long(&element, index*100+index);
        if( 0 != ret )
        {
            fprintf(stderr, "avro_value_append() error, ret=[%d], error msg=[%s]\n", ret, avro_strerror());            
            exit(0);
        }
        fprintf(stderr, "add long value[%ld] succ\n", index*100+index);
    }
 
    fprintf(stderr, "##########################################\n");
    /*从数组中取出之前加入的4个long值*/
    for( index = 0; index < 4; ++index)
    {
        avro_value_t element;
        size_t value;
        const char *unused;

        ret = avro_value_get_by_index(&wqs_array, index, &element, &unused);
        if( 0 != ret )
        {
            fprintf(stderr, "avro_value_append() error, ret=[%d], error msg=[%s]\n", ret, avro_strerror());            
            exit(0);
        }
        fprintf(stderr, "index=[%d], unused=[%s]\n", index, unused);

        wqs_type = avro_value_get_type(&element);
        fprintf(stderr, "wqs_type of wqs_array=[%d]\n", wqs_type);

        ret = avro_value_get_long(&element, &value);
        if( 0 != ret )
        {
            fprintf(stderr, "avro_value_get_long() error, ret=[%d], error msg=[%s]\n", ret, avro_strerror());            
            exit(0);
        }
        fprintf(stderr, "index=[%d], value=[%d]\n", index, value);
    }

    return 0;
}
