
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include <map>
#include <string>

#include "qh_palloc.h"

#if ( defined(_WIN32) || defined(WIN64) )
    #include <Windows.h>
    int getpagesize();
    #define snprintf _snprintf
#else
    #include <unistd.h>
#endif


#if defined(_DEBUG) && defined(__cplusplus)
//    #define TEST_POOL_MEMORY_ALLOC
#endif

#ifdef H_OS_WINDOWS
    #ifndef TEST_POOL_MEMORY_ALLOC
        #define TEST_POOL_MEMORY_ALLOC
    #endif
#endif

#ifdef TEST_POOL_MEMORY_ALLOC
    #include <map>
    #include <string>
    typedef std::map<void*, std::string> pointerstringmap;
    pointerstringmap* mallocmap = NULL;
#endif//#ifdef TEST_POOL_MEMORY_ALLOC

static void *qh_palloc_block(qh_pool_t *pool, size_t size);
static void *qh_palloc_large(qh_pool_t *pool, size_t size);

//malloc memory directly from OS memory
static void *_qh_alloc(size_t size );
static void *_qh_alloc4(size_t size, const char* funcname, const char* filename, int nLine);
static void _qh_free( void* p );

int qh_pagesize = 4096;

static qh_alloc_pt qh_alloc = &_qh_alloc;
static qh_free_pt qh_free = &_qh_free;

void* _qh_alloc(size_t size)
{/*{{{*/
    return _qh_alloc4(size, NULL, NULL, 0);
}/*}}}*/

void* _qh_alloc4(size_t size, const char* funcname, const char* filename, int nLine)
{/*{{{*/
    void* p = malloc(size);
    
#ifdef TEST_POOL_MEMORY_ALLOC
    if ( !funcname ) funcname = "";
    if ( !filename ) filename = "";
    char buf[4096] = {'\0'};
    snprintf( buf, sizeof(buf), "malloc p=%p size=%d, %s %s:%d\n", 
                    p, (int)size, funcname, filename, nLine );
    printf("%s\n", buf);
    mallocmap->insert( pointerstringmap::value_type( p, buf ) );
#else
    (void)funcname; (void)filename; (void)nLine;
#endif//#ifdef TEST_POOL_MEMORY_ALLOC

    return p;
}/*}}}*/

void _qh_free( void* p )
{/*{{{*/
    if ( p )
    {
        free(p);
            
#ifdef TEST_POOL_MEMORY_ALLOC
        printf("free p=0x%p\n", p );
        mallocmap->erase( p );
#endif//#ifdef TEST_POOL_MEMORY_ALLOC  

    }
}/*}}}*/




qh_pool_t *
qh_create_pool(size_t size, size_t max_alloc_from_pool/* = QH_MAX_ALLOC_FROM_POOL*/ )
{/*{{{*/
    qh_pool_t  *p = NULL;

#ifdef TEST_POOL_MEMORY_ALLOC
    mallocmap = new pointerstringmap();
#endif//#ifdef TEST_POOL_MEMORY_ALLOC
    
    size = qh_align( size, QH_POOL_ALIGNMENT );

#ifdef _DEBUG
    p = (qh_pool_t*)_qh_alloc4(size, __FUNCTION__, __FILE__, __LINE__ );
#else
    p = (qh_pool_t*)qh_alloc(size);
#endif
    
    if (p == NULL) {
        return NULL;
    }

    p->d.last = (unsigned char *) p + sizeof(qh_pool_t);
    p->d.end = (unsigned char *) p + size;
    p->d.next = NULL;
    p->d.failed = 0;

    size = size - sizeof(qh_pool_t);
    p->max = (unsigned int)(size < max_alloc_from_pool ? size : max_alloc_from_pool);

    p->current = p;
    p->large = NULL;
    p->cleanup = NULL;


    qh_pagesize = getpagesize();

    return p;
}/*}}}*/


void
qh_destroy_pool(qh_pool_t *pool)
{/*{{{*/
    qh_pool_t          *p, *n;
    qh_pool_large_t    *l;
    qh_pool_cleanup_t  *c;

    for (c = pool->cleanup; c; c = c->next) {
        if (c->handler) {
            c->handler(c->data);
        }
    }

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            qh_free(l->alloc);
        }
    }

#if defined(_DEBUG) && defined(TEST_POOL_MEMORY_ALLOC)
    /*
     * we could allocate the pool->log from this pool
     * so we can not use this log while the free()ing the pool
     */
    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        qh_dbgprint( "free: %x, unused: %d\n", p, (int)(p->d.end - p->d.last) );
        if (n == NULL) {
            break;
        }
    }

#endif

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        qh_free(p);

        if (n == NULL) {
            break;
        }
    }


#ifdef TEST_POOL_MEMORY_ALLOC
    pointerstringmap::iterator it (mallocmap->begin());
    pointerstringmap::iterator ite(mallocmap->end());
    if ( it != ite )
    {
        printf( "Memory leak!!!!!!\n" );
        for ( ; it != ite; ++it )
        {
            printf( "%p %s\n", it->first, it->second.c_str() );
        }
        assert( false );
    }
    else
    {
        printf("No memory leak! Great!\n");
    }
    delete mallocmap;
    mallocmap = NULL;
#endif//#ifdef TEST_POOL_MEMORY_ALLOC

}/*}}}*/


void
qh_reset_pool(qh_pool_t *pool)
{/*{{{*/
    qh_pool_t        *p;
    qh_pool_large_t  *l;

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            qh_free(l->alloc);
        }
    }

    pool->large = NULL;

    for (p = pool; p; p = p->d.next) {
        p->d.last = (unsigned char *) p + sizeof(qh_pool_t);
    }
}/*}}}*/


void *
qh_palloc(qh_pool_t *pool, size_t size)
{/*{{{*/
    unsigned char      *m;
    qh_pool_t  *p;

    if (size <= pool->max) {

        p = pool->current;

        do {
            m = qh_align_ptr(p->d.last, QH_ALIGNMENT);

            if ((size_t) (p->d.end - m) >= size) {
                p->d.last = m + size;

                return m;
            }

            p = p->d.next;

        } while (p);

        return qh_palloc_block(pool, size);
    }

    return qh_palloc_large(pool, size);
}/*}}}*/


void *
qh_pnalloc(qh_pool_t *pool, size_t size)
{/*{{{*/
    unsigned char      *m;
    qh_pool_t  *p;

    if (size <= pool->max) {

        p = pool->current;

        do {
            m = p->d.last;

            if ((size_t) (p->d.end - m) >= size) {
                p->d.last = m + size;

                return m;
            }

            p = p->d.next;

        } while (p);

        return qh_palloc_block(pool, size);
    }

    return qh_palloc_large(pool, size);
}/*}}}*/


static void *
qh_palloc_block(qh_pool_t *pool, size_t size)
{/*{{{*/
    unsigned char      *m;
    size_t       psize;
    qh_pool_t  *p, *npool, *current;

    psize = (size_t) (pool->d.end - (unsigned char *) pool);

#ifdef _DEBUG
    m = (unsigned char*)_qh_alloc4( qh_align(psize, QH_POOL_ALIGNMENT), __FUNCTION__, __FILE__, __LINE__ );
#else
    m = (unsigned char*)qh_alloc( qh_align(psize, QH_POOL_ALIGNMENT) );
#endif

    if (m == NULL) {
        return NULL;
    }

    npool = (qh_pool_t *) m;

    npool->d.end = m + psize;
    npool->d.next = NULL;
    npool->d.failed = 0;

    m += sizeof(qh_pool_data_t);
    m = qh_align_ptr(m, QH_ALIGNMENT);
    npool->d.last = m + size;

    current = pool->current;

    for (p = current; p->d.next; p = p->d.next) {
        if (p->d.failed++ > 4) {
            current = p->d.next;
        }
    }

    p->d.next = npool;

    pool->current = current ? current : npool;

    return m;
}/*}}}*/


static void *
qh_palloc_large(qh_pool_t *pool, size_t size)
{/*{{{*/
    void                  *p = NULL;
    int                    n = 0;
    qh_pool_large_t  *large = NULL;

#ifdef _DEBUG
    p = (qh_pool_t*)_qh_alloc4(size, __FUNCTION__, __FILE__, __LINE__ );
#else
    p = (qh_pool_t*)qh_alloc(size);
#endif

    if (p == NULL) {
        return NULL;
    }

    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            return p;
        }

        if (n++ > 3) {
            break;
        }
    }

    large = (qh_pool_large_t*)qh_palloc(pool, sizeof(qh_pool_large_t));
    if (large == NULL) {
        qh_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}/*}}}*/


void *
qh_pmemalign(qh_pool_t *pool, size_t size, size_t alignment)
{/*{{{*/
    void              *p;
    qh_pool_large_t  *large;

#ifdef _DEBUG
    p = (qh_pool_large_t*)_qh_alloc4( qh_align(size, alignment), __FUNCTION__, __FILE__, __LINE__ );
#else
    p = (qh_pool_large_t*)qh_alloc( qh_align(size, alignment) );
#endif

    if (p == NULL) {
        return NULL;
    }

    large = (qh_pool_large_t*)qh_palloc(pool, sizeof(qh_pool_large_t));
    if (large == NULL) {
        qh_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}/*}}}*/


int
qh_pfree(qh_pool_t *pool, void *p)
{/*{{{*/

    /**
     * modify the original implementation 
     */
    qh_pool_large_t  *pre = pool->large;
    qh_pool_large_t  *l = pool->large;

    for ( ; l; pre = l, l = l->next) {
        if (p == l->alloc) {
            qh_free(l->alloc);
            l->alloc = NULL;
            if ( pool->large == l )
            {
                pool->large = l->next;
            }
            else
            {
                pre->next = l->next;
            }
            return 0;
        }
    }

    /**  
    //original nginx implementation

    qh_pool_large_t  *l;

    for (l = pool->large; l; l = l->next) {
        if (p == l->alloc) {
            qh_free(l->alloc);
            l->alloc = NULL;
            return 0;
        }
    }
    */


    return -1;
}/*}}}*/


void *
qh_pcalloc(qh_pool_t *pool, size_t size)
{/*{{{*/
    void *p;

    p = qh_palloc(pool, size);
    if (p) {
        memset(p, 0, size);
    }

    return p;
}/*}}}*/


qh_pool_cleanup_t *
qh_pool_cleanup_add(qh_pool_t *p, size_t size)
{/*{{{*/
    qh_pool_cleanup_t  *c;

    c = (qh_pool_cleanup_t*)qh_palloc(p, sizeof(qh_pool_cleanup_t));
    if (c == NULL) {
        return NULL;
    }

    if (size) {
        c->data = qh_palloc(p, size);
        if (c->data == NULL) {
            return NULL;
        }

    } else {
        c->data = NULL;
    }

    c->handler = NULL;
    c->next = p->cleanup;

    p->cleanup = c;

    return c;
}/*}}}*/


void qh_set_alloc( qh_alloc_pt alloc_pt )
{/*{{{*/
    if ( alloc_pt )
    {
        qh_alloc = alloc_pt;
    }
    else
    {
        qh_alloc = &_qh_alloc;
    }
}/*}}}*/

void qh_set_free( qh_free_pt free_pt )
{/*{{{*/
    if ( free_pt )
    {
        qh_free = free_pt;
    }
    else
    {
        qh_free = &_qh_free;
    }
}/*}}}*/


int qh_dbgprint( const char* fmt, ... )
{/*{{{*/
#ifdef _DEBUG
#define _XX_BUF_SIZE 80960
    int n = 0;
    va_list ap;
    char str[_XX_BUF_SIZE];
    va_start(ap,fmt);
    vsnprintf(str, _XX_BUF_SIZE, fmt, ap);
    n = printf(str);
    va_end(ap);
    return n;
#undef _XX_BUF_SIZE
#else
    (void)fmt;
    return 0;
#endif
}/*}}}*/

#if ( defined(WIN32) || defined(WIN64) )
/* getpagesize for windows */
int getpagesize(void) {
    static int g_pagesize = 0;
    if (! g_pagesize) {
        SYSTEM_INFO system_info;
        GetSystemInfo (&system_info);
        g_pagesize = (int)system_info.dwPageSize;
    }
    return g_pagesize;
}
#endif


