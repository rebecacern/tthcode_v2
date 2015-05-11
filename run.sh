rm results/gen_histos.root
root.exe -b -l -q main.C
root.exe -b -l -q code_gen.C+\(0\,0\)
#root.exe -b -l -q main_ttW.C
#root.exe -b -l -q code_gen_ttW.C+\(0\,0\)
