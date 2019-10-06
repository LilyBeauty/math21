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
#include "number.h"
#include "_assert.h"

namespace math21 {
    class SerializeNumInterface {
    public:
        SerializeNumInterface() {}

        virtual void serialize(std::ostream &out, const NumN &m) = 0;

        virtual void serialize(std::ostream &out, const NumZ &m) = 0;

        virtual void serialize(std::ostream &out, const NumR &m) = 0;
    };

    class DeserializeNumInterface {
    public:
        DeserializeNumInterface() {}

        virtual void deserialize(std::istream &in, NumN &m) = 0;

        virtual void deserialize(std::istream &in, NumZ &m) = 0;

        virtual void deserialize(std::istream &in, NumR &m) = 0;
    };

    class SerializeNumInterface_simple : public SerializeNumInterface {
    public:
        SerializeNumInterface_simple() {}

        void serialize(std::ostream &out, const NumN &m) {
            out.write((const char *) &m, sizeof(m));
        }

        void serialize(std::ostream &out, const NumZ &m) {
            out.write((const char *) &m, sizeof(m));
        }

        void serialize(std::ostream &out, const NumR &m) {
            out.write((const char *) &m, sizeof(m));
        }
    };

    class DeserializeNumInterface_simple : public DeserializeNumInterface {
    public:
        DeserializeNumInterface_simple() {}

        void deserialize(std::istream &in, NumN &m) {
            in.read((char *) &m, sizeof(m));
        }

        void deserialize(std::istream &in, NumZ &m) {
            in.read((char *) &m, sizeof(m));
        }

        void deserialize(std::istream &in, NumR &m) {
            in.read((char *) &m, sizeof(m));
        }
    };

    void math21_io_serialize(std::ostream &out, const NumN &m, SerializeNumInterface &sn);

    void math21_io_serialize(std::ostream &out, const NumZ &m, SerializeNumInterface &sn);

    void math21_io_serialize(std::ostream &out, const NumR &m, SerializeNumInterface &sn);

    void math21_io_serialize(std::ostream &out, const std::string &m, SerializeNumInterface &sn);

    void math21_io_deserialize(std::istream &in, NumN &m, DeserializeNumInterface &sn);

    void math21_io_deserialize(std::istream &in, NumZ &m, DeserializeNumInterface &sn);

    void math21_io_deserialize(std::istream &in, NumR &m, DeserializeNumInterface &sn);

    void math21_io_deserialize(std::istream &in, std::string &m, DeserializeNumInterface &sn);

}