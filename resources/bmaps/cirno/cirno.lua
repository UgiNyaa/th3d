local PerfectFreeze = require "perfect-freeze"

local cirno = {}

cirno.shape = "block"

cirno.position = function(t)
  x = 0
  y = 0.1*math.sin(t*4)
  z = 10

  return x, y, z
end

cirno.patterns = {
  PerfectFreeze
}

return cirno
