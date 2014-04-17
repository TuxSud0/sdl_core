/* Copyright (c) 2014, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "cppgen/struct_type_report_erros_method.h"

#include "model/composite_type.h"
#include "utils/safeformat.h"
#include "utils/string_utils.h"

using typesafe_format::strmfmt;

namespace codegen {

StructTypeReportErrosMethod::StructTypeReportErrosMethod(const Struct* strct)
  : CppFunction(strct->name(), "ReportErrors", "void", kConst),
    strct_(strct) {
  Add(Parameter("report__", "rpc::ValidationReport*"));
}

void StructTypeReportErrosMethod::DefineBody(std::ostream* os) const {
  *os << "if (is_empty()) {\n";
  {
    Indent ind(*os);
    strmfmt(*os, "rpc::CompositeType::ReportErrors({0});\n", parameters_[0].name);
  }
  *os << "} else {\n";
  {
    Indent ind(*os);
    strmfmt(*os, "{0}->set_validation_info(\"structure initialized\");\n", parameters_[0].name);
    const Struct::FieldsList& fields = strct_->fields();
    for (size_t i = 0; i != fields.size(); ++i) {
      const Struct::Field& field = fields[i];
      strmfmt(*os, "if (!{0}.is_valid()) {\n", field.name());
      {
        Indent ind(*os);
        strmfmt(*os, "{0}.ReportErrors(&{1}->ReportSubobject(\"{0}\"));\n",
                field.name(),
                parameters_[0].name);
      }
      *os << "}\n";
    }
  }
  *os << "}\n";
}

} // namespace cppgen