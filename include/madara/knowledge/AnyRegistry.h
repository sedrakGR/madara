#include "Any.h"

#ifndef MADARA_KNOWLEDGE_ANY_REGISTRY_H_
#define MADARA_KNOWLEDGE_ANY_REGISTRY_H_

/**
 * @file AnyRegistry.h
 * @author David Kyle <david.kyle@shield.ai>
 *
 * This file contains the AnyRegistry class
 **/

#include <memory>
#include <map>
#include <functional>
#include <type_traits>

#include "madara/utility/SupportTest.h"
#include "madara/MadaraExport.h"
#include "madara/utility/StdInt.h"
#include "madara/logger/GlobalLogger.h"
#include "madara/utility/IntTypes.h"
#include "madara/exceptions/BadAnyAccess.h"

namespace capnp {
  class StructSchema;
}

namespace madara { namespace knowledge {

class Any;
class ConstAny;
struct TypeHandlers;

struct compare_const_char_ptr
{
  bool operator()(const char *l, const char *r) const {
    return std::strcmp(l, r) < 0;
  }
};

class AnyRegistry
{
public:
  template<typename T>
  static void register_type(const char *name);

  static void register_schema(const char *name,
      const capnp::StructSchema &schema);

  template<typename T>
  static const char * const &get_type_name()
  {
    return *get_type_name_ptr<T>();
  }

  static Any construct(const char *name);
  static ConstAny construct_const(const char *name);

protected:
  static void register_type_impl(const char *name,
      const TypeHandlers *handler);

  template<typename T>
  static const char **get_type_name_ptr()
  {
    static const char *type_name = nullptr;
    return &type_name;
  }
};

#define MADARA_ANY_REGISTER_TYPE(type) \
  ::madara::knowledge::Any::register_type<type>(#type)

} }

#endif  // MADARA_KNOWLEDGE_ANY_REGISTRY_H_
