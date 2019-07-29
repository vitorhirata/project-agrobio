using DataFrames, Colors, CSV, Gadfly, Printf, StatsBase, GLM


# Plot all csv files in the $folder. Assumes julia is in src/
function plotAll(folder = "test")
  for f in filter(x -> endswith(x, "csv"), readdir(folder))
    plotHandler(folder * "/" * f)
  end
end


function argumentParse(input_file)
  mode = split(input_file, "_")[2]
  mode = split(mode, ".")[1]
  output_file = input_file[1:end-4] * ".svg"
  return mode, output_file
end

function loadDf(input_file)
  try
    df = CSV.File(input_file, delim = "; ", header=4) |> DataFrame
    if size(df)[1] == 0
      println("ERROR: $(input_file) contain an empty file")
      return -1
    else
      return df
    end
  catch
    println("ERROR: input_file '$(input_file)' does not exist")
    return -1
  end
end

function plotHandler(input_file)
  mode, output_file = argumentParse(input_file)
  df = loadDf(input_file)
  if df == -1
    return
  end

  if mode == "standard"
    plotStandard(df, output_file)
  elseif mode == "varParam"
    plotVarParam(df, output_file)
  elseif mode == "duDistribution"
    plotduDistribution(df, output_file)
  elseif mode == "varietyDistribution"
    plotVarietyDistribution(df, output_file)
  elseif mode == "varietyQuantity"
    plotVarietyQuantity(df, output_file)
  elseif mode == "histogramProductivity"
    plotHistogramProductivity(df, output_file)
  elseif mode == "histogramProductivityVar"
    plotHistogramProductivityVar(df, output_file)
  elseif mode == "varParamFixedPoints"
    plotVarParamFixedPoints(df, output_file)
  else
    println("$(input_file) is an invalid file.")
  end
end

function plotStandard(df, output_file)
    p =plot(layer(df, x=:time, y=:nVar, Geom.line,
                       Theme(default_color=colorant"green")),
            layer(df, x=:time, y=:meanDU, Geom.line,
                      Theme(default_color=colorant"red")),
                      Guide.manual_color_key("",["Riqueza na Comunidade",
                                 "Riqueza Média por UD"],
                                 ["green", "red"]),
                      Guide.ylabel("Riqueza de Variedades"),
                      Guide.xlabel("Anos Agrícolas"))
    draw(SVG(output_file, 20cm, 10cm), p)
    println("Image $(output_file) successfully generated.")
    p2 = plot(df, x=:meanDU, y=:nVar, Geom.line,
                 Guide.ylabel("Riqueza de Variedades na Comunidade"),
                 Guide.xlabel("Riqueza Media de Variedades por UD"))
    output_file2 = output_file[1:end-4] * "2.svg"
    draw(SVG(output_file2, 15cm, 10cm), p2)
    println("Image $(output_file2) successfully generated.")
    p3 = plot(layer(df, x=:time, y=:totalPunctuation, Geom.line,
                        Theme(default_color=colorant"blue")),
               layer(df, x=:time, y=:productivityPunctuation, Geom.line,
                         Theme(default_color=colorant"red")),
               layer(df, x=:time, y=:qualityPunctuation, Geom.line,
                         Theme(default_color=colorant"green")),
               Guide.manual_color_key("",["Pontuação Total",
                                          "Pontuação Produtividade",
                                          "Pontuação Cultural"],
                                      ["blue", "red", "green"]),
               Coord.cartesian(ymin=0, ymax=1),
               Guide.ylabel("Pontuação"), Guide.xlabel("Anos Agrícolas"))
    output_file3 = output_file[1:end-4] * "3.svg"
    draw(SVG(output_file3, 20cm, 10cm), p3)
    println("Image $(output_file3) successfully generated.")
    p4 = plot(layer(df, x=:time, y=:simpsonCommunity, Geom.line,
                        Theme(default_color=colorant"blue")),
               layer(df, x=:time, y=:shannonCommunity, Geom.line,
                        Theme(default_color=colorant"red")),
               layer(df, x=:time, y=:simpsonDU, Geom.line,
                        Theme(default_color=colorant"green")),
               layer(df, x=:time, y=:bergerDU, Geom.line,
                        Theme(default_color=colorant"orange")),
               layer(df, x=:time, y=:shannonDU, Geom.line,
                        Theme(default_color=colorant"purple")),
               Guide.manual_color_key("Índice",["Simpson Comunidade",
                                         "Shannon evenness Comunidade",
                                         "Berger-Parker UD",
                                         "Simpson UD",
                                         "Shannon evenness UD"],
                                        ["blue", "red",
                                         "green", "orange", "purple"]),
                Guide.ylabel("Valor da Métrica"),
                Guide.xlabel("Anos Agrícolas"))
    output_file4 = output_file[1:end-4] * "4.svg"
    draw(SVG(output_file4, 20cm, 10cm), p4)
    println("Image $(output_file4) successfully generated.")
end

function plotVarParam(df, output_file)
    param = string(output_file[end-4])
    p = plot(df, x=:time, y=:nVar, color=:param, Geom.line,
                  Scale.color_discrete(), Guide.colorkey(title=param),
                  Guide.xlabel("Anos Agrícolas"),
                  Guide.ylabel("Riqueza de Variedades na Comunidade"))
    draw(SVG(output_file, 15cm, 10cm), p)
    println("Image $(output_file) successfully generated.")
    p2 = plot(df, x=:time, y=:meanDU, color=:param, Geom.line,
                  Scale.color_discrete(), Guide.colorkey(title=param),
                  Guide.xlabel("Anos Agrícolas"),
                  Guide.ylabel("Riqueza Média de Variedades por UD"))
    output_file2 = output_file[1:end-4] * "2.svg"
    draw(SVG(output_file2, 15cm, 10cm), p2)
    println("Image $(output_file2) successfully generated.")
    if param == "m"
      maxTime = maximum(df[:time])
      df2 = df[df[:time] .== maxTime, :]
      ols = lm(@formula(nVar ~ 0 + meanDU), df2)
      f(x) = coef(ols)[1]*x
      println("Printing fit result for varParam_m")
      println(coeftable(ols))
      p3 = plot(layer(df2, x=:meanDU, y=:nVar, Geom.point,
                  Theme(point_size=0.6mm, highlight_width=0.0mm)),
                 layer(f,0,8, Theme(default_color="black")),
                  Guide.xlabel("Numero Medio de Variedades por UD"),
                  Guide.ylabel("Numero de Variedades"),
                  Coord.cartesian(xmin=0, ymin=0))
      output_file3 = output_file[1:end-4] * "3.svg"
      draw(SVG(output_file3, 15cm, 10cm), p3)
      println("Image $(output_file3) successfully generated.")
    end
end

function plotduDistribution(df, output_file)
    param = string(output_file[end-4])
    if length(split(output_file, "_")) == 2
      p = plot(df, x=:value, y=:duDist, Geom.bar,
                  Guide.ylabel("Porcentagem (%)"),
                  Guide.xlabel("Riqueza de Variedades por UD"))
      draw(SVG(output_file, 15cm, 10cm), p)
      println("Image $(output_file) successfully generated.")
    elseif length(split(output_file, "_")) == 3
      p = plot(df, x=:value, y=:duDist, color=:param, Geom.line,
                   Scale.color_discrete(),Guide.colorkey(title=param),
                   Guide.ylabel("Porcentagem (%)"),
                   Guide.xlabel("Riqueza de Variedades por UD"))
      draw(SVG(output_file, 15cm, 10cm), p)
      println("Image $(output_file) successfully generated.")
    end
end

function plotVarietyDistribution(df, output_file)
    param = string(output_file[end-4])
    if length(split(output_file, "_")) == 2
      p = plot(df, x=:value, y=:varDist, Geom.bar,
                  Guide.xticks(ticks=[0.0,20,40,50,60,80,100]),
                  Guide.ylabel("Porcentagem de Variedades (%)"),
                  Guide.xlabel("Porcentagem de Unidades Domésticas (%)"),
                  Coord.cartesian(xmin=0, xmax=100))
      draw(SVG(output_file, 15cm, 10cm), p)
      println("Image $(output_file) successfully generated.")
    elseif length(split(output_file, "_")) == 3
      p = plot(df, x=:value, y=:varDist, color=:param, Geom.line,
                   Scale.color_discrete(),Guide.colorkey(title=param),
                  Guide.xticks(ticks=[0.0,20,40,50,60,80,100]),
                  Guide.ylabel("Porcentagem de Variedades (%)"),
                  Guide.xlabel("Porcentagem de Unidades Domésticas (%)"))
      draw(SVG(output_file, 15cm, 10cm), p)
      println("Image $(output_file) successfully generated.")
    end
end

function plotVarietyQuantity(df, output_file)
    param = string(output_file[end-4])
    if length(split(output_file, "_")) == 2
      p = plot(df, x=:quantity, y=:frequency, Geom.bar,
                  Guide.ylabel("Porcentagem de Variedades (%)"),
                  Guide.xlabel("Área Ocupada na Comunidade (%)"),
                  Scale.x_log10(labels=d-> @sprintf("%.1f",10^d)),
                  Guide.xticks(ticks=[-1, 0, 0.7, 1, 1.699, 2]))
      draw(SVG(output_file, 15cm, 10cm), p)
      println("Image $(output_file) successfully generated.")
    elseif length(split(output_file, "_")) == 3
      p = plot(df, x=:quantity, y=:frequency, color=:param, Geom.line,
                  Scale.color_discrete(),Guide.colorkey(title=param),
                  Guide.ylabel("Porcentagem de Variedades (%)"),
                  Guide.xlabel("Área Ocupada na Comunidade (%)"),
                  Scale.x_log10(labels=d-> @sprintf("%.1f",10^d)),
                  Guide.xticks(ticks=[-1, 0, 0.7, 1, 1.699, 2]))
      draw(SVG(output_file, 15cm, 10cm), p)
      println("Image $(output_file) successfully generated.")
    end
end

function plotHistogramProductivity(df, output_file)
    p=plot(layer(df, x=:value, y=:productivity, Geom.bar,
                     Theme(default_color=Colors.RGBA(0,80,0, 0.4))),
            layer(df, x=:value, y=:quality, Geom.bar,
                      Theme(default_color=Colors.RGBA(255,0,0, 0.4))),
            Guide.manual_color_key("",["Produtividade Média","Qualidade"],
                                   ["green", "red"]),
            Guide.ylabel("Área ocupada (%)"),
            Guide.xlabel("Valor"),
                      )
    draw(SVG(output_file, 15cm, 10cm), p)
    println("Image $(output_file) successfully generated.")
end

function plotHistogramProductivityVar(df, output_file)
    param = string(output_file[end-4])
    p = plot(df, x=:value, y=:productivity, color=:param, Geom.line,
                 Scale.color_discrete(), Guide.colorkey(title=param),
                 Scale.y_continuous(minvalue=0, maxvalue=0.2),
                 Guide.ylabel("Área ocupada (%)"),
                 Guide.xlabel("Produtividade"))
    draw(SVG(output_file, 15cm, 10cm), p)
    println("Image $(output_file) successfully generated.")
    p2 = plot(df, x=:value, y=:quality, color=:param, Geom.line,
                  Scale.color_discrete(), Guide.colorkey(title=param),
                  Scale.y_continuous(minvalue=0, maxvalue=0.1),
                 Guide.ylabel("Área ocupada (%)"),
                  Guide.xlabel("Qualidade"))
    output_file2 = output_file[1:end-4] * "2.svg"
    draw(SVG(output_file2, 15cm, 10cm), p2)
    println("Image $(output_file2) successfully generated.")
end

function plotVarParamFixedPoints(df, output_file)
    param = string(output_file[end-4])
    p=plot(layer(df, x=:param, y=:nVar, Geom.line,
                     Theme(default_color=colorant"green")),
            layer(df, x=:param, y=:meanDU, Geom.line,
                      Theme(default_color=colorant"red")),
            Guide.manual_color_key("",
                                   ["Número de Variedades",
                                    "Riqueza média de variedades por UD"],
                                   ["green", "red"]),
            Guide.ylabel("Ponto Fixo"),
            Guide.xlabel(param))
    draw(SVG(output_file, 20cm, 10cm), p)
    println("Image $(output_file) successfully generated.")
end

