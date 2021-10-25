class Vec2
  def to_s() = "Vec2[#{x}, #{y}]"

  def to_a() = [x, y]

  def to_h() = { x: x, y: y }

  alias inspect to_s
  alias + add
  alias - sub
  alias * mul
  alias / div
end

class Vec3
  def to_s() = "Vec3[#{x}, #{y}, #{z}]"

  def to_a() = [x, y, z]

  def to_h() = { x: x, y: y, z: z }

  alias inspect to_s
  alias + add
  alias - sub
  alias * mul
  alias / div
end
