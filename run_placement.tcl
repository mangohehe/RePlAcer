# Set environment variables and paths
set design wb_dma_top
set lib_dir /build/share/library/a2a/
set design_dir /build/share/design/a2a/${design}

# Commands to run with RePlAce
replace_external rep

# Import LEF/DEF files
rep import_lef ${lib_dir}/contest.lef
rep import_def ${design_dir}/${design}.def
rep set_output ./output/

# Timing-driven parameters
rep import_verilog ${design_dir}/${design}.v
rep import_sdc ${design_dir}/${design}.sdc
rep import_lib ${lib_dir}/contest.lib

rep set_unit_res 16
rep set_unit_cap 0.23e-15
rep set_timing_driven 1

rep set_bin_grid_count 64

# Initialize RePlAce
rep init_replace

# Place cell with Nesterov method
rep place_cell_nesterov_place

# Export DEF file
rep export_def ./${design}_a2a_td.def

puts "nesterovPlace HPWL: [rep get_hpwl]"
puts "final WNS: [rep get_wns]"
puts "final TNS: [rep get_tns]"

exit

