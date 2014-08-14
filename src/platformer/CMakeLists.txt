
include_directories (.)

add_ugdk_executable (
  circuit-of-mana-platformer
  main.cc
  body.cc
  body.h
  view/tilemap.cc
  view/tilemap.h
  view/stagerenderer.h
  view/stagerenderer.cc
)

