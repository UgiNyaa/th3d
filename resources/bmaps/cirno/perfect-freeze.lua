local PerfectFreeze = {}
PerfectFreeze.__index = PerfectFreeze

PerfectFreeze.n = 300

-- smooth used for fighting against crowding in top and bottom of sphere
local function smooth(value)
  if value < 0.5 then
    return math.pow(2*value, 1/3) / 2
  else
    return (-math.pow(math.abs(2*value-2), 1/3) + 2) / 2
  end
end

function PerfectFreeze.create(i)
  local self = setmetatable({}, PerfectFreeze)
  self.i = i

  self.shape = "block"
  self.start = (4*i) / PerfectFreeze.n

  self.speed = math.random(4, 7)

  local phi1 = math.random()
  local theta1 = smooth(math.random())

  -- first direction used when comeing from unit
  self.dir1 = {
    x = math.sin(phi1 * 2 * math.pi) * math.sin(theta1 * math.pi),
    y = math.cos(theta1 * math.pi),
    z = math.cos(phi1 * 2 * math.pi) * math.sin(theta1 * math.pi)
  }

  local phi2 = math.random()
  local theta2 = smooth(math.random())

  -- second direction used after freeze
  self.dir2 = {
    x = math.sin(phi2 * 2 * math.pi) * math.sin(theta2 * math.pi),
    y = math.cos(theta2 * math.pi),
    z = math.cos(phi2 * 2 * math.pi) * math.sin(theta2 * math.pi)
  }

  return self
end

function PerfectFreeze:position(t)
  _t = t - self.start
  stop = 5 - self.start
  if t < 7 then
    x = math.min(stop, _t) * self.speed * self.dir1.x
    y = math.min(stop, _t) * self.speed * self.dir1.y
    z = math.min(stop, _t) * self.speed * self.dir1.z
    return x, y, z
  else
    base = {
      x = stop * self.speed * self.dir1.x,
      y = stop * self.speed * self.dir1.y,
      z = stop * self.speed * self.dir1.z
    }
    x = base.x + math.pow(t-7, 2) * self.dir2.x
    y = base.y + math.pow(t-7, 2) * self.dir2.y
    z = base.z + math.pow(t-7, 2) * self.dir2.z
    return x, y, z
  end
end

return PerfectFreeze
