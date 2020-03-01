using Gadfly
using DataFrames
using Colors
using CSV
using StatsBase
using GLM
using Statistics
import Cairo, Fontconfig

function plotAll()
  df, df2, df3 = loadData()
  plotGeraneralData(df)
  plotAllHistogram(df)
  plotAllBoxplot(df2, df3)
  plotGeneral(df)
  plotIndex()
end

function loadData()
  fileName = "csv/data_revision.csv"
  df = CSV.File(fileName, delim = ";", missingstring="null", decimal=',') |> DataFrame

  fileName = ["csv/boxplot_UD_x_Var.csv", "csv/boxplot_Var_x_UD.csv"]
  df2 = CSV.File(fileName[1], delim = ";", decimal=',') |> DataFrame
  df3 = CSV.File(fileName[2], delim = ";", decimal=',') |> DataFrame
  return df, df2, df3
end

function plotGeraneralData(df)
  dfClean = df[[:Especie, :Fonte, :Ncomunidade, :NmedioUD]]
  dfClean = dropmissing(dfClean)

  dfMandioca = dfClean[dfClean[:Especie] .== "Mandioca", :]
  plotComunidadexDU(dfMandioca, "mandioca", [23*1.3cm, 10*1.3cm], true)

  dfMilho = dfClean[dfClean[:Especie] .== "Milho", :]
  plotComunidadexDU(dfMilho, "milho", [19*1.3cm, 10*1.3cm], true)

  dfBatata = dfClean[dfClean[:Especie] .== "Batata", :]
  plotComunidadexDU(dfBatata, "batata", [17*1.3cm, 10*1.3cm], false)

  ols1 = lm(@formula(Ncomunidade ~ 0 + NmedioUD),
             dfClean[dfClean[:Especie] .== "Mandioca", :])
  ols2 = lm(@formula(Ncomunidade ~ 0 + NmedioUD),
             dfClean[dfClean[:Especie] .== "Milho", :])
  f1(x) = coef(ols1)[1]*x
  f2(x) = coef(ols2)[1]*x
  img = PNG("plot/todos.png", 17*1.3cm, 10*1.3cm)
  p=plot(dfClean, x=:NmedioUD, y=:Ncomunidade,
          color=:Especie, Geom.point,
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
                        point_size=0.6mm, highlight_width=0.0mm),
          Guide.xlabel("Riqueza média de variedades por UD"),
          Guide.ylabel("Riqueza de variedades na Comunidade"),
          Guide.colorkey(title="Especie"),
          Guide.title("Agrobiodiversidade: dados para três espécies"));
  draw(img, p)

end

function plotComunidadexDU(df, str, size, runRegression)
  img = PNG("plot/$(str).png", size[1], size[2])
  if runRegression
    ols = lm(@formula(Ncomunidade ~ 0 + NmedioUD), df)
    f(x) = coef(ols)[1]*x
    p=plot(layer(df, x=:NmedioUD, y=:Ncomunidade, color=:Fonte, Geom.point),
      layer(f, 0, 1.1*maximum(df[:NmedioUD])),
      Guide.xlabel("Riqueza média de variedades por UD"),
      Guide.ylabel("Riqueza de variedades na Comunidade"),
      Guide.colorkey(title="Fonte"),
      Guide.title("Agrobiodiversidade de $(str)"),
      Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
            key_title_font_size=14pt, key_label_font_size=12pt,
            plot_padding=[30pt, 60pt, 10pt, 10pt],
        point_size=0.6mm, highlight_width=0.0mm));
      println("Printing fit result for $(str)")
      println(coeftable(ols))
  else
    p=plot(df, x=:NmedioUD, y=:Ncomunidade, color=:Fonte, Geom.point,
      Guide.xlabel("Riqueza média de variedades por UD"),
      Guide.ylabel("Riqueza de variedades na Comunidade"),
      Guide.colorkey(title="Fonte"),
      Guide.title("Agrobiodiversidade de $(str)"),
      Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
            key_title_font_size=14pt, key_label_font_size=12pt,
            plot_padding=[30pt, 60pt, 10pt, 10pt],
        point_size=0.6mm, highlight_width=0.0mm));
  end
  draw(img, p)
end

function plotGeneral(df)
  dfUD = df[[:Especie, :NmedioUD]]
  dfUD = dropmissing(dfUD)
  img = PNG("plot/todosUD.png", 15*1.3cm, 10*1.3cm)
  p=plot(dfUD, x=:Especie, y=:NmedioUD, Geom.boxplot,
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
    Guide.xlabel("Espécie"),
    Guide.ylabel("Riqueza média de variedades por UD"));
  draw(img, p)

  dfComunidade = df[[:Especie, :Ncomunidade]]
  dfComunidade = dropmissing(dfComunidade)
  img = PNG("plot/todosComunidade.png", 15*1.3cm, 10*1.3cm)
  p=plot(dfComunidade, x=:Especie, y=:Ncomunidade, Geom.boxplot,
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
    Guide.xlabel("Espécie"),
    Guide.ylabel("Riqueza de variedades na Comunidade"));
  draw(img, p)

end


function plotAllHistogram(df)
  dfClean = df[[:Especie, :Ncomunidade, :NmedioUD]]

  dfMandioca = dfClean[dfClean[:Especie] .== "Mandioca", :]
  plotHistogram(dfMandioca, "mandioca", [30,30])

  dfMilho = dfClean[dfClean[:Especie] .== "Milho", :]
  plotHistogram(dfMilho, "milho", [15,10])

  dfBatata = dfClean[dfClean[:Especie] .== "Batata", :]
  plotHistogram(dfBatata, "batata", [5,5])
end

function plotHistogram(df, str, binNumber)
  array1=collect(skipmissing(df[:Ncomunidade]))
  array2=collect(skipmissing(df[:NmedioUD]))

  img1 = PNG("plot/$(str)Comunidade.png", 15*1.3cm, 10*1.3cm)
  p1=plot(x=array1, Geom.histogram(bincount=binNumber[1], density=true),
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
    Guide.xlabel("Riqueza de variedades na Comunidade"),
    Guide.ylabel("Frequência (%)"),
    Guide.title("Riqueza de Variedades na Comunidade: $(str). N = $(size(array1)[1])"))
  draw(img1, p1)

  img2 = PNG("plot/$(str)UD.png", 15*1.3cm, 10*1.3cm)
  p2=plot(x=array2, Geom.histogram(bincount=binNumber[2], density=true),
    Guide.xlabel("Riqueza média de variedades por UD"),
    Guide.ylabel("Frequencia (%)"),
    Guide.title("Riqueza Média de variedades por Unidade Domestica: $(str). N = $(size(array2)[1])."),
    Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
          plot_padding=[50pt, 90pt, 10pt, 10pt]))
  draw(img2, p2)

end

function plotAllBoxplot(df, df2)

  dfMandioca1 = df[df[:Especie] .== "Mandioca", :]
  dfMandioca2 = df2[df2[:Especie] .== "Mandioca", :]
  plotBoxplot(dfMandioca1, "mandioca", dfMandioca2)

  #dfMilho1 = df[df[:Especie] .== "Milho", :]
  #plotBoxplot(dfMilho1, "milho")

  #dfBatata1 = df[df[:Especie] .== "Batata", :]
  #dfBatata2 = df2[df2[:Especie] .== "Batata", :]
  #plotBoxplot(dfBatata1, "batata", dfBatata2, [12.5*1.3cm, 7.5*1.3cm])
end

function plotBoxplot(df1, str, df2=nothing, size=[20*1.3cm,14*1.3cm])
  outputFile = "plot/$(str)_ud_var.png"
  #img = PNG(outputFile, size[1], size[2])
  sort!(df1, [order(:Data)])
  df1[:NUD]= 100 * df1[:NUD]
  #p = plot(df1, x=:Fonte, y=:NUD, Geom.boxplot(suppress_outliers=true),
  #  Guide.xlabel("Fonte"), Guide.ylabel("Porcentagem UDs (%)"),
  #  Guide.title("Presença da Variedade nas UDs: $(str)"),
  #  Theme(plot_padding=[20pt, 60pt, 10pt, 10pt]))
  #draw(img, p)

  if df2 != nothing
    outputFile = "plot/$(str)_var_ud.png"
    img = PNG(outputFile, size[1], size[2])
    sort!(df2, [order(:Data)])
    p = plot(df2, x=:Fonte, y=:Nvariedades,
      Geom.boxplot(suppress_outliers=true),
      Guide.xlabel("Fonte"), Guide.ylabel("Riqueza de Variedades na UD"),
      Guide.title("Riqueza de Variedades por UD: $(str)"),
      Coord.cartesian(ymax=30),
      Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
            plot_padding=[20pt, 40pt, 10pt, 10pt]))
    draw(img, p)
  end
end

function plotIndex()
  df = CSV.File("csv/index.csv", delim = ";", decimal=',') |> DataFrame

  dfMandioca = df[df[:Specie] .== "Mandioca", :]

  dfMandiocaUD = dfMandioca[dfMandioca[:Scale] .== "UD", :]
  p=plot(dfMandiocaUD, xgroup=:Indice, x=:Article, y=:Value, color=:Indice,
    Geom.subplot_grid(Geom.bar(position=:dodge), free_x_axis=true),
    Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
          bar_spacing = 2mm),
    Guide.ylabel("Valor do Índice"),
    Guide.colorkey(title="Índice"))
  draw(PNG("plot/mandiocaIdxUD.png", 25cm, 15cm), p)

  dfMandiocaCom = dfMandioca[dfMandioca[:Scale] .== "Comunidade", :]
  p=plot(dfMandiocaCom, xgroup=:Indice, x=:Article, y=:Value, color=:Indice,
    Geom.subplot_grid(Geom.bar(position=:dodge), free_x_axis=true),
    Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
          bar_spacing = 2mm),
    Guide.ylabel("Valor do Índice"),
    Guide.colorkey(title="Índice"))
  draw(PNG("plot/mandiocaIdxCom.png", 30cm, 15cm), p)


  dfMilho = df[df[:Specie] .== "Milho", :]

  dfMilhoUD = dfMilho[dfMilho[:Scale] .== "UD", :]
  p=plot(dfMilhoUD, xgroup=:Indice, x=:Article, y=:Value, color=:Indice,
    Geom.subplot_grid(Geom.bar(position=:dodge), free_x_axis=true),
    Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
          bar_spacing = 5mm),
    Guide.ylabel("Valor do Índice"),
    Guide.colorkey(title="Índice"))
  draw(PNG("plot/milhoIdxUD.png", 15cm, 15cm), p)

  dfMilhoCom = dfMilho[dfMilho[:Scale] .== "Comunidade", :]
    p=plot(dfMilhoCom, x=:Article, y=:Value, color=:Indice, Geom.bar,
    Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
          bar_spacing = 5mm),
    Guide.ylabel("Valor do Índice"),
    Guide.xlabel(""),
    Guide.colorkey(title="Índice"))
  draw(PNG("plot/milhoIdxCom.png", 8cm, 10cm), p)
end
