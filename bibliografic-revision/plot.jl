using Gadfly
using DataFrames
using Colors
using CSV
using StatsBase
using GLM
using Bootstrap
using Statistics
import Cairo, Fontconfig

function plotAll()
  df, df2, df3 = loadData()
  plotGeraneralData(df)
  plotAllHistogram(df)
  plotAllBoxplot(df2, df3)
  plotGeneral(df)
  plotBootstrap(df)
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
  p=plot(layer(dfClean, x=:NmedioUD, y=:Ncomunidade,
                  color=:Especie, Geom.point),
        layer(f1, 0, maximum(dfClean[:NmedioUD])),
        layer(f2, 0, maximum(dfClean[:NmedioUD]),
                  Theme(default_color="hotpink1")),
        Guide.xlabel("N Var Médio por UD"),
        Guide.ylabel("N Var Comunidade"),
        Guide.colorkey(title="Especie"),
        Guide.title("Agrobiodiversidade"),
        Theme(point_size=0.6mm, highlight_width=0.0mm));
  draw(img, p)

end

function plotComunidadexDU(df, str, size, runRegression)
  img = PNG("plot/$(str).png", size[1], size[2])
  if runRegression
    ols = lm(@formula(Ncomunidade ~ 0 + NmedioUD), df)
    f(x) = coef(ols)[1]*x
    p=plot(layer(df, x=:NmedioUD, y=:Ncomunidade, color=:Fonte, Geom.point),
      layer(f, 0, 1.1*maximum(df[:NmedioUD])),
      Guide.xlabel("N Var Médio por UD"),
      Guide.ylabel("N Var Comunidade"),
      Guide.colorkey(title="Fonte"),
      Guide.title("Agrobiodiversidade de $(str)"),
      Theme(plot_padding=[30pt, 60pt, 10pt, 10pt],
        point_size=0.6mm, highlight_width=0.0mm));
      println("Printing fit result for $(str)")
      println(coeftable(ols))
  else
    p=plot(df, x=:NmedioUD, y=:Ncomunidade, color=:Fonte, Geom.point,
      Guide.xlabel("N Var Médio por UD"), Guide.ylabel("N Var Comunidade"),
      Guide.colorkey(title="Fonte"),
      Guide.title("Agrobiodiversidade de $(str)"),
      Theme(plot_padding=[30pt, 60pt, 10pt, 10pt],
        point_size=0.6mm, highlight_width=0.0mm));
  end
  draw(img, p)
end

function plotGeneral(df)
  dfUD = df[[:Especie, :NmedioUD]]
  dfUD = dropmissing(dfUD)
  img = PNG("plot/todosUD.png", 15*1.3cm, 10*1.3cm)
  p=plot(dfUD, x=:Especie, y=:NmedioUD, Geom.boxplot,
    Guide.xlabel("Espécie"), Guide.ylabel("N Var Médio por UD"));
  draw(img, p)

  dfComunidade = df[[:Especie, :Ncomunidade]]
  dfComunidade = dropmissing(dfComunidade)
  img = PNG("plot/todosComunidade.png", 15*1.3cm, 10*1.3cm)
  p=plot(dfComunidade, x=:Especie, y=:Ncomunidade, Geom.boxplot,
    Guide.xlabel("Espécie"), Guide.ylabel("N Var Comunidade"));
  draw(img, p)

end


function plotAllHistogram(df)
  dfClean = df[[:Especie, :Ncomunidade, :NmedioUD]]

  dfMandioca = dfClean[dfClean[:Especie] .== "Mandioca", :]
  plotHistogram(dfMandioca, "mandioca", [30,30])

  dfMilho = dfClean[dfClean[:Especie] .== "Milho", :]
  plotHistogram(dfMilho, "milho", [15,30])

  dfBatata = dfClean[dfClean[:Especie] .== "Batata", :]
  plotHistogram(dfBatata, "batata", [5,5])
end

function plotHistogram(df, str, binNumber)
  array1=collect(skipmissing(df[:Ncomunidade]))
  array2=collect(skipmissing(df[:NmedioUD]))

  img1 = PNG("plot/$(str)Comunidade.png", 15*1.3cm, 10*1.3cm)
  p1=plot(x=array1, Geom.histogram(bincount=binNumber[1], density=true),
    Guide.xlabel("N Variedades na Comunidade"), Guide.ylabel("Frequencia"),
    Guide.title("Histograma Variedades na Comunidade: $(str). N = $(size(array1)[1])"))
  draw(img1, p1)

  img2 = PNG("plot/$(str)UD.png", 15*1.3cm, 10*1.3cm)
  p2=plot(x=array2, Geom.histogram(bincount=binNumber[2], density=true),
    Guide.xlabel("N Variedades Medio na UD"), Guide.ylabel("Frequencia"),
    Guide.title("Histograma Média Variedades por Unidade Domestica: $(str). N = $(size(array2)[1])."),
    Theme(plot_padding=[50pt, 90pt, 10pt, 10pt]))
  draw(img2, p2)

end

function plotAllBoxplot(df, df2)

  dfMandioca1 = df[df[:Especie] .== "Mandioca", :]
  dfMandioca2 = df2[df2[:Especie] .== "Mandioca", :]
  plotBoxplot(dfMandioca1, "mandioca", dfMandioca2)

  dfMilho1 = df[df[:Especie] .== "Milho", :]
  plotBoxplot(dfMilho1, "milho")

  dfBatata1 = df[df[:Especie] .== "Batata", :]
  dfBatata2 = df2[df2[:Especie] .== "Batata", :]
  plotBoxplot(dfBatata1, "batata", dfBatata2, [12.5*1.3cm, 7.5*1.3cm])
end

function plotBoxplot(df1, str, df2=nothing, size=[20*1.3cm,14*1.3cm])
  outputFile = "plot/$(str)_ud_var.png"
  img = PNG(outputFile, size[1], size[2])
  sort!(df1, [order(:Data)])
  p = plot(df1, x=:Fonte, y=:NUD, Geom.boxplot(suppress_outliers=true),
    Guide.xlabel("Fonte"), Guide.ylabel("Porcentagem UD"),
    Guide.title("Porcentagem de UDs cultivando cada variedade: $(str)"),
    Theme(plot_padding=[20pt, 60pt, 10pt, 10pt]))
  draw(img, p)

  if df2 != nothing
    outputFile = "plot/$(str)_var_ud.png"
    img = PNG(outputFile, size[1], size[2])
    sort!(df2, [order(:Data)])
    p = plot(df2, x=:Fonte, y=:Nvariedades, Geom.boxplot(suppress_outliers=true),
      Guide.xlabel("Fonte"), Guide.ylabel("Número de Variedades"),
      Guide.title("Número de Variedades por UD: $(str)"),
      Theme(plot_padding=[20pt, 40pt, 10pt, 10pt]))
    draw(img, p)
  end
end

function plotBootstrap(df)
  dfMandioca = df[df[:Especie] .== "Mandioca", :]
  dfMandiocaDU = dfMandioca[[:NmedioUD]]
  dfMandiocaDU = dropmissing(dfMandiocaDU)
  dfMandiocaCom = dfMandioca[[:Ncomunidade]]
  dfMandiocaCom = dropmissing(dfMandiocaCom)
  n_boot = 10000
  bsMandiocaDU = bootstrap(mean, dfMandiocaDU[:NmedioUD], BasicSampling(n_boot))
  bsMandiocaCom = bootstrap(mean, dfMandiocaCom[:Ncomunidade], BasicSampling(n_boot))

  img = PNG("plot/mandioca_bs_du.png", 20*1.3cm,14*1.3cm)
  p = plot(x=bsMandiocaDU.t1[1], Geom.histogram(density=true, bincount=30))
  draw(img, p)
  img2 = PNG("plot/mandioca_bs_com.png", 20*1.3cm,14*1.3cm)
  p2 = plot(x=bsMandiocaCom.t1[1], Geom.histogram(density=true, bincount=30))
  draw(img2, p2)

  dfMilho = df[df[:Especie] .== "Milho", :]
  dfMilhoDU = dfMilho[[:NmedioUD]]
  dfMilhoDU = dropmissing(dfMilhoDU)
  dfMilhoCom = dfMilho[[:Ncomunidade]]
  dfMilhoCom = dropmissing(dfMilhoCom)
  n_boot = 10000
  bsMilhoDU = bootstrap(mean, dfMilhoDU[:NmedioUD], BasicSampling(n_boot))
  bsMilhoCom = bootstrap(mean, dfMilhoCom[:Ncomunidade], BasicSampling(n_boot))
  img3 = PNG("plot/milho_bs_du.png", 20*1.3cm,14*1.3cm)
  p3 = plot(x=bsMilhoDU.t1[1], Geom.histogram(density=true, bincount=30))
  draw(img3, p3)
  img4 = PNG("plot/milho_bs_com.png", 20*1.3cm,14*1.3cm)
  p4 = plot(x=bsMilhoCom.t1[1], Geom.histogram(density=true, bincount=30))
  draw(img4, p4)
end
