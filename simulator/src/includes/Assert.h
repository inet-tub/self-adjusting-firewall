/*
 * assert.h
 *
 *  Created on: Jul 13, 2021
 *      Author: vamsi
 */

#ifndef INCLUDES_ASSERT_H_
#define INCLUDES_ASSERT_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace simulator{
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)

}
#endif /* INCLUDES_ASSERT_H_ */
