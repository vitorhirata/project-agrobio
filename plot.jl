using Gadfly
using DataFrames

input_file = ARGS[1]
mode = ARGS[2]
param = string(input_file[end-4])
output_file = input_file[1:end-4] * ".png"

df = readtable(input_file, separator = ';')

if mode == "simple"
    p = plot(df, x=:time, y=:nSpecie, Geom.line)
elseif mode == "multiple"
    p = plot(df, x=:time, y=:nSpecie, color=:param, Geom.line, Scale.color_discrete(), Guide.colorkey(param))
else
    println("Error, please enter a valid mode.")
    exit(-1)
end

draw(PNG(output_file, 12cm, 8cm), p)
println("Image $(output_file) successfully generated.")
