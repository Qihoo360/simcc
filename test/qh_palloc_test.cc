#include "test_common.h"

#include <simcc/qh_palloc.h>

namespace {
    struct SArray {
        char* _B16;
        char* _B24;
        char* _B32;
        char* _B40;
        char* _B48;
        char* _B56;
        char* _B64;
        char* _B72;
        char* _B80;
        char* _B88;
        char* _B96;
        char* _B114;
        char* _B128;
        char* _B136;
        char* _B144;
        char* _B152;
        char* _B160;
        char* _B168;
        char* _B176;
        char* _B184;
        char* _B192;

        qh_pool_t* pool;

        SArray(qh_pool_t* pool) {
            memset(this, 0, sizeof(SArray));
            this->pool = pool;
        }

        ~SArray() {
            freemem();
        }

        void allocmem() {
            int len = 8;
            _B16 = (char*)qh_palloc(pool, (len = len + 8));
            _B24 = (char*)qh_palloc(pool, (len = len + 8));
            _B32 = (char*)qh_palloc(pool, (len = len + 8));
            _B40 = (char*)qh_palloc(pool, (len = len + 8));
            _B48 = (char*)qh_palloc(pool, (len = len + 8));
            _B56 = (char*)qh_palloc(pool, (len = len + 8));
            _B64 = (char*)qh_palloc(pool, (len = len + 8));
            _B72 = (char*)qh_palloc(pool, (len = len + 8));
            _B80 = (char*)qh_palloc(pool, (len = len + 8));
            _B88 = (char*)qh_palloc(pool, (len = len + 8));
            _B96 = (char*)qh_palloc(pool, (len = len + 8));
            _B114 = (char*)qh_palloc(pool, (len = len + 8));
            _B128 = (char*)qh_palloc(pool, (len = len + 8));
            _B136 = (char*)qh_palloc(pool, (len = len + 8));
            _B144 = (char*)qh_palloc(pool, (len = len + 8));
            _B152 = (char*)qh_palloc(pool, (len = len + 8));
            _B160 = (char*)qh_palloc(pool, (len = len + 8));
            _B168 = (char*)qh_palloc(pool, (len = len + 8));
            _B176 = (char*)qh_palloc(pool, (len = len + 8));
            _B184 = (char*)qh_palloc(pool, (len = len + 8));
            _B192 = (char*)qh_palloc(pool, (len = len + 8));
        }

        void setvalue() {
            int len = 16;
            char a = 'A';
            memset(_B16, a, len); len = len + 8; a++;
            memset(_B24, a, len); len = len + 8; a++;
            memset(_B32, a, len); len = len + 8; a++;
            memset(_B40, a, len); len = len + 8; a++;
            memset(_B48, a, len); len = len + 8; a++;
            memset(_B56, a, len); len = len + 8; a++;
            memset(_B64, a, len); len = len + 8; a++;
            memset(_B72, a, len); len = len + 8; a++;
            memset(_B80, a, len); len = len + 8; a++;
            memset(_B88, a, len); len = len + 8; a++;
            memset(_B96, a, len); len = len + 8; a++;
            memset(_B114, a, len); len = len + 8; a++;
            memset(_B128, a, len); len = len + 8; a++;
            memset(_B136, a, len); len = len + 8; a++;
            memset(_B144, a, len); len = len + 8; a++;
            memset(_B152, a, len); len = len + 8; a++;
            memset(_B160, a, len); len = len + 8; a++;
            memset(_B168, a, len); len = len + 8; a++;
            memset(_B176, a, len); len = len + 8; a++;
            memset(_B184, a, len); len = len + 8; a++;
            memset(_B192, a, len); len = len + 8; a++;
        }

        void checkvalue() {
            int len = 16;
            char a = _B16[0];
            assert(assertto(_B16, len, a)); len = len + 8; a++;
            assert(assertto(_B24, len, a)); len = len + 8; a++;
            assert(assertto(_B32, len, a)); len = len + 8; a++;
            assert(assertto(_B40, len, a)); len = len + 8; a++;
            assert(assertto(_B48, len, a)); len = len + 8; a++;
            assert(assertto(_B56, len, a)); len = len + 8; a++;
            assert(assertto(_B64, len, a)); len = len + 8; a++;
            assert(assertto(_B72, len, a)); len = len + 8; a++;
            assert(assertto(_B80, len, a)); len = len + 8; a++;
            assert(assertto(_B88, len, a)); len = len + 8; a++;
            assert(assertto(_B96, len, a)); len = len + 8; a++;
            assert(assertto(_B114, len, a)); len = len + 8; a++;
            assert(assertto(_B128, len, a)); len = len + 8; a++;
            assert(assertto(_B136, len, a)); len = len + 8; a++;
            assert(assertto(_B144, len, a)); len = len + 8; a++;
            assert(assertto(_B152, len, a)); len = len + 8; a++;
            assert(assertto(_B160, len, a)); len = len + 8; a++;
            assert(assertto(_B168, len, a)); len = len + 8; a++;
            assert(assertto(_B176, len, a)); len = len + 8; a++;
            assert(assertto(_B184, len, a)); len = len + 8; a++;
            assert(assertto(_B192, len, a)); len = len + 8; a++;
        }

        void freemem() {
            qh_pfree(pool, _B16);   _B16 = NULL;
            qh_pfree(pool, _B24);   _B24 = NULL;
            qh_pfree(pool, _B32);   _B32 = NULL;
            qh_pfree(pool, _B40);   _B40 = NULL;
            qh_pfree(pool, _B48);   _B48 = NULL;
            qh_pfree(pool, _B56);   _B56 = NULL;
            qh_pfree(pool, _B64);   _B64 = NULL;
            qh_pfree(pool, _B72);   _B72 = NULL;
            qh_pfree(pool, _B80);   _B80 = NULL;
            qh_pfree(pool, _B88);   _B88 = NULL;
            qh_pfree(pool, _B96);   _B96 = NULL;
            qh_pfree(pool, _B114);   _B114 = NULL;
            qh_pfree(pool, _B128);   _B128 = NULL;
            qh_pfree(pool, _B136);   _B136 = NULL;
            qh_pfree(pool, _B144);   _B144 = NULL;
            qh_pfree(pool, _B152);   _B152 = NULL;
            qh_pfree(pool, _B160);   _B160 = NULL;
            qh_pfree(pool, _B168);   _B168 = NULL;
            qh_pfree(pool, _B176);   _B176 = NULL;
            qh_pfree(pool, _B184);   _B184 = NULL;
            qh_pfree(pool, _B192);   _B192 = NULL;
        }

        static bool assertto(const char* array, int arraysize, int content) {
            for (int j = 0; j < arraysize; ++j) {
                if (array[j] != content) {
                    return false;
                }
            }

            return true;
        }
    };


    void qh_pool_test_1() {
        {
            qh_pool_t* pool = qh_create_pool(1024 * 1024, 128); //1M pool
            qh_destroy_pool(pool);
        }
    }


    void qh_pool_test_2() {
        qh_pool_t* pool = qh_create_pool(1024, 128); //1M pool
        for (int i = 1; i < 10000; i++) {
            int len = i * 2;
            void* p1 = qh_palloc(pool, len);
            memset(p1, 'A', len);
            assert(SArray::assertto((char*)p1, len, 'A'));
            qh_pfree(pool, p1);

            void* p2 = qh_palloc(pool, len);
            memset(p2, 'B', len);
            assert(SArray::assertto((char*)p2, len, 'B'));
            qh_pfree(pool, p2);
        }
        qh_destroy_pool(pool);
    }

    void qh_pool_test_3() {
        qh_pool_t* pool = qh_create_pool(1024 * 1024, 32); //1M pool

        for (int i = 0; i < 20; ++i) {
            SArray sa(pool);
            sa.allocmem();
            sa.setvalue();
            sa.checkvalue();
            sa.freemem();
        }

        qh_destroy_pool(pool);
    }
}

TEST_UNIT(qh_pool) {
    qh_pool_test_1();
    qh_pool_test_2();
    qh_pool_test_3();
}
