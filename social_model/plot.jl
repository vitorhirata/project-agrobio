using Gadfly
using DataFrames

input_file = ARGS[1]
mode = ARGS[2]

param = string(input_file[end-4])
input_file = "test/" * input_file
output_file = input_file[1:end-4] * ".svg"

df = readtable(input_file, separator = ';', skipstart=3)

if mode == "simple"
    p = plot(df, x=:time, y=:nSpecie, Geom.line)
elseif mode == "multiple"
    p = plot(df, x=:time, y=:nSpecie, color=:param, Geom.line, Scale.color_discrete(), Guide.colorkey(param))
else
    println("Error, please enter a valid mode.")
    exit(-1)
end

draw(SVG(output_file, 15cm, 9cm), p)
println("Image $(output_file) successfully generated.")
