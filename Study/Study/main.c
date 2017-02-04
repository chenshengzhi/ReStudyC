//
//  main.c
//  Study
//
//  Created by csz on 2017/2/2.
//  Copyright © 2017年 csz. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

#define CLog(fmt, ...) printf(("%s %s [%d] : " fmt "\n"), __TIME__, __FUNCTION__, __LINE__,  ##__VA_ARGS__)


#define k_insert_new_line printf("\n")

/**
 测试大小端
 */
#define k_test_big_or_small_endian 1


/**
 打印取值极限
 */
#define k_print_digit_min_and_max 1


/**
 测试溢出
 */
#define k_test_add_ok 1

#if k_test_add_ok

#define k_test_add_ok_int     1
#define k_test_add_ok_short   1
#define k_test_add_ok_char    1

#if k_test_add_ok_int
typedef int k_test_add_ok_type;
#elif k_test_add_ok_short
typedef short k_test_add_ok_type;
#elif k_test_add_ok_char

typedef char k_test_add_ok_type;
#endif

#endif


/**
 测试分解数字成二进制表示
 */
#define k_test_digit_breakup 1


#pragma mark - utils -
/**
 字符串翻转

 @param p 字符串指针
 @param length 字符串长度
 */
void string_revert(char *p, size_t length) {
    char tmp;
    size_t mid = (length - 1) / 2;
    for (int i = 0; i < mid; i++) {
        tmp = p[i];
        p[i] = p[length - 1 - i];
        p[length - 1 - i] = tmp;
    }
}


/**
 具有换行的打印

 @param fmt 格式化字符串
 @param ... 变长参数
 @return 实际打印出的字符串长度
 */
int log_with_newline_suffix(char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    va_end(arg);
    size_t length = strlen(fmt);
    char new_fmt[length + 2];
    strcpy(new_fmt, fmt);
    strcat(new_fmt, "\n");
    new_fmt[length + 1] = '\0';
    int cnt = vprintf(new_fmt, arg);
    va_end(arg);
    return cnt;
}

#pragma mark - functions -

/**
 读取每个字节, 并以十六进制打印

 @param p 启始指针
 @param length 要打印的字节长度
 */
void dump_memory(char *p, int length) {
    for (int i = 0; i < length; i++) {
        CLog("0x%.2x", p[i]);
    }
}


/**
 判断2个数字相加是否正常或溢出

 @param x x
 @param y y
 @return 1-正常, 0-溢出
 */
int add_ok(k_test_add_ok_type x, k_test_add_ok_type y) {
    k_test_add_ok_type result = x + y;
    k_test_add_ok_type x2 = result - y;
    k_test_add_ok_type y2 = result - x;
    CLog("%d", result);
    CLog("%d", x2);
    CLog("%d", y2);
    CLog("%d", (result - y));
    CLog("%d", (result - x));
    return (result-x == y) && (result-y == x);
}


/**
 打印size字节的有符号数取值极限

 @param size 字节数
 */
void print_edge(unsigned short size) {
    if (size == 0) {
        return;
    }
    
    if (size > 8) {
        return;
    }
    
    int64_t result = 1;
    int count = size * 8 - 1;
    
    /*
     *  通过左移n位得到 2的n次方
     */
    result <<= count;
    CLog("%u字节取值范围: [%lld , %lld] ", size, -result, result - 1);
}


/**
 打印num的二进制形式的字符串, 求余方式

 @param num 要打印的数
 */
void print_binary(unsigned num) {
    size_t size = sizeof(num) * 8 + 1;
    char index_string[] = "0123456789ABCDEF";
    char x_binary_string[size];
    unsigned i = 0, j = 0, tmp = num;
    do {
        x_binary_string[i++] = index_string[tmp % 2];
        tmp >>= 1;
    } while (tmp > 0);
    x_binary_string[i] = '\0';
    
    string_revert(x_binary_string, i);
    
    long long validate_result = 0;
    char c = 0;
    for (j = 0; j < i; j++) {
        c = x_binary_string[j];
        if (c == '1') {
            validate_result += 1 << (i - j - 1);
        }
    }
    char *validate_result_string;
    if (validate_result != num) {
        validate_result_string = "fail !!!!!";
    } else {
        validate_result_string = "success ~ ~ ~";
    }
    CLog("validate %s x = %d, result = %lld", validate_result_string, num, validate_result);
    CLog("%u的二进制为: %s", num, x_binary_string);
}


/**
 打印x的二进制形式的字符串, 位移与操作方式

 @param num 要打印的数
 */
void print_binary2(int num) {
    int bit_num = sizeof(num) * 8;
    char x_binary_string[bit_num + 1];
    x_binary_string[bit_num] = '\0';
    
    for (int i = 0; i <= bit_num; i++) {
        if ((num & (1 << i)) > 0) {
            x_binary_string[i] = '1';
        } else {
            x_binary_string[i] = '0';
        }
    }
    
    for (int i = bit_num - 1; i > 0; i--) {
        if (x_binary_string[i] == '0') {
            x_binary_string[i] = '\0';
        } else {
            break;
        }
    }
    
    string_revert(x_binary_string, strlen(x_binary_string));
    
    CLog("%d的二进制为: %s", num, x_binary_string);
}


/**
 打印二进制次方字符串

 @param num unsigned
 */
void print_breakup_to_binary(unsigned num) {
    int bit_num = sizeof(num) * 8;
    int is_first = 1;
    printf(("%s %s [%d] : %d 分解成 : "), __TIME__, __FUNCTION__, __LINE__,  num);
    for (int i = 0; i < bit_num; i++) {
        if ((num & (1 << i)) > 0) {
            if (is_first) {
                is_first = 0;
                printf("2^%d", i);
            } else {
                printf("+ 2^%d", i);
            }
        }
    }
    printf("\n");
}


/**
 将乘法分解成位移

 @param x x
 @param y y-被分解的数
 */
void multy_breakup_to_add(unsigned x, unsigned y) {
    if (y > x) {
        unsigned tmp = y;
        y = x;
        x = tmp;
    }
    int64_t result = 0;
    int bit_num = sizeof(x) * 8;
    int is_first = 1;
    printf(("%s %s [%d] : %d * %d 分解成 : "), __TIME__, __FUNCTION__, __LINE__, x, y);
    for (int i = 0; i < bit_num; i++) {
        if ((y & (1 << i)) > 0) {
            if (is_first) {
                is_first = 0;
                printf("%d * 2^%d", x, i);
            } else {
                printf(" + %d * 2^%d", x, i);
            }
            result += x << i;
        }
    }
    printf("\n");
    CLog("validate result : %d * %d = %lld,  result = %lld", x, y, (int64_t)x * y, result);
}

#pragma mark - main -

int main(int argc, const char * argv[]) {
#if k_test_big_or_small_endian
    CLog("测试大小端： ");
    
    union {
        int i;
        char c;
    } union_i_c;
    union_i_c.i = 1;
    if (union_i_c.c == 1) {
        CLog("小端");
    } else {
        CLog("大端");
    }
    
    CLog("0x11223344的字节序为 ： ");
    int big_or_small_endian = 0x11223344;
    dump_memory((char *)&big_or_small_endian, sizeof(big_or_small_endian));
    
    k_insert_new_line;
#endif
    
#if k_print_digit_min_and_max
    print_edge(1);
    print_edge(2);
    print_edge(4);
    print_edge(8);
    k_insert_new_line;
#endif
    
#if k_test_add_ok
    CLog("测试溢出");
#if k_test_add_ok_int
    CLog("%d", add_ok(-2147483647, -15));
#elif k_test_add_ok_short
    CLog("%d", add_ok(32767, 55));
#elif k_test_add_ok_char
    CLog("%d", add_ok(127, 1));
#endif
    k_insert_new_line;
#endif
    
#if k_test_digit_breakup
    CLog("测试分解数字成二进制");
    print_binary(12345);
    print_binary2(12345);
    print_breakup_to_binary(12345);
    multy_breakup_to_add(54321, 12345);
    k_insert_new_line;
#endif
    
    return 0;
}
