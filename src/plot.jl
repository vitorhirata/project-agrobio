using Gadfly
using DataFrames

for i in ARGS
    input_file = i
    mode = split(input_file, "_")[2]
    mode = split(mode, ".")[1]
    param = string(input_file[end-4])
    input_file = "test/" * input_file
    output_file = input_file[1:end-4] * ".svg"
    df = readtable(input_file, separator = ';', skipstart=3)

    if mode == "standard"
        p = plot(df, x=:meanDU, y=:nVar, Geom.line, Guide.ylabel("Numero de Variedades"), Guide.xlabel("Numero Medio de Variedades por UD"))
        draw(SVG(output_file, 15cm, 10cm), p)
        println("Image $(output_file) successfully generated.")
        p2 =plot(layer(df, x=:time, y=:nVar, Geom.line, Theme(default_color=colorant"green")),
                layer(df, x=:time, y=:meanDU, Geom.line, Theme(default_color=colorant"red")),
                Guide.manual_color_key("",["Número de Variedades","Número Médio de Variedades por UD"],["green", "red"]),
                Guide.ylabel(""), Guide.xlabel("Tempo"))
        output_file2 = input_file[1:end-4] * "2.svg"
        draw(SVG(output_file2, 20cm, 10cm), p2)
        println("Image $(output_file2) successfully generated.")
    elseif mode == "varParam"
        p = plot(df, x=:meanDU, y=:nVar, color=:param, Geom.line, Scale.color_discrete(), Guide.colorkey(param), Guide.xlabel("Numero Medio de Variedades por UD"), Guide.ylabel("Numero de Variedades"))
        draw(SVG(output_file, 15cm, 10cm), p)
        println("Image $(output_file) successfully generated.")
        p2 = plot(df, x=:time, y=:nVar, color=:param, Geom.line, Scale.color_discrete(), Guide.colorkey(param), Guide.xlabel("Tempo"), Guide.ylabel("Numero de Variedades"))
        output_file2 = input_file[1:end-4] * "2.svg"
        draw(SVG(output_file2, 15cm, 10cm), p2)
        println("Image $(output_file2) successfully generated.")
    elseif mode == "varietyDistribution"
        if length(split(input_file, "_")) == 2
            p = plot(df, x=:value, y=:varDist, Geom.bar, Coord.cartesian(xmin=0, xmax=20, ymin=0, ymax=1),
                    Guide.ylabel("Frequencia"), Guide.xlabel("Numero Medio de Variedades por UD"))
            draw(SVG(output_file, 15cm, 10cm), p)
            println("Image $(output_file) successfully generated.")
        elseif length(split(input_file, "_")) == 3
            p = plot(df, x=:value, y=:varDist, color=:param, Geom.line, Scale.color_discrete(),
                    Guide.colorkey(param), Coord.cartesian(xmin=0, xmax=20, ymin=0, ymax=1),
                    Guide.ylabel("Frequencia"), Guide.xlabel("Numero Medio de Variedades por UD"))
            draw(SVG(output_file, 15cm, 10cm), p)
            println("Image $(output_file) successfully generated.")
        end
    elseif mode == "histogramFitness"
        p=plot(layer(df, x=:value, y=:fitness, Geom.bar, Theme(default_color=Colors.RGBA(0,80,0, 0.4))),
                layer(df, x=:value, y=:appearence, Geom.bar, Theme(default_color=Colors.RGBA(255,0,0, 0.4))),
                Guide.manual_color_key("",["Fitness","Appearence"],["green", "red"]))
        draw(SVG(output_file, 15cm, 10cm), p)
        println("Image $(output_file) successfully generated.")
    elseif mode == "histogramFitnessVar"
        p = plot(df, x=:value, y=:fitness, color=:param, Geom.line, Scale.color_discrete(), Guide.colorkey(param),
                Scale.y_continuous(minvalue=0, maxvalue=0.2), Guide.ylabel("Frequencia"), Guide.xlabel("Fitness"))
        draw(SVG(output_file, 15cm, 10cm), p)
        println("Image $(output_file) successfully generated.")
        p2 = plot(df, x=:value, y=:appearence, color=:param, Geom.line, Scale.color_discrete(), Guide.colorkey(param),
                Scale.y_continuous(minvalue=0, maxvalue=0.1), Guide.ylabel("Frequencia"), Guide.xlabel("Appearence"))
        output_file2 = input_file[1:end-4] * "2.svg"
        draw(SVG(output_file2, 15cm, 10cm), p2)
        println("Image $(output_file2) successfully generated.")
    elseif mode == "varParamFixedPoints"
        p=plot(layer(df, x=:param, y=:nVar, Geom.line, Theme(default_color=colorant"green")),
                layer(df, x=:param, y=:meanDU, Geom.line, Theme(default_color=colorant"red")),
                Guide.manual_color_key("",["Número de Variedades","Número Médio de Variedades por UD"],["green", "red"]),
                Guide.ylabel(""), Guide.xlabel("Time"))
        draw(SVG(output_file, 20cm, 10cm), p)
        println("Image $(output_file) successfully generated.")
    else
        println("Error, please enter a valid mode.")
        exit(-1)
    end
end
