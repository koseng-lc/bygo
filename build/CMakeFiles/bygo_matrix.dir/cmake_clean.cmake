file(REMOVE_RECURSE
  "libbygo_matrix.pdb"
  "libbygo_matrix.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/bygo_matrix.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
