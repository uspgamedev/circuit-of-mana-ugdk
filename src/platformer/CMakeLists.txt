
include_directories (.)

add_ugdk_executable (
  circuit-of-mana-platformer
  main.cc
  model/body.cc
  model/body.h
  model/material.h
  model/firematerial.cc
  model/firematerial.h
  model/solidmaterial.cc
  model/solidmaterial.h
  view/blank.h
  view/blank.cc
  view/stagerenderer.h
  view/stagerenderer.cc
  view/sprite.h
  view/sprite.cc
  view/tilemap.cc
  view/tilemap.h
)

