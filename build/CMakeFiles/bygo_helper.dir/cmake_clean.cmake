file(REMOVE_RECURSE
  "libbygo_helper.pdb"
  "libbygo_helper.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/bygo_helper.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
