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

  fileName = ["csv/boxplot_DU_x_Variety.csv", "csv/boxplot_Variety_x_DU.csv"]
  df2 = CSV.File(fileName[1], delim = ";", decimal=',') |> DataFrame
  df3 = CSV.File(fileName[2], delim = ";", decimal=',') |> DataFrame
  return df, df2, df3
end

function plotGeraneralData(df)
  dfClean = df[[:Specie, :Reference, :Ncommunity, :NaverageDU]]
  dfClean = dropmissing(dfClean)

  dfManioc = dfClean[dfClean[:Specie] .== "Manioc", :]
  plotCommunityxDU(dfManioc, "manioc", [23*1.3cm, 10*1.3cm], true)

  dfMaiz = dfClean[dfClean[:Specie] .== "Maiz", :]
  plotCommunityxDU(dfMaiz, "maiz", [19*1.3cm, 10*1.3cm], true)

  dfPotato = dfClean[dfClean[:Specie] .== "Potato", :]
  plotCommunityxDU(dfPotato, "potato", [17*1.3cm, 10*1.3cm], false)

  ols1 = lm(@formula(Ncommunity ~ 0 + NaverageDU),
             dfClean[dfClean[:Specie] .== "Manioc", :])
  ols2 = lm(@formula(Ncommunity ~ 0 + NaverageDU),
             dfClean[dfClean[:Specie] .== "Maiz", :])
  f1(x) = coef(ols1)[1]*x
  f2(x) = coef(ols2)[1]*x
  img = PNG("plot/all.png", 17*1.3cm, 10*1.3cm)
  p=plot(dfClean, x=:NaverageDU, y=:Ncommunity,
          color=:Specie, Geom.point,
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
                        point_size=0.6mm, highlight_width=0.0mm),
          Guide.xlabel("Average varietal richness per DU"),
          Guide.ylabel("Varietal richness in community"),
          Guide.colorkey(title="Specie"),
          Guide.title("Agrobiodiversity: data for the three species"));
  draw(img, p)

end

function plotCommunityxDU(df, str, size, runRegression)
  img = PNG("plot/$(str).png", size[1], size[2])
  if runRegression
    ols = lm(@formula(Ncommunity ~ 0 + NaverageDU), df)
    f(x) = coef(ols)[1]*x
    p=plot(layer(df, x=:NaverageDU, y=:Ncommunity, color=:Reference, Geom.point),
      layer(f, 0, 1.1*maximum(df[:NaverageDU])),
      Guide.xlabel("Average varietal richness per DU"),
      Guide.ylabel("Varietal richness in community"),
      Guide.colorkey(title="Reference"),
      Guide.title("Agrobiodiversity of $(str)"),
      Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
            key_title_font_size=14pt, key_label_font_size=12pt,
            plot_padding=[30pt, 60pt, 10pt, 10pt],
        point_size=0.6mm, highlight_width=0.0mm));
      println("Printing fit result for $(str)")
      println(coeftable(ols))
  else
    p=plot(df, x=:NaverageDU, y=:Ncommunity, color=:Reference, Geom.point,
      Guide.xlabel("Average varietal richness per DU"),
      Guide.ylabel("Varietal richness in community"),
      Guide.colorkey(title="Reference"),
      Guide.title("Agrobiodiversity of $(str)"),
      Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
            key_title_font_size=14pt, key_label_font_size=12pt,
            plot_padding=[30pt, 60pt, 10pt, 10pt],
        point_size=0.6mm, highlight_width=0.0mm));
  end
  draw(img, p)
end

function plotGeneral(df)
  dfDU = df[[:Specie, :NaverageDU]]
  dfDU = dropmissing(dfDU)
  img = PNG("plot/allDU.png", 15*1.3cm, 10*1.3cm)
  p=plot(dfDU, x=:Specie, y=:NaverageDU, Geom.boxplot,
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
    Guide.xlabel("Specie"),
    Guide.ylabel("Average varietal richness per DU"));
  draw(img, p)

  dfCommunity = df[[:Specie, :Ncommunity]]
  dfCommunity = dropmissing(dfCommunity)
  img = PNG("plot/allCommunity.png", 15*1.3cm, 10*1.3cm)
  p=plot(dfCommunity, x=:Specie, y=:Ncommunity, Geom.boxplot,
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
    Guide.xlabel("Specie"),
    Guide.ylabel("Varietal richness in community"));
  draw(img, p)

end


function plotAllHistogram(df)
  dfClean = df[[:Specie, :Ncommunity, :NaverageDU]]

  dfManioc = dfClean[dfClean[:Specie] .== "Manioc", :]
  plotHistogram(dfManioc, "manioc", [30,30])

  dfMaiz = dfClean[dfClean[:Specie] .== "Maiz", :]
  plotHistogram(dfMaiz, "maiz", [15,10])

  dfPotato = dfClean[dfClean[:Specie] .== "Potato", :]
  plotHistogram(dfPotato, "potato", [5,5])
end

function plotHistogram(df, str, binNumber)
  array1=collect(skipmissing(df[:Ncommunity]))
  array2=collect(skipmissing(df[:NaverageDU]))

  img1 = PNG("plot/$(str)Community.png", 15*1.3cm, 10*1.3cm)
  p1=plot(x=array1, Geom.histogram(bincount=binNumber[1], density=true),
                  Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt),
    Guide.xlabel("Varietal richness in community"),
    Guide.ylabel("Frequency (%)"),
    Guide.title("Varietal richness in community: $(str). N = $(size(array1)[1])"))
  draw(img1, p1)

  img2 = PNG("plot/$(str)DU.png", 15*1.3cm, 10*1.3cm)
  p2=plot(x=array2, Geom.histogram(bincount=binNumber[2], density=true),
    Guide.xlabel("Average varietal richness per DU"),
    Guide.ylabel("Frequency (%)"),
    Guide.title("Average varietal richness per domestic unity $(str). N = $(size(array2)[1])."),
    Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
          plot_padding=[50pt, 90pt, 10pt, 10pt]))
  draw(img2, p2)

end

function plotAllBoxplot(df, df2)

  dfManioc1 = df[df[:Specie] .== "Manioc", :]
  dfManioc2 = df2[df2[:Specie] .== "Manioc", :]
  plotBoxplot(dfManioc1, "manioc", dfManioc2)

  #dfMaiz1 = df[df[:Specie] .== "Maiz", :]
  #plotBoxplot(dfMaiz1, "maiz")

  #dfPotato1 = df[df[:Specie] .== "Potato", :]
  #dfPotato2 = df2[df2[:Specie] .== "Potato", :]
  #plotBoxplot(dfPotato1, "potato", dfPotato2, [12.5*1.3cm, 7.5*1.3cm])
end

function plotBoxplot(df1, str, df2=nothing, size=[20*1.3cm,14*1.3cm])
  outputFile = "plot/$(str)_du_variety.png"
  #img = PNG(outputFile, size[1], size[2])
  sort!(df1, [order(:Data)])
  df1[:NDU]= 100 * df1[:NDU]
  #p = plot(df1, x=:Reference, y=:NDU, Geom.boxplot(suppress_outliers=true),
  #  Guide.xlabel("Reference"), Guide.ylabel("Percentage of DU (%)"),
  #  Guide.title("Variety presence in DU: $(str)"),
  #  Theme(plot_padding=[20pt, 60pt, 10pt, 10pt]))
  #draw(img, p)

  if df2 != nothing
    outputFile = "plot/$(str)_variety_du.png"
    img = PNG(outputFile, size[1], size[2])
    sort!(df2, [order(:Data)])
    p = plot(df2, x=:Reference, y=:Nvariety,
      Geom.boxplot(suppress_outliers=true),
      Guide.xlabel("Reference"), Guide.ylabel("Varietal richness in DU"),
      Guide.title("Varietal richness per DU $(str)"),
      Coord.cartesian(ymax=30),
      Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
            plot_padding=[20pt, 40pt, 10pt, 10pt]))
    draw(img, p)
  end
end

function plotIndex()
  df = CSV.File("csv/index.csv", delim = ";", decimal=',') |> DataFrame

  dfManioc = df[df[:Specie] .== "Manioc", :]

  dfManiocDU = dfManioc[dfManioc[:Scale] .== "DU", :]
  p=plot(dfManiocDU, xgroup=:Index, x=:Article, y=:Value, color=:Index,
    Geom.subplot_grid(Geom.bar(position=:dodge), free_x_axis=true),
    Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
          bar_spacing = 2mm),
    Guide.ylabel("Value of index"),
    Guide.colorkey(title="Index"))
  draw(PNG("plot/maniocIdxDU.png", 25cm, 15cm), p)

  dfManiocCommunity = dfManioc[dfManioc[:Scale] .== "Community", :]
  p=plot(dfManiocCommunity, xgroup=:Index, x=:Article, y=:Value, color=:Index,
    Geom.subplot_grid(Geom.bar(position=:dodge), free_x_axis=true),
    Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
          bar_spacing = 2mm),
    Guide.ylabel("Value of index"),
    Guide.colorkey(title="Index"))
  draw(PNG("plot/maniocIdxCommunity.png", 30cm, 15cm), p)


  dfMaiz = df[df[:Specie] .== "Maiz", :]

  dfMaizDU = dfMaiz[dfMaiz[:Scale] .== "DU", :]
  p=plot(dfMaizDU, xgroup=:Index, x=:Article, y=:Value, color=:Index,
    Geom.subplot_grid(Geom.bar(position=:dodge), free_x_axis=true),
    Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
          bar_spacing = 5mm),
    Guide.ylabel("Value of index"),
    Guide.colorkey(title="Index"))
  draw(PNG("plot/maizIdxDU.png", 15cm, 15cm), p)

  dfMaizCom = dfMaiz[dfMaiz[:Scale] .== "Community", :]
    p=plot(dfMaizCom, x=:Article, y=:Value, color=:Index, Geom.bar,
    Theme(minor_label_font_size=10pt, major_label_font_size=14pt,
                        key_title_font_size=14pt, key_label_font_size=12pt,
          bar_spacing = 5mm),
    Guide.ylabel("Value of index"),
    Guide.xlabel(""),
    Guide.colorkey(title="Index"))
  draw(PNG("plot/maizIdxCommunity.png", 8cm, 12cm), p)
end
