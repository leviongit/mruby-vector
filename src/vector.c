#include <mruby.h>
#include <mruby/boxing_word.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/value.h>
#include <mruby/string.h>
#include <mruby/numeric.h>

typedef struct vec2 vec2;
typedef struct vec3 vec3;

struct vec2 {
  mrb_float x;
  mrb_float y;
};

struct vec3 {
  mrb_float x;
  mrb_float y;
  mrb_float z;
};

typedef struct {
  struct RClass *numeric;
} classes;

classes clss;

const mrb_data_type mrb_vec2_type = {"Vec2", mrb_free};
const mrb_data_type mrb_vec3_type = {"Vec3", mrb_free};

static mrb_value mrb_vec2_wrap(mrb_state *mrb, struct RClass *vc, vec2 *vec) {
  return mrb_obj_value(Data_Wrap_Struct(mrb, vc, &mrb_vec2_type, vec));
}

static mrb_value mrb_vec3_wrap(mrb_state *mrb, struct RClass *vc, vec3 *vec) {
  return mrb_obj_value(Data_Wrap_Struct(mrb, vc, &mrb_vec3_type, vec));
}

#define vec2_unwrap(self) ((vec2 *)DATA_PTR(self))

#define vec3_unwrap(self) ((vec3 *)DATA_PTR(self))

vec2 *vec2_alloc(mrb_state *mrb) {
  return (vec2 *)mrb_malloc(mrb, sizeof(vec2));
}

vec3 *vec3_alloc(mrb_state *mrb) {
  return (vec3 *)mrb_malloc(mrb, sizeof(vec3));
}

vec2 *vec2_init(mrb_state *mrb, mrb_float x, mrb_float y) {
  vec2 *vec = vec2_alloc(mrb);
  vec->x = x;
  vec->y = y;
  return vec;
}

mrb_value mrb_vec2_initialize(mrb_state *mrb, mrb_value self) {
  mrb_float x = 0;
  mrb_float y = 0;

  vec2 *vec;

  mrb_get_args(mrb, "|ff", &x, &y);

  vec = (vec2 *)DATA_PTR(self);
  
  if (vec) mrb_free(mrb, vec);
  
  mrb_data_init(self, NULL, &mrb_vec2_type);
  
  vec = vec2_init(mrb, x, y);
  
  mrb_data_init(self, vec, &mrb_vec2_type);
  
  return self;
}

vec3 *vec3_init(mrb_state *mrb, mrb_float x, mrb_float y, mrb_float z) {
  vec3 *vec = vec3_alloc(mrb);
  vec->x = x;
  vec->y = y;
  vec->z = z;
  return vec;
}

mrb_value mrb_vec3_initialize(mrb_state *mrb, mrb_value self) {
  mrb_float x = 0;
  mrb_float y = 0;
  mrb_float z = 0;

  vec3 *vec;

  mrb_get_args(mrb, "|fff", &x, &y, &z);

  vec = (vec3 *)DATA_PTR(self);
  
  if (vec) mrb_free(mrb, vec);

  mrb_data_init(self, NULL, &mrb_vec3_type);
  
  vec = vec3_init(mrb, x, y, z);

  mrb_data_init(self, vec, &mrb_vec3_type);
   
  return self;
}

mrb_value mrb_vec2_initialize_copy(mrb_state *mrb, mrb_value copy) {
  mrb_value src = mrb_get_arg1(mrb);
  vec2 *v1;
  vec2 *v2;

  if (mrb_obj_equal(mrb, copy, src)) return copy;
  if (!mrb_obj_is_instance_of(mrb, src, mrb_obj_class(mrb, copy))) mrb_raise(mrb, E_ARGUMENT_ERROR, "Wrong argument class");

  v1 = vec2_unwrap(copy);
  v2 = vec2_unwrap(src);

  if(!v2) mrb_raise(mrb, E_ARGUMENT_ERROR, "Uninitialized vec2");
  if(!v1) {
    v1 = vec2_alloc(mrb);
    mrb_data_init(copy, v1, &mrb_vec2_type);
  }

  *v1 = *v2;
  return copy;
}

mrb_value mrb_vec3_initialize_copy(mrb_state *mrb, mrb_value copy) {
  mrb_value src = mrb_get_arg1(mrb);
  vec3 *v1;
  vec3 *v2;

  if (mrb_obj_equal(mrb, copy, src)) return copy;
  if (!mrb_obj_is_instance_of(mrb, src, mrb_obj_class(mrb, copy))) mrb_raise(mrb, E_ARGUMENT_ERROR, "Wrong argument class");

  v1 = vec3_unwrap(copy);
  v2 = vec3_unwrap(src);

  if(!v2) mrb_raise(mrb, E_ARGUMENT_ERROR, "Uninitialized vec3");
  if(!v1) {
    v1 = vec3_alloc(mrb);
    mrb_data_init(copy, v1, &mrb_vec3_type);
  }

  *v1 = *v2;
  return copy;
}

mrb_value mrb_vec2_x(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec2_unwrap(self)->x);  
}

mrb_value mrb_vec2_y(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec2_unwrap(self)->y);
}

mrb_value mrb_vec3_x(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec3_unwrap(self)->x);
}

mrb_value mrb_vec3_y(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec3_unwrap(self)->y);
}

mrb_value mrb_vec3_z(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec3_unwrap(self)->z);
}

mrb_value mrb_vec2_add(mrb_state *mrb, mrb_value self) {
  vec2 *vec = vec2_unwrap(self);

  mrb_value arg = mrb_get_arg1(mrb);

  vec2 *new = vec2_init(mrb, 0, 0);
  mrb_value new_vec = mrb_vec2_wrap(mrb, mrb_obj_class(mrb, self), new);

  if(mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_float(arg);
    new->x = vec->x + other;
    new->y = vec->y + other;
  }
  else if(mrb_obj_is_kind_of(mrb, arg, mrb_obj_class(mrb, self))) {
    vec2 *other = vec2_unwrap(arg);
    new->x = vec->x + other->x;
    new->y = vec->y + other->y;
  }
  else { mrb_raisef(mrb, E_TYPE_ERROR, "%C is neither a `Numeric` nor a `Vec2`",
mrb_obj_class(mrb, arg)); }

  return new_vec;
}

mrb_value mrb_vec2_add_b(mrb_state *mrb, mrb_value self) {
  vec2 *vec = vec2_unwrap(self);
  mrb_value arg = mrb_get_arg1(mrb);

  if(mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_float(arg);
    vec->x += other;
    vec->y += other;
  }
  else if(mrb_obj_is_kind_of(mrb, arg, mrb_obj_class(mrb, self))) {
    vec2 *other = vec2_unwrap(arg);
    vec->x += other->x;
    vec->y += other->y;
  }
  else { mrb_raisef(mrb, E_TYPE_ERROR, "%C is neither a `Numeric` nor a `Vec2`", mrb_obj_class(mrb, arg)); }

  return self;
}

void mrb_mruby_vector_gem_init(mrb_state *mrb) { 
  clss.numeric = mrb_class_get(mrb, "Numeric");
  
  struct RClass *vec2_c = mrb_define_class(mrb, "Vec2", mrb->object_class);
  MRB_SET_INSTANCE_TT(vec2_c, MRB_TT_DATA);
  mrb_define_method(mrb, vec2_c, "initialize", mrb_vec2_initialize, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, vec2_c, "initialize_copy", mrb_vec2_initialize_copy, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2_c, "x", mrb_vec2_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec2_c, "y", mrb_vec2_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec2_c, "+", mrb_vec2_add, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2_c, "add!", mrb_vec2_add_b, MRB_ARGS_REQ(1));
  
  struct RClass *vec3_c = mrb_define_class(mrb, "Vec3", mrb->object_class);
  MRB_SET_INSTANCE_TT(vec3_c, MRB_TT_DATA);
  mrb_define_method(mrb, vec3_c, "initialize", mrb_vec3_initialize, MRB_ARGS_REQ(3));
  mrb_define_method(mrb, vec3_c, "initialize_copy", mrb_vec3_initialize_copy, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec3_c, "x", mrb_vec3_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec3_c, "y", mrb_vec3_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec3_c, "z", mrb_vec3_z, MRB_ARGS_NONE());
}

void mrb_mruby_vector_gem_final(mrb_state *mrb) {}
