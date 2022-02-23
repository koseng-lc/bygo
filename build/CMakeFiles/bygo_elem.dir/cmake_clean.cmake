file(REMOVE_RECURSE
  "libbygo_elem.pdb"
  "libbygo_elem.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/bygo_elem.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
