/* Copyright 2015 The math21 Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#pragma once

#include "inner.h"

namespace math21 {

    struct SpaceParas {
        char *address;//new and delete address.
        char *start;//available space start position.
        NumN *ref_count;
        NumN size;//size space is available.
        NumN unit;//unit size in byte, char
        SpaceParas() {
            address = 0;
            start = 0;
            ref_count = 0;
            size = 0;
            unit = 0;
        }

        virtual ~SpaceParas() {
        }
    };

    void m21log(const SpaceParas &paras);

    void m21log(std::ostream &io, const SpaceParas &paras);

    void math21_memory_getSpace(const SpaceParas &src, SpaceParas &dst, NumN offset, NumN size, NumN unit);

    /*
     * 1. setSize: allocate or set space.
     * 2.
     * */
    // space_unit is char.
    struct AutoBuffer {
    private:
//        SpaceParas paras;
        char *space_address; // address of all allocated buffer.
        char *space_start; // start address given to this buffer.
        NumN space_size; // space_size is the size given to this buffer, not all allocated buffer size.
        NumN nn;// size used by this buffer, nn <= space_size, but the two values often equal to each other.
        NumN *refcount;// when matrix points to user-allocated data, the pointer is NULL

        /////////////
        std::string name;

        void init();

        void deallocate();

        //Todo: clean
        void allocate();

        void addref();

        NumB setSpace(const SpaceParas &paras);

        void dataCopy(const void *data);

    public:

        //Construct empty matrix.
        AutoBuffer();

        virtual ~AutoBuffer();

        virtual SpaceParas getSpace() const;

        // unit is input unit, but char is used as unit inside the buffer.
        virtual SpaceParas getSpace(NumN offset, NumN n, NumN unit = sizeof(char)) const;

        int setDataDeep(const AutoBuffer &autoBuffer);

        /*
         * The step which call setDataDeep will be efficient than
         * the steps which call ensureIndependence, then call setDataDeep.
         * So, don't use this if necessary.
         * */
        void ensureIndependence();

        //Todo: copy all headers including start position, end position...
        int setDataShallow(const AutoBuffer &autoBuffer);

        void *getObj() const;

        NumN *getRefCount() const;

        NumN size() const;

        void clear();

        //return false if in share mode, or data is set by unknowns.
        NumB isIndependent();

        //Construct n*m matrix with element set to zero using xjcalloc. Fast.
        //must set to zero. Other functions rely on this.
        // data is not kept.
        void setSize(NumN n, const SpaceParas *paras = 0);

        //only for basic types.
//        void zeros();

        void setName(const char *_name);

        void log(const char *name = 0) const;

        void log(std::ostream &io, const char *name = 0) const;

        // check if data is null. m or n may not be 0 if data is null. So use this function to check instead of m and n.
        bool isEmpty() const;

        std::string getClassName() const;

        void swap(AutoBuffer &B);
    };
}