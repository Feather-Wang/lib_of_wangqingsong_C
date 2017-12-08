# 首先，需要安装CUnit
# 其次，将CUnit安装后的头文件目录和动态库或者静态库目录替换到以下两个环境变量中
# 然后，在使用本脚本编译测试程序，编译命令：. ./build.sh

# 初始化环境变量
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/home/jeff/test/CUnit/cunit_location/include/CUnit
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/jeff/test/CUnit/cunit_location/lib

# 编译测试单元，其中，CUnitCase.c为CUnit的测试程序，test.c为被测试代码
gcc CUnitCase.c test.c -lcunit
