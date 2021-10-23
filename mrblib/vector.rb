class Vec2
  def to_s() = "Vec2[#{x}, #{y}]"

  def to_a() = [x, y]

  def to_h() = { x: x, y: y }

  alias inspect to_s
end
