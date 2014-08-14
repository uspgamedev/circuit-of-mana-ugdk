
include_directories (.)

add_ugdk_executable (
  circuit-of-mana-platformer
  main.cc
  tilemap.cc
  tilemap.h
  body.cc
  body.h
  view/stagerenderer.h
  view/stagerenderer.cc
)

