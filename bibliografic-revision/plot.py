import numpy as np
import matplotlib.pyplot as plt
import geopandas as gpd
import pandas as pd
from shapely.geometry import Point, Polygon
from matplotlib.lines import Line2D

df = pd.read_csv("csv/data_revision.csv", sep=";")
geometry = [Point(xy) for xy in zip(df.Long, df.Lat)]
crs = {'init': 'epsg:4326'}
gdf = gpd.GeoDataFrame(df, crs=crs, geometry=geometry)
world = gpd.read_file(gpd.datasets.get_path('naturalearth_lowres'))

fig, ax = plt.subplots(1)
base = world.plot(ax=ax, color='white', edgecolor='black')
gdf[gdf.Especie == 'Mandioca'].plot(ax=base, color='red', markersize=7)
gdf[gdf.Especie == 'Milho'].plot(ax=base, color='blue', markersize=7)
gdf[gdf.Especie == 'Batata'].plot(ax=base, color='green', markersize=7)
_ = ax.axis('off')
plt.show()

leg = [Line2D([0], [0], marker='o', color='w', label='Mandioca', markerfacecolor='r', markersize=5),
        Line2D([0], [0], marker='o', color='w', label='Batata', markerfacecolor='g', markersize=5),
        Line2D([0], [0], marker='o', color='w', label='Milho', markerfacecolor='b', markersize=5)]
ax.legend(handles=leg, loc='lower left')
