#ifndef utils_h
#define utils_h

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

#define TERM_COL_ERROR(msg)     "\x1b[31;1m" msg "\x1b[0;0m"
#define TERM_COL_SUCCESS(msg)   "\x1b[32;1m" msg "\x1b[0;0m"
#define TERM_COL_INFO(msg)      "\x1b[35;1m" msg "\x1b[0;0m"

#define ERROR_MSG_LOG(msg, log) TERM_COL_ERROR("error")": "TERM_COL_INFO("%s") \
                                ": %s\n", msg, log

#define MODEL_CUBE_wTEXTURE    {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,\
                                 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,\
                                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,\
                                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,\
                                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,\
                                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,\
                                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
                                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,\
                                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,\
                                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,\
                                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,\
                                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
                                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,\
                                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,\
                                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,\
                                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,\
                                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
                                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,\
                                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,\
                                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,\
                                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,\
                                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,\
                                 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
                                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,\
                                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,\
                                 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,\
                                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,\
                                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,\
                                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
                                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,\
                                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,\
                                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,\
                                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,\
                                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,\
                                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,\
                                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f}
#define MODEL_CUBE {-0.5f, -0.5f, -0.5f,\
                     0.5f, -0.5f, -0.5f,\
                     0.5f,  0.5f, -0.5f,\
                     0.5f,  0.5f, -0.5f,\
                    -0.5f,  0.5f, -0.5f,\
                    -0.5f, -0.5f, -0.5f,\
                    -0.5f, -0.5f,  0.5f,\
                     0.5f, -0.5f,  0.5f,\
                     0.5f,  0.5f,  0.5f,\
                     0.5f,  0.5f,  0.5f,\
                    -0.5f,  0.5f,  0.5f,\
                    -0.5f, -0.5f,  0.5f,\
                    -0.5f,  0.5f,  0.5f,\
                    -0.5f,  0.5f, -0.5f,\
                    -0.5f, -0.5f, -0.5f,\
                    -0.5f, -0.5f, -0.5f,\
                    -0.5f, -0.5f,  0.5f,\
                    -0.5f,  0.5f,  0.5f,\
                     0.5f,  0.5f,  0.5f,\
                     0.5f,  0.5f, -0.5f,\
                     0.5f, -0.5f, -0.5f,\
                     0.5f, -0.5f, -0.5f,\
                     0.5f, -0.5f,  0.5f,\
                     0.5f,  0.5f,  0.5f,\
                    -0.5f, -0.5f, -0.5f,\
                     0.5f, -0.5f, -0.5f,\
                     0.5f, -0.5f,  0.5f,\
                     0.5f, -0.5f,  0.5f,\
                    -0.5f, -0.5f,  0.5f,\
                    -0.5f, -0.5f, -0.5f,\
                    -0.5f,  0.5f, -0.5f,\
                     0.5f,  0.5f, -0.5f,\
                     0.5f,  0.5f,  0.5f,\
                     0.5f,  0.5f,  0.5f,\
                    -0.5f,  0.5f,  0.5f,\
                    -0.5f,  0.5f, -0.5f}
#define MODEL_10_CUBES_POS    {{ 0.0f,  0.0f,  0.0f},\
                               { 2.0f,  5.0f, -15.0f},\
                               {-1.5f, -2.2f, -2.5f},\
                               {-3.8f, -2.0f, -12.3f},\
                               { 2.4f, -0.4f, -3.5f},\
                               {-1.7f,  3.0f, -7.5f},\
                               { 1.3f, -2.0f, -2.5f},\
                               { 1.5f,  2.0f, -2.5f},\
                               { 1.5f,  0.2f, -1.5f},\
                               {-1.3f,  1.0f, -1.5f}}

                                   // vertices.........  // normal.........  // texture 
#define MODEL_CUBE_NORMAL_TEXTURE {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,\
                                    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,\
                                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,\
                                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,\
                                   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,\
                                   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,\
                                   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,\
                                    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,\
                                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,\
                                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,\
                                   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,\
                                   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,\
                                   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,\
                                   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,\
                                   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,\
                                   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,\
                                   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,\
                                   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,\
                                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,\
                                    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,\
                                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,\
                                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,\
                                    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,\
                                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,\
                                   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,\
                                    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,\
                                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,\
                                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,\
                                   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,\
                                   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,\
                                   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,\
                                    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,\
                                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,\
                                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,\
                                   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,\
                                   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f}

                                   // vertices.........  // normal.........
#define MODEL_CUBE_NORMAL         {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,\
                                    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,\
                                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,\
                                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,\
                                   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,\
                                   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,\
                                   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,\
                                    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,\
                                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,\
                                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,\
                                   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,\
                                   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,\
                                   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,\
                                   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,\
                                   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,\
                                   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,\
                                   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,\
                                   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,\
                                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,\
                                    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,\
                                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,\
                                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,\
                                    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,\
                                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,\
                                   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,\
                                    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,\
                                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,\
                                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,\
                                   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,\
                                   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,\
                                   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,\
                                    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,\
                                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,\
                                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,\
                                   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,\
                                   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f}
        
#define PATH_MAX 4096
// sleeps for msec miliseconds
int msleep(long msec);
const char *file2str(const char *file_path);

#include <stdlib.h>
/**
 * @brief push content on a dynamic array
 * 
 * @param ptr the adress of the dynamic array
 * @param sz current bytes used in that array
 * @param tot_sz total capacity of array in bytes
 * @param v value tu be inserted
 * @param v_sz size in bytes of that value
 * @return new total size
 */
size_t push(void **ptr, size_t sz, size_t tot_sz, void *v, size_t v_sz);
/**
 * @brief push content on a dynamic array
 * 
 * @param ptr the adress of the dynamic array
 * @param sz current bytes used in that array
 * @param tot_sz total capacity of array in bytes
 * @param v value of the poped element
 * @param v_sz size in bytes of that value
 * @return new total size
 */

// error handeling
#include <assert.h>

#endif 