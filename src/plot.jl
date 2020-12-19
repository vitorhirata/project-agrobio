using DataFrames, Colors, ColorSchemes, CSV, Gadfly, Printf, StatsBase, GLM


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
  elseif mode == "hdDistribution"
    plothdDistribution(df, output_file)
  elseif mode == "varietyDistribution"
    plotVarietyDistribution(df, output_file)
  elseif mode == "varietyQuantity"
    plotVarietyQuantity(df, output_file)
  elseif mode == "histogramProductivity"
    plotHistogramProductivity(df, output_file)
  elseif mode == "histogramProductivityVar"
    plotHistogramProductivityVar(df, output_file)
  elseif mode == "finalStateVariation"
    plotModelFinalStates(df, output_file)
  elseif mode == "phaseDiagram"
    plotPhaseDiagram(df, output_file)
  else
    println("$(input_file) is an invalid file.")
  end
end

function plotStandard(df, output_file)
    p =plot(layer(df, x=:time, y=:nVar, Geom.line,
                       Theme(default_color=colorant"green")),
            layer(df, x=:time, y=:meanHD, Geom.line,
                      Theme(default_color=colorant"red")),
                      Guide.manual_color_key("",["Richness in community",
                                 "Average richness per HD"],
                                 ["green", "red"]),
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
                      Guide.ylabel("Varietal richness"),
                      Guide.xlabel("Agricultural cycle"))
    draw(SVG(output_file, 23cm, 10cm), p)
    println("Image $(output_file) successfully generated.")
    p2 = plot(df, x=:meanHD, y=:nVar, Geom.line,
                 Guide.ylabel("Varietal richness in community"),
                 Guide.xlabel("Average varietal richness per HD"))
    output_file2 = output_file[1:end-4] * "2.svg"
    draw(SVG(output_file2, 20cm, 10cm), p2)
    println("Image $(output_file2) successfully generated.")
    p3 = plot(layer(df, x=:time, y=:totalPunctuation, Geom.line,
                        Theme(default_color=colorant"blue")),
               layer(df, x=:time, y=:productivityPunctuation, Geom.line,
                         Theme(default_color=colorant"red")),
               layer(df, x=:time, y=:qualityPunctuation, Geom.line,
                         Theme(default_color=colorant"green")),
                Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
               Guide.manual_color_key("",["Total Punctuation",
                                          "Productivity Punctuation",
                                          "Quality Punctuation"],
                                      ["blue", "red", "green"]),
               Coord.cartesian(ymin=0, ymax=1),
               Guide.ylabel("Punctuation"), Guide.xlabel("Agricultural cycle"))
    output_file3 = output_file[1:end-4] * "3.svg"
    draw(SVG(output_file3, 23cm, 10cm), p3)
    println("Image $(output_file3) successfully generated.")
    p4 = plot(layer(df, x=:time, y=:bergerCommunity, Geom.line,
                        Theme(default_color=colorant"brown")),
              layer(df, x=:time, y=:simpsonCommunity, Geom.line,
                        Theme(default_color=colorant"blue")),
               layer(df, x=:time, y=:shannonCommunity, Geom.line,
                        Theme(default_color=colorant"red")),
               layer(df, x=:time, y=:simpsonHD, Geom.line,
                        Theme(default_color=colorant"green")),
               layer(df, x=:time, y=:bergerHD, Geom.line,
                        Theme(default_color=colorant"orange")),
               layer(df, x=:time, y=:shannonHD, Geom.line,
                        Theme(default_color=colorant"purple")),
                Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
               Guide.manual_color_key("Index",["Berger-Parker Community",
                                         "Simpson Community",
                                         "Shannon evenness Community",
                                         "Berger-Parker HD",
                                         "Simpson HD",
                                         "Shannon evenness HD"],
                                        ["brown", "blue", "red",
                                         "green", "orange", "purple"]),
                Guide.ylabel("Value of the metric"),
                Guide.xlabel("Agricultural cycle"))
    output_file4 = output_file[1:end-4] * "4.svg"
    draw(SVG(output_file4, 23cm, 10cm), p4)
    println("Image $(output_file4) successfully generated.")
end

function plotVarParam(df, output_file)
    param = string(output_file[end-4])
    p = plot(df, x=:time, y=:nVar, color=:param, Geom.line,
                  Scale.color_discrete, Guide.colorkey(title=param),
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
                  Guide.xlabel("Agricultural cycle"),
                  Guide.ylabel("Varietal richness in community"))
    draw(SVG(output_file, 16cm, 12cm), p)
    println("Image $(output_file) successfully generated.")
    p2 = plot(df, x=:time, y=:meanHD, color=:param, Geom.line,
                  Scale.color_discrete(), Guide.colorkey(title=param),
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
                  Guide.xlabel("Agricultural cycle"),
                  Guide.ylabel("Average varietal richness per HD"))
    output_file2 = output_file[1:end-4] * "2.svg"
    draw(SVG(output_file2, 16cm, 12cm), p2)
    println("Image $(output_file2) successfully generated.")
    if param == "m"
      maxTime = maximum(df.time)
      df2 = df[df.time .== maxTime, :]

      println("Printing diversity indexes for varParam_m")
      columns_print = append!([2, 3], 7:12)
      println(describe(df2[:, columns_print], :mean, :std, :min, :max, :median))
      print('\n')

      ols = lm(@formula(nVar ~ 0 + meanHD), df2)
      f(x) = coef(ols)[1]*x
      println("Printing fit result for varParam_m")
      println(coeftable(ols))
      p3 = plot(layer(df2, x=:meanHD, y=:nVar, Geom.point,
                  Theme(point_size=0.6mm, highlight_width=0.0mm)),
                 layer(f,0,9, Theme(default_color="black")),
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
                  Guide.xlabel("Average varietal richness per HD"),
                  Guide.ylabel("Varietal richness in community"),
                  Guide.yticks(ticks=[0,10,20,30,40]),
                  Guide.xticks(ticks=[0,1,2,3,4,5,6,7,8,9]),
                  Coord.cartesian(xmin=0, ymin=0))
      output_file3 = output_file[1:end-4] * "3.svg"
      draw(SVG(output_file3, 18cm, 12cm), p3)
      println("Image $(output_file3) successfully generated.")
    end
end

function plothdDistribution(df, output_file)
    param = string(output_file[end-4])
    if length(split(output_file, "_")) == 2
      p = plot(df, x=:value, y=:hdDist, Geom.bar,
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
                  Coord.cartesian(xmin=0, xmax=15),
                  Guide.ylabel("Percentage (%)"),
                  Guide.xlabel("Varietal richness per HD"))
      draw(SVG(output_file, 15cm, 10cm), p)
      println("Image $(output_file) successfully generated.")
    elseif length(split(output_file, "_")) == 3
      p = plot(df, x=:value, y=:hdDist, color=:param, Geom.line,
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
                  Scale.color_discrete(),Guide.colorkey(title=param),
                  Guide.ylabel("Percentage (%)"),
                  Guide.xlabel("Varietal richness per HD"))
      draw(SVG(output_file, 15cm, 10cm), p)
      println("Image $(output_file) successfully generated.")
    end
end

function plotVarietyDistribution(df, output_file)
    param = string(output_file[end-4])
    if length(split(output_file, "_")) == 2
      p = plot(df, x=:value, y=:varDist, Geom.bar,
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
                  Guide.xticks(ticks=[0.0,20,40,60,80,100]),
                  Guide.ylabel("Percentage of varieties (%)"),
                  Guide.xlabel("Percentage of households (%)"),
                  Coord.cartesian(xmin=0, xmax=100))
      draw(SVG(output_file, 15cm, 10cm), p)
      println("Image $(output_file) successfully generated.")
    elseif length(split(output_file, "_")) == 3
      p = plot(df, x=:value, y=:varDist, color=:param, Geom.line,
                   Scale.color_discrete(),Guide.colorkey(title=param),
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
                  Guide.xticks(ticks=[0.0,20,40,50,60,80,100]),
                  Guide.ylabel("Percentage of varieties (%)"),
                  Guide.xlabel("Percentage of households (%)"))
      draw(SVG(output_file, 15cm, 10cm), p)
      println("Image $(output_file) successfully generated.")
    end
end

function plotVarietyQuantity(df, output_file)
    param = string(output_file[end-4])
    if length(split(output_file, "_")) == 2
      p = plot(df, x=:quantity, y=:frequency, Geom.bar,
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
                  Guide.ylabel("Percentage of varieties (%)"),
                  Guide.xlabel("Occupied area in community (%)"),
                  Scale.x_log10(labels=d-> @sprintf("%.1f",10^d)),
                  Guide.xticks(ticks=[-1, 0, 0.7, 1, 1.699, 2]))
      draw(SVG(output_file, 15cm, 10cm), p)
      println("Image $(output_file) successfully generated.")
    elseif length(split(output_file, "_")) == 3
      p = plot(df, x=:quantity, y=:frequency, color=:param, Geom.line,
                  Scale.color_discrete(),Guide.colorkey(title=param),
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
                  Guide.ylabel("Percentage of varieties (%)"),
                  Guide.xlabel("Occupied area in community (%)"),
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
            Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                    key_title_font_size=14pt, key_label_font_size=12pt),
            Guide.manual_color_key("",["Productivity","Quality"],
                                   ["green", "red"]),
            Guide.ylabel("Occupied area (%)"),
            Guide.xlabel("Value of metric"))
    draw(SVG(output_file, 15cm, 10cm), p)
    println("Image $(output_file) successfully generated.")
end

function plotHistogramProductivityVar(df, output_file)
    param = string(output_file[end-4])
    p = plot(df, x=:value, y=:productivity, color=:param, Geom.line,
                 Scale.color_discrete(), Guide.colorkey(title=param),
                 Scale.y_continuous(minvalue=0, maxvalue=0.2),
                 Guide.ylabel("Occupied area (%)"),
                 Guide.xlabel("Productivity"))
    draw(SVG(output_file, 15cm, 10cm), p)
    println("Image $(output_file) successfully generated.")
    p2 = plot(df, x=:value, y=:quality, color=:param, Geom.line,
                  Scale.color_discrete(), Guide.colorkey(title=param),
                  Scale.y_continuous(minvalue=0, maxvalue=0.1),
                 Guide.ylabel("Occupied area (%)"),
                  Guide.xlabel("Quality"))
    output_file2 = output_file[1:end-4] * "2.svg"
    draw(SVG(output_file2, 15cm, 10cm), p2)
    println("Image $(output_file2) successfully generated.")
end

function plotModelFinalStates(df, output_file)
    param = string(output_file[end-4])
    p=plot(layer(df, x=:param, y=:nVar, Geom.line,
                     Theme(default_color=colorant"green")),
            layer(df, x=:param, y=:meanHD, Geom.line,
                      Theme(default_color=colorant"red")),
            Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                    key_title_font_size=14pt, key_label_font_size=12pt),
            Guide.manual_color_key("",
                                   ["Varietal richness in community",
                                    "Average varietal richness per HD"],
                                   ["green", "red"]),
            Guide.ylabel("Value at the end of simulation"),
            Guide.xlabel(param))
    draw(SVG(output_file, 23cm, 10cm), p)
    println("Image $(output_file) successfully generated.")
end

function plotPhaseDiagram(df, output_file)
    param = string(output_file[end-6])
    param2 = string(output_file[end-4])
    cpalette(pt) = get(ColorSchemes.viridis, pt)
    p=plot(df, x=:param, y=:param2, color=:nVar, Geom.rectbin,
            Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                    key_title_font_size=14pt, key_label_font_size=12pt),
            Scale.color_continuous(colormap=cpalette),
            Guide.xlabel(param),
            Guide.ylabel(param2),
            Guide.colorkey(title="Varietal richness\nin community"))
    draw(SVG(output_file, 23cm, 10cm), p)
    println("Image $(output_file) successfully generated.")

    p2=plot(df, x=:param, y=:param2, color=:meanHD, Geom.rectbin,
            Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                    key_title_font_size=14pt, key_label_font_size=12pt),
            Scale.color_continuous(colormap=cpalette),
            Guide.xlabel(param),
            Guide.ylabel(param2),
            Guide.colorkey(title="Average varietal\nrichness per HD"))
    output_file2 = output_file[1:end-4] * "2.svg"
    draw(SVG(output_file2, 23cm, 10cm), p2)
    println("Image $(output_file2) successfully generated.")
end
