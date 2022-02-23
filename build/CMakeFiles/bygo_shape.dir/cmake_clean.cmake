file(REMOVE_RECURSE
  "libbygo_shape.pdb"
  "libbygo_shape.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/bygo_shape.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
