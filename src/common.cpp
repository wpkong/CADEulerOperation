/**
 * ------------------------------------
 * @author: Weipeng Kong
 * @date: 2020/11/20
 * @email: yjxkwp@foxmail.com
 * @site: https://x86.design
 * @description: 
 * ------------------------------------
**/

#include "common.h"

void nullptr_assertion(const std::initializer_list<void *> &lst) {
    int i = 1;
    for (const auto &p : lst) {
        if (p == nullptr) {
            std::stringstream ss;
            ss << "Null pointer found at position " << i++ + 1 << "." << std::endl;
            throw std::runtime_error(ss.str());
        }
    }
}
