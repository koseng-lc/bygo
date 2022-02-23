file(REMOVE_RECURSE
  "libbygo_lib.pdb"
  "libbygo_lib.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/bygo_lib.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
