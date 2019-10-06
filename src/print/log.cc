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

#include "log.h"
#include "../variables/files.h"

namespace math21 {
    void math21_print(std::ostream &io, const NumN &t, const char *s) {
        io << t << std::endl;
    }

    void math21_print(std::ostream &io, const NumZ &t, const char *s) {
        io << t << std::endl;
    }

    void math21_print(std::ostream &io, const NumR &t, const char *s) {
        io << t << std::endl;
    }

    void math21_print(std::ostream &io, const int &t, const char *s) {
        io << t << std::endl;
    }

    void math21_print(std::ostream &io, const char &t, const char *s) {
        io << t << std::endl;
    }

    void math21_print(std::ostream &io, const char *t, const char *s) {
        io << t << std::endl;
    }

    void math21_print(std::ostream &io, const std::string &t, const char *s) {
        io << t << std::endl;
    }

    // we don't use name s.
    void math21_print(std::ostream &io, const ad::Variable *v, const char *s) {
        v->log(io);
    }

    // we don't use name s.
    void math21_print(std::ostream &io, ad::Variable *v, const char *s) {
        v->log(io);
    }

    void m21debug(char c) {
        m21logRepeat(c);
    }

    void m21input_getline(std::string s) {
        getline(std::cin, s);
    }

    void m21pause_zzz(const char *s) {
        if (s) {
            m21log(s);
        }
        char b;
        m21input(b);
        std::string str;
        m21input_getline(str);
    }

    void m21pause_pressEnter(const char *s) {
        if (s) {
            m21log(s);
        }
        getchar();
        std::string str;
        m21input_getline(str);
    }

    void math21_tool_exit(const char *s) {
        if (s) {
            m21log(s);
        }
        exit(EXIT_SUCCESS);
    }

    void math21_tool_log_title(const char *name) {
        if (name != 0) {
            std::cout << "######### " << name << " #########" << std::endl;
        } else {
            std::cout << "##################" << std::endl;
        }
    }


}