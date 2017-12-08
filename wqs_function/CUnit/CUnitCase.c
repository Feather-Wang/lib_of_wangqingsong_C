#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <Basic.h>
#include <Console.h>
#include <CUnit.h>
#include <TestDB.h>

#include "test.h"

/************************************************************************/
void testSuccessSimpleAssert(void)
{
	CU_ASSERT(1) ;
	CU_ASSERT(!0) ;
}

void testSuccessAssertTrue(void)
{
	CU_ASSERT_TRUE(1) ;
	CU_ASSERT_TRUE(!0) ;
}

void testSuccessAssertFalse(void)
{
	CU_ASSERT_FALSE(0) ;
	CU_ASSERT_FALSE(!1) ;
}

void testSuccessAssertEqual(void)
{
	CU_ASSERT_EQUAL(10, 10) ;
	CU_ASSERT_EQUAL(0, 0) ;
	CU_ASSERT_EQUAL(0, -0) ;
	CU_ASSERT_EQUAL(-12, -12) ;
}

void testSuccessAssertNotEqual(void)
{
	CU_ASSERT_NOT_EQUAL(10, 11) ;
	CU_ASSERT_NOT_EQUAL(0, -1) ;
	CU_ASSERT_NOT_EQUAL(-12, -11) ;
}

void testSuccessAssertPtrEqual(void)
{
	CU_ASSERT_PTR_EQUAL((void*)0x100, (void*)0x100) ;
}

void testSuccessAssertPtrNotEqual(void)
{
	CU_ASSERT_PTR_NOT_EQUAL((void*)0x100, (void*)0x101) ;
}

void testSuccessAssertPtrNull(void)
{
	CU_ASSERT_PTR_NULL(NULL) ;
	CU_ASSERT_PTR_NULL(0x0) ;
}

void testSuccessAssertPtrNotNull(void)
{
	CU_ASSERT_PTR_NOT_NULL((void*)0x23) ;
}

void testSuccessAssertStringEqual(void)
{
	char str1[] = "test" ;
	char str2[] = "test" ;

	CU_ASSERT_STRING_EQUAL(str1, str2) ;
}

void testSuccessAssertStringNotEqual(void)
{
	char str1[] = "test" ;
	char str2[] = "testtsg" ;

	CU_ASSERT_STRING_NOT_EQUAL(str1, str2) ;
}

void testSuccessAssertNStringEqual(void)
{
	char str1[] = "test" ;
	char str2[] = "testgfsg" ;

	CU_ASSERT_NSTRING_EQUAL(str1, str2, strlen(str1)) ;
	CU_ASSERT_NSTRING_EQUAL(str1, str1, strlen(str1)) ;
	CU_ASSERT_NSTRING_EQUAL(str1, str1, strlen(str1) + 1) ;
}

void testSuccessAssertNStringNotEqual(void)
{
	char str1[] = "test" ;
	char str2[] = "teet" ;
	char str3[] = "testgfsg" ;

	CU_ASSERT_NSTRING_NOT_EQUAL(str1, str2, 3) ;
	CU_ASSERT_NSTRING_NOT_EQUAL(str1, str3, strlen(str1) + 1) ;
}

void testSuccessAssertDoubleEqual(void)
{
	CU_ASSERT_DOUBLE_EQUAL(10, 10.0001, 0.0001) ;
	CU_ASSERT_DOUBLE_EQUAL(10, 10.0001, -0.0001) ;
	CU_ASSERT_DOUBLE_EQUAL(-10, -10.0001, 0.0001) ;
	CU_ASSERT_DOUBLE_EQUAL(-10, -10.0001, -0.0001) ;
}

void testSuccessAssertDoubleNotEqual(void)
{
	CU_ASSERT_DOUBLE_NOT_EQUAL(10, 10.001, 0.0001) ;
	CU_ASSERT_DOUBLE_NOT_EQUAL(10, 10.001, -0.0001) ;
	CU_ASSERT_DOUBLE_NOT_EQUAL(-10, -10.001, 0.0001) ;
	CU_ASSERT_DOUBLE_NOT_EQUAL(-10, -10.001, -0.0001) ;
}
void AddAssertTests(void)
{
	CU_pSuite pGroup = NULL;
	CU_pTest pTest = NULL;

	pGroup = CU_add_suite("TestSimpleAssert", NULL, NULL);
	pTest = CU_add_test(pGroup, "testSuccessSimpleAssert", testSuccessSimpleAssert);

	pGroup = CU_add_suite("TestBooleanAssert", NULL, NULL);
	pTest = CU_add_test(pGroup, "testSuccessAssertTrue", testSuccessAssertTrue);
	pTest = CU_add_test(pGroup, "testSuccessAssertFalse", testSuccessAssertFalse);

	pGroup = CU_add_suite("TestEqualityAssert", NULL, NULL);
	pTest = CU_add_test(pGroup, "testSuccessAssertEqual", testSuccessAssertEqual);
	pTest = CU_add_test(pGroup, "testSuccessAssertNotEqual", testSuccessAssertNotEqual);

	pGroup = CU_add_suite("TestPointerAssert", NULL, NULL);
	pTest = CU_add_test(pGroup, "testSuccessAssertPtrEqual", testSuccessAssertPtrEqual);
	pTest = CU_add_test(pGroup, "testSuccessAssertPtrNotEqual", testSuccessAssertPtrNotEqual);

	pGroup = CU_add_suite("TestNullnessAssert", NULL, NULL);
	pTest = CU_add_test(pGroup, "testSuccessAssertPtrNull", testSuccessAssertPtrNull);
	pTest = CU_add_test(pGroup, "testSuccessAssertPtrNotNull", testSuccessAssertPtrNotNull);

	pGroup = CU_add_suite("TestStringAssert", NULL, NULL);
	pTest = CU_add_test(pGroup, "testSuccessAssertStringEqual", testSuccessAssertStringEqual);
	pTest = CU_add_test(pGroup, "testSuccessAssertStringNotEqual", testSuccessAssertStringNotEqual);

	pGroup = CU_add_suite("TestNStringAssert", NULL, NULL);
	pTest = CU_add_test(pGroup, "testSuccessAssertNStringEqual", testSuccessAssertNStringEqual);
	pTest = CU_add_test(pGroup, "testSuccessAssertNStringNotEqual", testSuccessAssertNStringNotEqual);

	pGroup = CU_add_suite("TestDoubleAssert", NULL, NULL);
	pTest = CU_add_test(pGroup, "testSuccessAssertDoubleEqual", testSuccessAssertDoubleEqual);
	pTest = CU_add_test(pGroup, "testSuccessAssertDoubleNotEqual", testSuccessAssertDoubleNotEqual);
}

/************************************************************************/




/************************************************************************/
int success_init(void) { return 0; }
int success_clean(void) { return 0; }

void testSuccess1(void) { CU_ASSERT(1); }
void testSuccess2(void) { CU_ASSERT(1); }
void testSuccess3(void) { CU_ASSERT(1); }
void testSuccess4(void) { CU_ASSERT_EQUAL(add(1,2), 3); }
void testSuccess5(void) { CU_ASSERT_EQUAL(add(1,2), 5); }

void testfailure1(void) { CU_ASSERT(12 <= 10); }
void testfailure2(void) { CU_ASSERT(2); }
void testfailure3(void) { CU_ASSERT(3); }

int group_failure_init(void) { return 0;}
int group_failure_clean(void) { return 0; }

void testGroupFailure1(void) { CU_ASSERT(0); }
void testGroupFailure2(void) { CU_ASSERT(2); }

void AddTests(void)
{
#if 0
/*第一种添加suite和test的方式*/
	CU_pSuite pGroup = NULL;
	CU_pTest pTest = NULL;

	pGroup = CU_add_suite("Sucess_Suite", success_init, success_clean);
	pTest = CU_add_test(pGroup, "TestSuccess1", testSuccess1);
	pTest = CU_add_test(pGroup, "TestSuccess2", testSuccess2);
	pTest = CU_add_test(pGroup, "TestSuccess3", testSuccess3);
	pTest = CU_add_test(pGroup, "TestSuccess4", testSuccess4);
	pTest = CU_add_test(pGroup, "TestSuccess5", testSuccess5);

	pGroup = CU_add_suite("Failure_Suite", NULL, NULL);
	pTest = CU_add_test(pGroup, "TestFailure1", testfailure1);
	pTest = CU_add_test(pGroup, "TestFailure2", testfailure2);
	pTest = CU_add_test(pGroup, "TestFailure3", testfailure3);

	pGroup = CU_add_suite("Group_Failure_Suite", group_failure_init, group_failure_clean);
	pTest = CU_add_test(pGroup, "TestGroupFailure1", testGroupFailure1);
	pTest = CU_add_test(pGroup, "TestGroupFailure2", testGroupFailure2);
#else
    /*第二种方式*/
    CU_TestInfo success_test[] = {
        {"TestSuccess1", testSuccess1 },
        {"TestSuccess2", testSuccess2 },
        {"TestSuccess3", testSuccess3 },
        {"TestSuccess4", testSuccess4 },
        {"TestSuccess5", testSuccess5 },
        CU_TEST_INFO_NULL
    };
    CU_TestInfo failure_test[] = {
        {"TestFailure1", testfailure1 }, 
        {"TestFailure2", testfailure2 }, 
        {"TestFailure3", testfailure3 }, 
        CU_TEST_INFO_NULL
    };
    CU_TestInfo group_failure_test[] = {
        {"TestGroupFailure1", testGroupFailure1 },
        {"TestGroupFailure2", testGroupFailure2 },
        CU_TEST_INFO_NULL
    };
    CU_SuiteInfo suites[] = {
        {"Sucess_Suite", success_init, success_clean, NULL, NULL, success_test},
        {"Failure_Suite", NULL, NULL, NULL, NULL, failure_test},
        {"Group_Failure_Suite", group_failure_init, group_failure_clean, NULL, NULL, group_failure_test},
        CU_SUITE_INFO_NULL
    };
    if(CUE_SUCCESS != CU_register_suites(suites))
    {
        exit(1);
    }
#endif
}
/*************************************************************************/

int main(int argc, const char *argv[])
{
	setvbuf(stdout, NULL, _IONBF, 0);

    if (argc > 1) 
    {
		int Run = 0 ;

		if (CU_initialize_registry()) 
        {
			printf("\nInitialize of test Registry failed.");
		}

		if (!strcmp("--test", argv[1])) 
        {
			Run = 1 ;
			AddTests();
		}
		else if (!strcmp("--atest", argv[1])) 
        {
			Run = 1 ;
			AddAssertTests();
		}
		else if (!strcmp("--alltest", argv[1])) 
        {
			Run = 1 ;
			AddTests();
			AddAssertTests();
		}

		if (1 == Run) 
        {
            /**** Automated Mode *****************/
            fprintf(stderr, "Automated Mode ###########################################\n");
            /*设置报表的名称*/
            CU_set_output_filename("CUnitCase");
            /*如果不加这个函数，只会输出Result报表，不会有Listen报表*/
            CU_list_tests_to_file();
            /*执行测试*/
            CU_automated_run_tests();
            /***************************************/
            
            /***** Basic Mode *******************/
            fprintf(stderr, "Basic Mode ###########################################\n");
            //CU_basic_set_mode(CU_BRM_VERBOSE);
            //CU_basic_run_tests();
            /***************************************/

            /*****Console Mode ********************/
            fprintf(stderr, "Console Mode ###########################################\n");
            //CU_console_run_tests();
            /************************************/
        }

        /*清理registry*/
        CU_cleanup_registry();
    }

    return 0;
}
