
/*
 * Copyright (C) Igor Sysoev
 *
 * Modified from ngx_palloc.{h,c} of Nginx-0.6.39
 * By weizili <zieckey@gmail.com>
 * Date : 2011.5.10
 */


#pragma once

#ifndef QIHOO_MEMORY_POOL
    #define QIHOO_MEMORY_POOL
#endif

#include <stdio.h>

#define qh_align(size, boundary) (((size) + ((boundary) - 1)) & ~((boundary) - 1))

#ifndef uintptr_t
    #if ( defined( __linux__ ) || defined( __gnu_linux__ ) )
        #include <stdint.h>
        typedef uint64_t uintptr_t;
    #endif
#endif

#define qh_align_ptr(p, a)                                                   \
    (unsigned char*) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


/*
 * QH_MAX_ALLOC_FROM_POOL should be (qh_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define QH_MAX_ALLOC_FROM_POOL  (qh_pagesize - 1)

#define QH_DEFAULT_POOL_SIZE    (16 * 1024)
#define QH_ALIGNMENT (sizeof(unsigned long))
#define QH_POOL_ALIGNMENT       (16)
#define QH_MIN_POOL_SIZE                                        \
    qh_align((sizeof(qh_pool_t) + 2 * sizeof(qh_pool_large_t)), \
             QH_POOL_ALIGNMENT)



#ifdef __cplusplus
extern "C" {
#endif

extern int qh_pagesize;

typedef void  ( *qh_pool_cleanup_pt )( void *data );

typedef void* ( *qh_alloc_pt )( size_t size );
typedef void  ( *qh_free_pt )( void* p );



typedef struct qh_pool_cleanup_s       qh_pool_cleanup_t;
typedef struct qh_pool_s               qh_pool_t;
typedef struct qh_pool_large_s         qh_pool_large_t;
typedef struct qh_str_s                qh_str_t;
typedef struct qh_pool_data_s          qh_pool_data_t;

struct qh_pool_cleanup_s
{
    qh_pool_cleanup_pt handler;
    void              *data;
    qh_pool_cleanup_t *next;
};

struct qh_pool_large_s
{
    qh_pool_large_t *next;
    void            *alloc;
};

struct qh_str_s
{
    size_t         len;
    unsigned char *data;
};

struct qh_pool_data_s
{
    unsigned char *last;
    unsigned char *end;
    qh_pool_t     *next;
    unsigned int   failed;
};


struct qh_pool_s
{
    qh_pool_data_t     d;
    unsigned int       max;//the max size of memory allocated from pool
    qh_pool_t         *current;
    qh_pool_large_t   *large;
    qh_pool_cleanup_t *cleanup;
};

/**
 * @param pool_size - the pool size
 * @param max_alloc_from_pool - the max memory unit alloc from pool
 */
qh_pool_t *qh_create_pool( size_t pool_size, size_t max_alloc_from_pool /*= QH_MAX_ALLOC_FROM_POOL*/  );
void qh_destroy_pool( qh_pool_t *pool );
void qh_reset_pool( qh_pool_t *pool );

//malloc memory from qh_pool_t
void *qh_palloc( qh_pool_t *pool, size_t size );
void *qh_pnalloc( qh_pool_t *pool, size_t size );
void *qh_pcalloc( qh_pool_t *pool, size_t size );
void *qh_pmemalign( qh_pool_t *pool, size_t size, size_t alignment );
int   qh_pfree( qh_pool_t *pool, void *p );


qh_pool_cleanup_t *qh_pool_cleanup_add( qh_pool_t *p, size_t size );


void qh_set_alloc( qh_alloc_pt alloc_pt );
void qh_set_free( qh_free_pt free_pt );

int qh_dbgprint( const char* fmt, ... );

#ifdef __cplusplus
} // end of extern "C" {
#endif

/** define some micro to make it no change to using Nginx code */
#define NGX_MAX_ALLOC_FROM_POOL QH_MAX_ALLOC_FROM_POOL
#define NGX_DEFAULT_POOL_SIZE   QH_DEFAULT_POOL_SIZE
#define NGX_ALIGNMENT           QH_ALIGNMENT
#define NGX_POOL_ALIGNMENT      QH_POOL_ALIGNMENT
#define NGX_MIN_POOL_SIZE       QH_MIN_POOL_SIZE

#define ngx_pagesize            qh_pagesize

#define ngx_align               qh_align
#define ngx_align_ptr           qh_align_ptr

#define ngx_pool_cleanup_s      qh_pool_cleanup_s
#define ngx_pool_s              qh_pool_s
#define ngx_pool_large_s        qh_pool_large_s
#define ngx_str_s               qh_str_s
#define ngx_pool_data_s         qh_pool_data_s

#define ngx_pool_cleanup_t      qh_pool_cleanup_t
#define ngx_pool_t              qh_pool_t
#define ngx_pool_large_t        qh_pool_large_t
#define ngx_str_t               qh_str_t
#define ngx_pool_data_t         qh_pool_data_t

#define ngx_create_pool         qh_create_pool
#define ngx_destroy_pool        qh_destroy_pool
#define ngx_reset_pool          qh_reset_pool
#define ngx_palloc              qh_palloc
#define ngx_pnalloc             qh_pnalloc
#define ngx_pcalloc             qh_pcalloc
#define ngx_pmemalign           qh_pmemalign
#define ngx_pfree               qh_pfree
#define ngx_pool_cleanup_add    qh_pool_cleanup_add





#ifdef __cplusplus
namespace simcc {
namespace qh {
    class Pool {
    public:
        Pool(size_t pool_size, size_t max_alloc_from_pool = QH_MAX_ALLOC_FROM_POOL) {
            pool_ = qh_create_pool(pool_size, max_alloc_from_pool);
        }

        ~Pool() {
            qh_destroy_pool(pool_);
            pool_ = NULL;
        }

        qh_pool_t* pool() const {
            return pool_;
        }

        char* alloc(size_t len) {
            return static_cast<char*>(qh_palloc(pool_, len));
        }

        void free(void* p) {
            qh_pfree(pool_, p);
        }

    private:
        qh_pool_t * pool_;
    };
}
}
#endif // end of #ifdef __cplusplus

