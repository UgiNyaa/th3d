local PerfectFreeze = {}

PerfectFreeze.n = 500

-- smooth used for fighting against crowding in top and bottom of sphere
local function smooth(value)
  if value < 0.5 then
    return math.pow(2*value, 1/3) / 2
  else
    return (-math.pow(math.abs(2*value-2), 1/3) + 2) / 2
  end
end

PerfectFreeze.new = function(id)
  local self = {}

  self.shape = "Cube"

  speed = math.random(1, 5)

  phi1 = math.random()
  theta1 = smooth(math.random())

  -- first direction used when comeing from unit
  dir1 = {
    x = math.sin(phi1 * 2 * math.pi) * math.sin(theta1 * math.pi),
    y = math.cos(theta1 * math.pi),
    z = math.cos(phi1 * 2 * math.pi) * math.sin(theta1 * math.pi)
  }

  phi2 = math.random()
  theta2 = smooth(math.random())

  -- second direction used after freeze
  dir2 = {
    x = math.sin(phi2 * 2 * math.pi) * math.sin(theta2 * math.pi),
    y = math.cos(theta2 * math.pi),
    z = math.cos(phi2 * 2 * math.pi) * math.sin(theta2 * math.pi)
  }

  self.position = function(t)
    if t<7 then
      x = math.min(5, t) * speed * dir1.x
      y = math.min(5, t) * speed * dir1.y
      z = math.min(5, t) * speed * dir1.z
      return x, y, z
    else
      x = 5 * speed * dir2.x
      y = 5 * speed * dir2.y
      z = 5 * speed * dir2.z
      return x, y, z
    end
  end
end

return PerfectFreeze
