using Gadfly
using DataFrames

input_file = ARGS[1]
mode = ARGS[2]

param = string(input_file[end-4])
input_file = "test/" * input_file
output_file = input_file[1:end-4] * ".svg"

df = readtable(input_file, separator = ';', skipstart=3)

if mode == "simple"
    p = plot(df, x=:time, y=:nVar, Geom.line)
elseif mode == "multiple"
    p = plot(df, x=:time, y=:nVar, color=:param, Geom.line, Scale.color_discrete(), Guide.colorkey(param), Guide.xlabel("Tempo"), Guide.ylabel("Numero de Variedades"))
elseif mode == "histogramSimple"
    p = plot(df, x=:appearence, y=:frequency, Geom.line, Scale.y_continuous(minvalue=0, maxvalue=1))
elseif mode == "histogramMultiple"
    p = plot(df, x=:appearence, y=:frequency, color=:param, Geom.line, Scale.color_discrete(), Guide.colorkey(param), Scale.y_continuous(minvalue=0, maxvalue=1))
else
    println("Error, please enter a valid mode.")
    exit(-1)
end

draw(SVG(output_file, 15cm, 10cm), p)
println("Image $(output_file) successfully generated.")


# using Gadfly
# using DataFrames
# df = readtable("test/plot/Runplot_dist.csv", separator = ';')
# plot(df[df[:time] .== 9000,:], x="nVar", Geom.histogram)
#
#
