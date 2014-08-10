
local NAME, W, H, N, M = ...

W = tonumber(W)
H = tonumber(H)
M = tonumber(M)
  N = tonumber(N)

local json = [[
{
  "frames": {
%s
  },
  "meta": {
    "version": "1.0",
    "image": "%s.png",
    "format": "RGBA8888",
    "size": {"w":%d,"h":%d},
    "scale": "1"
  }
}
]]

local w, h = math.floor(W/N), math.floor(H/M)

local frames = ""
for i = 1,M do
  for j = 1,N do
    local frame = [[

    "%d": {
      "frame": {"x":%d, "y":%d, "w":%d, "h":%d},
      "rotated": false,
      "trimmed": false,
      "spriteSourceSize": {"x":0, "y":0, "w":%d, "h":%d},
      "sourceSize": {"w":%d, "h":%d}
    }%s]]
    frames = frames..frame:format(
      (i-1)*N+j,
      (j-1)*w, (i-1)*h, w, h,
      w, h,
      w, h,
      (i < M or j < N) and "," or ""
    )
  end
end

print(json:format(frames, NAME, W, H))

