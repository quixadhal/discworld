#include "/include/virtual.h"

object compile_object(string path) {
  if (file_size(path) > 0)
    return (object)SERVER->create_virtual_object(path);
} /* compile_object() */
