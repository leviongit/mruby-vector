#include <math.h>
#include <mruby.h>
#include <mruby/boxing_word.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <mruby/string.h>
#include <mruby/value.h>

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
  struct RClass *vec2;
  struct RClass *vec3;
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

mrb_value mrb_vec2_make_new(mrb_state *mrb, mrb_value _) {
  mrb_float x = 0;
  mrb_float y = 0;

  mrb_get_args(mrb, "|ff", &x, &y);

  vec2 *v2 = vec2_init(mrb, x, y);
  mrb_value rv2 = mrb_vec2_wrap(mrb, clss.vec2, v2);

  return rv2;
}

mrb_value mrb_vec2_initialize(mrb_state *mrb, mrb_value self) {
  mrb_float x = 0;
  mrb_float y = 0;

  mrb_get_args(mrb, "|ff", &x, &y);

  vec2 *v2 = vec2_unwrap(self);
  v2->x = x;
  v2->y = y;

  return self;
}

mrb_value mrb_vec2_make_polar(mrb_state *mrb, mrb_value _) {
  mrb_float r = 0;
  mrb_float theta = 0;

  mrb_get_args(mrb, "|ff", &r, &theta);

  vec2 *v2 = vec2_init(mrb, r * cos(theta), r * sin(theta));
  mrb_value rv2 = mrb_vec2_wrap(mrb, clss.vec2, v2);

  return rv2;
}

vec3 *vec3_init(mrb_state *mrb, mrb_float x, mrb_float y, mrb_float z) {
  vec3 *vec = vec3_alloc(mrb);
  vec->x = x;
  vec->y = y;
  vec->z = z;
  return vec;
}

mrb_value mrb_vec3_make_new(mrb_state *mrb, mrb_value _) {
  mrb_float x = 0;
  mrb_float y = 0;
  mrb_float z = 0;

  mrb_get_args(mrb, "|fff", &x, &y, &z);

  vec3 *v3 = vec3_init(mrb, x, y, z);
  mrb_value rv3 = mrb_vec3_wrap(mrb, clss.vec3, v3);

  return rv3;
}

mrb_value mrb_vec3_initialize(mrb_state *mrb, mrb_value self) {
  mrb_float x = 0;
  mrb_float y = 0;
  mrb_float z = 0;

  mrb_get_args(mrb, "|fff", &x, &y, &z);

  vec3 *v3 = vec3_unwrap(self);
  v3->x = x;
  v3->y = y;
  v3->z = z;

  return self;
}

mrb_value mrb_vec3_make_polar(mrb_state *mrb, mrb_value _) {
  mrb_float rho = 0;
  mrb_float phi = 0;
  mrb_float theta = 0;

  mrb_get_args(mrb, "|fff", &rho, &phi, &theta);

  vec3 *v3 = vec3_init(mrb, rho * sin(phi) * cos(theta),
                       rho * sin(phi) * sin(theta), rho * cos(phi));
  mrb_value rv3 = mrb_vec3_wrap(mrb, clss.vec3, v3);

  return rv3;
}

mrb_value mrb_vec2_initialize_copy(mrb_state *mrb, mrb_value copy) {
  mrb_value src = mrb_get_arg1(mrb);

  if (mrb_obj_equal(mrb, copy, src))
    return copy;
  if (!mrb_obj_is_instance_of(mrb, src, mrb_obj_class(mrb, copy)))
    mrb_raisef(mrb, E_TYPE_ERROR, "Wrong argument class `%C` expected `%C`",
               mrb_obj_class(mrb, src), mrb_obj_class(mrb, copy));

  vec2 *vcp = vec2_unwrap(copy);
  if (!vcp) {
    vcp = vec2_alloc(mrb);
    mrb_data_init(copy, vcp, &mrb_vec2_type);
  }

  *vcp = *vec2_unwrap(src);
  return copy;
}

mrb_value mrb_vec3_initialize_copy(mrb_state *mrb, mrb_value copy) {

  mrb_value src = mrb_get_arg1(mrb);

  if (mrb_obj_equal(mrb, copy, src))
    return copy;
  if (!mrb_obj_is_instance_of(mrb, src, mrb_obj_class(mrb, copy)))
    mrb_raisef(mrb, E_TYPE_ERROR, "Wrong argument class `%C` expected `%C`",
               mrb_obj_class(mrb, src), mrb_obj_class(mrb, copy));

  vec3 *vcp = vec3_unwrap(copy);
  if (!vcp) {
    vcp = vec3_alloc(mrb);
    mrb_data_init(copy, vcp, &mrb_vec2_type);
  }

  *vcp = *vec3_unwrap(src);
  return copy;
}

mrb_value mrb_vec2_x(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec2_unwrap(self)->x);
}

mrb_value mrb_vec2_set_x(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec2_unwrap(self)->x =
                                  mrb_as_float(mrb, mrb_get_arg1(mrb)));
}

mrb_value mrb_vec2_y(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec2_unwrap(self)->y);
}

mrb_value mrb_vec2_set_y(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec2_unwrap(self)->y =
                                  mrb_as_float(mrb, mrb_get_arg1(mrb)));
}

mrb_value mrb_vec2_sq_mag(mrb_state *mrb, mrb_value self) {
  vec2 *v2 = vec2_unwrap(self);
  return mrb_float_value(mrb,v2->x * v2->x + v2->y * v2->y);
}

mrb_value mrb_vec2_mag(mrb_state *mrb, mrb_value self) {
  vec2 *v2 = vec2_unwrap(self);
  return mrb_float_value(mrb, sqrt(v2->x * v2->x + v2->y * v2->y));
}

mrb_value mrb_vec3_x(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec3_unwrap(self)->x);
}

mrb_value mrb_vec3_set_x(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec3_unwrap(self)->x =
                                  mrb_as_float(mrb, mrb_get_arg1(mrb)));
}

mrb_value mrb_vec3_y(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec3_unwrap(self)->y);
}

mrb_value mrb_vec3_set_y(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec3_unwrap(self)->y =
                                  mrb_as_float(mrb, mrb_get_arg1(mrb)));
}

mrb_value mrb_vec3_z(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec3_unwrap(self)->z);
}

mrb_value mrb_vec3_set_z(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, vec3_unwrap(self)->z =
                                  mrb_as_float(mrb, mrb_get_arg1(mrb)));
}

mrb_value mrb_vec3_sq_mag(mrb_state *mrb, mrb_value self) {
  vec3 *v3 = vec3_unwrap(self);
  return mrb_float_value(mrb,v3->x * v3->x + v3->y * v3->y + v3->z * v3->z);
}

mrb_value mrb_vec3_mag(mrb_state *mrb, mrb_value self) {
  vec3 *v3 = vec3_unwrap(self);
  return mrb_float_value(mrb, sqrt(v3->x * v3->x + v3->y * v3->y + v3->z * v3->z));
}

mrb_value mrb_vec2_add(mrb_state *mrb, mrb_value self) {
  vec2 *vec = vec2_unwrap(self);

  mrb_value arg = mrb_get_arg1(mrb);

  vec2 *new = vec2_init(mrb, 0, 0);
  mrb_value new_vec = mrb_vec2_wrap(mrb, mrb_obj_class(mrb, self), new);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    new->x = vec->x + other;
    new->y = vec->y + other;
  } else if (mrb_obj_is_kind_of(mrb, arg, mrb_obj_class(mrb, self))) {
    vec2 *other = vec2_unwrap(arg);
    new->x = vec->x + other->x;
    new->y = vec->y + other->y;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is neither a `Numeric` nor a `Vec2`",
               mrb_obj_class(mrb, arg));
  }

  return new_vec;
}

mrb_value mrb_vec2_add_b(mrb_state *mrb, mrb_value self) {
  vec2 *vec = vec2_unwrap(self);
  mrb_value arg = mrb_get_arg1(mrb);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    vec->x += other;
    vec->y += other;
  } else if (mrb_obj_is_kind_of(mrb, arg, mrb_obj_class(mrb, self))) {
    vec2 *other = vec2_unwrap(arg);
    vec->x += other->x;
    vec->y += other->y;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is neither a `Numeric` nor a `Vec2`",
               mrb_obj_class(mrb, arg));
  }

  return self;
}

mrb_value mrb_vec2_sub(mrb_state *mrb, mrb_value self) {
  vec2 *vec = vec2_unwrap(self);

  mrb_value arg = mrb_get_arg1(mrb);

  vec2 *new = vec2_init(mrb, 0, 0);
  mrb_value new_vec = mrb_vec2_wrap(mrb, mrb_obj_class(mrb, self), new);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    new->x = vec->x - other;
    new->y = vec->y - other;
  } else if (mrb_obj_is_kind_of(mrb, arg, mrb_obj_class(mrb, self))) {
    vec2 *other = vec2_unwrap(arg);
    new->x = vec->x - other->x;
    new->y = vec->y - other->y;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is neither a `Numeric` nor a `Vec2`",
               mrb_obj_class(mrb, arg));
  }

  return new_vec;
}

mrb_value mrb_vec2_sub_b(mrb_state *mrb, mrb_value self) {
  vec2 *vec = vec2_unwrap(self);
  mrb_value arg = mrb_get_arg1(mrb);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    vec->x -= other;
    vec->y -= other;
  } else if (mrb_obj_is_kind_of(mrb, arg, mrb_obj_class(mrb, self))) {
    vec2 *other = vec2_unwrap(arg);
    vec->x -= other->x;
    vec->y -= other->y;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is neither a `Numeric` nor a `Vec2`",
               mrb_obj_class(mrb, arg));
  }

  return self;
}

mrb_value mrb_vec2_mul(mrb_state *mrb, mrb_value self) {
  vec2 *vec = vec2_unwrap(self);

  mrb_value arg = mrb_get_arg1(mrb);

  vec2 *new = vec2_init(mrb, 0, 0);
  mrb_value new_vec = mrb_vec2_wrap(mrb, mrb_obj_class(mrb, self), new);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    new->x = vec->x *other;
    new->y = vec->y *other;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is not a `Numeric`",
               mrb_obj_class(mrb, arg));
  }

  return new_vec;
}

mrb_value mrb_vec2_mul_b(mrb_state *mrb, mrb_value self) {
  vec2 *vec = vec2_unwrap(self);
  mrb_value arg = mrb_get_arg1(mrb);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    vec->x *= other;
    vec->y *= other;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is not a `Numeric`",
               mrb_obj_class(mrb, arg));
  }

  return self;
}

mrb_value mrb_vec2_div(mrb_state *mrb, mrb_value self) {
  vec2 *vec = vec2_unwrap(self);

  mrb_value arg = mrb_get_arg1(mrb);

  vec2 *new = vec2_init(mrb, 0, 0);
  mrb_value new_vec = mrb_vec2_wrap(mrb, mrb_obj_class(mrb, self), new);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    new->x = vec->x / other;
    new->y = vec->y / other;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is not a `Numeric`",
               mrb_obj_class(mrb, arg));
  }

  return new_vec;
}

mrb_value mrb_vec2_div_b(mrb_state *mrb, mrb_value self) {
  vec2 *vec = vec2_unwrap(self);
  mrb_value arg = mrb_get_arg1(mrb);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    vec->x /= other;
    vec->y /= other;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is not a `Numeric`",
               mrb_obj_class(mrb, arg));
  }

  return self;
}

mrb_value mrb_vec2_to_v2(mrb_state *mrb, mrb_value self) { return self; }

mrb_value mrb_vec2_to_v3(mrb_state *mrb, mrb_value self) {
  vec2 *vec = vec2_unwrap(self);

  vec3 *v3 = vec3_init(mrb, 0, 0, 0);
  mrb_value rv3 = mrb_vec3_wrap(mrb, clss.vec3, v3);

  *((vec2 *)v3) = *vec;
  return rv3;
}

mrb_value mrb_vec3_add(mrb_state *mrb, mrb_value self) {
  vec3 *vec = vec3_unwrap(self);

  mrb_value arg = mrb_get_arg1(mrb);

  vec3 *new = vec3_init(mrb, 0, 0, 0);
  mrb_value new_vec = mrb_vec3_wrap(mrb, mrb_obj_class(mrb, self), new);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    new->x = vec->x + other;
    new->y = vec->y + other;
    new->z = vec->z + other;
  } else if (mrb_obj_is_kind_of(mrb, arg, mrb_obj_class(mrb, self))) {
    vec3 *other = vec3_unwrap(arg);
    new->x = vec->x + other->x;
    new->y = vec->y + other->y;
    new->z = vec->z + other->z;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is neither a `Numeric` nor a `Vec3`",
               mrb_obj_class(mrb, arg));
  }

  return new_vec;
}

mrb_value mrb_vec3_add_b(mrb_state *mrb, mrb_value self) {
  vec3 *vec = vec3_unwrap(self);
  mrb_value arg = mrb_get_arg1(mrb);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    vec->x += other;
    vec->y += other;
    vec->z += other;
  } else if (mrb_obj_is_kind_of(mrb, arg, mrb_obj_class(mrb, self))) {
    vec3 *other = vec3_unwrap(arg);
    vec->x += other->x;
    vec->y += other->y;
    vec->z += other->z;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is neither a `Numeric` nor a `Vec3`",
               mrb_obj_class(mrb, arg));
  }

  return self;
}

mrb_value mrb_vec3_sub(mrb_state *mrb, mrb_value self) {
  vec3 *vec = vec3_unwrap(self);

  mrb_value arg = mrb_get_arg1(mrb);

  vec3 *new = vec3_init(mrb, 0, 0, 0);
  mrb_value new_vec = mrb_vec3_wrap(mrb, mrb_obj_class(mrb, self), new);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    new->x = vec->x - other;
    new->y = vec->y - other;
    new->z = vec->z - other;
  } else if (mrb_obj_is_kind_of(mrb, arg, mrb_obj_class(mrb, self))) {
    vec3 *other = vec3_unwrap(arg);
    new->x = vec->x - other->x;
    new->y = vec->y - other->y;
    new->z = vec->z - other->z;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is neither a `Numeric` nor a `Vec3`",
               mrb_obj_class(mrb, arg));
  }

  return new_vec;
}

mrb_value mrb_vec3_sub_b(mrb_state *mrb, mrb_value self) {
  vec3 *vec = vec3_unwrap(self);
  mrb_value arg = mrb_get_arg1(mrb);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    vec->x -= other;
    vec->y -= other;
    vec->z -= other;
  } else if (mrb_obj_is_kind_of(mrb, arg, mrb_obj_class(mrb, self))) {
    vec3 *other = vec3_unwrap(arg);
    vec->x -= other->x;
    vec->y -= other->y;
    vec->z -= other->z;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is neither a `Numeric` nor a `Vec3`",
               mrb_obj_class(mrb, arg));
  }

  return self;
}

mrb_value mrb_vec3_mul(mrb_state *mrb, mrb_value self) {
  vec3 *vec = vec3_unwrap(self);

  mrb_value arg = mrb_get_arg1(mrb);

  vec3 *new = vec3_init(mrb, 0, 0, 0);
  mrb_value new_vec = mrb_vec3_wrap(mrb, mrb_obj_class(mrb, self), new);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    new->x = vec->x *other;
    new->y = vec->y *other;
    new->z = vec->z *other;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is not a `Numeric`",
               mrb_obj_class(mrb, arg));
  }

  return new_vec;
}

mrb_value mrb_vec3_mul_b(mrb_state *mrb, mrb_value self) {
  vec3 *vec = vec3_unwrap(self);
  mrb_value arg = mrb_get_arg1(mrb);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    vec->x *= other;
    vec->y *= other;
    vec->z *= other;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is not a `Numeric`",
               mrb_obj_class(mrb, arg));
  }

  return self;
}

mrb_value mrb_vec3_div(mrb_state *mrb, mrb_value self) {
  vec3 *vec = vec3_unwrap(self);

  mrb_value arg = mrb_get_arg1(mrb);

  vec3 *new = vec3_init(mrb, 0, 0, 0);
  mrb_value new_vec = mrb_vec3_wrap(mrb, mrb_obj_class(mrb, self), new);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    new->x = vec->x / other;
    new->y = vec->y / other;
    new->z = vec->z / other;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is not a `Numeric`",
               mrb_obj_class(mrb, arg));
  }

  return new_vec;
}

mrb_value mrb_vec3_div_b(mrb_state *mrb, mrb_value self) {
  vec3 *vec = vec3_unwrap(self);
  mrb_value arg = mrb_get_arg1(mrb);

  if (mrb_obj_is_kind_of(mrb, arg, clss.numeric)) {
    mrb_float other = mrb_as_float(mrb, arg);
    vec->x /= other;
    vec->y /= other;
    vec->z /= other;
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "%C is not a `Numeric`",
               mrb_obj_class(mrb, arg));
  }

  return self;
}

mrb_value mrb_vec3_to_v2(mrb_state *mrb, mrb_value self) {
  vec3 *vec = vec3_unwrap(self);

  vec2 *v2 = vec2_init(mrb, 0, 0);
  mrb_value rv2 = mrb_vec2_wrap(mrb, clss.vec2, v2);
  *v2 = *((vec2 *)vec);

  return rv2;
}

mrb_value mrb_vec3_to_v3(mrb_state *mrb, mrb_value self) { return self; }

void mrb_mruby_vector_gem_init(mrb_state *mrb) {
  clss.numeric = mrb_class_get(mrb, "Numeric");

  struct RClass *vec2_c = mrb_define_class(mrb, "Vec2", mrb->object_class);
  clss.vec2 = vec2_c;

  mrb_undef_class_method(mrb, vec2_c, "allocate");
  mrb_define_class_method(mrb, vec2_c, "[]", mrb_vec2_make_new,
                          MRB_ARGS_OPT(2));
  mrb_define_class_method(mrb, vec2_c, "new", mrb_vec2_make_new,
                          MRB_ARGS_OPT(2));
  mrb_define_class_method(mrb, vec2_c, "polar", mrb_vec2_make_polar,
                          MRB_ARGS_OPT(2));
  mrb_define_method(mrb, vec2_c, "initialize_copy", mrb_vec2_initialize_copy,
                    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2_c, "x", mrb_vec2_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec2_c, "x=", mrb_vec2_set_x, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2_c, "y", mrb_vec2_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec2_c, "y=", mrb_vec2_set_y, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2_c, "add", mrb_vec2_add, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2_c, "add!", mrb_vec2_add_b, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2_c, "sub", mrb_vec2_sub, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2_c, "sub!", mrb_vec2_sub_b, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2_c, "mul", mrb_vec2_mul, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2_c, "mul!", mrb_vec2_mul_b, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2_c, "div", mrb_vec2_div, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2_c, "div!", mrb_vec2_div_b, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2_c, "to_v2", mrb_vec2_to_v2, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec2_c, "to_v3", mrb_vec2_to_v3, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec2_c, "sq_mag", mrb_vec2_sq_mag, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec2_c, "mag", mrb_vec2_mag, MRB_ARGS_NONE());

  struct RClass *vec3_c = mrb_define_class(mrb, "Vec3", mrb->object_class);
  clss.vec3 = vec3_c;

  mrb_undef_class_method(mrb, vec3_c, "allocate");
  mrb_define_class_method(mrb, vec3_c, "[]", mrb_vec3_make_new,
                          MRB_ARGS_OPT(3));
  mrb_define_class_method(mrb, vec3_c, "new", mrb_vec3_make_new,
                          MRB_ARGS_OPT(3));
  mrb_define_class_method(mrb, vec3_c, "polar", mrb_vec3_make_polar,
                          MRB_ARGS_OPT(3));
  mrb_define_method(mrb, vec3_c, "initialize_copy", mrb_vec3_initialize_copy,
                    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec3_c, "x", mrb_vec3_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec3_c, "x=", mrb_vec3_set_x, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec3_c, "y", mrb_vec3_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec3_c, "y=", mrb_vec3_set_y, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec3_c, "z", mrb_vec3_z, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec3_c, "z=", mrb_vec3_set_z, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec3_c, "add", mrb_vec3_add, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec3_c, "add!", mrb_vec3_add_b, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec3_c, "sub", mrb_vec3_sub, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec3_c, "sub!", mrb_vec3_sub_b, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec3_c, "mul", mrb_vec3_mul, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec3_c, "mul!", mrb_vec3_mul_b, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec3_c, "div", mrb_vec3_div, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec3_c, "div!", mrb_vec3_div_b, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec3_c, "to_v2", mrb_vec3_to_v2, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec3_c, "to_v3", mrb_vec3_to_v3, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec3_c, "sq_mag", mrb_vec3_sq_mag, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec3_c, "mag", mrb_vec3_mag, MRB_ARGS_NONE());

  // global func decls
  mrb_define_method(mrb, mrb->kernel_module, "vec2", mrb_vec2_make_new,
                    MRB_ARGS_OPT(2));
  mrb_define_method(mrb, mrb->kernel_module, "vec3", mrb_vec3_make_new,
                    MRB_ARGS_OPT(3));
}

void mrb_mruby_vector_gem_final(mrb_state *mrb) {}
