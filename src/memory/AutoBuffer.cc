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

#include "tool.h"
#include "AutoBuffer.h"

namespace math21 {

    void m21log(const SpaceParas &paras) {
        m21log(std::cout, paras);
    }

    void m21log(std::ostream &io, const SpaceParas &paras) {
#if defined(MATH21_DISABLE_LOG)
        return;
#endif
        io << "space paras:"
           << "\n\taddress: " << (void *) paras.address
           << "\n\tstart: " << (void *) paras.start
           << "\n\tref_count: " << (paras.ref_count == 0 ? (0) : (*paras.ref_count))
           << "\n\tsize: " << paras.size
           << "\n\tunit: " << paras.unit << " in byte (char)."
           << std::endl;
    }

    NumB AutoBuffer::setSpace(const SpaceParas &paras) {
        if (paras.address == 0) {
            return 0;
        }
        MATH21_ASSERT(paras.unit == sizeof(char),
                      "char has different size with space unit.");
        if ((char *) paras.address != space_address) {
            clear();
            space_address = (char *) paras.address;
            refcount = paras.ref_count;
            addref();
        }
        space_start = (char *) paras.start;
        space_size = paras.size;
        return 1;
    }

    void AutoBuffer::dataCopy(const void *data) {
        MATH21_ASSERT(!isEmpty() && data, "copy null data");
        math21_memory_memcpy(space_start, data, sizeof(char) * nn);
    }

    SpaceParas AutoBuffer::getSpace() const {
        SpaceParas paras;
        paras.address = (char *) space_address;
        paras.start = (char *) space_start;
        paras.ref_count = refcount;
        paras.size = space_size;
        paras.unit = sizeof(char);
        return paras;
    }

    void math21_memory_getSpace(const SpaceParas &src, SpaceParas &dst, NumN offset, NumN size, NumN unit) {
        MATH21_ASSERT(offset * unit + size * unit <= src.size)
        dst.address = (char *) src.address;
        dst.start = (char *) src.start + offset * unit;
        dst.ref_count = src.ref_count;
        dst.size = size * unit;
        dst.unit = src.unit;
    }

    SpaceParas AutoBuffer::getSpace(NumN offset, NumN size, NumN unit) const {
        MATH21_ASSERT(offset * unit + size * unit <= space_size)
        SpaceParas paras;
        paras.address = (char *) space_address;
        paras.start = (char *) space_start + offset * unit;
        paras.ref_count = refcount;
        paras.size = size * unit;
        paras.unit = sizeof(char);
        return paras;
    }

    void AutoBuffer::allocate() {
        if (space_address == 0) {
            math21_memory_malloc((void **) &space_address, sizeof(char) * nn);
//            xjmemset(space_address, 0, sizeof(char) * nn);

            MATH21_ASSERT((space_address != 0 && nn > 0) || (space_address == 0 && nn == 0));
            space_start = space_address;
            space_size = nn;
            if (space_address) {
                math21_memory_malloc((void **) &refcount, sizeof(*refcount));
                *refcount = 1;
            } else {
                refcount = 0;
            }
        }
    }


    void AutoBuffer::init() {
        space_address = 0;
        space_start = 0;
        space_size = 0;
        nn = 0;
        refcount = 0;
        name = "";
    }


    AutoBuffer::AutoBuffer() {
        init();
    }

    NumN AutoBuffer::size() const {
        return nn;
    }

    AutoBuffer::~AutoBuffer() {
        clear();
    }


    void AutoBuffer::deallocate() {
        math21_memory_free(space_address);
        math21_memory_free(refcount);
    }


    void AutoBuffer::setName(const char *_name) {
        name = _name;
    }


    void AutoBuffer::clear() {
        if (!isEmpty()) {
            if (refcount) {
//                m21log(name, *refcount);
                (*refcount)--;
                if (*refcount == 0) {
//                    m21log(name, "data deallocate");
                    deallocate();
                }
            }
            refcount = 0;
            nn = 0;
            space_address = 0;
            space_start = 0;
            space_size = 0;
        }
    }


    NumB AutoBuffer::isIndependent() {
        if (isEmpty() || (refcount && *refcount == 1)) {
            return 1;
        } else {
            return 0;
        }
    }

    // set size will set element zero.
    // Todo: remove xjmemset, don't clear to zero.
    void AutoBuffer::setSize(NumN n, const SpaceParas *paras) {
        if (paras == 0) {
            MATH21_ASSERT(isIndependent(), "call ensureIndependence() or clear() first!\n"
                    << "\tname: " << name)
            if (n == 0 || isEmpty() || !isIndependent()) {
                clear();
                nn = n;
                allocate();
            } else {
                if (n != nn) {
//                    xjrealloc((void**)&space_address, space_address, sizeof(char) * n);
//                    NumZ newStorage = n - nn;
//                    if (newStorage > 0)xjmemset(space_address + nn, 0, sizeof(char) * newStorage);
                    math21_memory_malloc((void **) &space_address, sizeof(char) * n);
//                    xjmemset(space_address, 0, sizeof(char) * n);
                    nn = n;
                    space_start = space_address;
                    space_size = nn;
                }
            }
        } else {
            setSpace(*paras);
            nn = n;
            MATH21_ASSERT(nn <= space_size, "space size mustn't be less than data size"
                    << "\n\trequired size: " << nn
                    << "\n\tgiven size: " << space_size);
        }
    }

    void AutoBuffer::addref() {
        if (refcount) {
            (*refcount)++;
        }
//        m21logDebug(*refcount);
    }


    void *AutoBuffer::getObj() const {
        return (void *) space_start;
    }


    NumN *AutoBuffer::getRefCount() const {
        return refcount;
    }

    int AutoBuffer::setDataShallow(const AutoBuffer &autoBuffer) {
        setSpace(autoBuffer.getSpace());
        nn = autoBuffer.size();
        return 1;
    }

    int AutoBuffer::setDataDeep(const AutoBuffer &autoBuffer) {
        if (size() != autoBuffer.size()) {
            setSize(autoBuffer.size());
        }
        dataCopy(autoBuffer.getObj());
        return 1;
    }

    void AutoBuffer::ensureIndependence() {
        if (!isEmpty() && !isIndependent()) {
            const void *data = space_start;
            NumN nn = size();
            clear();
            setSize(nn);
            dataCopy(data);
        }
    }


    bool AutoBuffer::isEmpty() const {
        if (this->space_address == 0) {
            MATH21_ASSERT(refcount == 0 && space_start == 0,
                          "error data is null but refcount isn't.");
            return 1;
        } else {
            MATH21_ASSERT((refcount == 0 || (refcount != 0 && *refcount > 0)),
                          "data is supposed to be not null, but"
                                  << "\n\trefcount is " << refcount
                                  << "\n\t*refcount is " << *refcount
                                  << "\n\tsize is " << size() << " (size is allowed not to be zero when is empty.)"
                                  << "\n\tgetClassName is " << getClassName());
            return 0;
        }
    }


    std::string AutoBuffer::getClassName() const {
        return "AutoBuffer";
    }

    void AutoBuffer::log(const char *s) const {
        log(std::cout, s);
    }

    void AutoBuffer::log(std::ostream &io, const char *s) const {
        if (s == 0) {
            s = name.c_str();
        }
        io << "AutoBuffer " << s << ":\n";
        m21log(getSpace());
    }

    void AutoBuffer::swap(AutoBuffer &B) {
        m21_swap(space_address, B.space_address);
        m21_swap(space_start, B.space_start);
        m21_swap(space_size, B.space_size);
        m21_swap(nn, B.nn);
        m21_swap(refcount, B.refcount);
    }

//    void AutoBuffer::zeros() {
//        if (!isEmpty()) {
//            xjmemset(space_start, 0, sizeof(char) * size());
//        }
//    }
}