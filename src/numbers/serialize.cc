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

#include "inner.h"
#include "serialize.h"
#include "std_wrapper.h"

namespace math21 {

    void math21_io_serialize(std::ostream &out, const NumN &m, SerializeNumInterface &sn) {
        sn.serialize(out, m);
    }

    void math21_io_serialize(std::ostream &out, const NumZ &m, SerializeNumInterface &sn) {
        sn.serialize(out, m);
    }

    void math21_io_serialize(std::ostream &out, const NumR &m, SerializeNumInterface &sn) {
        sn.serialize(out, m);
    }

    void math21_io_serialize(std::ostream &out, const std::string &m, SerializeNumInterface &sn) {
        NumN n = m.size();
        math21_io_serialize(out, n, sn);
        out.write(&m[0], m.size());
    }

    void math21_io_deserialize(std::istream &in, NumN &m, DeserializeNumInterface &sn) {
        sn.deserialize(in, m);
    }

    void math21_io_deserialize(std::istream &in, NumZ &m, DeserializeNumInterface &sn) {
        sn.deserialize(in, m);
    }

    void math21_io_deserialize(std::istream &in, NumR &m, DeserializeNumInterface &sn) {
        sn.deserialize(in, m);
    }

    void math21_io_deserialize(std::istream &in, std::string &m, DeserializeNumInterface &sn) {
        NumN n;
        math21_io_deserialize(in, n, sn);
        math21_tool_std_string_resize(m, n);

        in.read(&m[0], m.size());
    }
}