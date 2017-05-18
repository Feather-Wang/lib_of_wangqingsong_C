#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "oci.h"

#define SELECT_SQL "select name,age from students"
#define SELECT_SQL_COUNT "select COUNT(*) from students"

static OCIEnv *ocienv;
static OCIError *errhp;
static OCISession *authp = (OCISession *) 0;
static OCIServer *srvhp;
static OCISvcCtx *svchp;
static OCIStmt   *selecthp;
static OCIDefine *defnp[100] = {NULL};
static sword oracle_ret = 0;


typedef struct student_s
{
    char name[128];
    int age;
}student_t;

void OCI_error(OCIError *errhp, sword status)
{
    text errbuf[512];
    sb4 errcode = 0;

    switch (status)
    {
        case OCI_SUCCESS:
            break;
        case OCI_SUCCESS_WITH_INFO:
            (void) printf("Error - OCI_SUCCESS_WITH_INFO\n");
            break;
        case OCI_NEED_DATA:
            (void) printf("Error - OCI_NEED_DATA\n");
            break;
        case OCI_NO_DATA:
            (void) printf("Error - OCI_NODATA\n");
            break;
        case OCI_ERROR:
            (void) OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
                    errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
            (void) printf("Error - %.*s\n", 512, errbuf);
            break;
        case OCI_INVALID_HANDLE:
            (void) printf("Error - OCI_INVALID_HANDLE\n");
            break;
        case OCI_STILL_EXECUTING:
            (void) printf("Error - OCI_STILL_EXECUTE\n");
            break;
        case OCI_CONTINUE:
            (void) printf("Error - OCI_CONTINUE\n");
            break;
        default:
            break;
    }
}

static int oracle_select_get_count(char *sqlcommond)
{
    sword ret = 0;
    int return_num = 0;

    do{
        /*get count of select*/
        ret = OCIStmtPrepare(selecthp, errhp, sqlcommond, (ub4) strlen((char *) sqlcommond), (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
        if( 0 != ret )
        {
            printf("[sql:oracle_select_get_count] OCIStmtPrepare error, sql = [%s]\n", sqlcommond);
            OCI_error(errhp, ret);
            break;
        }
        ret = OCIDefineByPos(selecthp, &defnp[0], errhp, 1, (dvoid *) &return_num, sizeof(return_num), SQLT_INT, (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT);
        if( OCI_SUCCESS != ret )
        {
            printf("[sql:oracle_select_get_count] OCIDefineByPos(return_num) error for get count!\n");
            OCI_error(errhp, ret);
            break;
        }
        ret = OCIStmtExecute(svchp, selecthp, errhp, (ub4) 1, (ub4) 0, (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT);
        if( OCI_SUCCESS != ret )
        {
            printf("[sql:oracle_select_get_count] OCIStmtExecute error for get count! [%s]\n", sqlcommond);
            OCI_error(errhp, ret);
            break;
        }

        return return_num;

    }while(0);
   
    return -1;
}

static int oracle_select_students(void)
{
    char sqlcommond[512]={0};
    sword ret = 0;
    int i = 0;
    int len = 0;
    int return_value = -1;
    int num_of_stuents = 0;
    student_t *p_students;

    memset(sqlcommond, 0, sizeof(sqlcommond));
    sprintf(sqlcommond, SELECT_SQL_COUNT);
    printf("[sql:students] sql run=[%s]\n", sqlcommond);

    do{
        num_of_stuents = oracle_select_get_count(sqlcommond);
        if( -1 == num_of_stuents )
        {
            printf("[sql:students] oracle_select_get_count error, sql = [%s]\n", sqlcommond);
            break;
        }
        printf("[sql:students] the count of student is %d\n", num_of_stuents);

        if( 0 == num_of_stuents )
        {
            printf("[sql:students] the count of student is [%d], it's empty\n", num_of_stuents);
            return_value = 0;
            break;
        }

        /*malloc for accept select result*/
        p_students = (student_t *)malloc(sizeof(student_t) * num_of_stuents);
        if( NULL == p_students )
        {
            printf("[sql:students] malloc(one size = [%ld], count = [%d]) fail!\n", sizeof(student_t), num_of_stuents);
            break;
        }
        memset(p_students,0,sizeof(student_t)*num_of_stuents);

        /*get select result*/
        memset(sqlcommond, 0, sizeof(sqlcommond));
        sprintf(sqlcommond, SELECT_SQL);
        printf("[sql:students] sql run=[%s]\n", sqlcommond);
        ret = OCIStmtPrepare(selecthp, errhp, sqlcommond, (ub4) strlen((char *) sqlcommond), (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
        if( 0 != ret )
        {
            printf("[sql:students] OCIStmtPrepare error, sql = [%s]\n", sqlcommond);
            OCI_error(errhp, ret);
            break;
        }

        ret = OCIDefineByPos(selecthp, &defnp[0], errhp, 1, (dvoid *) &p_students[0].name, sizeof(p_students[0].name), SQLT_STR, (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT);
        if( OCI_SUCCESS != ret )
        {
            printf("[sql:students] OCIDefineByPos(name) error for get select result!\n");
            OCI_error(errhp, ret);
            break;
        }
        /*因为这里的学生信息不只一个，所以需要在每一个OCIDefineByPos()之后，都要执行一下下面的这个函数*/
        ret = OCIDefineArrayOfStruct(defnp[0], errhp, sizeof(student_t), 0, 0, 0);
        if( OCI_SUCCESS != ret )
        {
            printf("[sql:students] OCIDefineArrayOfStruct(name) error for get select result!\n");
            OCI_error(errhp, ret);
            break;
        }
        ret = OCIDefineByPos(selecthp, &defnp[1], errhp, 2, (dvoid *) &p_students[0].age, sizeof(p_students[0].age), SQLT_INT, (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT);
        if( OCI_SUCCESS != ret )
        {
            printf("[sql:students] OCIDefineByPos(age) error for get select result!\n");
            OCI_error(errhp, ret);
            break;
        }
        ret = OCIDefineArrayOfStruct(defnp[1], errhp, sizeof(student_t), 0, 0, 0);
        if( OCI_SUCCESS != ret )
        {
            printf("[sql:students] OCIDefineArrayOfStruct(student_t) error for get select result!\n");
            OCI_error(errhp, ret);
            break;
        }

        ret = OCIStmtExecute(svchp, selecthp, errhp, (ub4) num_of_stuents, (ub4) 0, (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT);
        if( OCI_SUCCESS != ret )
        {
            printf("[sql:students] OCIStmtExecute error [%s]\n", sqlcommond);
            OCI_error(errhp, ret);
            break;
        }

        /*show select result*/
        for( i = 0; i < num_of_stuents; ++i )
        {
            printf("[sql:students][index=%d] name = [%s]. age = [%d].\n", i, p_students[i].name, p_students[i].age);
        }

        return_value = 0;
    }while(0);

    return return_value;
}

int main(int argc, char *argv[])
{
    text *username = (text *) "system";
    text *password = (text *) "system";
    text *server_address = (text *) "168.1.50.10";

    do{
        /*创建OCI对象oci， 使用缺省模式*/
        oracle_ret = OCIEnvCreate((OCIEnv **) &ocienv, 
                (ub4) OCI_DEFAULT,
                (dvoid *) 0,
                (dvoid * (*)(dvoid *,size_t)) 0,
                (dvoid * (*)(dvoid *, dvoid *, size_t)) 0,
                (void (*)(dvoid *, dvoid *)) 0, 
                (size_t) 0, 
                (dvoid **) 0);

        if( 0 != oracle_ret ) 
        {
            (void) printf("OCIEnvCreate failed with errcode = %d.\n", oracle_ret);
            break;
        }

        /*为oci错误对象申请内存*/
        (void) OCIHandleAlloc( (dvoid *) ocienv, (dvoid **) &errhp, OCI_HTYPE_ERROR,
                (size_t) 0, (dvoid **) 0);

        /*OCI server handle*/
        (void) OCIHandleAlloc( (dvoid *) ocienv, (dvoid **) &srvhp, OCI_HTYPE_SERVER,
                (size_t) 0, (dvoid **) 0);

        /*OCI service context handle*/
        (void) OCIHandleAlloc( (dvoid *) ocienv, (dvoid **) &svchp, OCI_HTYPE_SVCCTX,
                (size_t) 0, (dvoid **) 0);

        (void) OCIServerAttach( srvhp, errhp, (text *)server_address, strlen(server_address), 0);

        /* set attribute server context in the service context */
        (void) OCIAttrSet( (dvoid *) svchp, OCI_HTYPE_SVCCTX, (dvoid *)srvhp,
                (ub4) 0, OCI_ATTR_SERVER, (OCIError *) errhp);

        /*OCI user session handle*/
        (void) OCIHandleAlloc((dvoid *) ocienv, (dvoid **)&authp,
                (ub4) OCI_HTYPE_SESSION, (size_t) 0, (dvoid **) 0);

        /*add username to session handle*/
        (void) OCIAttrSet((dvoid *) authp, (ub4) OCI_HTYPE_SESSION,
                (dvoid *) username, (ub4) strlen((char *)username),
                (ub4) OCI_ATTR_USERNAME, errhp);

        /*add password to session handle*/
        (void) OCIAttrSet((dvoid *) authp, (ub4) OCI_HTYPE_SESSION,
                (dvoid *) password, (ub4) strlen((char *)password),
                (ub4) OCI_ATTR_PASSWORD, errhp);

        oracle_ret = OCISessionBegin ( svchp,  errhp, authp, OCI_CRED_RDBMS, (ub4) OCI_DEFAULT);
        if( OCI_SUCCESS != oracle_ret )
        {
            OCI_error(errhp, oracle_ret);
            break;
        }

        (void) OCIAttrSet((dvoid *) svchp, (ub4) OCI_HTYPE_SVCCTX,
                (dvoid *) authp, (ub4) 0,
                (ub4) OCI_ATTR_SESSION, errhp);

        /*为insert_str和selecthp申请内存*/
        OCIHandleAlloc( (dvoid *) ocienv, (dvoid **) &selecthp,
                OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0);

        if( -1 == oracle_select_students() )
        {
            printf("oracle_select_students error\n");
            break;
        }

    }while(0);

    oracle_ret = OCISessionEnd(svchp, errhp, authp, OCI_DEFAULT);
    if( OCI_SUCCESS != oracle_ret )
    {
        printf("OCISessionEnd error\n");
        OCI_error(errhp, oracle_ret);
    }
    oracle_ret = OCIServerDetach(srvhp, errhp, OCI_DEFAULT);
    if( OCI_SUCCESS != oracle_ret )
    {
        printf("OCIServerDetach error\n");
        OCI_error(errhp, oracle_ret);
    }
    if( selecthp )
        (void) OCIHandleFree((dvoid *) selecthp, OCI_HTYPE_STMT);
    if( authp )
        (void) OCIHandleFree((dvoid *) authp, OCI_HTYPE_SESSION);
    if( srvhp )
        (void) OCIHandleFree((dvoid *) srvhp, OCI_HTYPE_SERVER);
    if( svchp )
        (void) OCIHandleFree((dvoid *) svchp, OCI_HTYPE_SVCCTX);
    if( errhp )
        (void) OCIHandleFree((dvoid *) errhp, OCI_HTYPE_ERROR);
    if (ocienv)
        (void) OCIHandleFree((dvoid *) ocienv, OCI_HTYPE_ENV);

    return 0;
}
